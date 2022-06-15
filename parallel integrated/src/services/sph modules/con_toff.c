/*
 * con_toff.c
 *
 * Created: 13/02/2019 06:38:58 م
 *  Author: ahmed
 */ 

#include <asf.h>
#include <pop_ups.h>
#include <rtc_interface.h>
#include <con_toff.h>
#include <menue_data_manager.h>

uint32_t device_off = 0, toff_counter = 0;

void con_toff_save_date(uint32_t year, uint32_t month, uint32_t day)
{
	menue_lift_data_setup.year = year;
	menue_lift_data_setup.month = month;
	menue_lift_data_setup.day = day;
}

void con_toff_save_time(uint32_t hour, uint32_t min, uint32_t sec)
{
	menue_lift_data_setup.hour = hour;
	menue_lift_data_setup.minute = min;
	menue_lift_data_setup.second = sec;
}

void con_toff_get_date(uint32_t *year, uint32_t *month, uint32_t *day)
{
	*year = menue_lift_data_setup.year;
	*month = menue_lift_data_setup.month;
	*day = menue_lift_data_setup.day;
}

void con_toff_get_time(uint32_t *hour, uint32_t *min, uint32_t *sec)
{
	*hour = menue_lift_data_setup.hour;
	*min = menue_lift_data_setup.minute;
	*sec = menue_lift_data_setup.second;
}

uint32_t con_toff_comp_services(void)
{
	if(toff_counter >= menue_lift_data_applied.services)
	{
		return 1;
	}
	return 0;
}

uint32_t con_toff_comp_time(void)
{
	uint32_t year, month, day, hour, min, sec;
	rtc_get_date(RTC, &year, &month, &day, 0);
	rtc_get_time(RTC, &hour, &min, &sec);
	
	if (year >= menue_lift_data_applied.year)
	if (month >= menue_lift_data_applied.month)
	if (day >= menue_lift_data_applied.day)
	if (hour >= menue_lift_data_applied.hour)
	if (min >= menue_lift_data_applied.minute)
	if (sec >= menue_lift_data_applied.second) 
	return 1;
	return 0;
}

void con_toff_monitor_handler(void)
{
	if (menue_lift_data_applied.m_cont_dis_technique == 1) device_off = con_toff_comp_services();
	else if (menue_lift_data_applied.m_cont_dis_technique == 2) device_off = con_toff_comp_time();
	else 
	{
		device_off = 0;
		toff_counter = 0;
	}
}