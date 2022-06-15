/*
 * regulators.c
 *
 * Created: 30/03/2017 03:25:16 م
 *  Author: ahmed
 */ 

#include <regulators.h>
#include <asf.h>

void reg_pi_add_i_memory_val(float val, struct pi_t *regulator)
{
	regulator->imemory += val / regulator->kic;
}

void reg_pi_change_gains(float kpc, float kic, struct pi_t *regulator)
{
	regulator->imemory = regulator->imemory * regulator->kic / kic;
	regulator->kic = kic;
	regulator->kpc = kpc;
}

void reg_pi_change_ki(float kic, struct pi_t *regulator)
{
	regulator->imemory = regulator->imemory * regulator->kic / kic;
	regulator->kic = kic;
}

void reg_pi_change_kp(float kpc, struct pi_t *regulator)
{
	regulator->kpc = kpc;
}

void reg_pi_init(float kpc, float kic, struct pi_t *regulator)
{
	regulator->imemory = 0;
	regulator->kic = kic;
	if (kic == 0) kic = 1;
	regulator->ksc = kic * 10;
	regulator->kpc = kpc;
}

void reg_pi_reset_memory(struct pi_t *regulator)
{
	regulator->imemory = 0;
}
#define reg_sign(x) ((x<0)?(-1):1)
//float reg_pi_exe(float sat_limit, float err, float ff, float reg_tsample, struct pi_t *regulator)
//{
// 	float reg_out, correction, ff_kp_contributiona;
	
// 	regulator->imemory += (reg_tsample * err);
// 	
// 	reg_out = (regulator->imemory * regulator->kic);
// 	ff_kp_contributiona = (regulator->kpc * err) + ff;
// 	//ff_kp_contributiona = 0;
// 	reg_out += ff_kp_contributiona;
// 	
// 	//return reg_out;
// 	
// 	if (abs(reg_out) > sat_limit)
// 	{
// 		if( abs(ff_kp_contributiona) >= sat_limit)
// 		{
// 			// kp and ff can drive output to saturation alone
// 			//reset integrator
// 			regulator->imemory = 0;
// 		}
// 		else
// 		{
// 			//integrator will be clamped
// 			correction = reg_sign(reg_out)*sat_limit - ff_kp_contributiona;
// 			correction = (correction / regulator->kic);
// 			regulator->imemory = correction;
// 		}
// 		
// 		reg_out = reg_sign(reg_out)*sat_limit;
// 	}
// 	return reg_out;
//}

float reg_pi_exe(float sat_limit, float err, float ff, float reg_tsample, struct pi_t *regulator)
{
	float reg_out, correction, ff_kp_contributiona, ks, temp_memory = regulator->imemory;
	
	temp_memory += (reg_tsample * err); //integrate controller
	
	reg_out = (temp_memory * regulator->kic);
	ff_kp_contributiona = (regulator->kpc * err) + ff;
	//ff_kp_contributiona = 0;
	reg_out += ff_kp_contributiona;
	
	if (fixedpt_abs(reg_out) > sat_limit)
	{
		correction = reg_sign(reg_out)*sat_limit - reg_out;
		//if(regulator->kic != 0)
		//temp_memory += correction / regulator->ksc; // decrease integrator memory (anti windup)
		if( (reg_out * err) > 0 ) temp_memory -= (reg_tsample * err); //stop integration if output saturates
		regulator->imemory = temp_memory;
		return reg_sign(reg_out)*sat_limit;
	}
	regulator->imemory = temp_memory;
	return reg_out;
}