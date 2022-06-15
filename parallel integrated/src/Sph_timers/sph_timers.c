/*
 * sph_timers.c
 *
 * Created: 23/05/2017 03:52:29 م
 *  Author: ahmed
 */ 

#include <sph_timers.h>

sph_timer_t timer_instances[TIMERS_COUNT];

//init the timer module
void sph_timer_init()
{
	uint32_t i;
	for (i=0; i<TIMERS_COUNT; i++)
	{
		timer_instances[i].start_handler = 0;
		timer_instances[i].stop_handler = 0;
		timer_instances[i].timer_counter = 0;
		timer_instances[i].timer_val = 0;
	}
		
}
//a fixed 1Khz tiemr handler routine
void sph_timer_handler()
{
	int32_t i;
	for (i=0; i<TIMERS_COUNT; i++)
	{
		if (timer_instances[i].timer_val)
		{
			//there is a timer at this location
			if (timer_instances[i].timer_counter > 0)
			{
				if (--timer_instances[i].timer_counter == 0)
				{
					//timer rolled over
					if (timer_instances[i].stop_handler)
					{
						timer_instances[i].stop_handler();
					}
				}
			}
					
		}
	}
		
}

uint32_t sph_timer_add(uint32_t *timer_val, uint16_t timer_multiplier, void ( *start_handler ) (void), void ( *stop_handler )  (void) )
{
	uint32_t i = 0;
	
	if(timer_multiplier == 0) timer_multiplier = 1;
	for (i=0; i<TIMERS_COUNT; i++)
	{
		if (timer_instances[i].timer_val == 0)
		{
			//empty timer space found
			timer_instances[i].timer_val = timer_val;
			timer_instances[i].timer_multiplier = timer_multiplier;
			timer_instances[i].start_handler = start_handler;
			timer_instances[i].stop_handler = stop_handler;
			return i;
		}
	}
	// no empty timer space found
	return 0xFFFFFFFF;
}

void sph_timer_delete(uint32_t timer_indx)
{
	if (timer_indx < TIMERS_COUNT)
	{
		//there might be a timer there
		timer_instances[timer_indx].start_handler = 0;
		timer_instances[timer_indx].stop_handler = 0;
		timer_instances[timer_indx].timer_val = 0;
	}
}

void sph_timer_reset(uint32_t timer_indx)
{
	if (timer_indx < TIMERS_COUNT)
	{
		//there might be a timer there
		if (timer_instances[timer_indx].timer_val)
		{
			if(timer_instances[timer_indx].start_handler) timer_instances[timer_indx].start_handler();
			timer_instances[timer_indx].timer_counter = *timer_instances[timer_indx].timer_val;
			timer_instances[timer_indx].timer_counter *= timer_instances[timer_indx].timer_multiplier;
		}
	}	
}