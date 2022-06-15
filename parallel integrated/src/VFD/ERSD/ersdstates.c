/*
 * ersdstates.c
 *
 * Created: 24/11/2017 11:20:31 ص
 *  Author: ahmed
 */ 
#include <vfd.h>
#include <ersdstates.h>
#include <SEDACIO.h>
#include <Inverter.h>
#include <menue_data_manager.h>
#include <axistransform.h>
#include <control_status_register.h>

#define ERSD_SPEED menue_lift_data_applied.m_ARDspeed
#define ERSD_ACC   menue_lift_data_applied.m_ARDAccDec
#define ERSD_J     menue_lift_data_applied.m_ARDJerk
#define ERSD_LDIR_TEST_TIME	 (2000 + 1000 * menue_lift_data_applied.m_ers_ldir_test_time)
#define ERSD_LDIR_WAIT_TIME	 (0 + 1000 * menue_lift_data_applied.m_ers_ldir_delay)
#define ERSD_EXIT_AUTO_DELAY (1500)
#define ERSD_TURNING_OFF_DELAY (1000 * menue_lift_data_applied.m_ers_toff_delay)

uint8_t turnoff_battery_mode = 0;
#define ERSD_TURNING_OFF_SIG ((ersin == 0)||(turnoff_battery_mode == 1))
#define ERSD_TURNING_ON_SIG (!ERSD_TURNING_OFF_SIG)

void ersd_force_turn_off()
{
	turnoff_battery_mode = 1;
}

void ersd_allowTo_turn_on()
{
	turnoff_battery_mode = 0;
}

void (*ersdstate_ptr)(void) = 0;

void ersdstates_init_exe(void);
void ersdstates_off_exe(void);
void ersdstates_find_ldir_exe(void);
void ersdstates_idle_enter(void);
void ersdstates_idle_exit(void);
void ersdstates_idle_exe(void);
void ersdstates_off_enter(void);
void ersdstates_move_exe(void);
void ersdstates_move_enter(void);
void ersdstates_turningOff_exe(void);
void ersdstates_turningOff_enter(void);

uint32_t ersd_init_delay_counter = 0;
#define  INIT_DELAY (5000)
void ersdstates_init_enter(void)
{
	ersdstate_ptr = &ersdstates_init_exe;	
	ersd_init_delay_counter = 0;
	
}
void ersdstates_init_exit(void)
{
	vfd_states_set_softcharge_mode(0); //clear softcharge
	ersdstates_idle_enter();
	if(menue_drive_data_applied.m_ard_type == 0)
		inverter_set_manual_dcVal(fixedpt_rconst(48));
	else 
		inverter_set_manual_dcVal(fixedpt_rconst(0));	
}
void ersdstates_init_exe(void)
{
	if(ersd_init_delay_counter++ > INIT_DELAY)
	{
		ersdstates_init_exit();
	}
}

int8_t ersd_ldir = 1, stage = 0;
uint32_t ersd_test_ldir_counter = 0;
fixedpt ard_motor_current;

void ers_set_dirup(int8_t speed_sign)
{
	if ((menue_lift_data_applied.m_up_dir == 0) && (speed_sign > 0))
	{
		direction_up = 1;
	}
	else if ((menue_lift_data_applied.m_up_dir == 1) && (speed_sign < 0))
	{
		direction_up = 1;
	}
	else
	{
		direction_up = 0;
	}
}
void ersdstates_find_ldir_enter(void)
{
	ersdstate_ptr = &ersdstates_find_ldir_exe;
	ersd_ldir = 1;
	stage = 0;
	
	vfd_set_motion_acc (ERSD_ACC);
	vfd_set_motion_j1  (ERSD_J);
	vfd_set_motion_j2  (ERSD_J);
	
	if(MOTOR_PM_CVC) vfd_set_ref_speed(ersd_ldir * fixedpt_rconst(0.0005));
	else vfd_set_ref_speed(ersd_ldir * ERSD_SPEED);
	ers_set_dirup(ersd_ldir);
	
}
void ersdstates_find_ldir_exit(void)
{

		
}
void ersdstates_find_ldir_exe(void)
{
	uint32_t stage_time;
	
	if (MOTOR_PM_CVC)
	{
		//PMSM motor L dir detection
		ersd_ldir = vfd_get_l_dir();
		if (ersd_ldir)
		{
			// l dir found
			vfd_set_ref_speed(ersd_ldir * ERSD_SPEED);
			ers_set_dirup(ersd_ldir);
			ersdstates_move_enter();
		}
		return;
	}
	if ((stage == 1) || (stage == 3))
	{
		//wait time
		stage_time = ERSD_LDIR_WAIT_TIME;
	} 
	else
	{
		//test time
		stage_time = ERSD_LDIR_TEST_TIME;
	}
	
	
	if (++ersd_test_ldir_counter > stage_time)
	{
		if (stage == 0)
		{
			//first direction tested
			stage ++;
			ard_motor_current = axt_get_current_avg(); // get current val to be tested later
			vfd_estop();
		}
		else if(stage == 1)
		{
			//second direction test
			ersd_test_ldir_counter = 0;
			ersd_ldir = -1;
			stage++;
			
			vfd_set_motion_acc (ERSD_ACC);
			vfd_set_motion_j1  (ERSD_J);
			vfd_set_motion_j2  (ERSD_J);
			
			vfd_set_ref_speed(ersd_ldir * ERSD_SPEED);
			ers_set_dirup(ersd_ldir);
		}
		else if(stage == 2)
		{
			//second direction tested
			if(ard_motor_current > axt_get_current_avg())
			{
				//second dir is preferred
				ersdstates_move_enter();
				return;
			}
			else
			{
				// first direction is preferred
				stage++;
				vfd_estop();
				ersd_ldir = 1;
				ersd_test_ldir_counter = 0;
				ers_set_dirup(ersd_ldir);
			}
		}
		else 
		{
			//return to first direction
			vfd_set_motion_acc (ERSD_ACC);
			vfd_set_motion_j1  (ERSD_J);
			vfd_set_motion_j2  (ERSD_J);
			
			vfd_set_ref_speed(ersd_ldir * ERSD_SPEED);
			ersdstates_move_enter();
			return;
		}
	}
}

