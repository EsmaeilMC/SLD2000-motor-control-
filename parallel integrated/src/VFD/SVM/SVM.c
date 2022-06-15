/*
 * SVM.c
 *
 * Created: 29/01/2017 12:29:51 م
 *  Author: ahmed
 */ 



#include "SVM.h"
#include <pwm_interface.h>
#include <Inverter.h>

#define svm_sign(x) ((x<0)?0:1)
#define SVM_SQRT3_OV_2 fixedpt_rconst(0.8660254038)

void svm_rectangular_to_ABC(uint16_t * a, uint16_t * b, uint16_t * c, fixedpt alpha, fixedpt beta)
{
	fixedpt fp_a, fp_b, fp_c;
	fixedpt t1, t2, t0_2, i, j, k;
	uint32_t sector ;
	int32_t dead_time_comp;
	
	if(fixedpt_abs(alpha) > FIXEDPT_ONE) alpha = fixedpt_div(alpha, fixedpt_abs(alpha));
	if(fixedpt_abs(beta) > FIXEDPT_ONE) beta = fixedpt_div(beta, fixedpt_abs(beta));
	
	j= beta;
	beta = (beta >> 1);
	alpha = fixedpt_mul(SVM_SQRT3_OV_2, alpha);
	i = alpha - beta;
	k = -alpha - beta; 
	
	sector = svm_sign(i) + 2 * svm_sign(j) + 4 * svm_sign(k);
		
	
	switch (sector)
	{
		case 3:
			//sector 1
			t1 = i * (0xfff);
			t2 = j * (0xfff);
			t0_2 = (fixedpt_rconst(4095) - t1 - t2) >> 1; //divided by 2
			
			fp_a = t1 + t2 + t0_2;
			fp_b = t2 + t0_2;
			fp_c = t0_2;
			break;
		case 2:
			//sector 2
			t1 = -k * (0xfff);
			t2 = -i * (0xfff);
			t0_2 = (fixedpt_rconst(4095) - t1 - t2) >> 1; //divided by 2
			
			fp_a = t1 + t0_2;
			fp_b = t1 + t2 + t0_2; 
			fp_c = t0_2;
			break;
		case 6:
			//sector 3
			t1 = j * (0xfff);
			t2 = k * (0xfff);
			t0_2 = (fixedpt_rconst(4095) - t1 - t2) >> 1; //divided by 2
			
			fp_a = t0_2; 
			fp_b = t1 + t2 + t0_2;
			fp_c = t2 + t0_2;
			break;
		case 4:
			//sector 4
			t1 = -i * (0xfff);
			t2 = -j * (0xfff);
			t0_2 = (fixedpt_rconst(4095) - t1 - t2) >> 1; //divided by 2
			
			fp_a = t0_2;
			fp_b = t1 + t0_2;
			fp_c = t1 + t2 + t0_2;
			break;
		case 5:
			//sector 5
			t1 = k * (0xfff);
			t2 = i * (0xfff);
			t0_2 = (fixedpt_rconst(4095) - t1 - t2) >> 1; //divided by 2
			
			fp_a = t2 + t0_2;  
			fp_b = t0_2;
			fp_c = t1 + t2 + t0_2;
			break;
		case 1:
			//sector 6
			t1 = -j * (0xfff);
			t2 = -k * (0xfff);
			t0_2 = (fixedpt_rconst(4095) - t1 - t2) >> 1; //divided by 2
			
			fp_a = t1 + t2 + t0_2;
			fp_b = t0_2;
			fp_c = t1 + t0_2; 
			break;
	}
	
	*a = fixedpt_toint(fp_a);
	*b = fixedpt_toint(fp_b);
	*c = fixedpt_toint(fp_c);
	
	dead_time_comp = *a;
	dead_time_comp += inverter_get_current_sign(0) * DEAD_TIME_INTERVAL;
	if (dead_time_comp > (4095))
	{
		dead_time_comp = 4095;
	} 
	else if (dead_time_comp < 0)
	{
		dead_time_comp = 0;
	}
	*a = dead_time_comp;
	
	dead_time_comp = *b;
	dead_time_comp += inverter_get_current_sign(1) * DEAD_TIME_INTERVAL;
	if (dead_time_comp > (4095))
	{
		dead_time_comp = 4095;
	}
	else if (dead_time_comp < 0)
	{
		dead_time_comp = 0;
	}
	*b = dead_time_comp;
	
	dead_time_comp = *c;
	dead_time_comp += inverter_get_current_sign(2) * DEAD_TIME_INTERVAL;
	if (dead_time_comp > (4095))
	{
		dead_time_comp = 4095;
	}
	else if (dead_time_comp < 0)
	{
		dead_time_comp = 0;
	}
	*c = dead_time_comp;
}

void svm_polar_to_ABC(uint16_t * a, uint16_t * b, uint16_t * c, fixedpt ang, fixedpt m)
{
	fixedpt fp_a, fp_b, fp_c;
	fixedpt t1, t2, t0_2;
	uint32_t sector = 1, angle_ptr;
	
	m = fixedpt_abs(m);
	
	if(m > fixedpt_rconst(1)) m = fixedpt_rconst(1);
	
	while (ang >= fixedpt_rconst(360)) ang-= fixedpt_rconst(360); // round it to the minimum
	while (ang < 0) ang += fixedpt_rconst(360);		// get the equivalent positive angle
	
	while(ang >= fixedpt_rconst(60)) // get SVM sector
	{
		ang -= fixedpt_rconst(60);
		sector++;
	}
	
	angle_ptr = fixedpt_toint( fixedpt_xmul(ang, fixedpt_rconst(5)) ); //pointer for the lookup table
	t2 = fixedpt_mul(m, SVM_SIN_tab[angle_ptr]);
	t1 = fixedpt_mul(m, SVM_SIN_tab[(60 * 5) - angle_ptr]);
	t0_2 = (fixedpt_rconst(4095) - t1 - t2) >> 1; //divided by 2
	
	
	
	switch (sector)
	{
		case 1:
		fp_a = t1 + t2 + t0_2;
		fp_b = t2 + t0_2;
		fp_c = t0_2;
		break;
		case 2:
		fp_a = t1 + t0_2;
		fp_b = t1 + t2 + t0_2;
		fp_c = t0_2;
		break;
		case 3:
		fp_a = t0_2;
		fp_b = t1 + t2 + t0_2;
		fp_c = t2 + t0_2;
		break;
		case 4:
		fp_a = t0_2;
		fp_b = t1 + t0_2;
		fp_c = t1 + t2 + t0_2;
		break;
		case 5:
		fp_a = t2 + t0_2;
		fp_b = t0_2;
		fp_c = t1 + t2 + t0_2;
		break;
		case 6:
		fp_a = t1 + t2 + t0_2;
		fp_b = t0_2;
		fp_c = t1 + t0_2;
		break;
	}
	*a = fixedpt_toint(fp_a);
	*b = fixedpt_toint(fp_b);
	*c = fixedpt_toint(fp_c);
}