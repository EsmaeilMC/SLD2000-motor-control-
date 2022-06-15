/*
 * DACC_interface.h
 *
 * Created: 26/04/2018 09:52:58 ص
 *  Author: ahmed
 */ 


#ifndef DACC_INTERFACE_H_
#define DACC_INTERFACE_H_

#include <asf.h>

void dac_interface_init(void);

void dacc_interface_writeData(uint16_t Dchannel, float val, float val_max, uint16_t signed_data);

#endif /* DACC_INTERFACE_H_ */