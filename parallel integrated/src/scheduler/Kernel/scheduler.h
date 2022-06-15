/*
 * scheduler.h
 *
 * Created: 19/11/2015 09:45:40 ص
 *  Author: ahmed
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <def.h>
#include <SCH.h>

#ifndef SCH_TASK_MAX
#define SCH_TASK_MAX 40
# warning SCH_TASK_MAX not defined.

#endif

#define SCH_TASKLIST_FULL 0XFF

struct task_t   
	{
		//the task handler at which it will be executed
		void ( *task_handler ) (void);
		
		// the delay period which will be loaded into delay variable after every task execution
		uint_t period;
		
		//the delay counter
		uint_t delay;
		
		//this variable holds how many times the task is overdue to execute.
		uByte_t run_me;
		
		//this variable holds if we should multi-exe the task when it is overdue.
		uByte_t overdue_enable;
	}sTasks[SCH_TASK_MAX];

void sch_init_timer(void);
#endif /* SCHEDULER_H_ */