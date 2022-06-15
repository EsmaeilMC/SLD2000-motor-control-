/*
 * menue_data_manager_drive.c
 *
 * Created: 02/05/2017 03:12:40 م
 *  Author: ahmed
 */ 
#include <menue_data_manager_drive.h>
#include <string.h>
#include <AT24Cxx_dynamic.h>
#include <motor_data.h>
#include <pop_ups.h>
#include <incremental.h>

/*#define SIZE_TO_STORE 512*/
menue_drive_data_t menue_drive_data_applied;
menue_drive_data_t menue_drive_data_setup;
uint8_t menue_drive_data_newdata_flag;

uint8_t				menue_drive_encoder_tune  = 0;
uint8_t				menue_drive_motor_tune    = 0;

/*uint8_t test_arr_save[SIZE_TO_STORE];*/
void menue_data_manager_drive_update(void)
{
	memcpy(&menue_drive_data_applied, &menue_drive_data_setup, sizeof(menue_drive_data_t));
}


uint32_t menue_data_manager_drive_load(uint32_t offset_add)
{
	//menue_data_manager_drive_restore();
	//menue_data_manager_drive_update();
	//return;
	at24cxx_dynamic_sch_read_array(0, offset_add, sizeof(menue_drive_data_t), &menue_drive_data_setup);
	return offset_add + sizeof(menue_drive_data_t);
}
uint32_t menue_data_manager_drive_save(uint32_t offset_add)
{
	at24cxx_dynamic_sch_write_array(0, offset_add, sizeof(menue_drive_data_t), &menue_drive_data_setup);
	return offset_add + sizeof(menue_drive_data_t);
}

uint32_t menue_data_manager_drive_getSize(uint32_t offset_add)
{
	return offset_add + sizeof(menue_drive_data_t);
}

