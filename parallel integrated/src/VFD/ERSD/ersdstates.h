/*
 * ersdstates.h
 *
 * Created: 24/11/2017 11:20:40 ص
 *  Author: ahmed
 */ 


#ifndef ERSDSTATES_H_
#define ERSDSTATES_H_


void ersdstates_handler(void);

uint32_t ersd_isReady(void);
void ersd_move(void);
void ersd_stop(void);
uint32_t ersd_isOff(void);
void ersd_force_turn_off();
void ersd_allowTo_turn_on();

#endif /* ERSDSTATES_H_ */