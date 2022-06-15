/*
 * pop_ups.c
 *
 * Created: 12/02/2019 12:41:13 م
 *  Author: ahmed
 */ 

#include <pop_ups.h>
#include <u_glcd.h>

uint32_t pups_timelapse = 0;
const char *msg;
void pop_ups_handler(void)
{
	if (pups_timelapse > 0) pups_timelapse--;
}

int32_t pop_ups_showMessage(void)
{
	if (pups_timelapse > 0)
	{
		glcd_clear();
		glcd_gotoxy(4,32);
		write_r_text(msg ,0);
		
		return 1;
	}
	return 0;
}

void pop_ups_add(const char *str, uint32_t ms_time)
{
	pups_timelapse = ms_time;
	msg = str;
}