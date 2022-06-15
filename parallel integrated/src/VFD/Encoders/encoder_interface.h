/*
 * encoder_interface.h
 *
 * Created: 24/03/2017 07:01:19 م
 *  Author: ahmed
 */ 


#ifndef ENCODER_INTERFACE_H_
#define ENCODER_INTERFACE_H_

#include <incremental.h>
#include <endat.h>
#include <sincos.h>

void encoder_init(void);
void encoder_handler(void);
float encoder_getangle_abs(void);
float encoder_getangle_elec_abs(uint32_t pole_pair);


#endif /* ENCODER_INTERFACE_H_ */