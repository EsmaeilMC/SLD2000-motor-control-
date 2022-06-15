/*
 * phasefail.c
 *
 * Created: 7/17/2017 6:27:07 AM
 *  Author: Ahmed
 */ 

#include <asf.h>
#include <phasefail.h>
#include <io_interface.h>
#include <d_faults.h>
#include <vfd.h>
#include <menue_data_manager_drive.h>

#define IO_PHR ioport_get_pin_level(IOPORT_CREATE_PIN(PIOD, 3))
#define IO_PHS ioport_get_pin_level(IOPORT_CREATE_PIN(PIOD, 2))
#define IO_PHT ioport_get_pin_level(IOPORT_CREATE_PIN(PIOD, 1))

#define phf_r io_interface_update(30 | (1 << IO_I_A_GR_POS), 0, 0)
#define phf_s io_interface_update(31 | (1 << IO_I_A_GR_POS), 0, 0)
#define phf_t io_interface_update(32 | (1 << IO_I_A_GR_POS), 0, 0)

uint16_t phf_r_f = 0, phf_s_f = 0, phf_t_f = 0;

#define PHF_FILTER_MAX 250

char phfail_stage = 0;
void phasefail_handler(void)
{
	if (menue_drive_data_applied.m_input_phase_fail == 1) return;
	
	if (phfail_stage == 0)
	{
		if ((IO_PHR == 0) && (IO_PHS == 0) && (IO_PHT == 1))
		{
			phfail_stage++;
			phf_r_f = 0;
		}
		
	} 
	else if (phfail_stage == 1)
	{
		if ((IO_PHR == 1) && (IO_PHS == 0) && (IO_PHT == 0))
		{
			phfail_stage++;
			phf_s_f = 0;
		}
	}
	else if (phfail_stage >= 2)
	{
		if ((IO_PHR == 0) && (IO_PHS == 1) && (IO_PHT == 0))
		{
			phfail_stage = 0;
			phf_t_f = 0;
		}
	}
	
	
	if(phf_r_f < PHF_FILTER_MAX) phf_r_f++;
	else phfail_err_set;
	
	if(phf_s_f < PHF_FILTER_MAX) phf_s_f++;
	else phfail_err_set;
	
	if(phf_t_f < PHF_FILTER_MAX) phf_t_f++;
	else phfail_err_set;
}