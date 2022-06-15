/*
 * SCH.h
 *
 * Created: 19/11/2015 09:56:42 ص
 *  Author: ahmed
 */ 

#include <def.h>

#ifndef SCH_H_
#define SCH_H_

//#define SCH_TASK_MAX 25
#define SCH_TICK_FREQ (10000ul)
#define sch_ms_to_tics(x) ((x*SCH_TICK_FREQ)/1000)

#define sch_add_task_ms(sch_ms_task, sch_ms_dalay, sch_ms_period)  \
			sch_add_task(sch_ms_task,sch_ms_to_tics(sch_ms_dalay), \
				sch_ms_to_tics(sch_ms_period), 1)

#define sch_add_task_ms_noOD(sch_ms_task, sch_ms_dalay,sch_ms_period)  \
						sch_add_task(sch_ms_task,sch_ms_to_tics(sch_ms_dalay), \
						sch_ms_to_tics(sch_ms_period), 0)
						
#define sch_add_task_100us_noOD(sch_ms_task, sch_ms_dalay,sch_ms_period)  \
						sch_add_task(sch_ms_task,sch_ms_to_tics(sch_ms_dalay), \
						(sch_ms_period), 0)

#define sch_add_task_100us(sch_ms_task, sch_ms_dalay,sch_ms_period)  \
						sch_add_task(sch_ms_task,sch_ms_to_tics(sch_ms_dalay), \
						(sch_ms_period), 1)

#define sch_edit_task_period_ms(sch_index, sch_ms_period)  \
			sch_edit_task_period(sch_index, sch_ms_to_tics(sch_ms_period))

void sch_update(void);
uByte_t sch_add_task(void (*a_task_handler)(void),
					uint_t task_delay,
					uint_t task_period, uint_t overdue_enable);

void sch_dispatch_tasks(void);
void sch_delete_task(uByte_t task_id);
void sch_init(void);
uByte_t sch_edit_task_period(uByte_t t_index, uint_t task_period);

#endif /* SCH_H_ */