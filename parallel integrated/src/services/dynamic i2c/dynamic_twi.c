/*
 * dynamic_twi.c
 *
 * Created: 03/12/2015 03:24:34 م
 *  Author: ahmed
 */ 
/**
 * \brief Write multiple bytes to a TWI compatible slave device using pdc.
 *
 * \note This function will NOT return until all data has been written or error occurred.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_packet Packet information and data (see \ref twi_packet_t).
 *
 * \return TWI_SUCCESS if all bytes were written, error code otherwise.
 */
#define  BOARD_AT24C_TWI_INSTANCE (TWI0)

//#include <twi.h>
#include <dynamic_twi.h>
//#include <pdc.h>


twistatus_t twi_status[TWI_COUNT];

//tick handler for the twi task
//check the statues of the twi module(s) and write/read data if it is pending

void d_twi_reset_state(uint32_t twi_instance)
{
	twi_status[twi_instance].state = DYNAMIC_TWI_IDLE;	
}

uint32_t d_twi_get_state(uint32_t twi_instance)
{
	return twi_status[twi_instance].state;
}

void d_twi_handler(void)
{
	/* uint32_t count, *status, *state, *time_counter;
	volatile uint32_t dummy_read;
	Twi *p_twi;
	Pdc *instance_pdc;
	
	for(count=0; count<TWI_COUNT; count++)
	{
		p_twi = twi_status[count].d_tw;
		instance_pdc = twi_get_pdc_base(p_twi);
		
		state = &(twi_status[count].state);
		time_counter = &(twi_status[count].time_counter);
		status = &p_twi->TWI_SR;
		
		if(++*time_counter > 100)
		{
			//reset twi
			*state = DYNAMIC_TWI_IDLE;
			*time_counter = 0;
			//p_twi->TWI_CR = TWI_CR_STOP;
			instance_pdc->PERIPH_TCR = 0;
			instance_pdc->PERIPH_RCR = 0;
			instance_pdc->PERIPH_RNCR = 0;
			instance_pdc->PERIPH_TNCR = 0;
			pdc_disable_transfer(instance_pdc, PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
			if((!(*status & TWI_SR_TXCOMP)))
			{
				p_twi->TWI_CR = TWI_CR_STOP;
			}
			//p_twi->TWI_CR = TWI_CR_STOP;
			//p_twi->TWI_CR = TWI_CR_SWRST;
			return;
		}
		else if(*state == DYNAMIC_TWI_BUSY)
		{	
			if((*status&TWI_SR_NACK))
			{
				// not ack indicates an error
				p_twi->TWI_CR = TWI_CR_STOP;
				*state = DYNAMIC_TWI_SBUSY;
				return;
			}
			else if((*status&TWI_SR_TXBUFE)&&(*status&TWI_SR_RXBUFF))
			{
				// TX AND RX buff end of sending
				dummy_read = p_twi->TWI_RHR; //read the last byte of the TWI
				pdc_disable_transfer(instance_pdc, PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
				p_twi->TWI_CR = TWI_CR_STOP;
				*state = DYNAMIC_TWI_TCOM;
				return;
				if((*status & TWI_SR_TXCOMP))
				{
					*state = DYNAMIC_TWI_IDLE;
				}
				else
				{
					*state = DYNAMIC_TWI_ERROR;
				}
				return;
				
			}
				
		}
		else if(*state == DYNAMIC_TWI_ERROR)
		{
			if(1)//(*status&TWI_SR_NACK))
			{
				// reset to idle later
				//p_twi->TWI_CR = TWI_CR_STOP;
				pdc_disable_transfer(instance_pdc, PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
				*state = DYNAMIC_TWI_IDLE;
			}	
		}
		else
		{
			*state = DYNAMIC_TWI_IDLE;	
		}
		
		if(*state == DYNAMIC_TWI_IDLE)
		{
			*time_counter=0;
		}
	}*/
	
}

void dtwi_init(void)
{
	/* #if (TWI_COUNT>0)
		twi_status[0].d_tw = TWI0;
	#endif
	#if (TWI_COUNT>1)
		twi_status[1].d_tw = TWI1;
	#endif */
}
static uint32_t dtwi_mk_addr(const uint8_t *addr, int len)
{
	uint32_t val;

	if (len == 0)
	return 0;

	val = addr[0];
	if (len > 1) {
		val <<= 8;
		val |= addr[1];
	}
	if (len > 2) {
		val <<= 8;
		val |= addr[2];
	}
	return val;
}

