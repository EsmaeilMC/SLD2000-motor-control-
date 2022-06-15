/*
 * vectorIM.c
 *
 * Created: 31/03/2017 10:32:05 ص
 *  Author: ahmed
 */ 

#include <vectorIM.h>
#include <speed_regulator.h>
#include <axistransform.h>
#include <motor_data.h>
#include <regulators.h>
#include <Inverter.h>
#include <encoder_interface.h>
#include <arm_math.h>
#include <vfd.h>
#include <trig.h>
#include <math.h>

#define FLUX_MIN (0.0001f)

#define BEMF_FILTER_TIME (menue_drive_data_applied.m_enc_filter_low / 1000)
float filter_bemf_weight = 0, new_bemf_weight = 1;
void vectorIM_set_filter_timeconst(void)
{
	float temp;

	if (BEMF_FILTER_TIME == 0)
	{
		new_bemf_weight = 1;
		filter_bemf_weight = 0;
	}
	else if(BEMF_FILTER_TIME > 0)
	{
		
		temp = BEMF_FILTER_TIME;
		temp /= TSAMPLE;
		temp += 1;
		temp = 1 / temp;
		new_bemf_weight = temp;
		
		temp = TSAMPLE;
		temp /= BEMF_FILTER_TIME;
		temp += 1;
		temp = 1 / temp;
		filter_bemf_weight = temp;
	}
}

float vectorIM_angle = 0;
float vectorIM_get_angle(float speed)
{
	float tmp;
	tmp = speed * TSAMPLE;
	tmp *= (360 / (2*PI));
	
	vectorIM_angle += tmp;

	//vectorIM_angle = -360 * 1.25;
	vectorIM_angle /= 360;
	vectorIM_angle -= (int32_t)vectorIM_angle;
	vectorIM_angle *= 360;
// 	while(vectorIM_angle < 0) vectorIM_angle += (360);
// 	while(vectorIM_angle >= (360)) vectorIM_angle -= (360);
	
	//volatile float m = fixedpt_tofloat(vectorIM_angle);
	return vectorIM_angle;
}

float vectorIM_slip_speed = 0, electrical_speed_rad = 0, flux_speed = 0;
float vector_IM_flux = 0.001;
void vectorIM_set_electrical_speed(float elec_speed_hz)
{
	electrical_speed_rad = elec_speed_hz * 6.283;
}

float vectorIM_get_estimated_shaft_speed_Hz(void)
{
	return electrical_speed_rad / (6.283 * MOTOR_POLEPAIR);
}

float vectorIM_get_estimated_shaft_speed_rad(void)
{
	return (electrical_speed_rad / MOTOR_POLEPAIR);
}

float vectorIM_get_measured_shaft_speed(void)
{
	//get encoder or estimated speed.
	float tmp = (enc_get_omega_rad_hat(MOTOR_POLEPAIR));
	tmp /= FIXEDPT_ONE;
	return  tmp;
}

#define sgn(x) (x<0?-1:1)

#define IM_SPEED_FILTER (0.1f)
float prev_ia = 0, prev_ib = 0;
float ed_mem  = 0, eq_mem = 0;
void vectorIM_estimate_sync_speed(float va, float vb)
{
	float ed, eq, ws;
	float ia, ib;
	float ea, eb;
	if (vfd_states_allowedtomove() == 0)
	{
		electrical_speed_rad = 0;
		return;
	}
	
	ia = axt_get_ialpha();
	ib = axt_get_ibeta();
	
	ea = va - ia * I_IM_RS - ((ia - prev_ia) * (I_IM_LSEG/TSAMPLE));
	eb = vb - ib * (I_IM_RS) - ((ib - prev_ib) * (I_IM_LSEG/TSAMPLE));
	
	axt_get_park_transform(ea, eb, &ed, &eq);
	
	ed_mem = (ed_mem * filter_bemf_weight) + (ed * new_bemf_weight);
	eq_mem = (eq_mem * filter_bemf_weight) + (eq * new_bemf_weight);
	
	eq = eq_mem - sgn(eq_mem)*(ed_mem);
	
	if (fixedpt_abs(vector_IM_flux) <= FLUX_MIN)
	{
		ws = 0;
	}
	else
	{
		ws = eq / vector_IM_flux;
	}
	
	
//	electrical_speed_rad += (ws - electrical_speed_rad) * (TSAMPLE / IM_SPEED_FILTER);
	electrical_speed_rad = ws - vectorIM_slip_speed;
	
 	prev_ia = ia;
 	prev_ib = ib;
}

void vectorIM_calc_speeds()
{
	
	if (MOTOR_IM_CVC)
	{
		//closed loop vector IM
		electrical_speed_rad = vectorIM_get_measured_shaft_speed();
	}
	else if (MOTOR_IM_OVC)
	{
		//Open loop vector IM
		//e_shaft_speed = e_flux_speed - vectorIM_slip_speed;
		//electrical_speed_rad = e_shaft_speed;
	}
	flux_speed = electrical_speed_rad + vectorIM_slip_speed;
}

float vectorIM_estimateflux(float ID)
{
	float flux_max  =  (ID * I_IM_LM);
	float t_ratio   =  (TSAMPLE / I_IM_TC);
	vector_IM_flux   += ((flux_max - vector_IM_flux) * t_ratio);
	
	return (vector_IM_flux);	
}

