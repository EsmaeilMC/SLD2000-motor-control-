/*
 * requests.c
 *
 * Created: 22/11/2015 12:20:09 ص
 *  Author: ahmed
 */ 
#include <requests.h>
#include <floor_counter.h>
#include <SEDACIO.h>
#include <timers.h>
#include <e_setup.h>
#include <auto_state.h>
#include <control_status_register.h>
#include <req_io_mb_reader.h>
#include <service_priority_organizer.h>
#include <SCH.h>
#include <con_toff.h>
#include <drive_interface_data.h>

uint32_t ireq=0x00, ereq=0x0, eereq=0;

uByte_t req_present()
{
	if(ireq||ereq||eereq)
		return 1;
	return 0;
}
#define park_timer  (menue_lift_data_applied.m_parking_time * 25)
#define parking_floor (menue_lift_data_applied.m_parking_floor)
unsigned int park_counter=0;

void insert_irequest(uint32_t request)
{
	if((get_current_floor() != request) && (request < f_max))
	{
		ireq |= msk(request);
	}
}

char selector_startUp = 1;
char request_init_selector_handler;

void request_init_selector(void)
{
	if(selector_startUp)
	{
		if((upfdl0 == 0) && (get_current_floor() == f_max))
		{
			//you are at the top floor, no need to reset selector
			selector_startUp = 0;
			sch_delete_task(request_init_selector_handler);
		}
		else if((dnfdl0 == 0) && (get_current_floor() == 0))
		{
			//you are at the bottom floor, no need to reset selector
			selector_startUp = 0;
			sch_delete_task(request_init_selector_handler);
		}
		else if(dnfdl0 == 0)
		{
			insert_irequest(f_max);
		}
		else
		{
			insert_irequest(0);
		}
	}
}

void init_requests(uint32_t task_delay)
{
	request_init_selector_handler = sch_add_task_ms(&request_init_selector, task_delay, 3000);
}


void parking_check()
{
	if(device_off)
		return;
	if(park_timer)//parking operation
	{
		if(req_present())
		{
			park_counter=0;
		}
		else
		{
			park_counter++;
			if(park_counter>=park_timer)
			{
				park_counter=0;
				if((get_current_floor()!=parking_floor) && (parking_floor < f_max))
				{
					if(auto_instate(auto_idle))
						ireq |= msk(parking_floor);
					//req_scan(msk[parking_floor],0);
				}
			}
		}
	}
}
void pos_next_floor() //passenger out safe in case like overload detection
{
	char nreq=0;
	if(auto_instate(auto_hspd))
	{
		nreq=get_current_floor();
		if(infloorzone())
		{//if car infloor zone schedule next floor
			if(!direction_up)
			{
				if(nreq>0)
				nreq-=1;
			}
			else
			{
				if(nreq < f_max)
				nreq++;
				else
				nreq = f_max;
			}
			ireq=msk(nreq);
			ereq=0;
			return;
		}
		//if not infloorzone schedule next floor or current floor if you're heading downstairs
		if(direction_up)
		{
			if(nreq < f_max)
			nreq++;
			else
			nreq = f_max;
		}
		ireq = msk(nreq);
		ereq = 0;
	}
	
}
void drop_all_req(void)
{
    ireq=0;
    ereq=0;
	eereq=0;    
}

void drop_req(char req_num)
{
	ireq&=~msk(req_num);
	uint32_t tmp;
	if ((req_num == f_max) || (req_num == 0))
	{
		ereq&=~msk(req_num);
		eereq&=~msk(req_num);
	}
	else if (collect == 2)
	{
		tmp = ireq|ereq|eereq;
		if (direction_up)
		{
			tmp &= (0xFFFFFFFF << (req_num + 1));
			eereq&=~msk(req_num);
			if(tmp == 0) ereq&=~msk(req_num);
		}
		else
		{
			tmp &= ~(0xFFFFFFFF << req_num);
			ereq&=~msk(req_num);
			if(tmp == 0) eereq&=~msk(req_num);
		}
	} 
	else
	{
		ereq&=~msk(req_num);
		eereq&=~msk(req_num);
	}
	
}

uint8_t req_enabled = 0;
void req_enable(uint8_t command)
{
	req_enabled = command;	
}

