/*
 * rtc_interface.h
 *
 * Created: 12/02/2019 04:55:45 م
 *  Author: ahmed
 */ 


#ifndef RTC_INTERFACE_H_
#define RTC_INTERFACE_H_

void rtc_init(void);
void rtc_date_check(uint32_t *p_ul_year, uint32_t *p_ul_month, uint32_t *p_ul_day);
void rtc_save_date(uint32_t year, uint32_t month, uint32_t day);
void rtc_time_check(uint32_t *p_ul_hour, uint32_t *p_ul_min, uint32_t *p_ul_sec);
void rtc_save_time(uint32_t hour, uint32_t min, uint32_t sec);
const char *rtc_get_week(uint32_t year, uint32_t month, uint32_t day);

#endif /* RTC_INTERFACE_H_ */