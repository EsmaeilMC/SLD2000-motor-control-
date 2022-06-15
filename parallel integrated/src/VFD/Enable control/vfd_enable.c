/*
 * vfd_enable.c
 *
 * Created: 12/06/2017 08:26:28 م
 *  Author: ahmed
 */ 

#include <asf.h>
#include <vfd_enable.h>
#include <io_interface.h>
#include <d_faults.h>
#include <vfd.h>
#include <SEDACIO.h>

uint8_t en_delay_counter= 0;

void vfd_enableCheck(void)
{
	if (E1 != E0)
	{
		if(++en_delay_counter > 5)
		{
			en_err_set;
			vfd_states_trip();
		}
	}
	else if (E0 == 1)
	{
		//device_enabled
		en_delay_counter = 0;
	}
	else
	{
		//device_disabled
		en_delay_counter = 0;
		if(vfd_states_allowedtomove) vfd_states_estop();
	}
}

uint32_t vfd_enable_isEnabled(void)
{
	if (E0 == E1)
	{
		if(E0 == 1) return 1;
	}
	return 0;
}

uint32_t vfd_enable_delay_counter = 0, vfd_enable_start_delay_counter = 0;
void vfd_enable_handler(void)
{
	if (vfd_enable_start_delay_counter < 2500)
	{
		vfd_enable_start_delay_counter++;
		return;
	}
	if (!(++vfd_enable_delay_counter < 40))
	{
		vfd_enable_delay_counter = 0;
		vfd_enableCheck();
	}
}