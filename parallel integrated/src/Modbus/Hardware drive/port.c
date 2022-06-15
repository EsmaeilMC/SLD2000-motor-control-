/*
 * port.c
 *
 * Created: 09/05/2017 12:23:53 م
 *  Author: ahmed
 */
#include <asf.h>
#include <port.h> 
#include <drive_interface_data.h>

#define PERIF_FREQ (150000000ul)
#define MB_FRAME_TO 36 //the frame timeout of modbus

#define MB_XDMAC_CH_TX0 (4) //todo empty channel generation
#define MB_XDMAC_CH_RX0 (5) //todo empty channel generation

#define MB_XDMAC_CH_TX1 (6) //todo empty channel generation
#define MB_XDMAC_CH_RX1 (7) //todo empty channel generation

#define MB_DMAC_ID_TX0                        (7)
#define MB_DMAC_ID_RX0                        (8)

#define MB_DMAC_ID_TX1                        (9)
#define MB_DMAC_ID_RX1                        (10)

static void mb_port_disable_xdmac(uint32_t xdmac_channel)
{
	uint32_t xdmaint;
	
	xdmaint = (XDMAC_CIE_BIE);

	xdmac_channel_disable_interrupt(XDMAC, xdmac_channel, xdmaint);
	xdmac_channel_disable(XDMAC, xdmac_channel);
	xdmac_disable_interrupt(XDMAC, xdmac_channel);
}

/** XDMAC channel configuration. */
xdmac_channel_config_t xdmac_tx_cfg, xdmac_rx_cfg;

void mb_port_xdmac_configure_write(Usart *p_usart, char *line, uint32_t lenght, uint32_t xdmac_channel, uint32_t xdmac_id)
{
	uint32_t xdmaint;
	/* Initialize and enable DMA controller */

	xdmaint = (XDMAC_CIE_BIE);

	/* Initialize channel config for transmitter */
	xdmac_tx_cfg.mbr_ubc = lenght;

	xdmac_tx_cfg.mbr_sa = (uint32_t)line;
	xdmac_tx_cfg.mbr_da = (uint32_t)&p_usart->US_THR;
	xdmac_tx_cfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN |
	XDMAC_CC_MBSIZE_SINGLE |
	XDMAC_CC_DSYNC_MEM2PER |
	XDMAC_CC_CSIZE_CHK_1 |
	XDMAC_CC_DWIDTH_BYTE|
	XDMAC_CC_SIF_AHB_IF0 |
	XDMAC_CC_DIF_AHB_IF1 |
	XDMAC_CC_SAM_INCREMENTED_AM |
	XDMAC_CC_DAM_FIXED_AM |
	XDMAC_CC_PERID(xdmac_id); // TODO dynamic id generation based on puart

	xdmac_tx_cfg.mbr_bc =  0;
	xdmac_tx_cfg.mbr_ds =  0;
	xdmac_tx_cfg.mbr_sus = 0;
	xdmac_tx_cfg.mbr_dus = 0;

	xdmac_channel_set_descriptor_control(XDMAC, xdmac_channel, 0);
	xdmac_configure_transfer(XDMAC, xdmac_channel, &xdmac_tx_cfg);

	
	xdmac_channel_enable_interrupt(XDMAC, xdmac_channel, xdmaint);
	xdmac_enable_interrupt(XDMAC, xdmac_channel);
	//xdmac_channel_enable(XDMAC, xdmac_channel);
	XDMAC->XDMAC_GE = (XDMAC_GE_EN0 << xdmac_channel);

	/*Enable XDMAC interrupt */
	//NVIC_ClearPendingIRQ(XDMAC_IRQn);
	//NVIC_SetPriority( XDMAC_IRQn ,1);
	//NVIC_EnableIRQ(XDMAC_IRQn);

}

