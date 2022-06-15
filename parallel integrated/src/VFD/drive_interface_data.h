/*
 * drive_interface_data.h
 *
 * Created: 30/05/2017 01:20:03 ص
 *  Author: Ahmed
 */ 


#ifndef DRIVE_INTERFACE_DATA_H_
#define DRIVE_INTERFACE_DATA_H_

#define _SERIAL_DRIVE

//#define D_5K5
//#define D_7K5
#define D_11K

#ifdef D_5K5
	#define I_I_OV_V (12.5) //current sensor i/v Characteristics
	#define DRIVE_UPPER_CURRENT_LIMIT (25.0f) //drive Dependant
#endif

#ifdef D_7K5
	#define I_I_OV_V (12.5) //current sensor i/v Characteristics
	#define DRIVE_UPPER_CURRENT_LIMIT (35.0f) //drive Dependant
#endif

#ifdef D_11K
	//#define I_I_OV_V (15) //current sensor i/v Characteristics
	#define I_I_OV_V (12.5) //current sensor i/v Characteristics
	#define DRIVE_UPPER_CURRENT_LIMIT (55.0f) //drive Dependant
#endif

#define PMSM_MODEL

#include <menue_data_manager.h>
#include <motor_data.h>

#define FMAX (menue_drive_data_applied.m_max_freq)
#define pwm_inv_freq (menue_drive_data_applied.m_carier_freq * 1000)


#define M_PHFAIL_DEV (0.2 * DRIVE_UPPER_CURRENT_LIMIT)
//V/F parameters


#endif /* DRIVE_INTERFACE_DATA_H_ */