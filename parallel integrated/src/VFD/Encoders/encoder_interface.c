/*
 * encoder_interface.c
 *
 * Created: 24/03/2017 07:00:09 م
 *  Author: ahmed
 */ 
#include <asf.h>
#include <fixedptc.h>
#include <encoder_interface.h>
#include <pwm_interface.h>

void encoder_handler(void)
{
	if(menue_drive_data_applied.m_encoder_type == 2) // endat encoder
	{
		if (MOTOR_PM_CVC) endat_handler();
	}
}

void encoder_init(void)
{
	inc_init();
	endat_init();
}

float encoder_getangle_abs(void)
{
	if(menue_drive_data_applied.m_encoder_type == 1)
		sincos_getangle();
	else if(menue_drive_data_applied.m_encoder_type == 2)
		endat_getangle();
}

float encoder_getangle_elec_abs(uint32_t pole_pair)
{
	if(menue_drive_data_applied.m_encoder_type == 1)
		sincos_getangle_elec(pole_pair);
	else if(menue_drive_data_applied.m_encoder_type == 2)
		endat_getangle_elec(pole_pair);
}

