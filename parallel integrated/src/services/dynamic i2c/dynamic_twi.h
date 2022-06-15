/*
 * dynamic_twi.h
 *
 * Created: 03/12/2015 03:26:04 م
 *  Author: ahmed
 */ 


#ifndef DYNAMIC_TWI_H_
#define DYNAMIC_TWI_H_

#include "compiler.h"
//#include <twi.h>

#define TWI_DYNAMIC_BUSY (0x1000ul)
#define TWI_COUNT (2)

#define DYNAMIC_TWI_IDLE	0
#define DYNAMIC_TWI_BUSY	1
#define DYNAMIC_TWI_ERROR	2
#define DYNAMIC_TWI_SBUSY	3
#define DYNAMIC_TWI_TCOM	4

#define DYNAMIC_TWI_T_success 0
#define DYNAMIC_TWI_T_BUSY 1

typedef struct
{
	//Twi *d_tw;
	uint32_t state;
	uint32_t time_counter; //to count time based on the sch tick
}twistatus_t;

void d_twi_handler(void);
void dtwi_init(void);
//uint32_t twi_master_write_pdc(uint32_t twi_instance, twi_packet_t *p_packet);
//uint32_t twi_master_read_pdc(uint32_t twi_instance, twi_packet_t *p_packet);
void d_twi_reset_state(uint32_t twi_instance);
uint32_t d_twi_get_state(uint32_t twi_instance);


#endif /* DYNAMIC_TWI_H_ */