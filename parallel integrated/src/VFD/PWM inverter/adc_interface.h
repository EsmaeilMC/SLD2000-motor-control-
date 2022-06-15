/*
 * adc_interface.h
 *
 * Created: 06/03/2017 06:43:50 م
 *  Author: ahmed
 */ 


#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

void adc_interface_init(void);
void afec_EOC_handler (void);
void afec1_EOC_handler (void);

#endif /* ADC_INTERFACE_H_ */