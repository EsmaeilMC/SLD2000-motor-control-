/*
 * scurve_speed.c
 *
 * Created: 23/02/2017 04:17:54 م
 *  Author: ahmed
 */ 

#include <scurve_speed.h>

#define motor_basefreq (menue_drive_data_applied.m_motor_rated_freq)

speedProfile_t scurve_speed[3];
speedProfile_t acc_profile[3], dec_profile[3], dec_profilez[3];

motion_dynamics_t scurve_get_abs_from_relative(motion_dynamics_t relative_motion)
{
	motion_dynamics_t temp;
	
	temp.a  = fixedpt_div(motor_basefreq, relative_motion.a);
	temp.j1 = fixedpt_mul(relative_motion.a, relative_motion.j1);
	temp.j2 = fixedpt_mul(relative_motion.a, relative_motion.j2);
	temp.j1 = fixedpt_div(temp.a, temp.j1);
	temp.j2 = fixedpt_div(temp.a, temp.j2);
	temp.j2 *= -1;
	
	return temp;
}

fixedpt get_displacement_from_sprofileUnit(speedProfile_t sprofile)
{
	fixedpt t_distance, t_time;
	t_time = fixedpt_mul(sprofile.duration, sprofile.duration);
	
	t_distance = fixedpt_mul(sprofile.w, sprofile.duration) + (fixedpt_mul(sprofile.a, t_time)>>1);
	
	t_time = fixedpt_mul(t_time, sprofile.duration);
	t_distance += (fixedpt_mul(sprofile.j, t_time)/6);
	
	return t_distance;
}

fixedpt get_displacement_from_sprofile(speedProfile_t *sprofile)
{
	fixedpt t_distance = get_displacement_from_sprofileUnit(*sprofile);
	t_distance += get_displacement_from_sprofileUnit(*(sprofile+1));
	t_distance += get_displacement_from_sprofileUnit(*(sprofile+2));
	
	return t_distance;
}

#define SPEED_DIVISION (+20l)
fixedpt scurve_get_maximumSpeed_fromDisplacement(fixedpt displacement,fixedpt Mspeed, fixedpt c_speed, 
							fixedpt c_acceleration, motion_dynamics_t acc_motion, 
							motion_dynamics_t dec_motion, motion_dynamics_t dec_motionz,
							fixedpt creep_speed, fixedpt *r_speed, fixedpt *decel_distancez)
{
	fixedpt decel_displacement, AccDec_displacement;
	fixedpt d_speed = Mspeed;
	uint32_t i;
	
	if(acc_motion.a == 0)  acc_motion.a  = fixedpt_rconst(2.5);
	if(acc_motion.j1 == 0) acc_motion.j1 = fixedpt_rconst(0.1);
	if(acc_motion.j2 == 0) acc_motion.j2 = fixedpt_rconst(0.1);
	
	if(dec_motion.a == 0)  dec_motion.a  = fixedpt_rconst(2.5);
	if(dec_motion.j1 == 0) dec_motion.j1 = fixedpt_rconst(0.1);
	if(dec_motion.j2 == 0) dec_motion.j2 = fixedpt_rconst(0.1);
	
	if(dec_motionz.a == 0)  dec_motionz.a  = fixedpt_rconst(2.5);
	if(dec_motionz.j1 == 0) dec_motionz.j1 = fixedpt_rconst(0.1);
	if(dec_motionz.j2 == 0) dec_motionz.j2 = fixedpt_rconst(0.1);
	
	acc_motion = scurve_get_abs_from_relative(acc_motion);
	dec_motion = scurve_get_abs_from_relative(dec_motion);
	dec_motionz = scurve_get_abs_from_relative(dec_motionz);
	scurve_generate_speed_profile(creep_speed, 0, 0, dec_motionz, &dec_profilez);
	*decel_distancez = get_displacement_from_sprofile(&dec_profilez);
	displacement -= *decel_distancez;
	
	//volatile float dis_dec, dis_all;	
	for (i=0; i<SPEED_DIVISION; i++)
	{
		//generate curve details
		scurve_generate_speed_profile(c_speed, d_speed, c_acceleration, acc_motion, &acc_profile);
		scurve_generate_speed_profile(d_speed, creep_speed, 0, dec_motion, &dec_profile);
		decel_displacement = get_displacement_from_sprofile(&dec_profile);
		AccDec_displacement = get_displacement_from_sprofile(&acc_profile) + decel_displacement;
		if (fixedpt_abs(AccDec_displacement) < fixedpt_abs(displacement))
		{
			//dis_dec = fixedpt_tofloat(decel_displacement);
			//dis_all = fixedpt_tofloat(AccDec_displacement);
			*r_speed = d_speed; //return running speed
			return decel_displacement;
		}
		
		d_speed  -=  (Mspeed / SPEED_DIVISION);
		//dis_dec = fixedpt_tofloat(d_speed);
		//dis_dec *= 1;
	}

	//return 0;
}

