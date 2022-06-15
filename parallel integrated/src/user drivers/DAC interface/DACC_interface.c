/*
 * DACC_interface.c
 *
 * Created: 26/04/2018 09:52:37 ص
 *  Author: ahmed
 */ 

#include <DACC_interface.h>
#include <fixedptc.h>

#define DACC_ANALOG_CONTROL (DACC_ACR_IBCTLCH0(0x02) | DACC_ACR_IBCTLCH1(0x02))

/** The maximal (peak-peak) amplitude value */
#define MAX_AMPLITUDE (DACC_MAX_DATA)





void dac_interface_init(void)
{
	pmc_enable_periph_clk(ID_DACC);
	
	/* Reset DACC registers */
	dacc_reset(DACC);
	/* Half word transfer mode */
	dacc_set_transfer_mode(DACC, 0);
	
	
	/* Enable output channel DACC_CHANNEL */
	dacc_enable_channel(DACC, 0);
	dacc_enable_channel(DACC, 1);
	
	/* Set up analog current */
	dacc_set_analog_control(DACC, DACC_ANALOG_CONTROL);
}


void dacc_interface_writeData(uint16_t Dchannel, float val, float val_max, uint16_t signed_data)
{
	uint16_t dac_val;
	val /= val_max; //get a perunit value
	if(fixedpt_abs(val) > 1) val /= fixedpt_abs(val);
	
	if(signed_data)
	{
		val += 1;
		dac_val = val * MAX_AMPLITUDE;
		dac_val = (dac_val>> 1); //divide by 2		
	}
	else
	{
		if(val < 0) val = 0;
		dac_val = val * MAX_AMPLITUDE;
	}
	
	dacc_write_conversion_data(DACC, dac_val, Dchannel);
}