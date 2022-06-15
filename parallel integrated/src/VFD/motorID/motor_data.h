/*
 * motor_data.h
 *
 * Created: 31/03/2017 02:15:18 م
 *  Author: ahmed
 */ 


#ifndef MOTOR_DATA_H_
#define MOTOR_DATA_H_

#include <pwm_interface.h>

#define  sqrt3 (1.7320508075688772935274463415059)
#define  sqrt2 (1.4142135623730950488016887242097)
#define M_PI (2 * 3.14159265358979323846f)
#define M_TWO_PI (2 * 3.14159265358979323846f)

#define ENCODER_COUNTREV	(menue_drive_data_applied.m_encoder_p_count * 2)
#define MOTOR_POLEPAIR		(menue_drive_data_applied.m_motor_pole_pair_count)

#define GB_REDUCTION_RATIO  (1)//42.577f)

#define MOTOR_PM_CVC	((menue_drive_data_applied.m_control_mode == 1) && (menue_drive_data_applied.m_motor_type == 1))
#define MOTOR_PM_OVC	((menue_drive_data_applied.m_control_mode == 2) && (menue_drive_data_applied.m_motor_type == 1))
#define MOTOR_IM_CVC	((menue_drive_data_applied.m_control_mode == 1) && (menue_drive_data_applied.m_motor_type == 0))
#define MOTOR_IM_OVC	((menue_drive_data_applied.m_control_mode == 2) && (menue_drive_data_applied.m_motor_type == 0))
#define MOTOR_IM		(menue_drive_data_applied.m_motor_type == 0)

#define vbase			(menue_drive_data_applied.m_motor_rated_voltage)
#define fbase			(menue_drive_data_applied.m_motor_rated_freq)
#define pbase			(menue_drive_data_applied.m_motor_rated_power)

#define I_IM_RR     (menue_drive_data_applied.m_motor_rotor_resistance)
#define I_IM_LM     (menue_drive_data_applied.m_motor_magnetizing_inductance / 1000)
#define I_IM_LSEG   (menue_drive_data_applied.m_motor_leakage_inductance / 1000)
#define I_IM_RS     (menue_drive_data_applied.m_motor_stator_resistance)
#define I_IM_I_NL   (menue_drive_data_applied.m_motor_noload_current * sqrt2)
#define I_IM_TC     (I_IM_LM / I_IM_RR)

#define I_PM_LD		(menue_drive_data_applied.m_motor_LD / 1000)
#define I_PM_LQ		(menue_drive_data_applied.m_motor_LQ / 1000)
#define I_PM_RS		(menue_drive_data_applied.m_motor_stator_resistance)

// #define REDFAC		20
// #define IKPD        ((2166.442294f) / REDFAC)
// #define IKID        ((307916.2924f) / REDFAC)
// 
// #define IKPQ        IKPD
// #define IKIQ        IKID

#define KE	(menue_drive_data_applied.m_motor_BEMF) //back EMF constant

//change this rep
#define TSAMPLE ((1.0f)/(PWM_FREQUENCY) )
#endif /* MOTOR_DATA_H_ */