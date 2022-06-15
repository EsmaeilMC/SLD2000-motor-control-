/*
 * GIO.h
 *
 * Created: 23/03/2017 01:32:36 م
 *  Author: ahmed
 */ 


#ifndef GIO_H_
#define GIO_H_


#include <pio.h>
#include <def.h>

void pio_set_lc_writeProtect(void);
void pio_set_data(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_data);
void pio_out_data(Pio *p_pio, uint32_t ul_mask, uint32_t ul_data, const uint32_t shift);


#endif /* GIO_H_ */