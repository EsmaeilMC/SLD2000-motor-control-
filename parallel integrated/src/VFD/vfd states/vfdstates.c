/*
 * vfdstates.c
 *
 * Created: 05/03/2017 07:47:24 م
 *  Author: ahmed
 */ 

#include <vfd.h>
#include <vfdstates.h>
#include <Inverter.h>
#include <pwm_interface.h>
#include <adc_interface.h>
#include <d_faults.h>
#include <axistransform.h>
#include <scCheck.h>
#include <speed_regulator.h>
#include <SPMstatic_tune.h>
#include <PM_param_tune1.h>
#include <IM_param_tune1.h>
#include <PM_param_tune2.h>
#include <VectorPM.h>
#include <motor_excitation.h>
#include <SPT.h>
#include <e_faults.h>


#define MOTOR_TUNE		(menue_drive_motor_tune)
#define ENCODER_TUNE	(menue_drive_encoder_tune)

void vfd_states_init_exe(void);
void vfd_states_active_exe(void);
void vfd_states_stop_exe(void);
void vfd_states_trip_exe(void);
void vfd_states_stop_enter(void);
void vfd_states_active_enter(void);
void vfd_states_csc_exe(void);
void vfd_states_stop_exit(void);
void vfd_states_tune_exe(void);
void vfd_states_tune_enter(void);


uint8_t softcharge = 0;
void vfd_states_set_softcharge_mode(uint32_t mode)
{
	if(mode) 
	{
		softcharge = 1;
		SCRELAY_Off;
	}
	else
	{
		softcharge = 0;
	}	
}

uint32_t vfd_states_faults(void);

void (*vfdstate_ptr)(void) = 0;

uint8_t vfdstates_get_d_state(void)
{
	if (vfdstate_ptr == &vfd_states_init_exe)
	{
		return 0;
	}
	else if (vfdstate_ptr == &vfd_states_active_exe)
	{
		return 1;
	}
	else if (vfdstate_ptr == &vfd_states_stop_exe)
	{
		return 2;
	}
	else if (vfdstate_ptr == &vfd_states_trip_exe)
	{
		return 3;
	}
	else if (vfdstate_ptr == &vfd_states_csc_exe)
	{
		return 4;
	}
	else if (vfdstate_ptr == &vfd_states_tune_exe)
	{
		return 5;
	}
	else
	{
		return 6;
	}
}

unsigned int vfd_states_allowedtomove(void)
{
	if(vfdstate_ptr == &vfd_states_active_exe) return 1;
	else return 0;
}

unsigned int vfd_states_allowedtostop(void)
{
	if(vfdstate_ptr == &vfd_states_trip_exe) return 0;
	if(vfdstate_ptr == &vfd_states_stop_exe) return 0;
	else return 1;
}

unsigned int vfd_states_isTuning(void)
{
	if(vfdstate_ptr == &vfd_states_tune_exe) return 1;
	else return 0;
}

unsigned int vfd_states_initialized(void)
{
	if(vfdstate_ptr == &vfd_states_init_exe) return 0;
	else if(vfdstate_ptr == 0) return 0;
	else return 1;
}

char sc_testing = 0;
unsigned int vfd_states_is_sctest_mode(void)
{
	if((vfdstate_ptr == &vfd_states_csc_exe) && (sc_testing)) return 1;
	else return 0;	
}

uint32_t init_delay_counter = 0;
#define  INIT_DELAY 3000
void vfd_states_init_enter(void)
{
	vfdstate_ptr = &vfd_states_init_exe;
	pwm_interface_init();
	adc_interface_init();
	
	init_delay_counter = 0;	
}

void vfd_states_init_exe(void)
{
	if(init_delay_counter++ == (INIT_DELAY >> 3))
	{
		inverter_set_override();
		vfd_activate_reg_reset();
		pwm_interface_enablesynchchannels();	
	}
	else if(init_delay_counter > INIT_DELAY)
	{
		//inverter_rewind_dcbus_f_val();
		vfd_states_stop_enter();
		inverter_init_dc_filter();
	}
}
void vfd_states_init_exit(void)
{
	
}

#define VFD_STATES_DCBREAK_DELAY			menue_drive_data_applied.m_dcbreak_delay
#define VFD_STATES_BREAK_RELEASE_DELAY		menue_drive_data_applied.m_brake_release_delay
#define VFD_STATES_BREAK_HOLD_DELAY			menue_drive_data_applied.m_brake_hold_delay
#define VFD_STATES_MOTOR_MOVE_DELAY			menue_drive_data_applied.m_motor_move_delay
#define VFD_STATES_TOURQUE_DECAY			menue_drive_data_applied.m_tourque_decay

