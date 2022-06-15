/*
 * timers.c
 *
 * Created: 20/11/2015 06:18:57 ص
 *  Author: ahmed
 */

#include <timers.h>
#include <auto_state.h>
#include <SEDACIO.h>

void timers_init(void)
{
	light_timer_init();
	stop_idle_init();
}
//___________________________ timer 1 stop timer ______________________________________________
void timer_count_handler(void)
{
	// a handler to make all timer to count
	idle_camt_count();
}

// turn on delay timers


//stop_idle_delay

uint32_t stop_idle_delay_indx;
void stop_idle_reset(void)
{
	sph_timer_reset(stop_idle_delay_indx);
}
void stop_idle_turnOn(void)
{
    //if(lock_safe) if(!external_auto) drop_all_req();
    if(auto_instate(auto_stop)==1)
	{
		if (ctb_light_curtain_fb) stop_idle_reset();
		else auto_idle_enter();
	} 
}
void stop_idle_init(void)
{
	stop_idle_delay_indx = sph_timer_add(&menue_lift_data_applied.m_stop_wait_time, 1000, 0, &stop_idle_turnOn);
}

//___________________________ timer 2 ______________________________________________

uint_t idle_camt_preset;
uint_t idle_camt_counter=0;
uByte_t idle_camt_run = 0;

void idle_camt_reset()
{
	idle_camt_counter=0;
	idle_camt_run=0;
}
void idle_camt_count()
{
	if(idle_camt_run) if((idle_camt_preset-idle_camt_counter)>0)
	{
		if((idle_camt_preset-idle_camt_counter)==1)
		{
			idle_camt_turnOn();
			idle_camt_run=0;
		}
		idle_camt_counter++;
	}
	else
	{
		idle_camt_counter=idle_camt_preset;
		idle_camt_run=0;
	}
}
void idle_camt_turnOn()
{
	if (auto_instate(auto_idle) == 1)
	{
		auto_idle_exit();
	}
	 
}
void idle_camt_init(unsigned int pre)
{
	idle_camt_counter=pre;
	idle_camt_preset=pre;
	idle_camt_run=0;
}
void idle_camt_start()
{
	idle_camt_run=1;
}


//_______________________________________Turn off delay_____________________________

uint32_t light_tmer_indx;

void light_timer_reset(void)
{
	sph_timer_reset(light_tmer_indx);
}
void light_timer_ton(void)
{
	march_light(1);
}
void light_timer_turnOff(void)
{
	march_light(0);
}
void light_timer_init(void)
{
	light_tmer_indx = sph_timer_add(&menue_lift_data_applied.m_light_time, 1000, &light_timer_ton, &light_timer_turnOff);
}