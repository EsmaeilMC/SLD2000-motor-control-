/*
 * pop_ups.h
 *
 * Created: 12/02/2019 12:41:26 م
 *  Author: ahmed
 */ 


#ifndef POP_UPS_H_
#define POP_UPS_H_
#include <asf.h>

void pop_ups_handler(void);
int32_t pop_ups_showMessage(void);
void pop_ups_add(const char *str, uint32_t ms_time);

#endif /* POP_UPS_H_ */