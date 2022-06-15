/*
 * VectorPM.c
 *
 * Created: 28/02/2018 06:55:35 م
 *  Author: ahmed
 */ 

#include <menue_data_manager.h>
#include <VectorPM.h>
#include <speed_regulator.h>
#include <axistransform.h>
#include <motor_data.h>
#include <regulators.h>
#include <Inverter.h>
#include <encoder_interface.h>
#include <DACC_interface.h>
#include <menue_data_manager.h>
#include <arm_math.h>
#include <motor_data.h>
#include <trig.h>
#include <vfd.h>

#define PM_ANGLE_OFFSET menue_drive_data_applied.m_pole_pos_offset

struct pi_t pi_d, pi_q; 
float cvd, cvq, cid, ciq;

uint32_t regPM_reset_flag = 1;
void vectorPM_activate_reg_reset(void)
{
	regPM_reset_flag = 1;
}

#define T_angle (65.0)
#define D_SAT_FACTOR (/*0.98 **/ cos(T_angle * PI / 180))
#define Q_SAT_FACTOR (/*0.98 **/ sin(T_angle * PI / 180))
void vectorPM_perform(float desired_tourque)
{
	float flux_angle = encoder_getangle_elec_abs(MOTOR_POLEPAIR) + (PM_ANGLE_OFFSET) ;
	float omega_ele_rad = (enc_get_omega_rad_hat(MOTOR_POLEPAIR));
	omega_ele_rad /= FIXEDPT_ONE;
	float id , iq, flux, vff_d, vff_q;
	float tmp, com_vd, com_vq;
	float dc_voltage = iverter_get_dc_v();
	float q_sat_limit;
	dc_voltage /= FIXEDPT_ONE;
	//omega_ele_rad_f += (omega_ele_rad - omega_ele_rad_f) * (TSAMPLE / SPEED_F_TIME);

	axt_set_theta(flux_angle);
	axt_park_exe();
	
	id = axt_get_id();
	iq = axt_get_iq();
	
	cid = id;
	ciq = iq;//desired_tourque;
	
	vff_d = ( -omega_ele_rad * (I_PM_LQ * iq) );
	vff_q = ( omega_ele_rad * ((I_PM_LD * id ) + KE)) ;
	
	//determine motoring or regenerating action
	if (omega_ele_rad > 0)
	{
		if (iq < 0) vfd_set_regencount();
		else vfd_clr_regencount();
	}
	else if (omega_ele_rad < 0)
	{
		if (iq > 0) vfd_set_regencount();
		else vfd_clr_regencount();
	}
 	//vff_d = 0; 
 	//vff_q = 0;
	
	tmp = (0 - id); //error signal.
	com_vd = reg_pi_exe(dc_voltage /*580 * D_SAT_FACTOR*/, tmp, 0/*vff_d*/, TSAMPLE, &pi_d);

	
	
	q_sat_limit = fixedpt_abs(com_vd) / dc_voltage;
	q_sat_limit += 0.005;
	q_sat_limit = com_sin(q_sat_limit) * dc_voltage;
	

	
	tmp = (desired_tourque - iq); //error signal.
	com_vq = reg_pi_exe(q_sat_limit/*dc_voltage*//*580 * Q_SAT_FACTOR*/, tmp, vff_q, TSAMPLE, &pi_q );
	
	float valpha, vbeta;
	
	axt_ipark_exe( com_vd, com_vq, &valpha, &vbeta );
	cvd = com_vd;
	cvq = com_vq;
	inverter_out_rect((valpha * FIXEDPT_ONE), (vbeta * FIXEDPT_ONE));
	
	if (regPM_reset_flag == 1)
	{
		reg_pi_reset_memory(&pi_d);
		reg_pi_reset_memory(&pi_q);
	}
}

#define T_CONST (KE * MOTOR_POLEPAIR * 1.5 / sqrt3)

#define IKPD        (menue_drive_data_applied.m_ACRD_p * menue_drive_data_applied.m_ACR_bandwidth / (1000))
#define IKID        (menue_drive_data_applied.m_ACRD_i * menue_drive_data_applied.m_ACR_bandwidth / (10))

#define IKPQ        (menue_drive_data_applied.m_ACRQ_p * menue_drive_data_applied.m_ACR_bandwidth / 1000)
#define IKIQ        (menue_drive_data_applied.m_ACRQ_i * menue_drive_data_applied.m_ACR_bandwidth / 10)

#define IKPD_ZS     (menue_drive_data_applied.m_ACRD_p * menue_drive_data_applied.m_ACR_bandwidth_zs / (1000))
#define IKID_ZS     (menue_drive_data_applied.m_ACRD_i * menue_drive_data_applied.m_ACR_bandwidth_zs / (10))

#define IKPQ_ZS     (menue_drive_data_applied.m_ACRQ_p * menue_drive_data_applied.m_ACR_bandwidth_zs / 1000)
#define IKIQ_ZS     (menue_drive_data_applied.m_ACRQ_i * menue_drive_data_applied.m_ACR_bandwidth_zs / 10)

void vector_PM_init(void)
{
	reg_pi_init(IKPD_ZS, IKID_ZS, &pi_d);
	reg_pi_init(IKPQ_ZS, IKIQ_ZS, &pi_q);
	if (MOTOR_PM_CVC) speed_regulator_setKt(T_CONST);
}

void vectorpm_PI_changeGains(void)
{
	regPM_reset_flag = 0;
	reg_pi_change_gains(IKPD, IKID, &pi_d);
	reg_pi_change_gains(IKPQ, IKIQ, &pi_q);
}