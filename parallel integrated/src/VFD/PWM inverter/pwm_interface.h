/*
 * pwm_interface.h
 *
 * Created: 31/01/2017 03:46:33 م
 *  Author: ahmed
 */ 


#ifndef PWM_INTERFACE_H_
#define PWM_INTERFACE_H_

#define DEAD_TIME (3.00f) //in usec
#define DEAD_TIME_INTERVAL (get_dead_time_interval()) //in usec
#include <drive_interface_data.h>

void pwm_interface_init(void);
void pwm_interface_enablesynchchannels(void);
uint32_t get_dead_time_interval(void);
// PWM frequency in Hz.

#define PWM_FREQUENCY pwm_real_inv_freq
#define PWM_BRAKE_FREQUENCY (8000)
extern float pwm_real_inv_freq;

#define PERIOD_VALUE 0xfff

#endif /* PWM_INTERFACE_H_ */