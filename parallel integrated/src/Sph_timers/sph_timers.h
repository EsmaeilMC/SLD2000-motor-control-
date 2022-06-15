/*
 * sph_timers.h
 *
 * Created: 23/05/2017 03:52:38 م
 *  Author: ahmed
 */ 


#ifndef SPH_TIMERS_H_
#define SPH_TIMERS_H_

#include <asf.h>

#define TIMERS_COUNT 15
typedef struct
{
	uint8_t *timer_val;
	uint32_t timer_counter;
	uint16_t timer_multiplier;
	void ( *start_handler ) (void);
	void ( *stop_handler )  (void);
}sph_timer_t;

void sph_timer_init();
//a fixed 1Khz tiemr handler routine
void sph_timer_handler();
uint32_t sph_timer_add(uint32_t *timer_val, uint16_t timer_multiplier, void ( *start_handler ) (void), void ( *stop_handler )  (void) );
void sph_timer_delete(uint32_t timer_indx);
void sph_timer_reset(uint32_t timer_indx);
#endif /* SPH_TIMERS_H_ */