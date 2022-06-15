/*
 * vfd.c
 *
 * Created: 23/02/2017 04:21:19 م
 *  Author: ahmed
 */ 

#include <vfd.h>
#include <VF.h>
#include <vectorIM.h>
#include <VectorPM.h>
#include <scurve_speed.h>
#include <vfdstates.h>
#include <encoder_interface.h>
#include <d_faults.h>
#include <speed_regulator.h>
#include <SPMstatic_tune.h>
#include <scCheck.h>
#include <PM_param_tune1.h>
#include <PM_param_tune2.h>
#include <IM_param_tune1.h>
#include <motor_excitation.h>
#include <distance_regulator.h>

#define MAXSPEED			(fixedpt_mul(D_MOTOR_SPEED_HZ, menue_lift_data_applied.m_high_speed) / 100)
#define CRPSPEED			(fixedpt_mul(D_MOTOR_SPEED_HZ, menue_lift_data_applied.m_creep_speed) / 100)
#define MAXACC				(menue_lift_data_applied.m_acc)
#define MAXDEC				(menue_lift_data_applied.m_dec)
#define J1					(menue_lift_data_applied.m_corner1)
#define J2					(menue_lift_data_applied.m_corner2)
#define J3					(menue_lift_data_applied.m_corner3)
#define CREEP_DISTANCE_m	(fixedpt_mul(fixedpt_rconst(0.125), (menue_lift_data_applied.m_creep_distance)))
#define CREEP_DISTANCE		(CREEP_DISTANCE_m * MOTOR_POLEPAIR)

fixedpt desired_speed = 0;
fixedpt desired_distance = 0;
fixedpt ref_speed = 0;

uint32_t speed_ref_stage = 0;
uint32_t time_tick = 0;
fixedpt c_acceleration =0;
uint32_t acc_dec = 0;

motion_dynamics_t  m_dynamics_settings;

fixedpt vfd_get_ref_speed(void)
{
	return ref_speed;
}
fixedpt vfd_get_desired_speed(void)
{
	return desired_speed;
}
fixedpt vfd_get_desired_distance(void)
{
	return desired_distance;
}
fixedpt vfd_get_ref_acceleration(void)
{
	return c_acceleration;
}

int32_t vfd_is_constant_speed(void)
{
	if (fixedpt_abs(ref_speed) > 0)
	{
		if(desired_speed == ref_speed)
		{
			return 1;
		}
	}
	return 0;
}
uint32_t vfd_get_accelerating_stage(void)
{
	return acc_dec;
}

void vfd_scurve_exe(void)
{
	if (vfd_states_allowedtomove() == 0)
	{
		desired_speed = 0;
		//desired_distance = 0;
	}
	else if(speed_ref_stage == 1)
	{
		//acceleration / deceleration stage
		fixedpt t = fixedpt_rconst(0.001) * time_tick;
		//t = fixedpt_mul(t, fixedpt_rconst(0.01)); // get time lapse in seconds depending on the pwm freq
		scurve_exe(&scurve_speed, t, &desired_speed, &desired_distance, &c_acceleration, &speed_ref_stage);
		if (fixedpt_abs(desired_speed) < fixedpt_abs(ref_speed))
		{
			acc_dec = 1;
		}
		else if (fixedpt_abs(desired_speed) > fixedpt_abs(ref_speed))
		{
			acc_dec = 2;
		}
		if (speed_ref_stage == 0)
		{
			desired_speed = ref_speed;
			acc_dec = 0;
		}

		time_tick++;
	}	
}
#define SPEED_DEVIATION_TIME (500)
uint32_t speed_dev_timer_counter = 0;
#define	SPEED_DEVIATION		(menue_drive_data_applied.m_speed_dev)

