/*
 * IM_param_tune1.c
 *
 * Created: 07/10/2018 11:33:28 ص
 *  Author: ahmed
 */ 

#include <IM_param_tune1.h>
#include <motor_data.h>
#include <vfd.h>
#include <arm_math.h>
#include <DACC_interface.h>
#include <axistransform.h>
#include <Inverter.h>
#include <pop_ups.h>

//filter definitions
#define h0 (40 * 1)
#define h1 (90 * 1)

#define Pgain (40)

float cc0, cc1, cc3, cc4, cc5;

#define c0 cc0
#define c1 cc1
#define c2 c1
#define c10 cc3
#define c11 cc4
#define c12 cc5

float est_out;


float time_lapse = 0;
float IM_stage_period;
float va, prev_v, prev_i;

float param_a[2], param_b[2]; //estimated parameter vecotrs
float pa[2][2], pb[2][2]; //covariance matricies
float phi_a[2], phi_b[2]; //measured data vectors
float g_a[2], g_b[2]; //gains vecotrs

void update_measured_data_vector1(float v, float i)
{
	phi_a[0] = (c0 * phi_a[0]) + (c1 * v) + (c2 * prev_v);
	phi_a[1] = (c0 * phi_a[1]) + (c1 * i) + (c2 * prev_i);
	
	prev_i = i;
	prev_v = v;
}

void update_measured_data_vector2(float v, float i)
{
	phi_a[0] = (c10 * phi_a[0]) + (c11 * i) - (c11 * prev_i);
	phi_a[1] = (c10 * phi_a[1]) + (c12 * v) + (c12 * prev_v);
	
	prev_i = i;
	prev_v = v;
}

void update_gains_vector(void)
{
	float tmp[2], tmp1;
	
	//update gain1
	tmp[0] = (phi_a[0] * pa[0][0]) + (phi_a[1] * pa[0][1]);
	tmp[1] = (phi_a[0] * pa[1][0]) + (phi_a[1] * pa[1][1]);
	
	tmp1 = ((phi_a[0] * tmp[0]) + (phi_a[1] * tmp[1]));
	tmp1 += (1.0f);
	
	g_a[0] = tmp[0] / tmp1;
	g_a[1] = tmp[1] / tmp1;
}

void update_covariance_matrix(void)
{
	float tmp[2];
	
	//update cov1
	tmp[0] = (phi_a[0] * pa[0][0]) + (phi_a[1] * pa[1][0]);
	tmp[1] = (phi_a[0] * pa[0][1]) + (phi_a[1] * pa[1][1]);
	
	pa[0][0] -= tmp[0] * g_a[0];
	pa[0][1] -= tmp[1] * g_a[0];
	pa[1][0] -= tmp[0] * g_a[1];
	pa[1][1] -= tmp[1] * g_a[1];
}

void update_parameters_vector(float output)
{
	float oerr;
	oerr = ((phi_a[0] * param_a[0] + phi_a[1] * param_a[1]));
	
	//est_out = oerr;
	//est_out = output;
	//oerr = output - oerr;
	est_out = oerr;
	oerr = output - oerr;

	param_a[0] += g_a[0] * oerr;
	param_a[1] += g_a[1] * oerr;
}

#define rs (1.72685f)
#define rr (1.45f)
#define ls (0.012f)
#define lr (0.001f)
#define lm (0.155f)

volatile float rs_h, ls_h, rr_h, lm_h;

void estimate_rs(void)
{
	rs_h = (h1 - param_a[1]) / param_a[0];
}

void estimate_ls(void)
{
	ls_h = 1 / param_a[0];
}

void estimate_rr(void)
{
	rr_h = param_a[0];
	lm_h = (rr_h) / (h0 - param_a[1]) ;
}

void estimate_lm(void)
{
	
}

#define TUNE_CONTROLLER_BANDWIDTH (0.00007)
#define TUNE_OMEGA (8 * 2 * PI)
void im_tune1_calc_PI(void)
{
	menue_drive_data_setup.m_ACRD_i = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * rs_h * 10000;
	menue_drive_data_setup.m_ACRD_p = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_leakage_inductance * 1000;
	menue_drive_data_setup.m_ACRQ_i = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * (rs_h + rr_h) * 10000;
	menue_drive_data_setup.m_ACRQ_p = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_leakage_inductance * 1000;
}

