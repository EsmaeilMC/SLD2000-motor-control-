/*
 * scheduler.c
 * basic function to run a time trigger embedded system.
 	
 * Created: 19/11/2015 09:44:37 ص
 *  Author: Ahmed
 */ 

#include <scheduler.h>
#include <pmc.h>
#include <tc.h>


void sch_update(void)
{
	uByte_t t_index;
	
	for (t_index=0; t_index < SCH_TASK_MAX; t_index++)
	{
		if (sTasks[t_index].task_handler > 0)
		{
			// There is a task handler at this pointer
			if (sTasks[t_index].delay == 0)
			{
				//the task is due to run
				sTasks[t_index].run_me++;
				if (!sTasks[t_index].overdue_enable)
				{
					if (sTasks[t_index].run_me > 1)
					{
						sTasks[t_index].run_me = 1;
					}
				}
				if(sTasks[t_index].period)
				{
					//Set periodic tasks to run again
					sTasks[t_index].delay = sTasks[t_index].period - 1;
				}
			} 
			else
			{
				sTasks[t_index].delay--;
			}
		}
	}
}

uByte_t sch_add_task(void (*a_task_handler)(void),
					uint_t task_delay,
					uint_t task_period, uint_t overdue_enable)
{
	uByte_t t_index = 0;
	while((sTasks[t_index].task_handler != 0) && (t_index < SCH_TASK_MAX))
	{
		//search for empty task record space
		t_index++;
	}
	
	if(t_index == SCH_TASK_MAX)
	{
		//we've reached the max tasks allowed
		
		return SCH_TASKLIST_FULL;
	}
	
	//if you passed all of the above then you have empty record in your list
	
	sTasks[t_index].task_handler = a_task_handler;
	
	sTasks[t_index].delay = task_delay;
	sTasks[t_index].period = task_period;
	sTasks[t_index].run_me = 0;
	if(overdue_enable) sTasks[t_index].overdue_enable = 1;
	else sTasks[t_index].overdue_enable = 0;
	
	return t_index;
}

void sch_dispatch_tasks(void)
{
	uByte_t t_index;
	
	for (t_index=0; t_index < SCH_TASK_MAX; t_index++)
	{
		if (sTasks[t_index].task_handler) if(sTasks[t_index].run_me)
		{
			(*sTasks[t_index].task_handler)();
			sTasks[t_index].run_me--;
			if (sTasks[t_index].period == 0)
			{
				//delete the one off task
				sch_delete_task(t_index);
			}
		}
	}
}

void sch_delete_task(uByte_t task_id)
{
	if(task_id >= SCH_TASK_MAX)	return;
	
	sTasks[task_id].task_handler=0;
	sTasks[task_id].period=0;
	sTasks[task_id].delay=0;
	sTasks[task_id].run_me=0;
}
void sch_init(void)
{
	uByte_t t_index;
	sch_init_timer();
	for (t_index=0; t_index<SCH_TASK_MAX; t_index++)
	{
		sch_delete_task(t_index);
	}
}

void sch_init_timer(void)
{
	//timer counter TC2 initialization for use with the schedular
	//return;
	pmc_enable_periph_clk(ID_TC7);
	
	tc_init(TC2, 1, TC_CMR_TCCLKS_TIMER_CLOCK2|TC_CMR_WAVSEL_UP_RC); //TC8 clk = MCK/8 = 18750000Hz
	tc_write_rc(TC2, 1, (18750000) / SCH_TICK_FREQ);
	tc_enable_interrupt(TC2, 1, TC_IER_CPCS);
	NVIC_SetPriority(TC7_IRQn,3);
	NVIC_EnableIRQ(TC7_IRQn);
	tc_start(TC2, 1);
}


uByte_t sch_edit_task_period(uByte_t t_index, uint_t task_period)
{
	if(t_index >= SCH_TASK_MAX) return 1;
	if(sTasks[t_index].task_handler==0) return 1;
	
	sTasks[t_index].period = task_period;
	return 0;
}