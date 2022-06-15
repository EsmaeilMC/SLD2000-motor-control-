/*
 * EMCM.c
 *
 * Created: 13/07/2018 07:25:57 م
 *  Author: ahmed
 */ 

#include <EMCM.h>
#include <menue_data_manager.h>
#include <HoistInfo.h>
#include <incremental.h>
#include <e_setup.h>
#include <floor_counter.h>
#include <vfd.h>
#include <service_priority_organizer.h>
#include <auto_state.h>
#include <requests.h>
#include <selfLearn.h>
#include <e_faults.h>
#include <SEDACIO.h>
#include <distance_regulator.h>

int32_t decel_displacement = 0, decel_displacement2 = 0;
int32_t dest_floor = 0;

#define  sign(x) (x<0?-1:1)
#define DECEL_DISP decel_displacement //(8192 * 2)

int32_t enc_pos;
int8_t speed_sign = 1;
int32_t get_decel_displacement(void)
{
	return DECEL_DISP;
}

int8_t EMCM_get_travel_speed_sign(void)
{
	return speed_sign;
}

int32_t decel_point = 0, decel_point2 = 0;
uint8_t allowedToExit_hspd = 0, allowedToExit_lspd = 0, dir_lmt0_posDev_corrected = 0, correct_at_stop = 0, stop_after_correction = 0;
uint32_t exit_hspd(void)
{
	return allowedToExit_hspd;
}

uint32_t exit_lspd(void)
{
	return allowedToExit_lspd;
}
int8_t temp_vfd_const = 1;
void change_dest_floor(void)
{
	int32_t temp_dest_floor = get_scheduled_floor();
	if (temp_dest_floor < 0 ) return;
	if ((allowedToExit_hspd) || (allowedToExit_lspd)) return;
	if (vfd_is_constant_speed())
	{
		temp_vfd_const = 1;
		if (temp_dest_floor != dest_floor)
		{
			//destination floor has changed
			if(fixedpt_abs(hoistinfo_getfloor_pos(dest_floor) - enc_get_theta()) /*available_displacement*/ > (fixedpt_abs(decel_displacement) + hoist_info.enc_count_per_meter/2))
			{
				//there is a room for inserting new floor level
				dest_floor = temp_dest_floor;
				decel_point = hoistinfo_getfloor_pos(dest_floor) - decel_displacement;
				decel_point2 = hoistinfo_getfloor_pos(dest_floor) - decel_displacement2;
			}
		}	
	}
	else temp_vfd_const = 0;
}

void set_Stop_afterCorrection(void)
{
	stop_after_correction = 1;
}

void set_correct_atStop(void)
{
	correct_at_stop = 1;
}

uint32_t set_Hspeed_profile(void)
{
	distance_regulator_disable();
	dest_floor = get_scheduled_floor();
	fixedpt ref_speed;
	
	if (hoist_info.Lrn)
	{
		LRN_req_err_set;
		drop_all_req();
		return 0;
	}
	if(dest_floor >= 0)
	{
		//vfd_set_ref_speed(speed_sign * HIGH_SPEED);
		decel_displacement = vfd_get_Max_possible_speed(hoistinfo_getfloor_pos(dest_floor) - enc_pos, &ref_speed, &decel_displacement2);
		if(decel_displacement == 0) 
		{
			drop_req(dest_floor);
			return 0;
		}
		
		decel_point = hoistinfo_getfloor_pos(dest_floor) - decel_displacement;
		decel_point2 = hoistinfo_getfloor_pos(dest_floor) - decel_displacement2;
		speed_sign = sign(ref_speed);
		
		vfd_set_accel_profile(ref_speed);
		allowedToExit_hspd = 0;
		allowedToExit_lspd = 0;
		dir_lmt0_posDev_corrected = 0;
		correct_at_stop = 0;
	}
	else
	{
		//no destination floor
		return 0;
	}
	allowedToExit_hspd = 0;
	return 1;
	
}