//unsigned int req_remove_mask=0;//to filter the unwanted requests
#ifndef _SERIAL_DRIVE
void req_scan(void)
{
	uint32_t i, c_f, req_disable_led;
	
	if(device_off == 0)
	{
		c_f = get_current_floor();

		for(i=0; i<24; i++)
		{	
			if(i <= f_max)
			{
				if( (get_ireq(i, ireq&msk(i)) == 1) || (get_ereq(i, ereq&msk(i)) == 1) )
					light_timer_reset();
				if(collect == 2)
					if ((get_eereq(i, eereq&msk(i)) == 1)) light_timer_reset();
					
				if (i != c_f)
				{
					set_ireq_ledMask(i, 1);
					set_ereq_ledMask(i, 1);
					if(collect == 2) set_eereq_ledMask(i, 1);
				}
				else
				{
					set_ireq_ledMask(i, 0);
					set_ereq_ledMask(i, 0);
					if(collect == 2) set_eereq_ledMask(i, 0);
				}
						
				if ((get_ireq(i, ireq & msk(i)) == 1) && (req_enabled))
				{
					if(i != c_f)
					ireq |= msk(i);
					//get_ireq(i, ireq&msk(i));
				}
				if ((get_ereq(i, ereq & msk(i)) == 1) && (req_enabled))
				{
					if(collect != 2)
					{
						if (i != c_f)
							ereq |= msk(i);
						else auto_reopen_door();//reopen door automatic
					}
					else
					{
						if ( (i != c_f) && (i != f_max))
							ereq |= msk(i);
						else auto_reopen_door();//reopen door automatic
					}
					//cf_press=1;
					//get_ereq(i, ereq&msk(i));
				}
				if (collect == 2)
				{
					if (((get_eereq(i, eereq&msk(i)) == 1)) && (req_enabled))
					{
						if((i != c_f) && (i != 0))
							eereq |= msk(i);
						else auto_reopen_door();//reopen door automatic
						//cf_press=1;
						//get_eereq(i, eereq&msk(i));
					}
				}
				
				get_ireq(i, ireq&msk(i));
				get_ereq(i, ereq&msk(i));
				if (collect == 2) get_eereq(i, eereq&msk(i));
			}
		}
		if(!req_enabled) return;
		parking_check();
	}
}
#else
void req_scan(void)
{
	uint32_t i, c_f, req_disable_led;
	
	if(ireq|ereq|eereq) light_timer_reset();
	if(device_off == 0)
	{
		c_f = get_current_floor();

		for(i=0; i<28; i++)
		{
			if(i <= f_max)
			{
				if( (get_ireq(i, ireq&msk(i)) == 1) || (get_ereq(i, ereq&msk(i)) == 1) )
					light_timer_reset();
				if(collect == 2)
				if ((get_eereq(i, eereq&msk(i)) == 1)) light_timer_reset();
				
				
				if ((get_ireq(i, ireq & msk(i)) == 1) && (req_enabled))
				{
					if(i != c_f)
					ireq |= msk(i);
					//get_ireq(i, ireq&msk(i));
				}
				if ((get_ereq(i, ereq & msk(i)) == 1) && (req_enabled))
				{
					if(collect != 2)
					{
						if (i != c_f)
						ereq |= msk(i);
						else auto_reopen_door();//reopen door automatic
					}
					else
					{
						if ( (i != c_f) && (i != 0))
						ereq |= msk(i);
						else auto_reopen_door();//reopen door automatic
					}
				}
				if (collect == 2)
				{
					if (((get_eereq(i, eereq&msk(i)) == 1)) && (req_enabled))
					{
						if((i != c_f) && (i != f_max))
						eereq |= msk(i);
						else auto_reopen_door();//reopen door automatic
					}
				}
				
				get_ireq(i, ireq&msk(i));
				get_ereq(i, ereq&msk(i));
				if (collect == 2) get_eereq(i, eereq&msk(i));
				
				if (i != c_f)
				{
					set_ireq_ledMask(i, 1);
					set_ereq_ledMask(i, 1);
					if(collect == 2) set_eereq_ledMask(i, 1);
				}
				else
				{
					set_ireq_ledMask(i, 0);
					
					set_ereq_ledMask(i, ereq&msk(i));
					if(collect == 2) set_eereq_ledMask(i, eereq&msk(i));
				}
			}
		}
		if (f_max < 30)
		{
			//open and close door internal inputs
			set_ireq_ledMask (f_max + 1, 1);
			set_ireq_ledMask (f_max + 2, 1);
		}
		if(!req_enabled) return;
		parking_check();
	}
}
#endif