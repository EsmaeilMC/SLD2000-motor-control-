/*
 * floorDisplay.h
 *
 * Created: 09/06/2017 05:48:14 م
 *  Author: ahmed
 */ 


#ifndef FLOORDISPLAY_H_
#define FLOORDISPLAY_H_

#include <asf.h>

extern uint8_t start_floor_floor_display;

void floor_dislplay_menue_enter(void);
char floor_dislplay_menue_exe(char enter, char esc, char up, char down, char left, char right);

#endif /* FLOORDISPLAY_H_ */