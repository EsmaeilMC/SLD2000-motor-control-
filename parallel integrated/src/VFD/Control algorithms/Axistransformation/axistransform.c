/*
 * axistransform.c
 *
 * Created: 26/03/2017 10:05:43 ص
 *  Author: ahmed
 */ 

#include <arm_math.h>
#include <axistransform.h>
#include <Inverter.h>
#include <fixedptc.h>
#include <fastmath.h>
#include <motor_data.h>
#include <d_faults.h>

#define AXT_1OV_SQRT3 (0.5773502692f)
#define AXT_2PI (6.283185307f)
float32_t axt_ialpha, axt_ibeta, axt_theta;
float32_t axt_ctheta, axt_stheta;
float32_t axt_id, axt_iq;



void axt_clark_exe(void)
{
	float32_t ia, ib, ic;
	
	inverter_get_creadings(&ia, &ib, &ic);
	
	//if( fixedpt_abs(ia + ib + ic) >= M_PHFAIL_DEV) ophfail_err_set;
	axt_ialpha = ia;
	axt_ibeta = ((ia + 2 * ib) * AXT_1OV_SQRT3);
}

void axt_set_theta(float32_t theta)
{
	axt_theta = (theta * AXT_2PI);
	axt_theta = (axt_theta / (360));
	
	axt_stheta = arm_sin_f32(axt_theta);
	axt_ctheta = arm_cos_f32(axt_theta);
}

float32_t iav = 0;
void axt_park_exe(void)
{
	
	float32_t tmp;

			
	axt_id = (axt_ialpha * axt_ctheta) + (axt_ibeta * axt_stheta);
	axt_iq = (axt_ibeta * axt_ctheta) - (axt_ialpha * axt_stheta);
}

void axt_get_park_transform(float ina, float inb, float *outd, float *outq)
{	
	*outd = (ina * axt_ctheta) + (inb * axt_stheta);
	*outq = (inb * axt_ctheta) - (ina * axt_stheta);
}

float32_t axt_get_id(void)
{
	//return iav;
	return axt_id;
}

float32_t axt_get_iq(void)
{
	return axt_iq;
	//volatile float m = fixedpt_tofloat(axt_iq);
}

float32_t axt_get_ialpha(void)
{
	return axt_ialpha;
}

float32_t axt_get_ibeta(void)
{
	return axt_ibeta;
}
void axt_ipark_exe(float32_t vd, float32_t vq, float32_t *valpha, float32_t *vbeta)
{
	*valpha = (vd * axt_ctheta) - (vq * axt_stheta);
	*vbeta = (vq * axt_ctheta) + (vd * axt_stheta);
}

#define AXT_N_MAX 1000
#define AXT_AVRAGE_ATTENUATION (0.09f)
float32_t axt_current = 0, axt_prev_sample = 0;
uint32_t n;
void axt_integrate_current(float32_t curr)
{
	axt_current += ((curr) - axt_current) * (TSAMPLE / AXT_AVRAGE_ATTENUATION);
}

void axt_integrarte_curr_sqr(void)
{
	
}

float32_t axt_get_current_avg(void)
{
	return axt_current;
}

void axt_reset_current_avg(void)
{
	axt_current = 0;
}