/*
uint32_t twi_master_write_pdc(uint32_t twi_instance, twi_packet_t *p_packet)
{
	uint32_t status;
	Twi *p_twi = twi_status[twi_instance].d_tw;
	uint32_t cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;
	pdc_packet_t write_pdc_packet;
	Pdc *instance_pdc;
	
	//set TWI busy flag
	if(twi_status[twi_instance].state == DYNAMIC_TWI_IDLE) twi_status[twi_instance].state = DYNAMIC_TWI_BUSY;
	else return DYNAMIC_TWI_T_BUSY;
	
	//fill pdc packet
	//return;
	write_pdc_packet.ul_addr = p_packet->buffer;
	write_pdc_packet.ul_size = p_packet->length;
	
	//get_pdc of twi instance.
	instance_pdc = twi_get_pdc_base(p_twi);
	
	//Check argument
	if (cnt == 0) {
		return ;
	}

	// Set write mode, slave address and 3 internal address byte lengths 
	p_twi->TWI_MMR = 0;
	p_twi->TWI_MMR = TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);

	// Set internal address for remote chip
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = dtwi_mk_addr(p_packet->addr, p_packet->addr_length);



// 	while (1) {
// 		status = p_twi->TWI_SR;
// 		if (status & TWI_SR_NACK) {
// 			return TWI_RECEIVE_NACK;
// 		}
// 
// 		if ((status & TWI_SR_TXRDY)) {
// 			break;
// 		}	
// 	}
	
	pdc_tx_init(instance_pdc, &write_pdc_packet, 0);
	//return;
	// start sending the buffer
	//PERIPH_PTCR_RXTEN
	instance_pdc->PERIPH_PTCR = (0 | PERIPH_PTCR_TXTEN);


	return DYNAMIC_TWI_T_success;
}*/


/**
 * \brief Read multiple bytes from a TWI compatible slave device.
 *
 * \note This function will NOT return until all data has been read or error occurs.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_packet Packet information and data (see \ref twi_packet_t).
 *
 * \return TWI_SUCCESS if all bytes were read, error code otherwise.
 */
//(uint32_t twi_instance, twi_packet_t *p_packet)


/*
uint32_t twi_master_read_pdc(uint32_t twi_instance, twi_packet_t *p_packet)
{
	
	volatile uint32_t status;
	Twi *p_twi=twi_status[twi_instance].d_tw;
	uint32_t cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;
	pdc_packet_t read_pdc_packet, read_next_pdc_packet;
	Pdc *instance_pdc;
	uint8_t stop_sent = 0;
	uint32_t timeout = TWI_TIMEOUT;;
	
	
	//set TWI busy flag
	if(twi_status[twi_instance].state == DYNAMIC_TWI_IDLE) twi_status[twi_instance].state = DYNAMIC_TWI_BUSY;
	else return DYNAMIC_TWI_T_BUSY;
	
	//clear the RHR before starting PDC transfer.
	status = p_twi->TWI_RHR;
	
	//fill pdc packet
	//return;
	read_pdc_packet.ul_addr = p_packet->buffer;
	read_pdc_packet.ul_size = p_packet->length ;
	
	read_next_pdc_packet.ul_addr = 0;
	read_next_pdc_packet.ul_size = 0;
	
	//get_pdc of twi instance.
	instance_pdc = twi_get_pdc_base(p_twi);
	
	// Check argument 
	if (cnt == 0) {
		return TWI_INVALID_ARGUMENT;
	}

	// Set read mode, slave address and 3 internal address byte lengths 
	p_twi->TWI_MMR = 0;
	p_twi->TWI_MMR = TWI_MMR_MREAD | TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);

	// Set internal address for remote chip 
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = dtwi_mk_addr(p_packet->addr, p_packet->addr_length);

	// Send a START condition
	//p_twi->TWI_CR = TWI_CR_START | TWI_CR_STOP;
	p_twi->TWI_CR = TWI_CR_START;
	
	timeout = TWI_TIMEOUT;
// 	while (cnt > 0) {
// 		status = p_twi->TWI_SR;
// 		if (status & TWI_SR_NACK) {
// 			return TWI_RECEIVE_NACK;
// 		}
// 
// 		if (!timeout--) {
// 			return TWI_ERROR_TIMEOUT;
// 		}
// 		
// 
// 		if ((status & TWI_SR_RXRDY)) {
// 			break;
// 		}		
// 	}
	
	pdc_rx_init(instance_pdc, &read_pdc_packet, &read_next_pdc_packet);
	//return;
	// start sending the buffer
	pdc_enable_transfer(instance_pdc, PERIPH_PTCR_RXTEN | 0);
	//instance_pdc->PERIPH_PTCR = (PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN);

	return DYNAMIC_TWI_T_success;
}*/