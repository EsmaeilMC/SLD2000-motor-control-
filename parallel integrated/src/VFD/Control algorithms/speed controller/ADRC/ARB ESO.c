

/*
 * ARB_ESO.c
 *
 * Created: 31/01/2019 11:59:16 Õ
 *  Author: ahmed
 */ 

#include <ARB ESO.h>
#include <menue_data_manager.h>
#include <encoder_interface.h>

#define P (menue_drive_data_applied.m_APR_zs_eso)
#define J (menue_drive_data_applied.m_mech_inertia)
#define l1 (2*P)
#define l2 (P*P)
#define T (0.001)


float c1, c2, c3;
volatile float omgm1;
typedef struct
{
	float zm1;
	float zm2;
}z_memory_t;

volatile z_memory_t zm21, zm22;
volatile float z2;

float z_calc(float x, z_memory_t *zmem)
{
	float z = (c1 * x) + (c2 * zmem->zm1) - zmem->zm2;
	z /= c3;
	
	zmem->zm2 = zmem->zm1;
	zmem->zm1 = z;
	
	return z;
}

void arb_eso_init(void)
{
	if(J == 0) J = 1;
	
	c1 = l2 * T * T;
	c2 = 2 + l1 * T;
	c3 = c2 + c1 - 1;
	
	zm21.zm1 = 0;
	zm21.zm2 = 0;
	z2 = 0;
	
	zm22.zm1 = 0;
	zm22.zm2 = 0;
	
	omgm1 = 0;
}

float arb_eso_exe(float tdot, float omega)
{
	volatile float z21, z22, ta;
	
	//ta = omega - omgm1;
	//ta /= T;
	ta = enc_get_acc_shaft_rad_hat();
	ta *= J; // multiply by system inertia to get the torque
	
	omgm1 = omega;
	
	z21 = z_calc(ta, &zm21);
	
	z22 = z_calc(-tdot, &zm22);
	z2 = z22 + z21;
	
	return (-z2);
}
