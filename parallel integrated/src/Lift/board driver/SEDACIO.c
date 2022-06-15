/*
 * SEDACIO.c
 *
 * Created: 13/11/2015 02:28:41 م
 *  Author: ahmed
 */

#include <SEDACIO.h>
#include <menue_data_manager_lift.h>

uint32_t outputs = 0, out_assigned = 0;

void sedacio_setOutputSignal(input_sigType O_signal, uint32_t val)
{
	if (val)
	{
		outputs |= (1ul << O_signal);
	} 
	else
	{
		outputs &= ~(1ul << O_signal);
	}
}

void sedacio_output_handler(void)
{
	uint32_t i = 0, l, out_assigned_image = 0;
	for (i=0; i<15; i++)
	{
		l = menue_lift_data_applied.m_outputs[i];
		out_assigned_image |= (1ul << l);
		if (i < 9)
		{
			if (l < 16)
			{
				io_interface_update(i | (5 << IO_I_A_GR_POS), outputs & (1ul << l), 2);
			}
		} 
		else
		{
			if (l < 16)
			{
				io_interface_update((i + 2 - 9) | (7 << IO_I_A_GR_POS), outputs & (1ul << l), 2);
			}
		}
	}
	out_assigned = out_assigned_image;
}

int32_t sedacio_is_output_assigned(input_sigType o_signal)
{
	if (out_assigned & (1ul << o_signal)) return 1;
	return 0;
}