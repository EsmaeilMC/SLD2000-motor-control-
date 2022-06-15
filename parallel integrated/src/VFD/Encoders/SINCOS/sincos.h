/*
 * sincos.h
 *
 * Created: 24/10/2018 07:12:28 م
 *  Author: ahmed
 */ 


#ifndef SINCOS_H_
#define SINCOS_H_

void sincos_store_abs_adc_val(uint16_t *adc_reading);
void sincos_store_val_abs(void);
float sincos_getangle(void);
float sincos_getangle_elec(uint32_t pole_pair);

#endif /* SINCOS_H_ */