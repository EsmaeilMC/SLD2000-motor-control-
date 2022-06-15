/*
 * VF.c
 *
 * Created: 31/01/2017 05:00:05 م
 *  Author: ahmed
 */ 


#include <VF.h>
#include <Inverter.h>
#include <axistransform.h>
#include <motor_data.h>

fixedpt theta = fixedpt_rconst(0);
void vf_speed_out_handler(fixedpt speed, uint32_t breakactive)
{
	fixedpt volts = 0, break_volts = 0, thetaincrement, freq = fixedpt_abs(speed);
	float tmp = TSAMPLE;
	tmp *= FIXEDPT_ONE;
	
	if (freq > FMAX)
	{
		if(speed < 0) speed = -FMAX;
		else speed = FMAX;
		
		freq = FMAX;
	}
	
	if (freq <= FMIN)
	{
		volts = VMIN;
	} 
	else if (freq <= FM)
	{
		volts = fixedpt_mul(sl1, freq - FMIN) + VMIN;
		volts = fixedpt_mul(volts, TBOOST);
		volts = fixedpt_div(volts, fixedpt_rconst(100));
	}
	else if (freq <= fbase)
	{
		volts = fixedpt_mul(sl2, freq - FM) + fixedpt_div(fixedpt_mul(VM, TBOOST), fixedpt_rconst(100));
	}
	else if (freq <= FMAX)
	{
		volts = vbase;
	}
	
	volts = fixedpt_abs(volts);
	
	break_volts = fixedpt_div(vbase, fixedpt_rconst(100));
	break_volts = fixedpt_mul(break_volts, BREAK_VOLTAGE);
	if (breakactive) if (speed <= BREAK_FREQ)
	{
		// apply break voltage at break speed
		if(volts < break_volts) volts = break_volts;
	}
	
	volts = fixedpt_mul(volts, fixedpt_rconst(1.414213562)); // multiply by sqrt(2)
	
	speed = fixedpt_mul(speed, fixedpt_rconst(360));
	
	thetaincrement = tmp;
	thetaincrement = fixedpt_mul(speed, thetaincrement);

	theta += thetaincrement;

	while(theta > fixedpt_rconst(360)) theta -= fixedpt_rconst(360);
	while(theta < 0) theta += fixedpt_rconst(360);
	
	//axt_set_theta(fixedpt_tofloat(theta));
	//axt_park_exe();
	float valpha, vbeta;
	//axt_ipark_exe(0, fixedpt_tofloat(volts), &valpha, &vbeta);
	inverter_out_polar(theta, volts);
	//inverter_out_rect(fixedpt_rconst(valpha), fixedpt_rconst(vbeta));	
}
