/*
 * NLEF.c
 *
 * Created: 02/02/2019 01:31:24 م
 *  Author: ahmed
 */ 
#include <asf.h>
#include <arm_math.h>
#include <NLEF.h>
#include <fixedptc.h>

#define reg_sign(x) ((x<0)?(-1):1)

#define alpha (0.5f)
#define segma (0.05f) //accuracy (0.0002) rad/sec
float nlef_exe(float err, float ff, float kp)
{
	float reg_out, err_abs = fixedpt_abs(err);
// 	if (err_abs >= 1)
// 	{
// 		reg_out = (kp * err);
// 	} 
// 	else 
	if (err_abs > segma)
	{
		reg_out = kp * (sqrt(err_abs) * reg_sign(err));
	}
	else if (err_abs <= segma)
	{
		reg_out = kp * (err / sqrt(segma));
	}
	
	reg_out += ff;
	
	return reg_out;
}

float nlef_saturation_trim(float sat_limit, float val)
{
	if (fixedpt_abs(val) > sat_limit)
	{
		return reg_sign(val) * sat_limit;
	}
	return val;
}