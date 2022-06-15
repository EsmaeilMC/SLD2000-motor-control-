/*
 * elevator_controller.c
 *
 * Created: 19/11/2015 05:05:35 ã
 *  Author: ahmed
 */ 
#include <elevator_controller.h>
// #include <control_status_register.h>
#include <controller_lcd_manager.h>
#include <timers.h>
#include <e_setup.h>
#include <e_faults.h>
#include <SEDACIO.h>
#include <menues.h>
#include <vfdIO.h>
#include <dynamic_twi.h>
#include <AT24Cxx_dynamic.h>
#include <req_io_mb_reader.h>
#include <requests.h>
#include <floor_counter.h>

void init_elevator_values(void)
{
	idle_camt_init(TIMER_TICK_DELAY_PER_SEC / 2);
}

uByte_t heart_beat_flasher_index;
#define  LIFT_INIT_DELAY 4800
void elevator_init(void)
{
// 	dtwi_init();
// 	
 	sch_add_task_ms(&scan_buttons_handler,75 + LIFT_INIT_DELAY,1);
 	sch_add_task_ms(&auto_update, 20 + LIFT_INIT_DELAY, AUTO_STATE_PERIOD_MS);
 	sch_add_task_ms(&controller_lcd_manager_handler,50 + LIFT_INIT_DELAY,160);
 	sch_add_task_ms(&timer_count_handler,5 + LIFT_INIT_DELAY, TIMER_DELAY_MS);
	sch_add_task_ms(&req_io_mb_scan_handler,5 + LIFT_INIT_DELAY, 80);
	sch_add_task_ms(&req_scan, 5 + LIFT_INIT_DELAY, 40);
 	sch_add_task_ms(&dir_limit_startup, (LIFT_INIT_DELAY - 148), 0);
	heart_beat_flasher_index = sch_add_task_ms(&ST2_LED,0 + LIFT_INIT_DELAY,1000); //heart beat flash
	sch_add_task_ms(&init_menues, (LIFT_INIT_DELAY - 720), 0);
	//init_requests(LIFT_INIT_DELAY + 1343); 

 	init_elevator_values();
	e_faults_inti();
	
	init_glcd();
	glcd_clear();
 	out_image(20, 0, intro_logo);
	lcd_update();
	
	sch_add_task_ms(&auto_stop_enter, (LIFT_INIT_DELAY - 200), 0);
	timers_init();	
}