uint32_t cntr = 0;
void upl0decel(void)
{
	if(dir_lmt0_posDev_corrected) return;
	if ( fixedpt_abs(enc_pos - hoist_info.dirUplimit0) >= fixedpt_abs(hoist_info.forced_decel_error_trig))
	{
		allowedToExit_hspd = 1;
		allowedToExit_lspd = 1;
		enc_set_theta(hoist_info.dirUplimit0);
		vfd_set_motion_j1(0);
		vfd_set_motion_j1(0);
		vfd_set_motion_acc(hoist_info.dirup0_decel);
		vfd_set_ref_speed(speed_sign * CREEP_SPEED);
		dir_lmt0_posDev_corrected = 1;
		correct_at_stop = 1;
		stop_after_correction = 1;
		cntr++;
	}
	else dir_lmt0_posDev_corrected = 1;
}

void dnl0decel(void)
{
	if(dir_lmt0_posDev_corrected) return;
	if ( fixedpt_abs(enc_pos - hoist_info.dirDnlimit0) >= fixedpt_abs(hoist_info.forced_decel_error_trig))
	{
		allowedToExit_hspd = 1;
		allowedToExit_lspd = 1;
		enc_set_theta(hoist_info.dirDnlimit0);
		vfd_set_motion_j1(0);
		vfd_set_motion_j1(0);
		vfd_set_motion_acc(hoist_info.dirdwn0_decel);
		vfd_set_ref_speed(speed_sign * CREEP_SPEED);
		dir_lmt0_posDev_corrected = 1;
		correct_at_stop = 1;
		stop_after_correction = 1;
	}
	else dir_lmt0_posDev_corrected = 1;
}

void mstp_met(void)
{
	int32_t plate_length;
	if(correct_at_stop)
	{
		plate_length = hoist_info.level_position_up[get_current_floor()] - hoist_info.level_position_dn[get_current_floor()];
		plate_length = fixedpt_abs(plate_length);
		enc_set_theta(hoist_info.level_position[get_current_floor()] - (speed_sign * plate_length/2));
		correct_at_stop = 0;
		if (stop_after_correction) 
		{
			vfd_set_ref_speed(0);
			stop_after_correction = 0;
		}
	}
}

void decelToStop(void)
{
	if(!allowedToExit_hspd)
	{
		if (speed_sign < 0)
		{
			//negative speed
			if (enc_pos <= decel_point)
			{
				//vfd_set_decel_profile( speed_sign );
				vfd_set_motion_acc(menue_lift_data_applied.m_dec);
				vfd_set_motion_j1(menue_lift_data_applied.m_corner1/100);
				vfd_set_motion_j2(menue_lift_data_applied.m_corner2/100);
				
				vfd_set_ref_speed(speed_sign * CREEP_SPEED);
				allowedToExit_hspd = 1;
			}
		}
		else if (speed_sign > 0)
		{
			//positive speed
			if (enc_pos >= decel_point)
			{
				//vfd_set_decel_profile( speed_sign );
				vfd_set_motion_acc(menue_lift_data_applied.m_dec);
				vfd_set_motion_j1(menue_lift_data_applied.m_corner1/100);
				vfd_set_motion_j2(menue_lift_data_applied.m_corner2/100);
				
				vfd_set_ref_speed(speed_sign * CREEP_SPEED);
				allowedToExit_hspd = 1;
			}
		}
		
	}
	else if(!allowedToExit_lspd)
	{
		if (speed_sign < 0)
		{
			//negative speed
			if (enc_pos <= decel_point2)
			{
				//vfd_set_decel_profilez();
				vfd_set_motion_acc(menue_lift_data_applied.m_dec);
				vfd_set_motion_j1(menue_lift_data_applied.m_corner1/100);
				vfd_set_motion_j2(menue_lift_data_applied.m_corner3/100);
				vfd_set_ref_speed(0);
				allowedToExit_lspd = 1;
				distance_regulator_enable();
			}
		}
		else if (speed_sign > 0)
		{
			//positive speed
			if (enc_pos >= decel_point2)
			{
				//vfd_set_decel_profilez();
				vfd_set_motion_acc(menue_lift_data_applied.m_dec);
				vfd_set_motion_j1(menue_lift_data_applied.m_corner1/100);
				vfd_set_motion_j2(menue_lift_data_applied.m_corner3/100);
				vfd_set_ref_speed(0);
				allowedToExit_lspd = 1;
				distance_regulator_enable();
			}
		}
	}
}

