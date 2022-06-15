/*
 * speed_regulator.h
 *
 * Created: 4/29/2018 2:59:06 AM
 *  Author: Ahmed
 */ 


#ifndef SPEED_REGULATOR_H_
#define SPEED_REGULATOR_H_

#include <asf.h>

float speed_regulate(float cmd_speed, float current_speed, uint32_t accel);
float speed_regulator_setKt(float kt);
void speed_PI_init(void);
void speed_PI_changeLSGains(void);
void pos_regulator_reset_keeping_pos(void);
void start_zero_servo(void);
void stop_zero_servo(void);
float get_zs_displacement(void);

float get_spt(void);
float get_pzs(void);
float get_loadtorque(void);
float get_pit(void);

#endif /* SPEED_REGULATOR_H_ */