/*
 * overload.c
 *
 * Created: 7/25/2017 4:25:51 AM
 *  Author: Ahmed
 */

#include <fixedptc.h>
#include <pwm_interface.h>
#include <overload.h>
#include <menue_data_manager_drive.h>
#include <d_faults.h>

#define SAMPLING_TIME (fixedpt_div (FIXEDPT_ONE , fixedpt_rconst (PWM_FREQUENCY)))
#define OL_VAL ((menue_drive_data_setup.m_overload_value/100))
#define OL_TIME (menue_drive_data_setup.m_overload_time)
#define OL_CURRENT (fixedpt_mul(OL_VAL, menue_drive_data_applied.m_motor_rated_current ))
#define OVERLOAD_VAL (fixedpt_tofloat(fixedpt_mul(OL_CURRENT, OL_CURRENT)) * OL_TIME)

float oval;
float sample_freq;
float insqr;


void overload_integrate(float *ol_integratorVal, float sqrPhase_current)
{
	*ol_integratorVal += (sqrPhase_current / sample_freq);	
}

float olIaVal = 0, olIbVal = 0, olIcVal = 0, olImVal = 0;

void overload_reset(void)
{
	olIaVal = 0;
	olIbVal = 0;
	olIcVal = 0;

	olImVal = 0;
}

float olsqr(float a)
{
	return (a * a);
}

void overload_check(float ia, float ib, float ic)
{
	overload_integrate(&olIaVal, olsqr(ia) - insqr);
	overload_integrate(&olIbVal, olsqr(ib) - insqr);
	overload_integrate(&olIcVal, olsqr(ic) - insqr);
}

void overload_checkIm(fixedpt imSqr)
{
	overload_integrate(&olImVal, imSqr - insqr);
}

void overload_update(void)
{
	if (menue_drive_data_setup.m_overload_type == 2)
	{
		return;
	}
	
	if(olImVal < 0)
	{
		olImVal = 0;
	}
	else if(olImVal >= oval)
	{
		ol_err_set;
		overload_reset();
	}
	
	//return;
	
	if(olIaVal < 0)
	{
		olIaVal = 0;
	}
	else if(olIaVal >= oval)
	{
		ol_err_set;
		overload_reset();
	}	

	if(olIbVal < 0)
	{
		olIbVal = 0;
	}
	else if(olIbVal >= oval)
	{
		ol_err_set;
		overload_reset();
	}

	if(olIcVal < 0)
	{
		olIcVal = 0;
	}
	else if(olIcVal >= oval)
	{
		ol_err_set;
		overload_reset();
	}
}
void overload_init(void)
{
	oval = OVERLOAD_VAL;
	insqr = olsqr(fixedpt_tofloat(menue_drive_data_applied.m_motor_rated_current));
	//insqrmax = insqr * 2;
	sample_freq = PWM_FREQUENCY;
}

float get_olval()
{
	return oval;
}

float get_olia()
{
	return olIaVal;
}

float get_olib()
{
	return olIbVal;
}

float get_olic()
{
	return olIcVal;
}

float get_olim()
{
	return olImVal;
}