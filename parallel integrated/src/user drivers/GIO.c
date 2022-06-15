/*
 * GIO.c
 *
 * Created: 23/03/2017 01:32:19 م
 *  Author: ahmed
 */ 

#include <GIO.h>
uint32_t lc_writeP = 0;
void pio_set_lc_writeProtect(void)
{
	lc_writeP = 1;
}
void pio_set_data(Pio *p_pio, const uint32_t ul_mask, uint32_t ul_data)
{
	//pio_set_writeprotect(PIOC, 0);
	if (ul_data==1)
	{
		p_pio->PIO_SODR = ul_mask;
	}
	else
	{
		p_pio->PIO_CODR = ul_mask;
	}
	//if (lc_writeP) pio_set_writeprotect(PIOC, 1);
};
void pio_out_data(Pio *p_pio, uint32_t ul_mask, uint32_t ul_data, const uint32_t shift)
{
	pio_set_writeprotect(PIOC, 0);
	
	p_pio->PIO_OWER = (ul_mask << shift);
	p_pio->PIO_ODSR = (p_pio->PIO_ODSR & ~(ul_mask << shift))|(ul_data << shift);
	p_pio->PIO_OWDR = (ul_mask << shift);
	
	if (lc_writeP) pio_set_writeprotect(PIOC, 1);
};