float check_speed_deviation(float cmd_speed, float measured_speed, uint32_t fault_set)
{
	float base_speed = (menue_drive_data_applied.m_motor_rated_freq);
	base_speed /= FIXEDPT_ONE;
	base_speed /= MOTOR_POLEPAIR;
	base_speed *= M_TWO_PI;
	
	if (MOTOR_IM)
	{
		base_speed /= GB_REDUCTION_RATIO;
	}
	
	if (SPEED_DEVIATION == 0) return; //speed deviation error disabled
	float dev = fixedpt_abs(((measured_speed - cmd_speed)/base_speed) * 100);
	if((dev >= SPEED_DEVIATION) && fault_set) 
	{
		if(speed_dev_timer_counter < SPEED_DEVIATION_TIME)
			speed_dev_timer_counter++;
		else
			speedDev_set; //todo dev_fault
	}
	else
	{
		if (speed_dev_timer_counter > 0) 
		{
			speed_dev_timer_counter--;
		}
	}
	return dev;
}

void regulators_init(void)
{
	vector_PM_init();
	vector_IM_init();
	speed_PI_init();
}

float cmd_tourque = 0;

char c_decay_f = 0;
float current_decay_step = 0;
float flux_ratio = 0, flux_rat_step = 0;
float flux_rat_raise_step = 0;
#define CURRENT_DECAY_STEP_VAL current_decay_step

uint32_t flux_raise_permission = 0;

void vfd_set_c_decay(uint32_t decay_period)
{
	current_decay_step = -cmd_tourque / decay_period;
	flux_rat_step = decay_period;
	flux_rat_step = -1.0 / flux_rat_step;
	speed_PI_init();
	c_decay_f = 1;
	flux_raise_permission = 0;
}


void vfd_set_flux_raise(uint32_t raise_period)
{
	if (raise_period > 0)
	{
		flux_rat_raise_step = raise_period;
		flux_rat_raise_step = 1.0 / flux_rat_raise_step;
		flux_ratio = 0;
		
		flux_raise_permission = 1;
	} 
	else
	{
		flux_ratio = 0;
	}
}

void vfd_clear_c_decay(void)
{
	c_decay_f = 0;
}

void vfd_current_decay_exe(void)
{
	cmd_tourque += current_decay_step;
	if (flux_ratio > 0) flux_ratio += flux_rat_step;
	if(flux_ratio < 0) flux_ratio = 0;
}

void vfd_flux_raise_exe(void)
{
	if (flux_raise_permission == 0) return;
	
	if (flux_ratio < 1)
	{
		flux_ratio += flux_rat_raise_step;
	}
	else flux_ratio = 1;
	
}

uint8_t l_dir_delay_counter = 0;
int32_t vfd_get_l_dir(void)
{
	// use it with PM CVC only
	if(vfd_states_allowedtomove())
	{
		if (l_dir_delay_counter > 5)
		{
			if(cmd_tourque < 0) return 1;
			else return -1;
		}
		else l_dir_delay_counter++;
	}
	else l_dir_delay_counter = 0;
	return 0;
}

#define REGEN_FILTER_HALF (25)
#define REGEN_FILTER	  (2 * REGEN_FILTER_HALF)
uint8_t regen_filter_counter = 0;
void vfd_set_regencount(void)
{
	if (regen_filter_counter < REGEN_FILTER) regen_filter_counter++;
}

void vfd_clr_regencount(void)
{
	if (regen_filter_counter > 0) regen_filter_counter--;
}

uint32_t vfd_is_motor_regen(void)
{
	if (regen_filter_counter >= REGEN_FILTER_HALF)
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}

void vfd_activate_reg_reset(void)
{
	vectorIM_activate_reg_reset();
	vectorPM_activate_reg_reset();
}

