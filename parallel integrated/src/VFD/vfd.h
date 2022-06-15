/*
 * vfd.h
 *
 * Created: 27/01/2017 10:53:18 ص
 *  Author: ahmed
 */ 


#ifndef VFD_H_
#define VFD_H_

//#define VFD5K5
#define VFD7K5
//#define VFD11K

#include <vfdIO.h>
#include <motor_data.h>
#include <fixedptc.h>
#include <vfdstates.h>
#include <vfd_enable.h>
#include <ersdstates.h>

extern float cvd, cvq, cid, ciq;

//acceleration deceleration and jerk
typedef struct
{
	fixedpt a;
	fixedpt j1;
	fixedpt j2;
}motion_dynamics_t;

void vfd_speed_loop_handler(void);
void vfd_current_loop_handler(void);
void vfd_set_ref_speed(fixedpt speed);
void vfd_set_decel_profile(int32_t sgn);
void vfd_set_decel_profilez(void);
void vfd_set_accel_profile(fixedpt speed);
fixedpt vfd_get_ref_speed(void);
fixedpt vfd_get_desired_speed(void);
fixedpt vfd_get_desired_distance(void);
fixedpt vfd_get_ref_acceleration(void);

void vfd_set_motion_dynamics(motion_dynamics_t motion_profile);
void vfd_estop(void);
void vfd_trip(void);

void regulators_init(void);
void vfd_activate_reg_reset(void);

void vfd_set_motion_acc(fixedpt acc);
void vfd_set_motion_j1(fixedpt j1);
void vfd_set_motion_j2(fixedpt j2);
int32_t vfd_get_Max_possible_speed(int64_t int_displacement, fixedpt *r_speed, int32_t *stopping_displacement);
int32_t vfd_is_constant_speed(void);
uint32_t vfd_get_accelerating_stage(void);

int32_t vfd_get_l_dir(void);

void vfd_set_c_decay(uint32_t decay_period);
void vfd_clear_c_decay(void);
void vfd_set_flux_raise(uint32_t raise_period);

void vfd_set_regencount(void);
void vfd_clr_regencount(void);
uint32_t vfd_is_motor_regen(void);
#endif /* VFD_H_ */