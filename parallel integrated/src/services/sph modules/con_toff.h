/*
 * con_toff.h
 *
 * Created: 13/02/2019 06:38:49 م
 *  Author: ahmed
 */ 


#ifndef CON_TOFF_H_
#define CON_TOFF_H_

extern uint32_t device_off, toff_counter;

void con_toff_save_date(uint32_t year, uint32_t month, uint32_t day);
void con_toff_save_time(uint32_t hour, uint32_t min, uint32_t sec);
void con_toff_get_date(uint32_t *year, uint32_t *month, uint32_t *day);
void con_toff_get_time(uint32_t *hour, uint32_t *min, uint32_t *sec);
void con_toff_monitor_handler(void);

#endif /* CON_TOFF_H_ */