#define MAX_BEMF (270)
void im_tune1_calc_i0(void)
{
	float iasqr, rated_omega = menue_drive_data_applied.m_motor_rated_speed;
	float tmp, bemf;
	rated_omega /= (FIXEDPT_ONE * 60 / menue_drive_data_applied.m_motor_pole_pair_count);
	rated_omega *= 2 * PI;
	
	tmp = menue_drive_data_applied.m_motor_rated_power * 1000;
	tmp /= lm_h * rated_omega;
	tmp *= tmp * 4 / 3;
	
	iasqr = menue_drive_data_applied.m_motor_rated_current;
	iasqr /= FIXEDPT_ONE;
	iasqr *= iasqr;
	
	tmp = (iasqr * iasqr) - tmp;
	//todo check tmp > 1
	tmp = sqrt(tmp);
	
	tmp = (iasqr - tmp) / 2;
	//todo check tmp > 1
	tmp = sqrt(tmp);
	
	bemf = lm_h * tmp * rated_omega;
	
	if (1)//bemf > MAX_BEMF)
	{
		tmp = MAX_BEMF / (lm_h * rated_omega);
	}
	
	menue_drive_data_setup.m_motor_noload_current = tmp;
	
}

uint8_t im_tune1_comm = 0;
float lm_dev, rr_dev;
void save_estimated_motor_parameters(void)
{
	menue_drive_data_setup.m_motor_stator_resistance = rs_h  * (sqrt3 / 1.5 );
	menue_drive_data_setup.m_motor_leakage_inductance = ls_h * 1000 * (sqrt3 / 1.5 );
	menue_drive_data_setup.m_motor_rotor_resistance = rr_h * (sqrt3 / 1.5 );
	menue_drive_data_setup.m_motor_magnetizing_inductance = lm_h * 1000 * (sqrt3 / 1.5 );	
	
 	if (im_tune1_comm > 1) im_tune1_calc_PI();
 	if (im_tune1_comm > 2) im_tune1_calc_i0();
	
	//lm_dev = (lm - lm_h)*100 / lm;
	//rr_dev = (rr - rr_h)*100 / rr;
	ls_h *= 1;
}

#define RS_err (1.00f)
#define LS_err (1.00f)

#define SIG1_FREQ (5.0f * 2 * PI)
#define SIG2_FREQ (250.0f * 2 * PI)
#define SIG3_FREQ (10.0f * 2 * PI)
#define SIG4_FREQ (25.0f * 2 * PI)

#define red_fac (0.0)


#define a  (lm / lr)
#define lrts (lr / TSAMPLE)
#define lmts (lm / TSAMPLE)


#define RS (rs)
#define RR rr//(a * a * rr)
#define LS ls//(ls - a * lm)
#define LM lm//(a * lm)
#define TR	(LM / RR)
#define RAT_T (TR / TSAMPLE)

float prev_ia, prev_vm;
float rated_c = 1;
float motor_sim(float mi)
{
	float vma, dia, vmotor;
	dia = (mi - prev_ia) / TSAMPLE;
	
	va = RS * mi + LS * dia;
	
	vma = LM * dia + prev_vm * RAT_T;
	vma /= (1 + RAT_T);
	
	prev_vm = vma;
	
	return vma + va;
}

float motor_sim2(float mi)
{
	float vma, dia, vmotor;
	dia = (mi - prev_ia) / TSAMPLE;
	
	va = 0;
	
	vma = LM * dia + prev_vm * RAT_T;
	vma /= (1 + RAT_T);
	
	prev_vm = vma;
	
	return vma + va;
}