void scurve_apply_precalc_accel_profile(void)
{
	uint32_t i;
	for (i=0; i<3; i++)
	{
		scurve_speed[i].a = acc_profile[i].a;
		scurve_speed[i].j = acc_profile[i].j;
		scurve_speed[i].w = acc_profile[i].w;
		scurve_speed[i].duration = acc_profile[i].duration;
	}
}

void scurve_apply_precalc_decel_profile(void)
{
	uint32_t i;
	for (i=0; i<3; i++)
	{
		scurve_speed[i].a = dec_profile[i].a;
		scurve_speed[i].j = dec_profile[i].j;
		scurve_speed[i].w = dec_profile[i].w;
		scurve_speed[i].duration = dec_profile[i].duration;
	}
}

void scurve_apply_precalc_decel_profilez(void)
{
	uint32_t i;
	for (i=0; i<3; i++)
	{
		scurve_speed[i].a = dec_profilez[i].a;
		scurve_speed[i].j = dec_profilez[i].j;
		scurve_speed[i].w = dec_profilez[i].w;
		scurve_speed[i].duration = dec_profilez[i].duration;
	}
}

void scurve_generate_speed_profile(fixedpt c_speed, fixedpt d_speed, fixedpt c_acceleration, motion_dynamics_t motion, speedProfile_t *speedprof )
{
	fixedpt t0, t02, t1, t2, t22, del_w0, del_w1, del_w2;
	
	//finding the critical case where where there is no constant acceleration only jerks
	del_w1 = d_speed - c_speed; 
	if (d_speed < c_speed)
	{
		motion.a = -motion.a;
		motion.j1 = -motion.j1;
		motion.j2 = -motion.j2;
	}
	
	t0 = fixedpt_div ((motion.a - c_acceleration), motion.j1);
	t2 = fixedpt_div (motion.a, motion.j2);
	
	
	t0 = fixedpt_abs(t0);
	t2 = fixedpt_abs(t2);
	

		
	t02 = fixedpt_mul(t0, t0);
	t22 = fixedpt_mul(t2, t2);
	
	del_w0 = fixedpt_mul((motion.j1>>1),t02) + fixedpt_mul(c_acceleration,t0);
	del_w2 = fixedpt_mul((motion.j2>>1),t22) + fixedpt_mul(motion.a,t2);

	if (fixedpt_abs(del_w1) <= fixedpt_abs(del_w0) + fixedpt_abs(del_w2))
	{
		fixedpt m = (fixedpt_rconst(1) - fixedpt_div(motion.j1, motion.j2));
		fixedpt D = fixedpt_div(-c_acceleration, motion.j1);
		t0 = fixedpt_div(del_w1*2, motion.j1);
		t0 = t0 + fixedpt_mul(D, D);
		t0 = fixedpt_div(t0, m);
		t0 = fixedpt_sqrt(t0);
		
		t1 = D - t0;
		t0 = D + t0;
// 		volatile float ft0 = fixedpt_tofloat(t0);
 		if(t1 >= 0) t0 = t1; //get the lowest value
// 		ft0 = fixedpt_tofloat(t0);
		
		D = fixedpt_div(-c_acceleration, motion.j2); //calculate D2
		t2 = D + fixedpt_mul((m - fixedpt_rconst(1)), t0);

		t02 = fixedpt_mul(t0, t0);
		t22 = fixedpt_mul(t2, t2);
		
// 		volatile float fm = fixedpt_tofloat(m);
// 		volatile float fd = fixedpt_tofloat(D);
// 		volatile float fa0 = fixedpt_tofloat(c_acceleration);
// 		volatile float fcs = fixedpt_tofloat(c_speed);
// 		volatile float fds = fixedpt_tofloat(d_speed);
		t1 = 0;
		del_w1 =0;
	} 
	else if (fixedpt_abs(del_w1) > fixedpt_abs(del_w0) + fixedpt_abs(del_w2))
	{
		del_w1 = del_w1 - del_w0 - del_w2; //set w1 to be the difference between dw and dw0,2
		t1 = fixedpt_div(del_w1, motion.a);
		t1 = fixedpt_abs(t1);
	}
	
	
	speedprof->a = c_acceleration;
	speedprof->j = motion.j1;
	speedprof->w = c_speed;
	speedprof->duration = t0;

	(speedprof+1)->a = c_acceleration + fixedpt_mul (speedprof->j, speedprof->duration);
	(speedprof+1)->j = 0;
	(speedprof+1)->w = fixedpt_mul (motion.j1 >> 1, t02) + fixedpt_mul (c_acceleration, t0) + c_speed;
	(speedprof+1)->duration = t1;
	
	(speedprof+2)->a = (speedprof+1)->a;
	(speedprof+2)->j = motion.j2;
	(speedprof+2)->w = fixedpt_mul ((speedprof+1)->a, t1) + (speedprof+1)->w;
	(speedprof+2)->duration = t2;
	
// 	volatile float f_j1 = fixedpt_tofloat(motion.j1);
// 	volatile float f_j2 = fixedpt_tofloat(motion.j2);
// 	volatile float f_cacc = fixedpt_tofloat(c_acceleration);
// 	volatile float f_a = fixedpt_tofloat((speedprof+1)->a);
// 	volatile float f_t0 = fixedpt_tofloat(t0);
// 	volatile float f_t1 = fixedpt_tofloat(t1);
// 	volatile float f_t2 = fixedpt_tofloat(t2);
// 	volatile float f_del_w0 = fixedpt_tofloat(del_w0);
// 	volatile float f_del_w1 = fixedpt_tofloat(del_w1);
// 	volatile float f_del_w2 = fixedpt_tofloat(del_w2);
// 	volatile float f_w00 = fixedpt_tofloat((speedprof+0)->w);
// 	volatile float f_w01 = fixedpt_tofloat((speedprof+1)->w);
// 	volatile float f_w02 = fixedpt_tofloat((speedprof+2)->w);	

}
fixedpt d0 = 0;
fixedpt d1 = 0;
fixedpt d2 = 0;
void scurve_exe(speedProfile_t *speedprof, fixedpt t, fixedpt *speed, fixedpt *distance, fixedpt *acc, uint32_t *prof_end)
{
	fixedpt t2, t3;
	if (t <= (speedprof->duration ))
	{
		t2 = fixedpt_mul(t,t);
		t3 = fixedpt_mul(t2,t);		
		d0 = (fixedpt_mul (speedprof->j, t3) / 6) + fixedpt_mul (speedprof->a >> 1, t2) + fixedpt_mul (speedprof->w, t);
		d1 = 0;
		d2 = 0;
	}
	else if  (t <= (speedprof->duration + (speedprof+1)->duration))
	{
		t -= speedprof->duration;
		speedprof++;
		t2 = fixedpt_mul(t,t);
		t3 = fixedpt_mul(t2,t);
		
		d1 = ( fixedpt_mul (speedprof->j, t3) / 6) + fixedpt_mul (speedprof->a >> 1, t2) + fixedpt_mul (speedprof->w, t);
		d2 = 0;
	} 
	else if (t <= (speedprof->duration + (speedprof+1)->duration + (speedprof+2)->duration))
	{
		t -= (speedprof->duration + (speedprof+1)->duration);
		speedprof += 2;
		t2 = fixedpt_mul(t,t);
		t3 = fixedpt_mul(t2,t);
		
		d2 = ( fixedpt_mul (speedprof->j, t3) / 6) + fixedpt_mul (speedprof->a >> 1, t2) + fixedpt_mul (speedprof->w, t);
	}
	else
	{
		*prof_end = 0;
		return;
	}
		
	
	*acc = speedprof->a + fixedpt_mul(speedprof->j, t);
	*speed = fixedpt_mul (speedprof->j >> 1, t2) + fixedpt_mul (speedprof->a, t) + speedprof->w;
	*distance = d0 + d1 + d2;
	
	/*volatile float f_speed = fixedpt_tofloat(*speed);*/
		
	return;
	
	
}