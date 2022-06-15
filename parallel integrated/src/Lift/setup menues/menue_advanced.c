/*
 * menue_advanced.c
 *
 * Created: 02/05/2017 01:48:21 م
 *  Author: ahmed
 */ 
#include <menue_advanced.h>
#include <menue_data_manager.h>
//advanced list

menue_design_t menue_advanced;
#define OPT_ADVANCED_MAX 5
option_t opt_advanced[OPT_ADVANCED_MAX];

#define ladvanced_cam_trials					(opt_advanced[0])
#define ladvanced_parking_floor					(opt_advanced[1])
#define ladvanced_req_pins						(opt_advanced[2])
#define ladvanced_controller_dis_tech	        (opt_advanced[3])
#define ladvanced_controller_dis_val		    (opt_advanced[4])
//#define ladvanced_service_phone					(opt_advanced[4])

option_val_t oval_ladvanced_cam_trials;
option_val_t oval_ladvanced_parking_floor;
option_val_t oval_ladvanced_controller_dis_val;

//__________________________________________________
//req pins list
menue_design_t menue_advanced_req_pins;
#define OPT_ADVANCED_REQ_PINS_MAX 3
option_t opt_advanced_req_pins[OPT_ADVANCED_REQ_PINS_MAX];

#define req_pin_3inputs				(opt_advanced_req_pins[0])
#define req_pin_2inputs				(opt_advanced_req_pins[1])
#define req_pin_1inputs				(opt_advanced_req_pins[2])
//__________________________________________________
//req pins list
menue_design_t menue_advanced_controller_dis_tech;
#define OPT_ADVANCED_CONTROLLER_DIS_TECH_MAX 3
option_t opt_advanced_controller_dis_tech[OPT_ADVANCED_CONTROLLER_DIS_TECH_MAX];

#define contr_dis_off					(opt_advanced_controller_dis_tech[0])
#define contr_dis_timeout				(opt_advanced_controller_dis_tech[2])
#define contr_dis_services				(opt_advanced_controller_dis_tech[1])

menue_design_t menue_date_time;
#define OPT_DATE_TIME_MAX 2
option_t opt_date_time[OPT_DATE_TIME_MAX];

#define ddate	          (opt_date_time[0])
#define dtime	          (opt_date_time[1])
void req_dis_off(void)
{
	ladvanced_controller_dis_val.text = "off";
	oval_ladvanced_controller_dis_val.data_types = uint16_dt;
	oval_ladvanced_controller_dis_val.mantissa_count = 0;
	oval_ladvanced_controller_dis_val.real_count = 3;
	oval_ladvanced_controller_dis_val.max_value = 999;
	oval_ladvanced_controller_dis_val.min_value = 1;
	oval_ladvanced_controller_dis_val.new_val_flag = &menue_lift_data_newdata_flag;
	//oval_ladvanced_controller_dis_val.unit = "Sec";
	//oval_ladvanced_controller_dis_val.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_ladvanced_controller_dis_val.val = &menue_lift_data_setup.m_unit_out;
	ladvanced_controller_dis_val.option_save_val = &oval_ladvanced_controller_dis_val;
	ladvanced_controller_dis_val.option_characteristics = OPTION_HIDDEN;
	ladvanced_controller_dis_val.child = 0;
}

