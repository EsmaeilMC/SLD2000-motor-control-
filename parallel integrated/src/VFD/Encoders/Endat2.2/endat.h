/*
 * endat.h
 *
 * Created: 10/02/2018 07:01:58 م
 *  Author: ahmed
 */ 


#ifndef ENDAT_H_
#define ENDAT_H_

#include <GIO.h>
#include <fixedptc.h>

#define endat_clkena(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOD, 19), x)
#define endat_txena(x)  ioport_set_pin_level(IOPORT_CREATE_PIN(PIOD, 18), x)

#define ENDAT_MAX_VAL 8192

void endat_init();
void endat_handler(void);

uint16_t endat_getpos(void);
float endat_getangle(void);
float endat_getangle_elec(uint32_t pole_pair);
char endat_getfbits(void);
uint32_t makeCRCPos(uint16_t clocks, uint16_t AlmPos);

void encoder_endat_filter(void);
float endat_getangle_hat(void);

extern int32_t end_theta_hat;
#endif /* ENDAT_H_ */