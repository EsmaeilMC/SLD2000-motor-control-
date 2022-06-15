/*
 * vectorIM.h
 *
 * Created: 31/03/2017 10:31:57 ص
 *  Author: ahmed
 */ 


#ifndef VECTORIM_H_
#define VECTORIM_H_

#include <fixedptc.h>

void vector_IM_init(void);

void vectorIM_perform(float desired_tourque, float desired_flux_ratio);
float vectorIM_get_estimated_shaft_speed_Hz(void);
float vectorIM_get_estimated_shaft_speed_rad(void);
void vectorIM_set_shaft_speed_Hz(float sh_speed);
void vectorIM_activate_reg_reset(void);

#endif /* VECTORIM_H_ */