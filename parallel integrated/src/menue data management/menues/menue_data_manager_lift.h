/*
 * menue_data_manager_lift.h
 *
 * Created: 29/04/2017 11:30:50 ص
 *  Author: ahmed
 */ 


#ifndef MENUE_DATA_MANAGER_LIFT_H_
#define MENUE_DATA_MANAGER_LIFT_H_

#include <asf.h>
#include <fixedptc.h>

#define DIRECT_STOP (menue_lift_data_applied.m_direct_stop)

typedef struct
{
	uint8_t m_display_type;
	uint8_t m_door_type;
	uint8_t m_collection;
	uint8_t m_slow_time;
	uint8_t m_fast_time;
	uint8_t m_cam_time;
	uint8_t m_stop_wait_time;
	uint8_t m_light_time;	
	uint8_t m_cam_trials;
	uint8_t m_req_pin_config;
	uint8_t m_floor_count;
	uint8_t m_up_dir;
	uint8_t m_parking_floor;
	uint16_t m_unit_out;
	uint16_t m_parking_time;
	uint16_t m_up_stop_delay;
	uint16_t m_down_stop_delay;
	uint16_t m_ers_toff_delay;
	uint16_t m_ers_stop_delay;
	uint16_t m_ers_ldir_delay;
	uint16_t m_ers_ldir_test_time;
	uint16_t m_ers_stop_time;
	uint32_t m_req_int_dis;
	uint32_t m_req_ext_dis; 
/////////////////////////////// curve data //////////////////////////////////
	fixedpt m_high_speed;
	fixedpt m_medium_speed;
	fixedpt m_creep_speed;
	fixedpt m_relevel_speed;
	fixedpt m_ins_speed;
	fixedpt m_pre_speed;
	uint16_t m_pre_spd_time;
	fixedpt m_acc;
	fixedpt m_dec;
	fixedpt m_ins_acc;
	fixedpt m_ins_dec;
	fixedpt m_ins_corner;
	fixedpt m_corner1;
	fixedpt m_corner2;
	fixedpt m_corner3;
	uint8_t m_ins_stopping_mode;
	fixedpt m_ARDspeed;
	fixedpt m_ARDAccDec;
	fixedpt m_ARDJerk;
///////////////////////////////HoistWay info//////////////////////////////////
	fixedpt m_lift_speed;
	uint8_t m_stop_plate_length;
	fixedpt m_creep_distance;
	uint8_t m_direct_stop;
///////////////////////////////hoist addressing//////////////////////////////////
	uint8_t m_hoist[25];
///////////////////////////////Floor display///////////////////////////////////////////
	uint8_t floor_display[24];
///////////////////////////////Outputs///////////////////////////////////////////
	uint8_t m_outputs[16];
///////////////////////////////device off///////////////////////////////////////////
	uint8_t m_cont_dis_technique;
	uint8_t day;
	uint8_t month;
	uint8_t hour;	
	uint8_t minute;
	uint8_t second;
	uint32_t year;
	uint32_t services;
///////////////////////////////Set Password///////////////////////////////////////////
uint16_t set_password;
}menue_lift_data_t;

extern menue_lift_data_t menue_lift_data_applied;
extern menue_lift_data_t menue_lift_data_setup;
extern uint8_t menue_lift_data_newdata_flag;
extern uint8_t menue_lift_data_startSelfLrn;


void menue_data_manager_lift_update(void);
uint32_t menue_data_manager_lift_load(uint32_t offset_add);
uint32_t menue_data_manager_lift_save(uint32_t offset_add);
void menue_data_manager_lift_restore(void);
uint32_t menue_data_manager_lift_getSize(uint32_t offset_add);
uint32_t menue_data_manager_lift_Hinfo_save(uint32_t offset_add);
uint32_t menue_data_manager_lift_Hinfo_load(uint32_t offset_add);

#endif /* MENUE_DATA_MANAGER_LIFT_H_ */