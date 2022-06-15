/*
 * HoistInfo.c
 *
 * Created: 13/07/2018 06:45:36 م
 *  Author: ahmed
 */ 

#include <fixedptc.h>
#include <HoistInfo.h>
#include <incremental.h>

#define  sign(x) (x<0?-1:1)

hoist_info_t hoist_info;

#define FLOOR_ENC_COUNT (32563)
void Hoist_init(void)
{
	uint32_t i;
	hoist_info.Lrn = 1;
	for (i=0; i<MAX_POSSIBLE_FLOOR_COUNT; i++)
	{
		hoist_info.level_position[i] = -1 * (i-0) * FLOOR_ENC_COUNT;
		hoist_info.level_position_correction[i] = 0;
	}
	hoist_info.dirDnlimit0 = -FLOOR_ENC_COUNT/3;
	hoist_info.dirUplimit0 = (-7 * FLOOR_ENC_COUNT) + FLOOR_ENC_COUNT/3;
	hoist_info.dirup0_decel = fixedpt_rconst(1);
	hoist_info.dirdwn0_decel = fixedpt_rconst(1);
	hoist_info.forced_decel_error_trig = 2170; //enc count
	hoist_info.mstp_plate_length = 1085; //enc_count
	//enc_set_theta(hoist_info.level_position[5]);
	//enc_set_theta(hoist_info.dirUplimit0);
}

int32_t hoistinfo_getfloor_pos(uint32_t floor)
{
	int32_t up_sgn;
	/*volatile*/ int64_t tmp;
	if (floor > MAX_F_NUM) floor = MAX_F_NUM;
	
	up_sgn = sign(hoist_info.level_position[1] - hoist_info.level_position[0]);
	//tmp = fixedpt_toint(hoist_info.level_position_correction[floor]) ;
	tmp = hoist_info.enc_count_per_meter;
	tmp *= up_sgn * (fixedpt_toint(hoist_info.level_position_correction[floor]));
	tmp /= 1000;
	tmp += hoist_info.level_position[floor];
	return tmp;
	
}