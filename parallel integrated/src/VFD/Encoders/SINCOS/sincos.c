/*
 * sincos.c
 *
 * Created: 24/10/2018 07:12:18 م
 *  Author: ahmed
 */ 

#include <asf.h>
#include <fixedptc.h>
#include <encoder_interface.h>
#include <pwm_interface.h>
#include <motor_data.h>
#include <trig.h>
#include <speed_regulator.h>
#include <d_faults.h>
#include <DACC_interface.h>
#include <arm_math.h>

#define S_ABS_MAX (3822)
#define S_ABS_MIN (340)

#define SINVALMAX savg_max//3745 //3822
#define SINVALMIN savg_min//340
#define COSVALMAX cavg_max//3745 //3822
#define COSVALMIN cavg_min//340

#define SINVALAVG (SINVALMAX - SINVALMIN) 
#define COSVALAVG (COSVALMAX - COSVALMIN) 

#define ERR_MARGIN 100


uint16_t savg_max = S_ABS_MAX, savg_min = S_ABS_MIN, cavg_max = S_ABS_MAX, cavg_min = S_ABS_MIN;
uint16_t s_max = S_ABS_MAX, s_min = S_ABS_MIN, c_max = S_ABS_MAX, c_min = S_ABS_MIN;

fixedpt abs_angle;

#define SINCOS_ERR_MAX 50

uint8_t err_counter;
void increment_error(void)
{
	if (err_counter >= SINCOS_ERR_MAX) sincos_err_set;
	else err_counter++;
}

void decrement_error(void)
{
	if (err_counter > 0) err_counter--;
}

void sincos_detect_error(uint16_t *adc_reading)
{
	if ( (adc_reading[0] <= ((2048) + ERR_MARGIN)) && (adc_reading[0] >= ((2048) - ERR_MARGIN)) )
	{
		if ( (adc_reading[1] <= ((2048) + ERR_MARGIN)) && (adc_reading[1] >= ((2048) - ERR_MARGIN)) )
		{
			increment_error();
			return;
		}
	}
	decrement_error();
}

uint16_t adc_reading_abs[2];
void sincos_store_abs_adc_val(uint16_t *adc_reading)
{
	adc_reading_abs[0] = adc_reading[0];
	adc_reading_abs[1] = adc_reading[1];
}

#define ANG_FILTER (300 * 2 * PI)
fixedpt f_sin = 0, f_cos = 0;
//volatile float TST_ANGLE  = (2 * PI * (360.0 - 0.1) / 360);
#define BOUNDRY_UP (3000)
#define BOUNDRY_DN (500)

uint8_t s_edge = 0;
void s_edges_detect(void)
{
	if (s_edge)
	{
		if (f_sin < (-FIXEDPT_ONE / 4)) //transition occured
		{
			s_edge = 0;
			savg_max = (s_max + savg_max)/2;
			s_max = 0;
			if(savg_max < BOUNDRY_UP) savg_max = BOUNDRY_UP;
			return;
		} 
		else //detect edge
		{
			if (s_max < adc_reading_abs[1]) s_max = adc_reading_abs[1];
		}
	} 
	else
	{
		if (f_sin > (FIXEDPT_ONE / 4)) //transition occured
		{
			s_edge = 1;
			savg_min = (s_min + savg_min)/2;
			s_min = (FIXEDPT_ONE / 4);
			if(savg_min > BOUNDRY_DN) savg_min = BOUNDRY_DN;
			return;
		}
		else //detect edge
		{
			if (s_min > adc_reading_abs[1]) s_min = adc_reading_abs[1];
		}
	}
}

uint8_t c_edge = 0;
void c_edges_detect(void)
{
	if (c_edge)
	{
		if (f_cos < (-FIXEDPT_ONE / 4)) //transition occured
		{
			c_edge = 0;
			cavg_max = (c_max + cavg_max)/2;
			c_max = 0;
			if(cavg_max < BOUNDRY_UP) cavg_max = BOUNDRY_UP;
			return;
		}
		else //detect edge
		{
			if (c_max < adc_reading_abs[0]) c_max = adc_reading_abs[0];
		}
	}
	else
	{
		if (f_cos > (FIXEDPT_ONE / 4)) //transition occured
		{
			c_edge = 1;
			cavg_min = (c_min + cavg_min)/2;
			c_min = (FIXEDPT_ONE / 4);
			if(cavg_min > BOUNDRY_DN) cavg_min = BOUNDRY_DN;
			return;
		}
		else //detect edge
		{
			if (c_min > adc_reading_abs[0]) c_min = adc_reading_abs[0];
		}
	}
}
void sincos_store_val_abs(void)
{
	uint32_t i;
	sincos_detect_error(adc_reading_abs);
// 	abs_angle = adc_reading_abs[0] * FIXEDPT_ONE;
// 	return;

	s_edges_detect();
	c_edges_detect();
	if( adc_reading_abs[0] > COSVALMAX) adc_reading_abs[0] = COSVALMAX;
	if( adc_reading_abs[1] > SINVALMAX) adc_reading_abs[1] = SINVALMAX;
	
	if( adc_reading_abs[0] < COSVALMIN) adc_reading_abs[0] = COSVALMIN;
	if( adc_reading_abs[1] < SINVALMIN) adc_reading_abs[1] = SINVALMIN;
	
	adc_reading_abs[0] -= COSVALMIN;
	adc_reading_abs[1] -= SINVALMIN;
	
	fixedpt fsin = adc_reading_abs[1];
	fsin -= SINVALAVG/2;
	fsin *= FIXEDPT_ONE;
	
	fixedpt fcos = adc_reading_abs[0];
	fcos -= COSVALAVG/2;
	fcos *= FIXEDPT_ONE;
	

	fsin /= (SINVALAVG/2);
	fcos /= (COSVALAVG/2);
	
	f_sin += fixedpt_mul((fsin - f_sin), (TSAMPLE * ANG_FILTER * FIXEDPT_ONE));
	f_cos += fixedpt_mul((fcos - f_cos), (TSAMPLE * ANG_FILTER * FIXEDPT_ONE));
	fsin = f_sin;
	fcos = f_cos;
	//dacc_interface_writeData(0, f_sin, FIXEDPT_ONE*2, 1);
	//dacc_interface_writeData(1, f_cos, FIXEDPT_ONE*2, 1);
	
	//fsin = arm_sin_f32(TST_ANGLE) * FIXEDPT_ONE;
	//fcos = arm_cos_f32(TST_ANGLE) * FIXEDPT_ONE;;
	
	abs_angle = arctan2(fcos, fsin); // get sincos phase val
	//f_angle += (abs_angle - f_angle) * (TSAMPLE * ANG_FILTER);
	//dacc_interface_writeData(0, abs_angle, fixedpt_rconst(400), 0);
}

float sincos_getangle(void)
{
	float ret_val;
	ret_val = abs_angle;
	ret_val /= FIXEDPT_ONE;

	return ret_val;
}

float sincos_getangle_elec(uint32_t pole_pair)
{
	float ret_val;
	uint64_t tmp;
	
	tmp = abs_angle;
	tmp = ((tmp * pole_pair) % fixedpt_rconst(360));
	ret_val = tmp / FIXEDPT_ONE;
	
	return ret_val;
}