void mb_port_xdmac_configure_read(Usart *p_usart, char *line, uint32_t lenght, uint32_t xdmac_channel, uint32_t xdmac_id)
{
	uint32_t xdmaint;
	uint32_t dummy_read;
	/* Initialize and enable DMA controller */

	xdmaint = (XDMAC_CIE_BIE);

	/* Initialize channel config for receiver */
	xdmac_rx_cfg.mbr_ubc = lenght;

	xdmac_rx_cfg.mbr_da = (uint32_t)line;
	dummy_read = p_usart->US_RHR; //read any remaining bytes in RHR
	
	xdmac_rx_cfg.mbr_sa = (uint32_t)&p_usart->US_RHR;
	xdmac_rx_cfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN |
	XDMAC_CC_MBSIZE_SINGLE |
	XDMAC_CC_DSYNC_PER2MEM |
	XDMAC_CC_CSIZE_CHK_1 |
	XDMAC_CC_DWIDTH_BYTE|
	XDMAC_CC_SIF_AHB_IF1 |
	XDMAC_CC_DIF_AHB_IF0 |
	XDMAC_CC_SAM_FIXED_AM |
	XDMAC_CC_DAM_INCREMENTED_AM |
	XDMAC_CC_PERID(xdmac_id); // TODO dynamic id generation based on puart

	xdmac_rx_cfg.mbr_bc =   0;
	xdmac_tx_cfg.mbr_ds =   0;
	xdmac_rx_cfg.mbr_sus =  0;
	xdmac_rx_cfg.mbr_dus =  0;

	xdmac_configure_transfer(XDMAC, xdmac_channel, &xdmac_rx_cfg);

	xdmac_channel_set_descriptor_control(XDMAC, xdmac_channel, 0);

	xdmac_channel_enable_interrupt(XDMAC, xdmac_channel, xdmaint);
	//xdmac_channel_enable(XDMAC, xdmac_channel);
	XDMAC->XDMAC_GE = (XDMAC_GE_EN0 << xdmac_channel);
	xdmac_enable_interrupt(XDMAC, xdmac_channel);

	/*Enable XDMAC interrupt */
	//NVIC_ClearPendingIRQ(XDMAC_IRQn);
	//NVIC_SetPriority( XDMAC_IRQn ,1);
	//NVIC_EnableIRQ(XDMAC_IRQn);

}
void mb_port_init(uint32_t usart_id, const sam_usart_opt_t *c_opt_usart, Usart *p_usart)
{
	pmc_enable_periph_clk(usart_id);
	usart_init_rs485(p_usart, c_opt_usart, 150000000);

	usart_enable_tx(p_usart);
	usart_enable_rx(p_usart);
	usart_lin_disable_pdc_mode(p_usart);
	usart_set_rx_timeout(p_usart, MB_FRAME_TO);
	if(p_usart == USART0) //SAME70 PIO multiplexing
	{
		pio_set_peripheral(PIOB, PIO_PERIPH_C, PIO_PB0);
		pio_set_peripheral(PIOB, PIO_PERIPH_C, PIO_PB1);
#ifdef _SERIAL_DRIVE
		pio_set_peripheral(PIOB, PIO_PERIPH_C, PIO_PB3);
#endif
	}
	else if(p_usart == USART1) //SAME70 PIO multiplexing
	{
		pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA24);
		pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA21);
		
		pio_set_output(PIOB, PIO_PB4, HIGH, DISABLE, ENABLE);
		pio_set_peripheral(PIOB, PIO_PERIPH_D, PIO_PB4);
	}
	
}

#define PORT_USART_NOT_READY 0
#define PORT_USART_PDA_SUCESS 1
uint32_t port_usart_write_pda_line(Usart *p_usart, char *line, uint32_t lenght)
{
	//if(!(p_usart->UART_SR & UART_SR_TXRDY)) return PORT_USART_NOT_READY;
	
	if (p_usart == USART0)
	{
		//disable transfer
		mb_port_disable_xdmac(MB_XDMAC_CH_TX0);
		//enable transfer
		mb_port_xdmac_configure_write(p_usart, line, lenght, MB_XDMAC_CH_TX0, MB_DMAC_ID_TX0);
	}
	else if (p_usart == USART1)
	{
		//disable transfer
		mb_port_disable_xdmac(MB_XDMAC_CH_TX1);
		//enable transfer
		mb_port_xdmac_configure_write(p_usart, line, lenght, MB_XDMAC_CH_TX1, MB_DMAC_ID_TX1);	
	}
	return PORT_USART_PDA_SUCESS;
}
uint32_t port_usart_set_rx_pda_buff(Usart *p_usart, char *line, uint32_t lenght)
{
	if (p_usart == USART0)
	{
		//disable transfer
		mb_port_disable_xdmac(MB_XDMAC_CH_RX0);
		//enable transfer
		mb_port_xdmac_configure_read(p_usart, line, lenght, MB_XDMAC_CH_RX0, MB_DMAC_ID_RX0);
	}
	else if (p_usart == USART1)
	{
		//disable transfer
		mb_port_disable_xdmac(MB_XDMAC_CH_RX1);
		//enable transfer
		mb_port_xdmac_configure_read(p_usart, line, lenght, MB_XDMAC_CH_RX1, MB_DMAC_ID_RX1);
	}
	return PORT_USART_PDA_SUCESS;
		
}

