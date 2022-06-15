/*
 * menue_drive_brake.c
 *
 * Created: 10/06/2017 02:44:43 م
 *  Author: Ahmed
 */ 

#include <menue_drive_brake.h>
#include <menue_data_manager.h>

//vector control list

menue_design_t menue_driver_brake;
#define OPT_DRIVER_BRAKE_MAX 4
option_t opt_driver_brake[OPT_DRIVER_BRAKE_MAX];
		
#define dmotor_dcbreak_delay						(opt_driver_brake[0])
#define dmotor_brake_release_delay					(opt_driver_brake[1])
#define dmotor_brake_hold_delay						(opt_driver_brake[2])
#define dmotor_motor_move_delay						(opt_driver_brake[3])


option_val_t oval_dcbreak_delay;
option_val_t oval_brake_release_delay;
option_val_t oval_brake_hold_delay;
option_val_t oval_motor_move_delay;


menue_design_t *menue_driver_brake_init(void)
{

	dmotor_dcbreak_delay.text = "DC brake delay";
	oval_dcbreak_delay.data_types = uint16_dt;
	oval_dcbreak_delay.mantissa_count = 0;
	oval_dcbreak_delay.real_count = 4;
	oval_dcbreak_delay.max_value = 3000;
	oval_dcbreak_delay.min_value = 0.0;
	oval_dcbreak_delay.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dcbreak_delay.unit = "ms";
	//oval_dcbreak_delay.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dcbreak_delay.val = &menue_drive_data_setup.m_dcbreak_delay;
	dmotor_dcbreak_delay.option_save_val = &oval_dcbreak_delay;
	//__________________________________________________
	dmotor_brake_release_delay.text = "Brake release delay";
	oval_brake_release_delay.data_types = uint16_dt;
	oval_brake_release_delay.mantissa_count = 0;
	oval_brake_release_delay.real_count = 4;
	oval_brake_release_delay.max_value = 3000;
	oval_brake_release_delay.min_value = 0.0;
	oval_brake_release_delay.new_val_flag = &menue_drive_data_newdata_flag;
	oval_brake_release_delay.unit = "ms";
	//oval_brake_release_delay.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_brake_release_delay.val = &menue_drive_data_setup.m_brake_release_delay;
	dmotor_brake_release_delay.option_save_val = &oval_brake_release_delay;
	//__________________________________________________
	dmotor_brake_hold_delay.text = "Brake hold delay";
	oval_brake_hold_delay.data_types = uint16_dt;
	oval_brake_hold_delay.mantissa_count = 0;
	oval_brake_hold_delay.real_count = 4;
	oval_brake_hold_delay.max_value = 3000;
	oval_brake_hold_delay.min_value = 0.0;
	oval_brake_hold_delay.new_val_flag = &menue_drive_data_newdata_flag;
	oval_brake_hold_delay.unit = "ms";
	//oval_brake_hold_delay.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_brake_hold_delay.val = &menue_drive_data_setup.m_brake_hold_delay;
	dmotor_brake_hold_delay.option_save_val = &oval_brake_hold_delay;
	//__________________________________________________
	dmotor_motor_move_delay.text = "Motor move delay";
	oval_motor_move_delay.data_types = uint16_dt;
	oval_motor_move_delay.mantissa_count = 0;
	oval_motor_move_delay.real_count = 4;
	oval_motor_move_delay.max_value = 3000;
	oval_motor_move_delay.min_value = 0.0;
	oval_motor_move_delay.new_val_flag = &menue_drive_data_newdata_flag;
	oval_motor_move_delay.unit = "ms";
	//oval_motor_move_delay.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_motor_move_delay.val = &menue_drive_data_setup.m_motor_move_delay;
	dmotor_motor_move_delay.option_save_val = &oval_motor_move_delay;
	//__________________________________________________	
	fill_menue_list(&menue_driver_brake, opt_driver_brake, OPT_DRIVER_BRAKE_MAX, 0, 0);
	return &menue_driver_brake;
}