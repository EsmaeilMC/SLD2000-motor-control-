/*
 * selfLearn.c
 *
 * Created: 8/5/2018 5:44:18 AM
 *  Author: Ahmed
 */ 

#define HW_LRN (menue_lift_data_startSelfLrn)

#include <asf.h>
#include <fixedptc.h>
#include <incremental.h>
#include <floor_counter.h>
#include <menue_data_manager.h>
#include <e_setup.h>
#include <SEDACIO.h>
#include <e_faults.h>
#include <selfLearn.h>
#include <motor_data.h>
#include <pop_ups.h>

#define elevator_speed_ms (menue_lift_data_applied.m_lift_speed)
#define ENCODER_COUNT_METER lrn_encCount_perMeter
#define PLATE_LENGTH_MM (menue_lift_data_applied.m_stop_plate_length)

int32_t lrn_encCount_perMeter;
uint8_t lrn_floor_counter = 0;
uint8_t hw_learning = 0, lrn_stage;
uint8_t prev_dnfdl0, prev_upfdl0, prev_mstp;
int8_t lrn_active = 0;

uint32_t check_initial_conditions (void)
{
	//set initial conditions
	prev_mstp = 0;
	prev_upfdl0 = 1;
	prev_dnfdl0 = 0;
	lrn_floor_counter = 0;

	if (mstop == 1)
	{
		return 0;
	} 
	else if (dnfdl0 == 1)
	{
		return 0;
	}
	else if (upfdl0 == 0)
	{
		return 0;
	}
	
	return 1;
}

uint32_t enter_Hlearn(int32_t up_dir)
{
	if(HW_LRN == 0)
	{
		hw_learning = 0;
		return 0;
	}
	if (up_dir == 0)
	{
		hw_learning = 0;
		return 0;
	}
	
	if(check_initial_conditions() == 0) 
	{
		LRN_err_set;
		pop_ups_add("Learn failed", 800);
		HW_LRN = 0;
		return 0;
	}
	hw_learning = 1;
	enc_set_theta(0);
	sel_set(0);
	lrn_stage = 0;
	lrn_active = 1;
	return 1;
}

uint32_t selfLearn_isLearning(void)
{
	if ((HW_LRN) && (hw_learning)) return 1;
	return 0;
}

void selfLearn_terminateLearning(void)
{
	if (hw_learning == 1)
	{
		//todo lrn error
		LRN_err_set;
		pop_ups_add("Learn failed", 800);
		HW_LRN = 0;
	}
	hw_learning = 0;
}

uint32_t selfLearn_stage0_exe()
{
	if (mstop == 1)
	{
		if (prev_mstp == 0)
		{
			hoist_info.level_position_up[0] = enc_get_theta();
			hoist_info.level_position[0] = enc_get_theta();
		}
		prev_mstp = mstop;
	}
	if (dnfdl0 == 1)
	{
		if (mstop == 0) 
		{
			LRN_err_set;
			pop_ups_add("Learn failed", 800);
		}
		if (prev_dnfdl0 == 0)
		{
			hoist_info.dirDnlimit0 = enc_get_theta();
		}
		prev_dnfdl0 = dnfdl0;
	}
	if (upfdl0 == 0)
	{
		LRN_err_set;
		pop_ups_add("Learn failed", 800);
	}
	if ((prev_mstp == 1) && (prev_dnfdl0 == 1)) return 1;
	return 0;
}

uint32_t selfLearn_stage1_exe()
{
	if ((f_max < 2) || (((f_max - lrn_floor_counter) == 1) && (prev_mstp == 1))) return 1;
	if (mstop == 0)
	{
		if (prev_mstp == 1)
		{
			lrn_floor_counter++;
			sel_set(lrn_floor_counter<<1);
			hoist_info.level_position_dn[lrn_floor_counter] = enc_get_theta();
			hoist_info.level_position[lrn_floor_counter] = enc_get_theta()/2;
			if (lrn_floor_counter == 1)
			{
				hoist_info.mstp_plate_length = -enc_get_theta();
			}
		}
		prev_mstp = mstop;
	}
	else if (mstop == 1)
	{
		if (prev_mstp == 0)
		{
			hoist_info.level_position_up[lrn_floor_counter] = enc_get_theta();
			hoist_info.level_position[lrn_floor_counter] += enc_get_theta()/2;
			if (lrn_floor_counter == 1)
			{
				hoist_info.mstp_plate_length += enc_get_theta();
			}
			
			
		}
		prev_mstp = mstop;
	}
	
	if (dnfdl0 == 0)
	{
		LRN_err_set;
		pop_ups_add("Learn failed", 800);
	}
	if (upfdl0 == 0)
	{
		LRN_err_set;
		pop_ups_add("Learn failed", 800);
	}
	return 0;
}

