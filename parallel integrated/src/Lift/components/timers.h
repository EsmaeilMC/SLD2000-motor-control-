/*
 * timers.h
 *
 * Created: 19/11/2015 04:28:56 م
 *  Author: ahmed
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

#include <menue_data_manager.h>
#include <sph_timers.h>

void timer_count_handler(void);
void timers_init(void);

//___________________________ timer 1 stop timer ______________________________________________
// turn on delay timers

//stop_idle_delay

void stop_idle_reset(void);
void stop_idle_turnOn(void);
void stop_idle_init(void);

//___________________________ timer 2 ______________________________________________


void idle_camt_reset(void);
void idle_camt_count(void);
void idle_camt_turnOn(void);
void idle_camt_init(unsigned int pre);
void idle_camt_start(void);

//___________________________ timer 3 ______________________________________________


void ers_ton_reset(void);
void ers_ton_count(void);
void ers_ton_turnOn(void);
void ers_ton(unsigned int pre);
void ers_ton_start(void);

//___________________________ timer 4 ______________________________________________


void star_delta_refresh(void);
void star_delta_reset(void);
void star_delta_count(void);
void star_delta_init(unsigned int pre);


//_______________________________________Turn off delay_____________________________

void light_timer_reset(void);
void light_timer_ton(void);
void light_timer_turnOff(void);
void light_timer_init(void);

#endif /* TIMERS_H_ */