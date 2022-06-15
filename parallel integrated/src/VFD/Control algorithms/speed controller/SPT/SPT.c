/*
 * SPT.c
 * 
  Stationary pre torque algorithm
 * Created: 14/04/2020 09:01:42 ص
 *  Author: ahmed
 */ 


#include <SPT.h>
#include <fixedptc.h>
#include <motor_data.h>
#include <encoder_interface.h>
#include <SEDACIO.h>

#define SPT_ACTIVE (ersin == 0)
#define SPT__ENC_STEP (0.4f)
#define SPT_TIME_MS (350)
#define SPT_REGRESS_TIME_MS (80)
#define SPT_WAIT_TIME_MS (100)
#define SPT_FILTER (3)

float rated_torque, trq_neg = 0, trq_pos = 0, trq_neatural;
uint8_t brake_allow_to_open = 0, pre_torque_ready = 0;

float spt_phase_to_keep;
int64_t spt_pos_to_keep = 0;
float spt_pos_err = 0;
void spt_reset_zero_servo(void)
{
	spt_pos_to_keep = enc_get_lin_theta();
	spt_phase_to_keep = enc_get_phase();
	spt_phase_to_keep /= FIXEDPT_ONE;
	spt_pos_err = 0;	
}

int8_t spt_check_pos_shift_limit(void)
{
	float tmp, tmp_spt_pos_err;
	
	tmp_spt_pos_err = spt_pos_to_keep - enc_get_lin_theta(); //generate error signal
	tmp = -enc_get_phase();
	tmp /= FIXEDPT_ONE;
	tmp += spt_phase_to_keep;
	tmp_spt_pos_err += tmp;
	
	spt_pos_err += (tmp_spt_pos_err - spt_pos_err) / SPT_FILTER;
	if (fixedpt_abs(spt_pos_err) >= SPT__ENC_STEP)
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}

uint8_t spt_stage = 0;
uint16_t spt_wait_counter = 0;
float spt_perform(void)
{
	uint8_t pos_shifted = 0;
	if (spt_stage == 0)
	{
		pos_shifted = spt_check_pos_shift_limit();
		spt_wait_counter = 0;
		if ( (fixedpt_abs(trq_neg) <= rated_torque) && (pos_shifted == 0) )
		{
			trq_neg -= fixedpt_abs(rated_torque / SPT_TIME_MS);
		} 
		else
		{
			trq_neatural = trq_neg;
			spt_stage++;
		}
		return trq_neg;
	} 
	else if (spt_stage == 1)
	{
		if (trq_neatural < 0)
		{
			trq_neatural += fixedpt_abs(rated_torque / SPT_REGRESS_TIME_MS);
		}
		else
		{
			if(++spt_wait_counter > SPT_WAIT_TIME_MS)
			{
				trq_pos = trq_neatural;
				spt_reset_zero_servo();
				spt_stage++;
			}
		}
		return trq_neatural;
	}
	else if (spt_stage == 2)
	{
		pos_shifted = spt_check_pos_shift_limit();
		if ( (fixedpt_abs(trq_pos) <= rated_torque) && (pos_shifted == 0) )
		{
			trq_pos += fixedpt_abs(rated_torque / SPT_TIME_MS);
		}
		else
		{
			trq_neatural = trq_neg + trq_pos;
			spt_stage++;
		}
		return trq_pos;
	}
	else if (spt_stage == 3)
	{
		if (trq_pos < trq_neatural)
		{
			trq_pos += fixedpt_abs(rated_torque / SPT_REGRESS_TIME_MS);
			if (fixedpt_abs(trq_pos - trq_neatural) < fixedpt_abs(rated_torque / SPT_REGRESS_TIME_MS))
			{
				spt_stage++;
			}
		}
		else if (trq_pos > trq_neatural)
		{
			trq_pos -= fixedpt_abs(rated_torque / SPT_REGRESS_TIME_MS);
			if (fixedpt_abs(trq_pos - trq_neatural) < fixedpt_abs(rated_torque / SPT_REGRESS_TIME_MS))
			{
				spt_stage++;
			}
		}
		return trq_pos;
	}
	else if (spt_stage >= 4)
	{
		pre_torque_ready = 1;
		return trq_neatural;
	}
}

uint8_t spt_permission_brake_open(void)
{
	if(SPT_ACTIVE == 0) return 1;
	if (!MOTOR_PM_CVC) return 1;
	return (brake_allow_to_open & pre_torque_ready);
}

void spt_vfd_set_ready_to_open_brake(void)
{
	brake_allow_to_open = 1;
}

void spt_init(float r_torque)
{
	brake_allow_to_open = 0;
	pre_torque_ready = 0;
	rated_torque = r_torque;
	
	trq_neg = 0;
	trq_pos = 0; 
	trq_neatural = 0;
	
	spt_stage = 0;
	
	spt_reset_zero_servo();
}