uint32_t selfLearn_stage2_exe()
{
	if (mstop == 0)
	{
		if (prev_mstp == 1)
		{
			if (upfdl0 == 1) 
			{
				LRN_err_set;
				pop_ups_add("Learn failed", 800);
			}
			lrn_floor_counter++;
			hoist_info.level_position_dn[f_max] = enc_get_theta();
			hoist_info.level_position[f_max] = enc_get_theta();
			sel_set(lrn_floor_counter<<1);
		}
		prev_mstp = mstop;
	}
	if (dnfdl0 == 0)
	{
		LRN_err_set;
		pop_ups_add("Learn failed", 800);
	}
	if (upfdl0 == 0)
	{
		if (prev_upfdl0 == 1)
		{
			if (mstop == 0) 
			{
				LRN_err_set;
				pop_ups_add("Learn failed", 800);
			}
			hoist_info.dirUplimit0 = enc_get_theta();
		}
		prev_upfdl0 = upfdl0;
	}
	if ((prev_mstp == 0) && (prev_upfdl0 == 0)) return 1;
	return 0;
}

uint32_t selfLearn_finalize_calculations()
{
	//volatile int32_t tmptst ;
	if ((f_max < 2))
	{
		//level lenght isn't measured so estimate it
	} 

	
	hoist_info.level_position[0] -= hoist_info.mstp_plate_length/2;
	hoist_info.level_position_dn[0] = hoist_info.level_position_up[0] - hoist_info.mstp_plate_length;
	
	hoist_info.level_position[f_max] += hoist_info.mstp_plate_length/2;
	hoist_info.level_position_up[f_max] = hoist_info.level_position_dn[f_max] + hoist_info.mstp_plate_length;
	
	hoist_info.mstp_plate_length = fixedpt_abs(hoist_info.mstp_plate_length);
	 
	ENCODER_COUNT_METER = (hoist_info.mstp_plate_length * 1000) / PLATE_LENGTH_MM;
	hoist_info.enc_count_per_meter = ENCODER_COUNT_METER;
	
	hoist_info.forced_decel_error_trig = ENCODER_COUNT_METER / 13; //(7.5 CM)
	
	hoist_info.dirup0_decel = fixedpt_rconst(hoist_info.level_position[f_max] - hoist_info.dirUplimit0) / (ENCODER_COUNTREV / MOTOR_POLEPAIR); 
	hoist_info.dirup0_decel = fixedpt_div(hoist_info.dirup0_decel, fbase)<<1;
	hoist_info.dirup0_decel = fixedpt_mul(hoist_info.dirup0_decel, fixedpt_rconst(0.78));
	hoist_info.dirup0_decel = fixedpt_abs(hoist_info.dirup0_decel);
	
	hoist_info.dirdwn0_decel = fixedpt_rconst(hoist_info.dirDnlimit0 - hoist_info.level_position[0]) / (ENCODER_COUNTREV / MOTOR_POLEPAIR);
	hoist_info.dirdwn0_decel = fixedpt_div(hoist_info.dirdwn0_decel, fbase)<<1;
	hoist_info.dirdwn0_decel = fixedpt_mul(hoist_info.dirdwn0_decel, fixedpt_rconst(0.78));
	hoist_info.dirdwn0_decel = fixedpt_abs(hoist_info.dirdwn0_decel);
	
	hoist_info.Lrn = 0;
	//tmptst = hoist_info.dirDnlimit0;
	pop_ups_add("Learn Successful", 1500);
}

void self_learn_handler(void)
{
	if (hw_learning == 0) return;
	if (lrn_stage == 0)
	{
		if (selfLearn_stage0_exe())
		{
			lrn_stage++;
		}
	}
	else if (lrn_stage == 1)
	{
		if (selfLearn_stage1_exe())
		{
			lrn_stage++;
		}
	}
	else if (lrn_stage == 2)
	{
		if (selfLearn_stage2_exe())
		{
			
			selfLearn_finalize_calculations();
			hw_learning = 0;
			HW_LRN = 0;
			lrn_stage++;
			lrn_active = 0;
		}
	}
	else lrn_active = 0;
}

uint32_t selfLearn_exe(void)
{
	if (hw_learning == 0) return 0;
	return lrn_active;
}

