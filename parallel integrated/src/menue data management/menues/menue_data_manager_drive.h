/*
 * menue_data_manager_drive.h
 *
 * Created: 02/05/2017 03:12:50 م
 *  Author: ahmed
 */ 


#ifndef MENUE_DATA_MANAGER_DRIVE_H_
#define MENUE_DATA_MANAGER_DRIVE_H_

#include <asf.h>
#include <fixedptc.h>

typedef struct
{
///////////////////////////// Motor parameters /////////////////////////////////////////////
	uint8_t m_motor_type;
	float m_motor_rated_power;
	fixedpt m_motor_rated_freq;
	fixedpt m_motor_rated_speed;
	fixedpt m_motor_rated_voltage;
	fixedpt m_motor_rated_current;
	float m_motor_rated_pfactor;
	float m_motor_stator_resistance;
	float m_motor_rotor_resistance;
	float m_motor_leakage_inductance;
	float m_motor_LD;
	float m_motor_LQ;
	float m_motor_magnetizing_inductance;
	float m_motor_noload_current;
	float m_motor_BEMF;
	uint8_t m_motor_pole_pair_count;
//_________________________________vector parameters_____________________________________
	float m_ASR_lows_p;
	float m_ASR_lows_i;
	float m_ASR_highs_p;
	float m_ASR_highs_i;
	float m_ASR_speed_band;
	float m_APR_level_p;
	float m_ASR_zs_p;
	float m_ASR_zs_i;
	float m_APR_zs_p;
	float m_APR_zs_eso;
	float m_ASR_sw_hi_point;
	float m_ASR_sw_lw_point;
	float m_ACRD_p;
	float m_ACRD_i;
	float m_ACRQ_p;
	float m_ACRQ_i;
	float m_ACR_bandwidth;
	float m_ACR_bandwidth_zs;
	float m_com_t_filter;
	uint16_t m_tourque_decay;
	uint16_t m_speed_dev;
//////////////////////////////////V/F parameters////////////////////////////////////////
	fixedpt m_vmin;
	fixedpt m_fmin;
	fixedpt m_vm;
	fixedpt m_fm;
	fixedpt m_break_freq;
	fixedpt m_break_voltage;
	fixedpt m_tboost;
////////////////////////////////// encoder parameters ////////////////////////////////////////
	uint8_t m_encoder_type;
	uint8_t m_pg_direction;
	uint16_t m_encoder_p_count;
	uint8_t m_encoder_search_current;
	float m_pole_pos_offset;
	float m_enc_filter_low;
	float m_enc_filter_high;	
//////////////////////////////drive Menues////////////////////////////////////////////
	fixedpt m_max_freq;
	uint8_t m_carier_freq;
	uint8_t m_control_mode;
////////////////////////////Brake parameters//////////////////////////////////////////////
	uint16_t m_brake_release_delay;
	uint16_t m_brake_hold_delay;
	uint16_t m_motor_move_delay;
	uint16_t m_dcbreak_delay;
////////////////////////////Phase fail//////////////////////////////////////////////
	uint8_t m_input_phase_fail;
////////////////////////////Over load//////////////////////////////////////////////
	uint8_t m_overload_type;
	uint8_t m_overload_time;
	fixedpt m_overload_value;
///////////////////////////////ARD data//////////////////////////////////
	uint8_t m_ard_type;	
	float m_ard_current_max;
///////////////////////////////mech data//////////////////////////////////
	uint16_t m_mech_sheave_dia;
	uint8_t m_mech_sus_ratio;
	float m_mech_motor_inertia;
	float m_mech_counter_weight;
	float m_mech_cabin_weight;
	float m_mech_payload_weight;
	float m_mech_inertia;
}menue_drive_data_t;

extern menue_drive_data_t	menue_drive_data_applied;
extern menue_drive_data_t	menue_drive_data_setup;
extern uint8_t				menue_drive_data_newdata_flag;
extern uint8_t				menue_drive_encoder_tune;
extern uint8_t				menue_drive_motor_tune;

#define D_MOTOR_FREQ (menue_drive_data_applied.m_motor_rated_freq)
#define D_MOTOR_RPM (menue_drive_data_applied.m_motor_rated_speed)
#define D_MOTOR_POLE_PAIR (menue_drive_data_applied.m_motor_pole_pair_count)
#define D_MOTOR_SPEED_HZ (D_MOTOR_RPM * D_MOTOR_POLE_PAIR / 60)

#define SPM_ENCODER_TUNE_STATIC (menue_drive_encoder_tune == 2)
#define IPM_ENCODER_TUNE_STATIC (menue_drive_encoder_tune == 3)
#define PM_ENCODER_TUNE_DYNAMIC (menue_drive_encoder_tune == 1)

#define PM_PARAM_TUNE ((menue_drive_motor_tune) && (menue_drive_data_applied.m_motor_type == 1))
#define IM_PARAM_TUNE ((menue_drive_motor_tune) && (menue_drive_data_applied.m_motor_type == 0))

void menue_data_manager_drive_update(void);
uint32_t menue_data_manager_drive_load(uint32_t offset_add);
uint32_t menue_data_manager_drive_save(uint32_t offset_add);
void menue_data_manager_drive_restore(void);
uint32_t menue_data_manager_drive_getSize(uint32_t offset_add);



#endif /* MENUE_DATA_MANAGER_DRIVE_H_ */