/*
 * VectorPM.h
 *
 * Created: 28/02/2018 06:55:48 م
 *  Author: ahmed
 */ 


#ifndef VECTORPM_H_
#define VECTORPM_H_

#include <fixedptc.h>

void vectorPM_perform(float desired_tourque);
void vector_PM_init(void);
void vectorpm_PI_changeGains(void);
void vectorPM_activate_reg_reset(void);

#endif /* VECTORPM_H_ */