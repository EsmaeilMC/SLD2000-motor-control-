/*
 * regulators.h
 *
 * Created: 30/03/2017 03:25:29 م
 *  Author: ahmed
 */ 


#ifndef REGULATORS_H_
#define REGULATORS_H_

#include <fixedptc.h>

typedef struct pi_t
{
	float kpc;
	float kic;
	float ksc;
	float imemory;
};

void reg_pi_change_gains(float kpc, float kic, struct pi_t *regulator);
void reg_pi_change_ki(float kic, struct pi_t *regulator);
void reg_pi_change_kp(float kpc, struct pi_t *regulator);
void reg_pi_init(float kpc, float kic, struct pi_t *regulator);
float reg_pi_exe(float sat_limit, float err, float ff, float reg_tsample, struct pi_t *regulator);
void reg_pi_add_i_memory_val(float val, struct pi_t *regulator);
void reg_pi_reset_memory(struct pi_t *regulator);

#endif /* REGULATORS_H_ */