void ersdstates_move_enter(void)
{
	ersdstate_ptr = &ersdstates_move_exe;

}
void ersdstates_move_exit(void)
{

}
void ersdstates_move_exe(void)
{

}

void ersdstates_idle_enter(void)
{
	ersdstate_ptr = &ersdstates_idle_exe;
	
}
void ersdstates_idle_exit(void)
{
	if(ERSD_TURNING_OFF_SIG) ersdstates_turningOff_enter();
}
void ersdstates_idle_exe(void)
{
	ersdstates_idle_exit();
}

// a delay state to turnoff battery mode
uint32_t ersd_turningoff_counter = 0;
void ersdstates_turningOff_enter(void)
{
	ersdstate_ptr = &ersdstates_turningOff_exe;
	if(vfd_states_initialized())
	{
		vfd_states_set_softcharge_mode(1); //set softcharge
		vfd_trip();
	}
	ersd_turningoff_counter = 0;
}
void ersdstates_turningOff_exit(void)
{
	vfd_states_set_softcharge_mode(0); //clear softcharge
	ersdstates_off_enter();
}
void ersdstates_turningOff_exe(void)
{
	if(++ersd_turningoff_counter == (ERSD_TURNING_OFF_DELAY/4))
	{
		m_ers(0);
	}
	else if(ersd_turningoff_counter > (ERSD_TURNING_OFF_DELAY))
	{
		ersdstates_turningOff_exit();
	}
}
uint8_t prev_ersin = 0;
uint8_t ersd_exitingAuto = 0;
uint32_t ersd_exit_delay_counter = 0;
void ersdstates_off_enter(void)
{
	ersdstate_ptr = &ersdstates_off_exe;
	inverter_ignore_uv(0);
	
	inverter_set_manual_dcVal(0); // set the DC bus to its original val
	ersd_exit_delay_counter = 0;
	ersd_exitingAuto = 0;
	prev_ersin = 0;
	m_ers(0);
}


void ersdstates_off_exit(void)
{
	
	if (ersd_exitingAuto == 0)
	{
		if(vfd_states_initialized())
		{
			vfd_trip();
		}
		vfd_states_set_softcharge_mode(1); //set softcharge
		inverter_ignore_uv(1);
		
		ersd_exitingAuto = 1;
		ersd_exit_delay_counter = 0;
	}
	else
	{
		if(++ersd_exit_delay_counter > ERSD_EXIT_AUTO_DELAY)
		{
			m_ers(1);
			ersdstates_init_enter();
		}
	}
	
}
void ersdstates_off_exe(void)
{

	if(ERSD_TURNING_ON_SIG) 
	{
		ersdstates_off_exit();
		prev_ersin = 1;
	}
	else if(prev_ersin == 1) ersdstates_turningOff_enter();
	else ersd_exitingAuto = 0;
}

uint32_t ersd_isOff(void)
{
	if(ersdstate_ptr == &ersdstates_off_exe) return 1;
	return 0;
}

uint32_t ersd_isReady(void)
{
	
	if(ersdstate_ptr == &ersdstates_idle_exe)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}
void ersd_move(void)
{
	if(ersd_isReady) ersdstates_find_ldir_enter();
}
void ersd_stop(void)
{
	if(ersdstate_ptr == &ersdstates_init_exe) return;
	if(ersdstate_ptr == &ersdstates_off_exe) return;
	if(ersdstate_ptr == 0) return;
	ersdstates_idle_enter();
}

void ersdstates_handler(void)
{
	if(ersdstate_ptr == 0) 
		ersdstates_off_enter();
	(*ersdstate_ptr)();
}