uint8_t current_floor = 0;
void virsual_selector_conrol(void)
{
	int32_t floor_middle;
	int32_t upSgn = sign(hoist_info.level_position[1] - hoist_info.level_position[0]);
	
	
	if (selfLearn_isLearning()) return; //don't rely on enocder counts during self learning
	
	//search for the floor
	if (current_floor > MAX_F_NUM) current_floor = MAX_F_NUM;
	
	if (upSgn < 0)
	{
		//encoder decrement in up direction
		if (enc_pos <= hoist_info.level_position[current_floor])
		{
			//elevator moved up
			while (current_floor < MAX_F_NUM)
			{
				if (enc_pos < hoist_info.level_position[current_floor + 1])
				{
					// go to the upper next floor
					current_floor++;
				}
				else
				{
					// determine if you're in this floor zone or the next
					floor_middle = 	(hoist_info.level_position[current_floor] +
					hoist_info.level_position[current_floor + 1] ) / 2;
					if (enc_pos < floor_middle) current_floor++;
					break;
				}
			}
		}
		else
		{
			// elevator moved down
			while (current_floor > 0)
			{
				if (enc_pos > hoist_info.level_position[current_floor - 1])
				{
					// go to the upper next floor
					current_floor--;
				}
				else
				{
					// determine if you're in this floor zone or the previous
					floor_middle = 	(hoist_info.level_position[current_floor] +
					hoist_info.level_position[current_floor - 1] ) / 2;
					if (enc_pos > floor_middle) current_floor--;
					break;
				}
			}
		}
	} 
	else
	{
		//encoder increment in up direction
		if (enc_pos >= hoist_info.level_position[current_floor])
		{
			//elevator moved up
			while (current_floor < MAX_F_NUM)
			{
				if (enc_pos > hoist_info.level_position[current_floor + 1])
				{
					// go to the upper next floor
					current_floor++;
				}
				else
				{
					// determine if you're in this floor zone or the next
					floor_middle = 	(hoist_info.level_position[current_floor] +
					hoist_info.level_position[current_floor + 1] ) / 2;
					if (enc_pos > floor_middle) current_floor++;
					break;
				}
			}
		}
		else
		{
			// elevator moved down
			while (current_floor > 0)
			{
				if (enc_pos < hoist_info.level_position[current_floor - 1])
				{
					// go to the upper next floor
					current_floor--;
				}
				else
				{
					// determine if you're in this floor zone or the previous
					floor_middle = 	(hoist_info.level_position[current_floor] +
					hoist_info.level_position[current_floor - 1] ) / 2;
					if (enc_pos < floor_middle) current_floor--;
					break;
				}
			}
		}
	}
	
	if (current_floor > f_max) current_floor = f_max;
	sel_set(current_floor<<1);
}


void EMCM_handler(void)
{
	if (DIRECT_STOP)
	{
		//direct stop mode
		//EMCM_decel_point_watch();
		virsual_selector_conrol();
		change_dest_floor();
	}
}

int32_t enc_flashing, prev_enc_val = 0;
int32_t EMCM_get_enc_flash(void)
{
	return speed_sign;
	
	//return hoist_info.forced_decel_error_trig;
	 //hoist_info.forced_decel_error_trig =63* 25;
	//return hoist_info.forced_decel_error_trig;
	//return temp_vfd_const;
	//return fixedpt_abs(enc_flashing);
}

int32_t EMCM_get_floorErr(void)
{
	//return allowedToExit_lspd;
	//return decel_point2;
	return enc_pos - hoistinfo_getfloor_pos(get_current_floor());
}

void EMCM_decel_point_watch(void)
{
	self_learn_handler();
	if (DIRECT_STOP)
	{
		//direct stop mode
		enc_pos = enc_get_theta();
		enc_flashing = enc_pos - prev_enc_val;
		prev_enc_val = enc_pos;
		decelToStop();
		if(auto_instate(auto_lspd))
		{
			if (mstop == 0)
			{
				auto_set_mstp_met();
				mstp_met();
			}
		}
	}
}