void vfd_states_csc_enter(void)
{
	if (vfd_get_ref_speed() != 0)
	{
		vfdstate_ptr = &vfd_states_csc_exe;
		regulators_init();
		sc_testing = 1;
		scCheck_init();
		inverter_fan_timer_reset();
	}
}
void vfd_states_csc_exit(void)
{
	if (vfd_get_ref_speed() != 0)
	{
		inverter_clear_override();
		if (scCheck_exe())
		{
			sc_testing = 0;
// 			inverter_set_override();
// 			vfd_states_trip();
// 			sleep_err_set;
// 			return;
			SC_LEVEL_HIGH;
			if ((MOTOR_TUNE)||(ENCODER_TUNE))
			{
				vfd_states_tune_enter();
			} 
			else
			{
				vfd_states_stop_exit();
				//Final_limit_err;
				//vfd_states_stop_enter();
			}
		}
	}
	else
	{
		sc_testing = 0;
		vfd_states_stop_enter();
	}
}
void vfd_states_csc_exe(void)
{
	vfd_states_csc_exit();
}

uint32_t vfd_states_dcbreak_timer = 0;
uint32_t vfd_states_break_release_timer = 0;
uint32_t vfd_states_break_hold_timer = 0;
uint32_t vfd_states_motor_move_timer = 0;
uint32_t vfd_states_tourque_decay_timer = 0;


uint8_t is_stopping = 0;
uint8_t vfd_states_isStopping(void)
{
	if (is_stopping) return 1;
	return 0;
}
void vfd_states_stop_enter(void)
{
	vfdstate_ptr = &vfd_states_stop_exe;
	inverter_set_dead_comp(0);
	SCRELAY_On;
	vfd_states_break_hold_timer = 0;
	vfd_states_motor_move_timer = 0;
	is_stopping = 1;
	pwm_interface_enablesynchchannels();
	stop_zero_servo();
	if (vfd_get_desired_speed() != 0)
	{
		inverter_set_override();
		motor_excitation_disconnect_motor();
		vfd_activate_reg_reset();
		//break release
		BRKS_set(0);
		vfd_clear_c_decay();
	}
	else 
	{
		vfd_states_dcbreak_timer = 0;
		vfd_states_break_release_timer = 0;
		vfd_states_tourque_decay_timer = 0;
	}
}
void vfd_states_stop_exit(void)
{
	if (vfd_get_ref_speed() != 0)
	{
		motor_excitation_connect_motor();
		if (motor_excitation_is_motor_connected())
		{
			vfd_clear_c_decay();
			vfd_set_flux_raise(VFD_STATES_BREAK_HOLD_DELAY + VFD_STATES_MOTOR_MOVE_DELAY);
			
			if (vfd_states_break_hold_timer < VFD_STATES_BREAK_HOLD_DELAY)
			{
				vfd_states_break_hold_timer++;
				inverter_clear_override();
				if (MOTOR_PM_CVC) start_zero_servo();
			}
			else
			{
				//break timer time-out
				//break hold routine
				inverter_clear_override();
				if (MOTOR_PM_CVC) start_zero_servo();
				spt_vfd_set_ready_to_open_brake();
				
				if(spt_permission_brake_open())
				{
					BRKS_set(1);
					
					if (vfd_states_motor_move_timer < VFD_STATES_MOTOR_MOVE_DELAY)
					{
						vfd_states_motor_move_timer++;
					}
					else if (vfd_states_motor_move_timer == VFD_STATES_MOTOR_MOVE_DELAY)
					{
						vfd_states_motor_move_timer++;
						vfd_states_active_enter();
					}	
				}
				
			}
		}
	}
	else vfd_states_break_hold_timer = 0;	
}
void vfd_states_stop_exe(void)
{
	
	if (vfd_states_break_release_timer < VFD_STATES_BREAK_RELEASE_DELAY)
	{
		vfd_states_break_release_timer++;
	}
	else
	{
		//break timer time-out
		//break release routine
		BRKS_set(0);
		if (vfd_states_dcbreak_timer < VFD_STATES_DCBREAK_DELAY)
		{
			vfd_states_dcbreak_timer++;
		}
		else if (vfd_states_dcbreak_timer == VFD_STATES_DCBREAK_DELAY)
		{
			if ((menue_drive_data_applied.m_control_mode > 0) /*&& (menue_drive_data_applied.m_motor_type == 1)*/)
			{
				//closed loop vector PMSM
				if (vfd_states_tourque_decay_timer == 0)
				{
					vfd_set_c_decay(VFD_STATES_TOURQUE_DECAY);
					vfd_states_tourque_decay_timer++;
				}
				else if (vfd_states_tourque_decay_timer < VFD_STATES_TOURQUE_DECAY)
				{
					vfd_states_tourque_decay_timer++;
				}
				else
				{
					vfd_states_dcbreak_timer++;
					inverter_set_override();
					motor_excitation_disconnect_motor();
					vfd_activate_reg_reset();
					is_stopping = 0;
					vfd_clear_c_decay();
				}
			}
			else
			{
				vfd_states_dcbreak_timer++;
				inverter_set_override();
				motor_excitation_disconnect_motor();
				vfd_activate_reg_reset();
				is_stopping = 0;
			}
			
		}
	}
	vfd_states_csc_enter();	
	inverter_brake_chopper_handler();
}