void req_dis_servicess(void)
{
	ladvanced_controller_dis_val.text = "Servicess count";
	oval_ladvanced_controller_dis_val.data_types = uint16_dt;
	oval_ladvanced_controller_dis_val.mantissa_count = 0;
	oval_ladvanced_controller_dis_val.real_count = 4;
	oval_ladvanced_controller_dis_val.max_value = 9999;
	oval_ladvanced_controller_dis_val.min_value = 1;
	oval_ladvanced_controller_dis_val.new_val_flag = &menue_lift_data_newdata_flag;
	//oval_ladvanced_controller_dis_val.unit = "Sec";
	//oval_ladvanced_controller_dis_val.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_ladvanced_controller_dis_val.val = &menue_lift_data_setup.services;
	ladvanced_controller_dis_val.option_save_val = &oval_ladvanced_controller_dis_val;
	ladvanced_controller_dis_val.option_characteristics = 0;
	ladvanced_controller_dis_val.child = 0;
}
void req_dis_time(void)
{
	ladvanced_controller_dis_val.text = "Date/Time";
	ladvanced_controller_dis_val.option_save_val = 0;
	ladvanced_controller_dis_val.option_characteristics = 0;
	
	ddate.text = "Set date";
	ddate.option_save_val = 3; //set date option
	//////////////////////////////////////////////////////////////////////////
	
	dtime.text = "Set time";
	dtime.option_save_val = 4; //set time option
	//////////////////////////////////////////////////////////////////////////

	fill_menue_list(&menue_date_time, opt_date_time, OPT_DATE_TIME_MAX, 0, 0);

	ladvanced_controller_dis_val.child = &menue_date_time;
}
menue_design_t *menue_advanced_init(void)
{
	ladvanced_cam_trials.text = "CAM trials";
	oval_ladvanced_cam_trials.data_types = uint8_dt;
	oval_ladvanced_cam_trials.mantissa_count = 0;
	oval_ladvanced_cam_trials.real_count = 1;
	oval_ladvanced_cam_trials.max_value = 9;
	oval_ladvanced_cam_trials.min_value = 1;
	oval_ladvanced_cam_trials.new_val_flag = &menue_lift_data_newdata_flag;
	//oval_ladvanced_cam_trials.unit = "Sec";
	//oval_ladvanced_cam_trials.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_ladvanced_cam_trials.val = &menue_lift_data_setup.m_cam_trials;
	ladvanced_cam_trials.option_save_val = &oval_ladvanced_cam_trials;
//__________________________________________________
	ladvanced_parking_floor.text = "Parking floor";
	oval_ladvanced_parking_floor.data_types = uint8_dt;
	oval_ladvanced_parking_floor.mantissa_count = 0;
	oval_ladvanced_parking_floor.real_count = 2;
	oval_ladvanced_parking_floor.max_value = 23;
	oval_ladvanced_parking_floor.min_value = 0;
	oval_ladvanced_parking_floor.new_val_flag = &menue_lift_data_newdata_flag;
	//oval_ladvanced_parking_floor.unit = "Sec";
	//oval_ladvanced_parking_floor.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_ladvanced_parking_floor.val = &menue_lift_data_setup.m_parking_floor;
	ladvanced_parking_floor.option_save_val = &oval_ladvanced_parking_floor;
	//__________________________________________________
	ladvanced_req_pins.text = "Request Pins config";
	req_pin_3inputs.text = "3 req buttons";
	req_pin_2inputs.text = "2 req buttons";
	req_pin_1inputs.text = "2 shared req buttons";


	fill_menue_list(&menue_advanced_req_pins, &opt_advanced_req_pins, OPT_ADVANCED_REQ_PINS_MAX, 0, &menue_lift_data_setup.m_req_pin_config);

	// set the 'option ladvanced_req_pins' child

	ladvanced_req_pins.child = &menue_advanced_req_pins;
//__________________________________________________
	ladvanced_controller_dis_tech.text = "Req disable tech";
	contr_dis_off.text = "Off";
	contr_dis_off.enter = &req_dis_off;
	contr_dis_timeout.text = "Time disable";
	contr_dis_timeout.enter = &req_dis_time;
	contr_dis_services.text = "Services disable";
	contr_dis_services.enter = &req_dis_servicess;


	fill_menue_list(&menue_advanced_controller_dis_tech, &opt_advanced_controller_dis_tech, OPT_ADVANCED_CONTROLLER_DIS_TECH_MAX, 0, &menue_lift_data_setup.m_cont_dis_technique);

	// set the 'option ladvanced_req_pins' child

	ladvanced_controller_dis_tech.child = &menue_advanced_controller_dis_tech;
//__________________________________________________
	if(menue_lift_data_setup.m_cont_dis_technique == 1) req_dis_servicess();
	else if(menue_lift_data_setup.m_cont_dis_technique == 2) req_dis_time();
	else req_dis_off();
//__________________________________________________
	
	
	fill_menue_list(&menue_advanced, opt_advanced, OPT_ADVANCED_MAX, 0, 0);
	return &menue_advanced;
}