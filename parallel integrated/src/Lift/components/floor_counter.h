/*
 * floor_counter.h
 *
 * Created: 22/11/2015 12:56:12 ص
 *  Author: ahmed
 */ 


#ifndef FLOOR_COUNTER_H_
#define FLOOR_COUNTER_H_
#include <def.h>

uByte_t get_current_floor(void);
uByte_t infloorzone(void);
void selector_count(void);
void direction_limits_check();
void dir_limit_startup();
void sel_set(char selector);

extern uint_t selector_counter;

#endif /* FLOOR_COUNTER_H_ */