void vfd_states_active_enter(void)
{
	inverter_fan_timer_reset();
	vfd_clear_c_decay();
	if(! vfd_enable_isEnabled()) vfd_states_stop_enter();
	is_stopping = 1;
	vfdstate_ptr = &vfd_states_active_exe;
	inverter_clear_override();
	vfd_states_break_hold_timer = 0;
	BRKS_set(1);
	axt_reset_current_avg();
	speed_PI_changeLSGains();
	vectorpm_PI_changeGains();	
}
void vfd_states_active_exit(void)
{
	if (vfd_get_ref_speed() == 0)
	{
		if(vfd_get_desired_speed() == 0) vfd_states_stop_enter();
	}	
}
void vfd_states_active_exe(void)
{
	if(! vfd_enable_isEnabled()) vfd_states_stop_enter();
	inverter_fan_timer_reset();
	inverter_brake_chopper_handler();
	vfd_states_active_exit();	
}


void vfd_states_trip_enter(void)
{
	vfdstate_ptr = &vfd_states_trip_exe;
	inverter_set_override();
	pwm_interface_enablesynchchannels();
	BRKS_set(0);
	motor_excitation_disconnect_motor();
	vfd_activate_reg_reset();
	vfd_clear_c_decay();
}
void vfd_states_trip_exit(void)
{
	if (!vfd_states_faults() && (!softcharge))
	{
		//todo conditions on exiting trip
		SCRELAY_On;
		vfd_states_stop_enter();
	}
		
}
void vfd_states_trip_exe(void)
{
	if (uv_err_pre)
	{
		//todo low voltage trip
		SCRELAY_Off;
	}
	inverter_brake_chopper_handler();
	vfd_states_trip_exit();	
}


uint32_t vfd_states_faults(void)
{
	if(ov_err_pre) return 1;
	if(uv_err_pre) return 1;
	if(oc_err_pre) return 1;
	if(ol_err_pre) return 1;
	if(ot_err_pre) return 1;
	if(sc_err_pre) return 1;
	return 0;	
}

void vfd_states_handler(void)
{
	if(vfdstate_ptr == 0) vfd_states_init_enter();
	(*vfdstate_ptr)();	
}

void vfd_states_estop(void)
{
	inverter_set_override();
	//todo break release routine
	BRKS_set(0);
	if(vfd_states_allowedtostop()) vfd_states_stop_enter();	
	is_stopping = 0;
	vfd_clear_c_decay();
}

void vfd_states_trip(void)
{
	inverter_set_override();
	//todo break release routine
	BRKS_set(0);
	vfd_states_trip_enter();
}

void vfd_states_tune_enter(void)
{
	if(! vfd_enable_isEnabled()) vfd_states_stop_enter();
	is_stopping = 0;

	inverter_clear_override();
	motor_excitation_connect_motor();
	axt_reset_current_avg();
	SPMtune_init();
	pm_param_tune1_init();
	PM_param_tune2_init();
	IM_param_tune1_init();
	
	vfdstate_ptr = &vfd_states_tune_exe;
}
void vfd_states_tune_exit(void)
{
	if (vfd_get_ref_speed() == 0)
	{
		if(vfd_get_desired_speed() == 0) 
		{
			vfd_states_stop_enter();
			MOTOR_TUNE = 0;
			ENCODER_TUNE = 0;
		}
	}
}
void vfd_states_tune_exe(void)
{
	if(! vfd_enable_isEnabled()) 
	{
		vfd_states_stop_enter();
		MOTOR_TUNE = 0;
		ENCODER_TUNE = 0;
	}
	inverter_brake_chopper_handler();
	vfd_states_tune_exit();
	inverter_fan_timer_reset();
}