void vfd_speed_loop_handler(void)
{
	if(vfd_states_isTuning())
	{
		return;
	}
	vfd_scurve_exe();
	float cmd_speed, act_speed, sp_dev = 0;
	cmd_speed = (desired_speed);
	cmd_speed /= FIXEDPT_ONE;
	cmd_speed /= MOTOR_POLEPAIR;
	cmd_speed *= M_TWO_PI;
	
	//if(!vfd_states_allowedtomove() && !vfd_states_isStopping()) speed_PI_init(); // reset speed pi during vfd stop
	if (MOTOR_IM_CVC)
	{
		//closed loop vector IM
		//cmd_speed /= GB_REDUCTION_RATIO;
		act_speed = enc_get_omega_shaft_rad_hat();
		//cmd_speed /= GB_REDUCTION_RATIO;
		sp_dev = check_speed_deviation(cmd_speed, act_speed, vfd_states_allowedtomove());
		if(c_decay_f) vfd_current_decay_exe();
		else
		{
			vfd_flux_raise_exe();
			cmd_tourque = speed_regulate(cmd_speed, act_speed, speed_ref_stage);
		}
	}
	else if (MOTOR_PM_CVC)
	{
		//closed loop vector PMSM
		act_speed = enc_get_omega_shaft_rad_hat();
		sp_dev = check_speed_deviation(cmd_speed, act_speed, vfd_states_allowedtomove());
		//cmd_tourque = 0.2;
		//return;
		if(c_decay_f) vfd_current_decay_exe();
		else cmd_tourque = speed_regulate(cmd_speed, act_speed, speed_ref_stage);
		
	}
	else if (MOTOR_IM_OVC)
	{
		//Open loop vector IM

		act_speed = vectorIM_get_estimated_shaft_speed_rad();
		sp_dev = check_speed_deviation(cmd_speed, act_speed, vfd_states_allowedtomove());
		if(c_decay_f) vfd_current_decay_exe();
		else
		{
			vfd_flux_raise_exe();
			cmd_tourque = speed_regulate(cmd_speed, act_speed, speed_ref_stage);
		}
	}
}

void vfd_tune_current_loop_handler(void)
{
	if (motor_excitation_is_motor_connected())
	{
		if (IM_PARAM_TUNE)
		{
			IM_param_tune1_current_handler();
		}
		else if (PM_PARAM_TUNE)
		{
			PM_param_tune2_current_handler();
		}
		else if(SPM_ENCODER_TUNE_STATIC)
		{
			SPMstatic_tune_current_handler();
		}
	}
}

void vfd_current_loop_handler(void)
{
	if (vfd_states_is_sctest_mode()) 
	{
		vector_PM_init();
		vector_IM_init();
		return;
	}
	
	if(vfd_states_isTuning())
	{
		vfd_tune_current_loop_handler();
		return;
	}
	
	if (MOTOR_IM_CVC)
	{
		//closed loop vector IM
		vectorIM_perform(cmd_tourque, flux_ratio);
	} 
	else if (MOTOR_PM_CVC)
	{
		//closed loop vector PMSM
		vectorPM_perform(cmd_tourque);
	}
	else if (MOTOR_IM_OVC)
	{
		//Open loop vector IM
		vectorIM_perform(cmd_tourque, flux_ratio);
	}
	else if (MOTOR_PM_OVC)
	{
		//Open loop vector PMSM (not supported)
		// issue fault
		control_fault_set;
	}
	else
	{
		//V/F control
		if (ref_speed == 0)
		{
			vf_speed_out_handler(desired_speed, 1); //braking
		}
		else
		{
			vf_speed_out_handler(desired_speed, 0); // don't apply braking current
		}
	}
}

void vfd_set_motion_dynamics(motion_dynamics_t motion_profile)
{
	m_dynamics_settings.a = motion_profile.a;
	m_dynamics_settings.j1 = motion_profile.j1;
	m_dynamics_settings.j2 = motion_profile.j2;
}

void vfd_set_motion_acc(fixedpt acc)
{
	m_dynamics_settings.a = acc;
}

void vfd_set_motion_j1(fixedpt j1)
{
	m_dynamics_settings.j1 = j1;
}

void vfd_set_motion_j2(fixedpt j2)
{
	m_dynamics_settings.j2 = j2;
}

void vfd_set_accel_profile(fixedpt speed)
{
	ref_speed = speed;
	
	
	//generate curve details
	scurve_apply_precalc_accel_profile();
	time_tick = 0;
	speed_ref_stage = 1;
}

void vfd_set_decel_profile(int32_t sgn)
{
	ref_speed = sgn * CRPSPEED;
	
	
	//generate curve details
	scurve_apply_precalc_decel_profile();
	time_tick = 0;
	speed_ref_stage = 1;
}

