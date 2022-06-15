/*
 * elevator_controller.h
 *
 * Created: 19/11/2015 03:05:42 م
 *  Author: ahmed
 */ 


#ifndef ELEVATOR_CONTROLLER_H_
#define ELEVATOR_CONTROLLER_H_

#define TIMER_TICK_DELAY_PER_SEC 100
#define TIMER_DELAY_MS (1000/TIMER_TICK_DELAY_PER_SEC)
#define AUTO_STATE_UPDATE_SEC 25
#define AUTO_STATE_PERIOD_MS (1000/AUTO_STATE_UPDATE_SEC)

#include <auto_state.h>
#include <SCH.h>
#include <graphics_data.h>
#include <u_glcd.h>

void elevator_init(void);
void flash_LED(void);


#endif /* ELEVATOR CONTROLLER_H_ */