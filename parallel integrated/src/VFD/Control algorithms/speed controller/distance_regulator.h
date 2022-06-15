/*
 * distance_regulator.h
 *
 * Created: 3/1/2021 2:23:15 AM
 *  Author: Ahmed
 */ 


#ifndef DISTANCE_REGULATOR_H_
#define DISTANCE_REGULATOR_H_
#include <asf.h>

float distance_regulator_get_ffspeed(void);

void distance_regulator_enable(void);
void distance_regulator_disable(void);
void distance_regulator_correct_offset(int64_t new_offset);

#endif /* DISTANCE_REGULATOR_H_ */