#define T_angle (60.0)
#define D_SAT_FACTOR (/*0.98 **/ cos(T_angle * PI / 180))
#define Q_SAT_FACTOR (/*0.98 **/ sin(T_angle * PI / 180))

#define T_CONST (I_IM_I_NL * I_IM_LM * MOTOR_POLEPAIR * 1.5 / sqrt3)
#define desired_id (I_IM_I_NL)
struct pi_t pi_d, pi_q; 

uint32_t reg_reset_flag = 1;

void vectorIM_activate_reg_reset(void)
{
	reg_reset_flag = 1;
}

void vectorIM_perform(float desired_tourque, float desired_flux_ratio)
{
	float id , iq, flux, vff_d, vff_q, vff_qEMF;
	float tmp, com_vd, com_vq;
	float dc_voltage = iverter_get_dc_v();
	float q_sat_limit;
	dc_voltage /= FIXEDPT_ONE;
	
	vectorIM_calc_speeds();
	
	float flux_angle = vectorIM_get_angle(flux_speed);

	
	axt_set_theta(flux_angle);
	axt_park_exe();	
	
	id = axt_get_id();
	iq = axt_get_iq();
	
	cid = id;
	ciq = iq;
			
	flux = vectorIM_estimateflux(id);
	
	//speed_regulator_setKt(T_CONST);
	vff_d = (-flux_speed * I_IM_LSEG * iq);
	//vff_d=0;

	vff_qEMF = (flux * electrical_speed_rad);
	
	//determine motoring or regenerating action
	if (vff_qEMF > 0)
	{
		if (iq < 0) vfd_set_regencount();
		else vfd_clr_regencount();
	} 
	else if (vff_qEMF < 0)
	{
		if (iq > 0) vfd_set_regencount();
		else vfd_clr_regencount();
	}
	
 	vff_q = (flux_speed * I_IM_LSEG * id);
	//vff_q=0;
	
	
	//com_vd = 380 * sqrt2 * flux_speed / (6.283 * 50 );
	//com_vq = 0;
	
	if (reg_reset_flag == 1)
	{
		reg_pi_reset_memory(&pi_d);
		reg_pi_reset_memory(&pi_q);
		com_vd = 0;
		com_vq = 0;
	}
	else
	{
		tmp = ((desired_id * desired_flux_ratio) - id); //error signal.
		com_vd = reg_pi_exe(dc_voltage * D_SAT_FACTOR, tmp, vff_d, TSAMPLE, &pi_d);
		
		q_sat_limit = fixedpt_abs(com_vd) / dc_voltage;
		q_sat_limit += 0.005;
		q_sat_limit = com_sin(q_sat_limit) * dc_voltage;
		
		tmp = (desired_tourque - iq); //error signal.
		com_vq = reg_pi_exe(q_sat_limit, tmp, vff_q + vff_qEMF, TSAMPLE, &pi_q );	
	}
	
	//cvd = com_vd;
	cvq = com_vq;
	
	
	float valpha, vbeta;
	axt_ipark_exe(com_vd, com_vq, &valpha, &vbeta);
	inverter_out_rect((valpha * FIXEDPT_ONE), (vbeta * FIXEDPT_ONE));
	
	
	//calc slip speed
	tmp = ( desired_tourque * I_IM_RR );
	if (fixedpt_abs(flux) <= FLUX_MIN)
	{
		vectorIM_slip_speed = 0;
	} 
	else
	{
		vectorIM_slip_speed = (tmp / flux);
	}
	cvd = vectorIM_slip_speed / 6.283;	
	
	if (MOTOR_IM_OVC)
	{
		//Open loop vector IM
		vectorIM_estimate_sync_speed(valpha, vbeta);
	}
	//vectorIM_slip_speed = 0;
	//cid = flux;
	//cid = (electrical_speed_rad - 0/*vectorIM_slip_speed*/) / 6.283; //electrical_speed_rad;
	//cvd /= FIXEDPT_ONE;
	//cvq = electrical_speed_rad;
	
	
}

 #define IKPD        (menue_drive_data_applied.m_ACRD_p * menue_drive_data_applied.m_ACR_bandwidth / 1000)
 #define IKID        (menue_drive_data_applied.m_ACRD_i * menue_drive_data_applied.m_ACR_bandwidth / 10)

 #define IKPQ        (menue_drive_data_applied.m_ACRQ_p * menue_drive_data_applied.m_ACR_bandwidth / 1000)
 #define IKIQ        (menue_drive_data_applied.m_ACRQ_i * menue_drive_data_applied.m_ACR_bandwidth / 10)
void vector_IM_init(void)
{
	reg_reset_flag = 0;
	vector_IM_flux = 0;
	vectorIM_slip_speed = 0;
	flux_speed = 0;
	vectorIM_angle = 0;
	reg_pi_init(IKPD, IKID, &pi_d);
	reg_pi_init(IKPQ, IKIQ, &pi_q);	
	if (MOTOR_IM_CVC) speed_regulator_setKt(T_CONST);
	vectorIM_set_filter_timeconst();
}