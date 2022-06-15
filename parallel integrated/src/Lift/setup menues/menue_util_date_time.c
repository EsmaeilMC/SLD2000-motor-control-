/*
 * menue_util_date_time.c
 *
 * Created: 12/02/2019 08:19:46 م
 *  Author: ahmed
 */ 

#include <menue_util_date_time.h>

//util io list
menue_design_t menue_util_date_time;
#define OPT_UTIL_DATE_TIME_MAX 2
option_t opt_util_date_time[OPT_UTIL_DATE_TIME_MAX];

#define udate	          (opt_util_date_time[0])
#define utime	          (opt_util_date_time[1])


menue_design_t *menue_util_date_time_init(void)
{
	udate.text = "Set date";
	udate.option_save_val = 1; //set date option
	//////////////////////////////////////////////////////////////////////////
	
	utime.text = "Set time";
	utime.option_save_val = 2; //set time option
	//////////////////////////////////////////////////////////////////////////

	fill_menue_list(&menue_util_date_time, opt_util_date_time, OPT_UTIL_DATE_TIME_MAX, 0, 0);

	return &menue_util_date_time;
}