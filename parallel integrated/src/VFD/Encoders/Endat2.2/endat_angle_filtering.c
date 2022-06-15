/*
 * endat_angle_filtering.c
 *
 * Created: 4/29/2018 9:41:43 AM
 *  Author: Ahmed
 */ 

#include <endat.h>

#define amp1 1.2
#define amp2 3
#define END_ALPHA fixedpt_rconst(0.85*amp1)
#define END_BETA  fixedpt_rconst(80*amp2)


int32_t end_theta_hat = 0;
fixedpt end_omega_hat = 0;
fixedpt end_theta_hat_frac = 0;
void encoder_endat_filter(void)
{
	fixedpt theta_error;
	fixedpt temp_v;
	
	//a loop to deal with the discontinuity of the periodic signal
	while(end_theta_hat > ENDAT_MAX_VAL) end_theta_hat -= ENDAT_MAX_VAL;
	while(end_theta_hat < 0) end_theta_hat += ENDAT_MAX_VAL;
	
	//get the projected theta value
	end_theta_hat_frac += fixedpt_div(end_omega_hat, fixedpt_rconst(10000));
	end_theta_hat += fixedpt_toint(end_theta_hat_frac);
	end_theta_hat_frac = fixedpt_fracpart(end_theta_hat_frac);
	
	//calculate the error
	theta_error = fixedpt_fromint(endat_getpos() - end_theta_hat);
	theta_error -= end_theta_hat_frac;
	
	end_theta_hat_frac += fixedpt_mul(END_ALPHA, theta_error);
	
	temp_v = fixedpt_mul(END_BETA, theta_error);
	//temp_v = fixedpt_mul(temp_v, fixedpt_rconst(PWM_FREQUENCY));
	end_omega_hat += temp_v;
}

float endat_getangle_hat(void)
{
	float ret_val;
	ret_val = (end_theta_hat * 360);
	ret_val /= ENDAT_MAX_VAL;
	return ret_val;
}