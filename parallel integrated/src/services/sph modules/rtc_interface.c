/*
 * rtc_interface.c
 *
 * Created: 12/02/2019 04:55:55 م
 *  Author: ahmed
 */ 

#include <asf.h>
#include <pop_ups.h>
#include <rtc_interface.h>

const uint8_t month_max[] = {31, 29, 31, 30 ,31, 30, 31, 31, 30, 31, 30, 31};
#define M_YEAR  (2096) 
#define M_MONTH (02)
#define M_WEEK  (4)
#define M_DAY   (29)


void RTC_Handler (void)
{
	uint32_t status = rtc_get_status(RTC);
	if (status & RTC_SR_ALARM)
	{
		rtc_disable_interrupt(RTC, RTC_IDR_ALRDIS);
		rtc_clear_date_alarm(RTC);
		rtc_clear_time_alarm(RTC);
		rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
		rtc_enable_interrupt(RTC, RTC_IER_ALREN);
	}
}

void rtc_init(void)
{
	volatile uint32_t year, day, week, month;
	rtc_get_date(RTC, &year, &month, &day, &week);
	//rtc_set_time_alarm(RTC, 1, 0, 1, 10, 1, 1);
	//rtc_set_date_alarm(RTC, 0, 1, 0, 1);
	//NVIC_EnableIRQ(RTC_IRQn);
	//rtc_enable_interrupt(RTC, RTC_IER_ALREN);
	
	return;

	//rtc_set_date(RTC, M_YEAR, M_MONTH, M_DAY, M_WEEK);
	if(year < M_YEAR)
	{
		rtc_set_date(RTC, M_YEAR, M_MONTH, M_DAY, M_WEEK);
	}
}

void rtc_date_check(uint32_t *p_ul_year, uint32_t *p_ul_month, uint32_t *p_ul_day)
{
	if (*p_ul_year < 10) *p_ul_year = 10;
	else if (*p_ul_year > 99) *p_ul_year = 99;
	
	if (*p_ul_month < 1) *p_ul_month = 1;
	else if (*p_ul_month > 12) *p_ul_month = 12;
	
	if (*p_ul_day < 1) *p_ul_day = 1;
	else if (*p_ul_day > month_max[*p_ul_month - 1]) *p_ul_day = month_max[*p_ul_month - 1];
	
	if ((*p_ul_month == 2) && (*p_ul_day == 29))
	{
		if (((*p_ul_year) % 4) != 0) (*p_ul_day)--;
	}
}

void rtc_save_date(uint32_t year, uint32_t month, uint32_t day)
{
	//rtc_set_date(RTC, year + 2000, month, day, 1);
	pop_ups_add("Date set!", 1200);
}

void rtc_time_check(uint32_t *p_ul_hour, uint32_t *p_ul_min, uint32_t *p_ul_sec)
{
	uint32_t tmp;
	if (*p_ul_hour > 23) *p_ul_hour = 23;
	
	if (*p_ul_min > 59) *p_ul_min = 59;
	
	if (*p_ul_sec > 59) *p_ul_sec = 59;

}

void rtc_save_time(uint32_t hour, uint32_t min, uint32_t sec)
{
	//rtc_set_time(RTC, hour, min, sec);
	pop_ups_add("Time set!", 1200);
}

/**
 * \brief Calculate week from year, month, day.
 */
static uint32_t calculate_week(uint32_t ul_year, uint32_t ul_month,
		uint32_t ul_day)
{
	uint32_t ul_week;

	if (ul_month == 1 || ul_month == 2) {
		ul_month += 12;
		--ul_year;
	}

	ul_week = (ul_day + 2 * ul_month + 3 * (ul_month + 1) / 5 + ul_year +
			ul_year / 4 - ul_year / 100 + ul_year / 400) % 7;

	++ul_week;

	return ul_week;
}

const char *rtc_get_week_name(uint32_t week)
{
	switch(week)
	{
		case 6:
			return "Sat";
			break;
		case 7:
			return "Sun";
			break;
		case 1:
			return "Mon";
			break;
		case 2:
			return "Tue";
			break;
		case 3:
			return "Wed";
			break;
		case 4:
			return "Thu";
			break;
		default:
			return "Fri";
	}
	
}
const char *rtc_get_week(uint32_t year, uint32_t month, uint32_t day)
{
	return rtc_get_week_name(calculate_week(year, month, day));
}