/*
 * HoistInfo.h
 *
 * Created: 13/07/2018 06:45:46 م
 *  Author: ahmed
 */ 


#ifndef HOISTINFO_H_
#define HOISTINFO_H_

#define  MAX_POSSIBLE_FLOOR_COUNT 24
#define MAX_F_NUM (MAX_POSSIBLE_FLOOR_COUNT - 1)

#include <asf.h>
#include <fixedptc.h>

typedef struct
{
	uint8_t Lrn;
	int32_t level_position_up[MAX_POSSIBLE_FLOOR_COUNT];
	int32_t level_position[MAX_POSSIBLE_FLOOR_COUNT];
	int32_t level_position_dn[MAX_POSSIBLE_FLOOR_COUNT];
	fixedpt level_position_correction[MAX_POSSIBLE_FLOOR_COUNT];
	int32_t dirUplimit0;
	int32_t dirDnlimit0;
	fixedpt dirup0_decel;
	fixedpt dirdwn0_decel;
	uint32_t forced_decel_error_trig;
	int32_t mstp_plate_length;
	uint32_t enc_count_per_meter;	
}hoist_info_t;

extern hoist_info_t hoist_info;
void Hoist_init(void);
int32_t hoistinfo_getfloor_pos(uint32_t floor);

#endif /* HOISTINFO_H_ */