void vfd_set_decel_profilez(void)
{
	ref_speed = 0;
	
	
	//generate curve details
	scurve_apply_precalc_decel_profilez();
	time_tick = 0;
	speed_ref_stage = 1;
}

void vfd_set_ref_speed(fixedpt speed)
{
	ref_speed = speed;
// 	if (fixedpt_abs( desired_speed - speed) <= fixedpt_mul(fixedpt_rconst(0.02),fixedpt_rconst(50)))
// 	{
// 		desired_speed = speed;
// 		return;
// 	}
	
	// generate curve abs values
	motion_dynamics_t m_dynamics;
	
	if(m_dynamics_settings.a == 0)  m_dynamics_settings.a  = fixedpt_rconst(2.5);
	if(m_dynamics_settings.j1 == 0) m_dynamics_settings.j1 = fixedpt_rconst(0.1);
	if(m_dynamics_settings.j2 == 0) m_dynamics_settings.j2 = fixedpt_rconst(0.1);
	
	m_dynamics = scurve_get_abs_from_relative(m_dynamics_settings);	
	
	//generate curve details
	scurve_generate_speed_profile(desired_speed, speed, c_acceleration, m_dynamics, &scurve_speed);
	//scurve_generate_speed_profile(desired_speed, speed, c_acceleration, m_dynamics, &scurve_speed);
	time_tick = 0;
	speed_ref_stage = 1;
}


int32_t vfd_get_Max_possible_speed(int64_t int_displacement, fixedpt *r_speed, int32_t *stopping_displacement)
{
	motion_dynamics_t acc_motion, dec_motion, dec_motionz;
	int32_t sign = 1;
	int64_t enc_count;
	fixedpt displacement, stp_displacement;
	int_displacement = fixedpt_fromint(int_displacement) / ((ENCODER_COUNTREV) / MOTOR_POLEPAIR);
	if(int_displacement < 0) int_displacement += CREEP_DISTANCE;
	else if(int_displacement > 0) int_displacement -= CREEP_DISTANCE;
	else return 0; //un-realizable distance
	
	if(int_displacement < 0) sign = -1;
	
	if(fixedpt_abs(int_displacement) > 0x7FFFFFFF) int_displacement = sign * 0x7FFFFFFF;
	
	displacement = int_displacement;
	
	acc_motion.a = MAXACC;
	acc_motion.j1 = J1/100;
	acc_motion.j2 = J2/100;
	
	dec_motion.a = MAXDEC;
	dec_motion.j1 = J1/100;
	dec_motion.j2 = J2/100;
	
	dec_motionz.a = MAXDEC;
	dec_motionz.j1 = J1/100;
	dec_motionz.j2 = J3/100;
	
	if(displacement < 0) sign = -1;
	enc_count = scurve_get_maximumSpeed_fromDisplacement(displacement, sign * MAXSPEED, desired_speed,
			c_acceleration, acc_motion, dec_motion, dec_motionz, sign * CRPSPEED, r_speed, &stp_displacement) ;
	enc_count *= ((ENCODER_COUNTREV) / MOTOR_POLEPAIR);
	*stopping_displacement = fixedpt_toint(stp_displacement * ((ENCODER_COUNTREV) / MOTOR_POLEPAIR));
	return fixedpt_toint(enc_count + (sign * CREEP_DISTANCE_m * ENCODER_COUNTREV)) + *stopping_displacement;
}

void vfd_estop(void)
{
	//protective function to immediately stop the motor and release the break.
	vfd_states_estop();
	ref_speed = 0;
	desired_speed = 0;
	speed_ref_stage =0;
	c_acceleration = 0;
	time_tick = 0;
}
void vfd_trip(void)
{
	//protective function to immediately stop the motor and release the break.
	//enter trip state
	if(vfd_states_initialized()) vfd_states_trip();
	ref_speed=0;
	desired_speed=0;
	speed_ref_stage =0;
	c_acceleration = 0;
	time_tick = 0;
}