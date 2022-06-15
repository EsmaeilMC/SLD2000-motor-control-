/*
 * scurve_speed.h
 *
 * Created: 23/02/2017 04:17:29 م
 *  Author: ahmed
 */ 


#ifndef SCURVE_SPEED_H_
#define SCURVE_SPEED_H_

#include <fixedptc.h>
#include <vfd.h>

typedef struct
{
	fixedpt j;
	fixedpt a;
	fixedpt w;
	fixedpt duration;	
}speedProfile_t;


extern speedProfile_t scurve_speed[];
motion_dynamics_t scurve_get_abs_from_relative(motion_dynamics_t relative_motion);
void scurve_generate_speed_profile(fixedpt c_speed, fixedpt d_speed, fixedpt c_acceleration, motion_dynamics_t motion, speedProfile_t *speedprof );
void scurve_apply_precalc_decel_profile(void);
void scurve_apply_precalc_accel_profile(void);
void scurve_apply_precalc_decel_profilez(void);
void scurve_exe(speedProfile_t *speedprof, fixedpt t, fixedpt *speed, fixedpt *distance, fixedpt *acc, uint32_t *prof_end);
fixedpt scurve_get_maximumSpeed_fromDisplacement(fixedpt displacement,fixedpt Mspeed, fixedpt c_speed,
												fixedpt c_acceleration, motion_dynamics_t acc_motion,
												motion_dynamics_t dec_motion, motion_dynamics_t dec_motionz,
												fixedpt creep_speed, fixedpt *r_speed, fixedpt *decel_distancez);

#endif /* SCURVE_SPEED_H_ */