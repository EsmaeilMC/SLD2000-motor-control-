/*
 * PM_param_tune2.c
 *
 * Created: 10/23/2018 10:23:10 AM
 *  Author: Ahmed
 */ 

/*
 * IM_param_tune1.c
 *
 * Created: 07/10/2018 11:33:28 ?
 *  Author: ahmed
 */ 

#include <PM_param_tune2.h>
#include <motor_data.h>
#include <vfd.h>
#include <arm_math.h>
#include <DACC_interface.h>
#include <axistransform.h>
#include <Inverter.h>
#include <encoder_interface.h>
#include <pop_ups.h>

//filter definisions
#define h0 (40 * 1)
#define h1 (90 * 1)

#define Pgain (40)

float cc0, cc1;

#define c0 cc0
#define c1 cc1
#define c2 c1

float PM_time_lapse = 0;
float va, prev_v, prev_i;

float param_a[2]; //estimated parameter vecotrs
float pa[2][2]; //covariance matricies
float phi_a[2]; //measured data vectors
float g_a[2]; //gains vecotrs

void PM_update_measured_data_vector(float v, float i)
{
	phi_a[0] = (c0 * phi_a[0]) + (c1 * v) + (c2 * prev_v);
	phi_a[1] = (c0 * phi_a[1]) + (c1 * i) + (c2 * prev_i);
	
	prev_i = i;
	prev_v = v;
}


void PM_update_gains_vector(void)
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

void PM_update_covariance_matrix(void)
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

void PM_update_parameters_vector(float output)
{
	float oerr;
	oerr = ((phi_a[0] * param_a[0] + phi_a[1] * param_a[1]));

	oerr = output - oerr;

	param_a[0] += g_a[0] * oerr;
	param_a[1] += g_a[1] * oerr;
}



float rs_h, ld_h, lq_h;

void PM_estimate_rs(void)
{
	rs_h = (h1 - param_a[1]) / param_a[0];
}

void PM_estimate_ld(void)
{
	ld_h = 1 / param_a[0];
}

void PM_estimate_lq(void)
{
	lq_h = 1 / param_a[0];
}

#define TUNE_CONTROLLER_BANDWIDTH (0.00007)
#define TUNE_OMEGA (8 * 2 * PI)
void pm_tune2_calc_PI(void)
{
	menue_drive_data_setup.m_ACRD_i = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_stator_resistance * 10000;
	menue_drive_data_setup.m_ACRD_p = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_LD * 1000;
	menue_drive_data_setup.m_ACRQ_i = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_stator_resistance * 10000;
	menue_drive_data_setup.m_ACRQ_p = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_LQ * 1000;
}

void pm_tune2_calc_ke(void)
{
	float ke = menue_drive_data_applied.m_motor_rated_power * 1000;
	ke /= sqrt(6) * PI * fixedpt_tofloat(fixedpt_mul(menue_drive_data_applied.m_motor_rated_freq, menue_drive_data_applied.m_motor_rated_current));
	menue_drive_data_setup.m_motor_BEMF = ke;
}
uint8_t pm_tune2_comm = 0;
void PM_save_estimated_motor_parameters(void)
{
	menue_drive_data_setup.m_motor_stator_resistance = rs_h;
	menue_drive_data_setup.m_motor_LD = ld_h * 1000;
	menue_drive_data_setup.m_motor_LQ = lq_h * 1000;
	if (pm_tune2_comm > 1) pm_tune2_calc_PI();
	if (pm_tune2_comm > 2) pm_tune2_calc_ke();
}

#define SIG2_FREQ (200.0f * 2 * PI)




#define RS_FILTER (2)
void PM_stage1_tune(void)
{
	float com_ia, ia;
	ia = axt_get_ialpha();
	
	com_ia = menue_drive_data_applied.m_motor_rated_current;
	com_ia /= FIXEDPT_ONE;
	com_ia *= 2 * 1.4;//1.5;
	
	
	//rs_h += ((va/ia) - rs_h) * (TSAMPLE / RS_FILTER);
	PM_update_measured_data_vector(va, ia);
	PM_update_gains_vector();
	PM_update_covariance_matrix();
	PM_update_parameters_vector(ia);
	
	va = (com_ia - ia) * Pgain;
	
	//dacc_interface_writeData(0, com_ia/*est_out*/, 10, 1);
	
	inverter_out_rect((va * FIXEDPT_ONE), 0);
}

#define PM_ANGLE_OFFSET menue_drive_data_applied.m_pole_pos_offset

