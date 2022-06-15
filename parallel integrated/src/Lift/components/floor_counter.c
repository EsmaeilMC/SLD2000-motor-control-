/*
 * floor_counter.c
 *
 * Created: 22/11/2015 12:56:28 ص
 *  Author: ahmed
 */ 

#include <floor_counter.h>
#include <SEDACIO.h>
#include <auto_state.h>
#include <control_status_register.h>
#include <def.h>
#include <req_io_mb_reader.h>
#include <menue_data_manager.h>
#include <e_faults.h>
#include <requests.h>
#include <EMCM.h>
#include <selfLearn.h>

uint_t selector_counter=4;



uByte_t get_current_floor(void)
{
	return (selector_counter>>1);
}

uByte_t infloorzone(void)
{
	if(selector_counter&1)
		return 0;
	return 1;
}

void display_out(char data)
{
	mb_display_data = menue_lift_data_applied.floor_display[data];
	mb_display_type(menue_lift_data_applied.m_display_type);		
}

void sel_set(char selector)
{

	selector_counter = selector;
	display_out(selector >> 1);

}

void selector_count(void)
{	
	if((sel) && !DIRECT_STOP)
	{
		if((auto_instate(auto_hspd)==1)||((auto_instate(auto_lspd)==1)&&(mnt==1)))//chek if car is moving
		{
			if(direction_up)
			{
				if(selector_counter < (f_max<<1))
				selector_counter++;
			}
			else
			{
				if(selector_counter > 0)
				selector_counter--;
			}
			if(fast_cntr < FAST_TIME_OUT)
				fast_cntr=0;//reset fast counter
		}
	}
	else if(DIRECT_STOP)
	{
		//direct stop mode
		//selector control is on encoder motion control module EMCM
		fast_cntr=0;//reset fast counter, no time out to be issued
	}
	display_out(selector_counter>>1);
}

char dir_limit_started = 1;
void direction_limits_check()
{
	char select;
	
	if(dir_limit_started) return; // to avoid direction limit startup fault

	select = selector_counter;
	
	if ((upfl == 0) && (dnfl == 0)) Final_limit_err;
	
	if ((upfdl0 == 0) && (dnfdl0 == 0)) 
	{
		dnfdl0_err;
		upfdl0_err;
	}
	
	if(auto_instate(auto_hspd) || auto_instate(auto_lspd))
	{
		if(dnfdl0==0)
		{
			if(!direction_up)
			{
				if (!DIRECT_STOP) sel_set(0);
				if(mnt==0)
				{
					if(!DIRECT_STOP) 
					{
						if(auto_instate(auto_hspd))
							auto_lspd_enter();
					}
					else
					{
						dnl0decel();
					}
				}
				else if(mnt==1)
				{
					if (((mstop == 0) && (dnfl == 2)) || (dnfl == 0))
					{
						auto_stopping_enter();
					}
					else
					{
						auto_lspd_creep();
					}
				}
			}
			else
			{
				if( (select>2) && (!DIRECT_STOP) )
				{
					//todo iniate error
					dnfdl0_err;
					drop_all_req();
					auto_stop_enter();
				}
			}
		}
		if(upfdl0==0)
		{
			if(direction_up)
			{
				if (!DIRECT_STOP) sel_set(f_max<<1);
				if(mnt==0)
				{
					if(!DIRECT_STOP)
					{ 
						if(auto_instate(auto_hspd))
							auto_lspd_enter();
					}
					else
					{
						upl0decel();
					}
					
				}
				else if(mnt==1)
				{
					if (((mstop == 0) && (upfl == 2)) || (upfl == 0))
					{
						if(selfLearn_isLearning() == 0) 
						{
							auto_stopping_enter();
						}
					} 
					else
					{
						auto_lspd_creep();
					}
				}
			}
			else
			{
				if ( (select < (((f_max-1)<<1)-1)) && (!DIRECT_STOP) )
				{
					//todo iniate error
					upfdl0_err;
					drop_all_req();
					auto_stop_enter();
				}
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		
		if(dnfl==0)
		{
			if(!direction_up)
			{
				if(mnt==0)
				{
					Final_limit_err;
				}
				auto_stopping_enter();
			}
		}
		if(upfl==0)
		{
			if(direction_up)
			{
				if(mnt==0)
				{
					Final_limit_err;
				}
				auto_stopping_enter();
			}
		}
	}
	else
	{
		if ((!DIRECT_STOP))
		{
			if(dnfdl0==0)
			{
				if(select>1)
				dnfdl0_err;
			}
			if(upfdl0==0)
			{
				if(select<((f_max<<1)-1))
				upfdl0_err;
			}
		}
	}
	
}

void dir_limit_startup()
{
	dir_limit_started = 0;
	if(0)//(upl == 0)&&(dnl == 0))
	{
		if(get_current_floor() == f_max)
		{
			//todo dnl error
			return;
		}
		else if(get_current_floor() == 0)
		{
			//todo dnl error
			return;
		}
		else
		{
			sel_set(0);
			//todo up and dn l error
			return;
		}
	}
	if(upfdl0==0)
	{
		sel_set((f_max<<1));
	}
	else if(dnfdl0==0)
	{
		sel_set(0);	
	}
}