/*
 * floorDisplay.c
 *
 * Created: 09/06/2017 05:47:48 م
 *  Author: ahmed
 */ 

#include <floorDisplay.h>
#include <u_glcd.h>
#include <menue_data_manager.h>

uint8_t start_floor_floor_display = 0;
#define  DISPLAY_OPTIONS_MAX 10
const char dispOptions[] = "0123456789";
char dispOptionsCounter[2];

char disSegmentData[] = "00", displayIndex=0;


void floor_display_save_data(void)
{
	volatile char floor_disp_val = (disSegmentData[0] - 0x30) * 10;
	floor_disp_val += (disSegmentData[1] - 0x30);
	menue_lift_data_setup.floor_display[start_floor_floor_display] = floor_disp_val;
	menue_lift_data_newdata_flag = 1;	
}
void floor_display_analyze(void)
{
	#define floor_display_floor menue_lift_data_setup.floor_display[start_floor_floor_display]
	if (floor_display_floor > 100) floor_display_floor = 100;
	disSegmentData[0] = floor_display_floor / 10;
	disSegmentData[1] = floor_display_floor - disSegmentData[0] * 10;
	disSegmentData[0] += 0x30;
	disSegmentData[1] += 0x30;
}
void floor_dislplay_menue_enter(void)
{
	displayIndex = 0;
	floor_display_analyze();
}

char floor_dislplay_menue_exe(char enter, char esc, char up, char down, char left, char right)
{
	if (esc)
	{
		return 0;
	}
	else if(right)
	{
		if(++displayIndex > 1) displayIndex = 1;
	}
	else if (left)
	{
		if(displayIndex > 0) displayIndex--;
	}
	else if(up)
	{
		if(++dispOptionsCounter[displayIndex] >= DISPLAY_OPTIONS_MAX) dispOptionsCounter[displayIndex] = DISPLAY_OPTIONS_MAX -1;
		disSegmentData[displayIndex] = dispOptions[dispOptionsCounter[displayIndex]];
	}
	else if (down)
	{
		if(dispOptionsCounter[displayIndex] > 0) dispOptionsCounter[displayIndex]--;
		disSegmentData[displayIndex] = dispOptions[dispOptionsCounter[displayIndex]];
	}
	if (enter)
	{
		floor_display_save_data();
		if(++start_floor_floor_display >= 24) 
		{
			start_floor_floor_display = 0;
			return 0;
		}
		floor_display_analyze();
	}
	
	glcd_gotoxy(1, 18);
	write_r_text("Floor(", 0);
	glcd_puts(itos(start_floor_floor_display), 0);
	write_r_text(")", 0);
	
	glcd_gotoxy(50, 54);
	out_char(disSegmentData[0], displayIndex == 0);
	out_char(disSegmentData[1], displayIndex == 1);

	return 1;
}