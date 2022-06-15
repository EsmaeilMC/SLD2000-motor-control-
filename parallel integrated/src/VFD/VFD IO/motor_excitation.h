/*
 * motor_excitation.h
 *
 * Created: 07/04/2020 05:39:03 م
 *  Author: ahmed
 */ 


#ifndef MOTOR_EXCITATION_H_
#define MOTOR_EXCITATION_H_

#include <asf.h>

void motor_excitation_handler(void);

void motor_excitation_connect_motor(void);
void motor_excitation_disconnect_motor(void);
uint8_t motor_excitation_is_motor_connected(void);

void BRKS_set(uint32_t bsign);
#endif /* MOTOR_EXCITATION_H_ */