#define RS_FILTER (2)
void IM_stage1_tune(void)
{
	float com_ia, ia;
	ia = axt_get_ialpha();
	
	com_ia = rated_c * 1.0;//1.5;
	//com_ia = com_ia + 1 * arm_sin_f32(SIG2_FREQ * time_lapse);
	
	//com_ia *= 8 / 1.5;
	//ia = com_ia;
	//com_ia *= menue_drive_data_setup.m_motor_rated_current;
	//com_ia /= (3.8 * FIXEDPT_ONE);
	
	//va = motor_sim(ia);
	
	//ia = com_ia;
	//va = motor_sim(ia);
	//rs_h += ((va/ia) - rs_h) * (TSAMPLE / RS_FILTER);
	update_measured_data_vector1(va, ia);
	update_gains_vector();
	update_covariance_matrix();
	update_parameters_vector(ia);
	
	va = (com_ia - ia) * Pgain;
	//va = ia;
	//dacc_interface_writeData(0, com_ia/*est_out*/, 10, 1);
	
	inverter_out_single_axis(va);
	//inverter_out_rect((va * FIXEDPT_ONE), 0);
	prev_ia = ia;
}
void IM_stage2_tune(void)
{
	float com_ia, ia, im;
	float dia, vma = 0;
	ia = axt_get_ialpha();
	
	com_ia = 1.8;
	com_ia = com_ia + 1 * arm_sin_f32(SIG2_FREQ * time_lapse);
	
	com_ia *= rated_c * 0.5;
	//ia = com_ia;
	//com_ia *= menue_drive_data_setup.m_motor_rated_current;
	//com_ia /= (3.8 * FIXEDPT_ONE);
	
	//va = motor_sim(ia);
	//ia = com_ia;
	//va = motor_sim(ia);
	
	update_measured_data_vector1(va, ia);
	update_gains_vector();
	update_covariance_matrix();
	update_parameters_vector(ia);
	
	va = (com_ia - ia) * Pgain;
	
	//dacc_interface_writeData(0, com_ia/*est_out*/, 20, 1);
	
	inverter_out_single_axis(va);
	//inverter_out_rect((va * FIXEDPT_ONE), 0);
	prev_ia = ia;
}
float vfa = 0;
#define	VFA_FILTER (0.01)
void IM_stage3_tune(void)
{
	float com_ia, ia;
	float dia, vma = 0;
	ia = axt_get_ialpha();
	
	com_ia = 0;
	//com_ia = 0.2;
	//com_ia = (1.0f/IM_stage_period);
	//com_ia *= time_lapse;
	com_ia = com_ia + 1 * arm_sin_f32(SIG1_FREQ * time_lapse);
	com_ia = com_ia + 1 * arm_sin_f32(SIG3_FREQ * time_lapse);
	//com_ia = com_ia + 1 * arm_sin_f32(SIG4_FREQ * time_lapse);
	
	com_ia *= rated_c * 0.6;
	com_ia += rated_c * 0.7;
	//ia = com_ia;
	//com_ia *= menue_drive_data_setup.m_motor_rated_current;
	//com_ia /= (3.8 * FIXEDPT_ONE);
	
	//va = motor_sim(ia);
	//ia = com_ia;
	//va = motor_sim(ia);
	
	dia += (ia - prev_ia) / TSAMPLE;
	vma = va - /*(RS * RS_err)*/ rs_h * ia - /*(LS * LS_err)*/ ls_h * dia;
/*	vma = motor_sim2(ia);*/
	//vfa += (vma - vfa) * VFA_FILTER;
	//vma = vfa;
	update_measured_data_vector2(vma, ia);
	update_gains_vector();
	update_covariance_matrix();
	update_parameters_vector(vma);
	
	va = (com_ia - ia) * Pgain;
	//va = com_ia;
	
	//dacc_interface_writeData(0, com_ia/*est_out*/, 20, 1);
	
	inverter_out_single_axis(va);
	//inverter_out_rect((va * FIXEDPT_ONE), 0);
	//inverter_out_polar(0, va * FIXEDPT_ONE);
	prev_ia = ia;
}
#define FLUX_CHARGE_TIME (3)
char im_lm_stage = 0;
float im_stage4_flux = 0, ia_final;
char zero_current_timeout = 0;
void IM_stage4_tune(void)
{
	float com_ia, ia , im, rotor_flux;
	
	ia = axt_get_ialpha();
	
	if (im_lm_stage == 0)
	{
		com_ia = (rated_c * 0.6);
		va = (com_ia - ia) * Pgain;
		ia_final = ia;
		if(time_lapse >= FLUX_CHARGE_TIME) im_lm_stage = 1;
	} 
	else
	{
		if(ia <= 0) 
		{
			if(++zero_current_timeout > 100)
			{
				time_lapse = IM_stage_period;
				return;	
			}
		}

		im_stage4_flux += rs_h * ia * TSAMPLE;
		
		/*vfa += (vma - vfa) * VFA_FILTER;*/
		lm_h = im_stage4_flux / ia_final;//(lm_h - rotor_flux / im) * (0.01);///TSAMPLE;
		lm_h -= ls_h;
		va = 0;
		
	}
	
	inverter_out_single_axis(va);
	prev_ia = ia;
}

// void IM_stage4_tune(void)
// {
// 	float com_ia, ia , im, rotor_flux;
// 	float dia, vma = 0;
// 	ia = axt_get_ialpha();
// 	
// 	com_ia = (rated_c * 1.5);// * time_lapse / IM_stage_period);
// 	
// 	vma = va - /*(RS * RS_err)*/ rs_h * ia;
// 	im_stage4_flux += vma * TSAMPLE;
// 	rotor_flux = im_stage4_flux - (ls_h * ia);
// 	
// 	dia += (ia - prev_ia) / TSAMPLE;
// 	vma -= /*(LS * LS_err)*/ ls_h * dia;
// 	
// 	
// 	vfa += (vma - vfa) * VFA_FILTER;
// 	vma = vfa;
// 	im = ia - (vma / rr_h);
// 	lm_h = (lm_h - rotor_flux / im) * (0.01);///TSAMPLE;
// 	
// 	va = (com_ia - ia) * Pgain;
// 	//va = com_ia;
// 	
// 	//dacc_interface_writeData(0, com_ia/*est_out*/, 20, 1);
// 	
// 	inverter_out_single_axis(va);
// 	//inverter_out_rect((va * FIXEDPT_ONE), 0);
// 	//inverter_out_polar(0, va * FIXEDPT_ONE);
// 	prev_ia = ia;
// }