void PM_stage2_tune(void)
{
	float com_id, id;
	float flux_angle = encoder_getangle_elec_abs(MOTOR_POLEPAIR) + (PM_ANGLE_OFFSET) ;
	axt_set_theta(flux_angle);
	axt_park_exe();
	
	id = axt_get_id();
	
	com_id = 1.5;
	com_id = com_id + 1 * arm_sin_f32(SIG2_FREQ * PM_time_lapse);
	
	com_id *= menue_drive_data_applied.m_motor_rated_current;
	com_id /= FIXEDPT_ONE;
	com_id /= 2;

	PM_update_measured_data_vector(va, id);
	PM_update_gains_vector();
	PM_update_covariance_matrix();
	PM_update_parameters_vector(id);
	
	va = (com_id - id) * Pgain;
	
	//dacc_interface_writeData(0, com_ia/*est_out*/, 20, 1);
	float valpha, vbeta;
	axt_ipark_exe( va, 0, &valpha, &vbeta );
	inverter_out_rect((valpha * FIXEDPT_ONE), (vbeta * FIXEDPT_ONE));
}

void PM_stage3_tune(void)
{
	float com_iq, iq;
	float flux_angle = encoder_getangle_elec_abs(MOTOR_POLEPAIR) + (PM_ANGLE_OFFSET) ;
	axt_set_theta(flux_angle);
	axt_park_exe();
	
	iq = axt_get_iq();
	
	com_iq = 1.5;
	com_iq = com_iq + 1 * arm_sin_f32(SIG2_FREQ * PM_time_lapse);
	
	com_iq *= menue_drive_data_applied.m_motor_rated_current;
	com_iq /= FIXEDPT_ONE;
	com_iq /= 2;

	PM_update_measured_data_vector(va, iq);
	PM_update_gains_vector();
	PM_update_covariance_matrix();
	PM_update_parameters_vector(iq);
	
	va = (com_iq - iq) * Pgain;
	
	//dacc_interface_writeData(0, com_ia/*est_out*/, 20, 1);
	float valpha, vbeta;
	axt_ipark_exe( 0, va, &valpha, &vbeta );
	inverter_out_rect((valpha * FIXEDPT_ONE), (vbeta * FIXEDPT_ONE));
}
#define seg_sqr (9 * 1000000)

void PM_init_tune_param(void)
{
	PM_time_lapse = 0;
	prev_v = 0;
	prev_i = 0;
	va = 0;
	
	//init constants
	cc0 = ((2 - h1*TSAMPLE) / (2 + h1*TSAMPLE));
	cc1 = ((TSAMPLE) / (2 + h1*TSAMPLE));
	
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
}

#define PM_TUNE1_PERIOD (1)
#define PM_TUNE2_PERIOD (5)
#define PM_TUNE3_PERIOD (5)
float PM_stage_period;
#define PM_TUNE_PERIOD (PM_stage_period)

uint8_t PM_tune_stage = 0;
void PM_param_tune2_current_handler(void)
{
	if (PM_time_lapse < PM_TUNE_PERIOD)
	{
		if(PM_tune_stage == 0) PM_stage1_tune();
		else if(PM_tune_stage == 1) PM_stage2_tune();
		else if(PM_tune_stage == 2) PM_stage3_tune();
	} 
	else
	{
		if(PM_tune_stage == 0)
		{
			PM_tune_stage++;
			PM_TUNE_PERIOD = PM_TUNE2_PERIOD;
			PM_estimate_rs();
			PM_init_tune_param();
			inverter_set_dead_comp(0);
		}
		else if(PM_tune_stage == 1)
		{
			PM_tune_stage++;
			PM_TUNE_PERIOD = PM_TUNE3_PERIOD;
			PM_estimate_ld();
			PM_init_tune_param();
		}
		else if(PM_tune_stage == 2)
		{
			PM_tune_stage++;
			PM_TUNE_PERIOD = PM_TUNE1_PERIOD;
			PM_estimate_lq();
		}
		else
		{
			vfd_estop();
			inverter_set_dead_comp(0);
			PM_save_estimated_motor_parameters();
			pop_ups_add("Tune Successful", 1500);
		}
		
		return;
	}
	PM_time_lapse += TSAMPLE;
}

void PM_param_tune2_init(void)
{
	PM_tune_stage = 0;
	PM_TUNE_PERIOD = PM_TUNE1_PERIOD;
	pm_tune2_comm = menue_drive_motor_tune;
	inverter_set_dead_comp(0);
	PM_init_tune_param();

}