void menue_data_manager_drive_restore(void)
{
//__________________________vector parameters_________________________________
	menue_drive_data_setup.m_ACRD_i = ((307916.2924f) / 100); 
	menue_drive_data_setup.m_ACRD_p = ((2166.442294f)); 
	menue_drive_data_setup.m_ACRQ_i = ((307916.2924f) / 100);
	menue_drive_data_setup.m_ACRQ_p = ((2166.442294f));
	menue_drive_data_setup.m_ACR_bandwidth = (20);
	menue_drive_data_setup.m_ACR_bandwidth_zs = (80);
	menue_drive_data_setup.m_ASR_highs_i = (0.8f);
	menue_drive_data_setup.m_ASR_highs_p = (35.0f);
	menue_drive_data_setup.m_ASR_lows_i = (0.6f);
	menue_drive_data_setup.m_ASR_lows_p = (40.0f);
	menue_drive_data_setup.m_ASR_zs_i = (100.8f);
	menue_drive_data_setup.m_ASR_zs_p = (10.0f);
	menue_drive_data_setup.m_APR_zs_p = (0.0f);
	menue_drive_data_setup.m_APR_zs_eso = (5.0f);
	menue_drive_data_setup.m_ASR_sw_hi_point = (5.0);
	menue_drive_data_setup.m_ASR_sw_lw_point = (1.0f);
	menue_drive_data_setup.m_tourque_decay = (250);
	menue_drive_data_setup.m_speed_dev = (30);
	menue_drive_data_setup.m_ASR_speed_band = (5);
	menue_drive_data_setup.m_APR_level_p = (5.0);
////////////////////////////////Drive and motor//////////////////////////////////////////
	menue_drive_data_setup.m_carier_freq = 4;
	menue_drive_data_setup.m_control_mode = 0;
	menue_drive_data_setup.m_max_freq = fixedpt_rconst(50.0);
	menue_drive_data_setup.m_motor_leakage_inductance = (43.1f * sqrt3);
	menue_drive_data_setup.m_motor_LD = (60);
	menue_drive_data_setup.m_motor_LQ = (80);
	menue_drive_data_setup.m_motor_magnetizing_inductance = (24.0f);
	menue_drive_data_setup.m_motor_noload_current = (2.1f);
	menue_drive_data_setup.m_motor_rated_current = fixedpt_rconst(25.0);
	menue_drive_data_setup.m_motor_rated_freq = fixedpt_rconst(50.0);
	menue_drive_data_setup.m_motor_rated_pfactor = (82.0f);
	menue_drive_data_setup.m_motor_rated_power = (7.5f);
	menue_drive_data_setup.m_motor_rated_speed = fixedpt_rconst(1444);
	menue_drive_data_setup.m_motor_rated_voltage = fixedpt_rconst(380);
	menue_drive_data_setup.m_motor_rotor_resistance = (2.2f);
	menue_drive_data_setup.m_motor_stator_resistance = (5.125 * sqrt3);
	menue_drive_data_setup.m_motor_BEMF = (2.65558068887f);
	menue_drive_data_setup.m_motor_type = 0;
	menue_drive_data_setup.m_motor_pole_pair_count = 2;
////////////////////////////////// encoder parameters ////////////////////////////////////////
	menue_drive_data_setup.m_encoder_p_count = 2048;
	menue_drive_data_setup.m_encoder_type = 0;
	menue_drive_data_setup.m_pg_direction = 0;
	menue_drive_data_setup.m_pole_pos_offset = (50.65);
	menue_drive_data_setup.m_encoder_search_current = 75;
	menue_drive_data_setup.m_enc_filter_low = (1);
	menue_drive_data_setup.m_enc_filter_high = (1);
//////////////////////////////////V/F parameters////////////////////////////////////////
	menue_drive_data_setup.m_vmin			=	fixedpt_rconst(15.0f);
	menue_drive_data_setup.m_fmin			=	fixedpt_rconst(1.5f);
	menue_drive_data_setup.m_vm				=	fixedpt_rconst(30.0f);
	menue_drive_data_setup.m_fm				=	fixedpt_rconst(5.0f);
	menue_drive_data_setup.m_tboost			=	fixedpt_rconst(150.00);
	menue_drive_data_setup.m_break_freq		=	fixedpt_rconst(3);
	menue_drive_data_setup.m_break_voltage	=	fixedpt_rconst(20);
	
//////////////////////////////////Brake delays////////////////////////////////////////
	menue_drive_data_setup.m_dcbreak_delay = 250;
	menue_drive_data_setup.m_brake_release_delay = 250;
	menue_drive_data_setup.m_brake_hold_delay = 250;
	menue_drive_data_setup.m_motor_move_delay = 550;
	
//////////////////////////////////Phase fail////////////////////////////////////////
	menue_drive_data_setup.m_input_phase_fail = 1;
	
////////////////////////////Overload//////////////////////////////////////////////
	menue_drive_data_setup.m_overload_type = 0;
	menue_drive_data_setup.m_overload_time = 60;
	menue_drive_data_setup.m_overload_value = fixedpt_rconst(100);
////////////////////////////ARD//////////////////////////////////////////////
	menue_drive_data_setup.m_ard_type = 0;
	menue_drive_data_setup.m_ard_current_max = 2.5;
////////////////////////////Mech data//////////////////////////////////////////////
	menue_drive_data_setup.m_mech_sheave_dia = 210;
	menue_drive_data_setup.m_mech_sus_ratio = 2;
	menue_drive_data_setup.m_mech_motor_inertia = 0.15;
	menue_drive_data_setup.m_mech_counter_weight = 650;
	menue_drive_data_setup.m_mech_cabin_weight = 650;
	menue_drive_data_setup.m_mech_payload_weight = 400;
	menue_drive_data_setup.m_mech_inertia = 20;
	
	menue_drive_data_newdata_flag = 1;
	
	enc_set_theta(0);
	pop_ups_add("Drive set restored!", 1000);
}