#define seg_sqr (9 * 1000000)

void IM_init_tune_param(void)
{
	time_lapse = 0;
	prev_v = 0;
	prev_i = 0;
	va = 0;
	
	//init constants
	cc0 = ((2 - h1*TSAMPLE) / (2 + h1*TSAMPLE));
	cc1 = ((TSAMPLE) / (2 + h1*TSAMPLE));
	cc3 = ((2 - h0*TSAMPLE) / (2 + h0*TSAMPLE));
	cc4 = ((2) / (2 + h0*TSAMPLE));
	cc5 = ((TSAMPLE) / (2 + h0*TSAMPLE));
	
	// init parameter vectors a to 0
	param_a[0] = 0;
	param_a[1] = 0;
	
	// init covariance matrix a to seg2 * I
	pa[0][0] = seg_sqr;
	pa[0][1] = 0;
	pa[1][0] = 0;
	pa[1][1] = seg_sqr;
	
	phi_a[0] = 0;
	phi_a[1] = 0;
	
	im_stage4_flux = 0;
	zero_current_timeout = 0;
}

#define IM_TUNE1_INTERMEDIATE (0.5)
#define IM_TUNE1_PERIOD (2)
#define IM_TUNE2_PERIOD (4)
#define IM_TUNE3_PERIOD (6)
#define IM_TUNE4_PERIOD (20 + FLUX_CHARGE_TIME)
#define IM_TUNE_PERIOD (IM_stage_period)
float intermediate_time = 0;

uint8_t IM_tune_stage = 0;
void IM_param_tune1_current_handler(void)
{
	if (intermediate_time > 0)
	{
		intermediate_time -= TSAMPLE;
		inverter_out_single_axis(0);
		return;
	}
	else if (time_lapse < IM_TUNE_PERIOD)
	{
		if(IM_tune_stage == 0) IM_stage1_tune();
		else if(IM_tune_stage == 1) IM_stage2_tune();
		else if(IM_tune_stage == 2) IM_stage3_tune();
		else if(IM_tune_stage == 3) IM_stage4_tune();
	} 
	else
	{
		if(IM_tune_stage == 0)
		{
			IM_tune_stage++;
			IM_TUNE_PERIOD = IM_TUNE2_PERIOD;
			estimate_rs();
			IM_init_tune_param();
			intermediate_time = IM_TUNE1_INTERMEDIATE;
		}
		else if(IM_tune_stage == 1)
		{
			IM_tune_stage++;
			estimate_ls();
			IM_TUNE_PERIOD = IM_TUNE3_PERIOD;
			IM_init_tune_param();
			intermediate_time = IM_TUNE1_INTERMEDIATE;
		}
		else if(IM_tune_stage == 2)
		{
			IM_tune_stage++;
			IM_TUNE_PERIOD = IM_TUNE4_PERIOD;
			estimate_rr();
			IM_init_tune_param();
			intermediate_time = IM_TUNE1_INTERMEDIATE;
			im_lm_stage = 0;
		}
		else if(IM_tune_stage == 3)
		{
			IM_tune_stage++;
			IM_TUNE_PERIOD = IM_TUNE1_PERIOD;
			estimate_lm();
			//intermediate_time = IM_TUNE1_INTERMEDIATE;
		}
		else
		{
			vfd_estop();
			inverter_set_dead_comp(0);
			save_estimated_motor_parameters();
			pop_ups_add("Tune Successful", 1500);
		}
		
		return;
	}
	time_lapse += TSAMPLE;
}

void IM_param_tune1_init(void)
{
	IM_tune_stage = 0;
	rated_c = fixedpt_tofloat(menue_drive_data_applied.m_motor_rated_current) * sqrt2;
	IM_TUNE_PERIOD = IM_TUNE1_PERIOD;
	im_tune1_comm = menue_drive_motor_tune;
	inverter_set_dead_comp(0);
	IM_init_tune_param();

}

float IM_param_tune1_getMdata(void)
{
	return lm_h;
}