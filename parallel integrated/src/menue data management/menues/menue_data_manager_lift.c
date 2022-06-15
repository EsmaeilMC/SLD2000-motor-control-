/*
 * menue_data_manager_lift.c
 *
 * Created: 29/04/2017 11:31:10 ص
 *  Author: ahmed
 */ 
#include <menue_data_manager_lift.h>
#include <string.h>
#include <AT24Cxx_dynamic.h>
#include <HoistInfo.h>

menue_lift_data_t menue_lift_data_applied;
menue_lift_data_t menue_lift_data_setup;
uint8_t menue_lift_data_newdata_flag = 0;
uint8_t menue_lift_data_startSelfLrn = 0;
#include <pop_ups.h>
 
void menue_data_manager_lift_update(void)
{
	memcpy(&menue_lift_data_applied, &menue_lift_data_setup, sizeof(menue_lift_data_t));	
}

uint32_t menue_data_manager_lift_load(uint32_t offset_add)
{
	//menue_data_manager_lift_restore();
	//menue_data_manager_lift_update();
	//return;
	at24cxx_dynamic_sch_read_array(0, offset_add, sizeof(menue_lift_data_t), &menue_lift_data_setup);
	return offset_add + sizeof(menue_lift_data_t); 
}

uint32_t menue_data_manager_lift_Hinfo_load(uint32_t offset_add)
{
	at24cxx_dynamic_sch_read_array(0, offset_add, sizeof(hoist_info_t), &hoist_info);
	return offset_add + sizeof(hoist_info_t);
}

uint32_t menue_data_manager_lift_save(uint32_t offset_add)
{
	at24cxx_dynamic_sch_write_array(0, offset_add, sizeof(menue_lift_data_t), &menue_lift_data_setup);
	return offset_add + sizeof(menue_lift_data_t);
}
uint32_t menue_data_manager_lift_save_nomsg(uint32_t offset_add)
{
	at24cxx_dynamic_sch_write_array(0, offset_add, sizeof(menue_lift_data_t), &menue_lift_data_setup);
	return offset_add + sizeof(menue_lift_data_t);
}

uint32_t menue_data_manager_lift_Hinfo_save(uint32_t offset_add)
{
	at24cxx_dynamic_sch_write_array(0, offset_add, sizeof(hoist_info_t), &hoist_info);
	return offset_add + sizeof(hoist_info_t);
}

uint32_t menue_data_manager_lift_getSize(uint32_t offset_add)
{
	return offset_add + sizeof(menue_lift_data_t);
}

void menue_data_manager_lift_restore(void)
{
	uint32_t i;
	menue_lift_data_setup.m_cam_time = 8;
	menue_lift_data_setup.m_cam_trials = 2;
	menue_lift_data_setup.m_collection = 1;
	menue_lift_data_setup.m_cont_dis_technique = 0;
	menue_lift_data_setup.m_display_type = 0;
	menue_lift_data_setup.m_door_type = 3;
	menue_lift_data_setup.m_down_stop_delay = 4;
	menue_lift_data_setup.m_ers_stop_delay = 4;
	menue_lift_data_setup.m_ers_stop_time = 4;
	menue_lift_data_setup.m_ers_toff_delay = 20;
	menue_lift_data_setup.m_ers_ldir_delay = 1;
	menue_lift_data_setup.m_ers_ldir_test_time = 1;
	menue_lift_data_setup.m_fast_time = 40;
	menue_lift_data_setup.m_floor_count = 6;
	menue_lift_data_setup.m_light_time = 15;
	menue_lift_data_setup.m_parking_time = 0;
	menue_lift_data_setup.m_req_ext_dis = 0xFFFFFFFF;
	menue_lift_data_setup.m_req_int_dis = 0xFFFFFFFF;
	menue_lift_data_setup.m_slow_time = 15;
	menue_lift_data_setup.m_stop_wait_time = 7;
	menue_lift_data_setup.m_unit_out = 0;
	menue_lift_data_setup.m_up_dir = 1;
	menue_lift_data_setup.m_up_stop_delay = 5;
	menue_lift_data_setup.m_req_pin_config = 1;
/////////////////////////////hoist info/////////////////////////////////////////////
	menue_lift_data_setup.m_lift_speed = fixedpt_rconst(1);
	menue_lift_data_setup.m_stop_plate_length = 150;
	menue_lift_data_setup.m_creep_distance = fixedpt_rconst(0.04);
	menue_lift_data_setup.m_direct_stop = 0;
//////////////////////////////////////////////////////////////////////////
	menue_lift_data_setup.m_high_speed = fixedpt_rconst(100.0);
	menue_lift_data_setup.m_medium_speed = fixedpt_rconst(60.0);
	menue_lift_data_setup.m_creep_speed = fixedpt_rconst(10.0);
	menue_lift_data_setup.m_relevel_speed = fixedpt_rconst(20.0);
	menue_lift_data_setup.m_ins_speed = fixedpt_rconst(30.0);
	menue_lift_data_setup.m_pre_speed = fixedpt_rconst(2.0);
	menue_lift_data_setup.m_pre_spd_time = 1000;
	menue_lift_data_setup.m_acc = fixedpt_rconst(1.8);
	menue_lift_data_setup.m_dec = fixedpt_rconst(2.5);	
	menue_lift_data_setup.m_ins_acc = fixedpt_rconst(1.8);
	menue_lift_data_setup.m_ins_dec = fixedpt_rconst(0.8);

	menue_lift_data_setup.m_ins_corner = fixedpt_rconst(20.0);
	menue_lift_data_setup.m_corner1 = fixedpt_rconst(20.0);
	menue_lift_data_setup.m_corner2 = fixedpt_rconst(20.0);
	menue_lift_data_setup.m_corner3 = fixedpt_rconst(20.0);
	menue_lift_data_setup.m_ins_stopping_mode = 1;
	
	menue_lift_data_setup.m_ARDspeed = fixedpt_rconst(3.0);
	menue_lift_data_setup.m_ARDAccDec = fixedpt_rconst(1.0);
	menue_lift_data_setup.m_ARDJerk = fixedpt_rconst(0.0);

	menue_lift_data_setup.set_password = 0;
/////////////////////////////Turn off/////////////////////////////////////////////
	menue_lift_data_setup.m_cont_dis_technique = 0;
	menue_lift_data_setup.services = 2;
	menue_lift_data_setup.year = 2019;
	menue_lift_data_setup.month = 1;
	menue_lift_data_setup.day = 1;
	menue_lift_data_setup.hour = 0;
	menue_lift_data_setup.minute = 0;
	menue_lift_data_setup.second = 0;
//////////////////////////////////////////////////////////////////////////
	for (i=0; i<13; i++)
	{
		//hoist addressing
		menue_lift_data_setup.m_hoist[i] = i;
	}
	for (i=13; i<25; i++)
	{
		//hoist addressing
		menue_lift_data_setup.m_hoist[i] = 30;
	}
	
	for (i=0; i<24; i++)
	{
		//reset floor display
		menue_lift_data_setup.floor_display[i] = i;
	}
	for (i=0; i<8; i++)
	{
		menue_lift_data_setup.m_outputs[i] = i;
	}
	menue_lift_data_setup.m_outputs[0] = 7; //brake
	menue_lift_data_setup.m_outputs[2] = 0;//light
	menue_lift_data_setup.m_outputs[7] = 2;//gong
	
	menue_lift_data_newdata_flag = 1;
	pop_ups_add("Lift set restored!", 1000);
}