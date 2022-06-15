/*
 * distance_regulator.c
 *
 * Created: 3/1/2021 2:23:03 AM
 *  Author: Ahmed
 */ 

#include <distance_regulator.h>
#include <vfd.h>
#include <encoder_interface.h>
#include <d_faults.h>

#define ENABLE_REG ((distance_regulation_enable == 1))// && (menue_drive_data_applied.m_control_mode == 1))
#define DKP (menue_drive_data_applied.m_APR_level_p)


uint8_t distance_regulation_enable = 0;

int64_t encoder_distance_offset = 0;
float distance_regulator_exe(void)
{
	float distance_error, tmp;
	
	distance_error = vfd_get_desired_distance();
	distance_error /= FIXEDPT_ONE;
	distance_error /= MOTOR_POLEPAIR; //distance_error = motor desired shaft distance in revolutions
	
	tmp = enc_get_lin_theta() - encoder_distance_offset;
	tmp /= ENCODER_COUNTREV; // tmp = motor real shaft distance in revolutions
	
	distance_error -= tmp; //get distance error
	
	tmp = hoist_info.enc_count_per_meter;
	tmp /= ENCODER_COUNTREV * 10; //10 cm error
	
	if(fixedpt_abs(distance_error)  >= fixedpt_abs(tmp))
	{
		//distance error >20 cm
		dev_dis_err_set;
		return 0;
	}
	distance_error *= M_TWO_PI; //convert error to radian
	
	return (distance_error * DKP); //return feed forward speed;
}

float distance_regulator_get_ffspeed(void)
{
	if (ENABLE_REG)
	{
		return distance_regulator_exe();
	} 
	else
	{
		return 0;
	}
}

void distance_regulator_enable(void)
{
	encoder_distance_offset = enc_get_lin_theta();
	distance_regulation_enable = 1;
}

void distance_regulator_correct_offset(int64_t new_offset)
{
	encoder_distance_offset += new_offset;
}

void distance_regulator_disable(void)
{
	distance_regulation_enable = 0;
}