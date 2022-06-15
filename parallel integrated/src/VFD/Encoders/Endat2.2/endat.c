/*
 * endat.c
 *
 * Created: 10/02/2018 07:01:49 م
 *  Author: ahmed
 */ 

#include <asf.h>

#include <fixedptc.h>
#include <encoder_interface.h>
#include <pwm_interface.h>
#include <endat.h>
#include <d_faults.h>
#include <DACC_interface.h>

#define MAIN_PERIF_FREQ (150000000)
#define ENDAT_MAXIMUM_COMM_TRIALS 10

uint32_t endat_write_pda_packet(char *line, uint32_t lenght);

char invertbyte(char inv_byte)
{
	inv_byte = (inv_byte & 0xF0) >> 4 | (inv_byte & 0x0F) << 4;
	inv_byte = (inv_byte & 0xCC) >> 2 | (inv_byte & 0x33) << 2;
	inv_byte = (inv_byte & 0xAA) >> 1 | (inv_byte & 0x55) << 1;
	return inv_byte;
}

#define ENDAT_BUFFER_SIZE 6
uint8_t test_line[ENDAT_BUFFER_SIZE+8];


/** XDMAC channel used in this example. */
#define ENDAT_XDMAC_TX_CH 22
#define ENDAT_XDMAC_RX_CH 23

#define USART2_DMAC_ID_TX                        11
#define USART2_DMAC_ID_RX                        12

static void spi_disable_xdmac(void)
{
	 uint32_t xdmaint;
	 
	 xdmaint = (XDMAC_CIE_BIE |
	 XDMAC_CIE_DIE   |
	 XDMAC_CIE_FIE   |
	 XDMAC_CIE_RBIE  |
	 XDMAC_CIE_WBIE  |
	 XDMAC_CIE_ROIE);

	 xdmac_channel_disable_interrupt(XDMAC, ENDAT_XDMAC_RX_CH, xdmaint);
	 xdmac_channel_disable(XDMAC, ENDAT_XDMAC_RX_CH);
	 xdmac_disable_interrupt(XDMAC, ENDAT_XDMAC_RX_CH);

	 xdmac_channel_disable_interrupt(XDMAC, ENDAT_XDMAC_TX_CH, xdmaint);
	 xdmac_channel_disable(XDMAC, ENDAT_XDMAC_TX_CH);
	 xdmac_disable_interrupt(XDMAC, ENDAT_XDMAC_TX_CH);

	 //NVIC_ClearPendingIRQ(XDMAC_IRQn);
	 //NVIC_DisableIRQ(XDMAC_IRQn);
}
 
/** XDMAC channel configuration. */
xdmac_channel_config_t xdmac_tx_cfg, xdmac_rx_cfg;

void spi_xdmac_configure(Usart *const pusart, uint32_t buff_size)
{
	uint32_t xdmaint;
	/* Initialize and enable DMA controller */

	xdmaint = (XDMAC_CIE_BIE);
// 	 |
// 	XDMAC_CIE_DIE   |
// 	XDMAC_CIE_FIE   |
// 	XDMAC_CIE_RBIE  |
// 	XDMAC_CIE_WBIE  |
// 	XDMAC_CIE_ROIE);

	/* Initialize channel config for transmitter */
	xdmac_tx_cfg.mbr_ubc = buff_size;

	xdmac_tx_cfg.mbr_sa = (uint32_t)test_line;
	xdmac_tx_cfg.mbr_da = (uint32_t)&pusart->US_THR;
	xdmac_tx_cfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN |
	XDMAC_CC_MBSIZE_SINGLE |
	XDMAC_CC_DSYNC_MEM2PER |
	XDMAC_CC_CSIZE_CHK_1 |
	XDMAC_CC_DWIDTH_BYTE|
	XDMAC_CC_SIF_AHB_IF0 |
	XDMAC_CC_DIF_AHB_IF1 |
	XDMAC_CC_SAM_INCREMENTED_AM |
	XDMAC_CC_DAM_FIXED_AM |
	XDMAC_CC_PERID(USART2_DMAC_ID_TX);

	xdmac_tx_cfg.mbr_bc =  0;
	xdmac_tx_cfg.mbr_ds =  0;
	xdmac_tx_cfg.mbr_sus = 0;
	xdmac_tx_cfg.mbr_dus = 0;

	xdmac_channel_set_descriptor_control(XDMAC, ENDAT_XDMAC_TX_CH, 0);
	xdmac_configure_transfer(XDMAC, ENDAT_XDMAC_TX_CH, &xdmac_tx_cfg);

	
	xdmac_channel_enable_interrupt(XDMAC, ENDAT_XDMAC_TX_CH, xdmaint);
	xdmac_enable_interrupt(XDMAC, ENDAT_XDMAC_TX_CH);
	//xdmac_channel_enable(XDMAC, ENDAT_XDMAC_TX_CH);
	XDMAC->XDMAC_GE = (XDMAC_GE_EN0 << ENDAT_XDMAC_TX_CH);

	/* Initialize channel config for receiver */
	xdmac_rx_cfg.mbr_ubc = buff_size;

	xdmac_rx_cfg.mbr_da = (uint32_t)test_line;

	xdmac_rx_cfg.mbr_sa = (uint32_t)&pusart->US_RHR;
	xdmac_rx_cfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN |
	XDMAC_CC_MBSIZE_SINGLE |
	XDMAC_CC_DSYNC_PER2MEM |
	XDMAC_CC_CSIZE_CHK_1 |
	XDMAC_CC_DWIDTH_BYTE|
	XDMAC_CC_SIF_AHB_IF1 |
	XDMAC_CC_DIF_AHB_IF0 |
	XDMAC_CC_SAM_FIXED_AM |
	XDMAC_CC_DAM_INCREMENTED_AM |
	XDMAC_CC_PERID(USART2_DMAC_ID_RX);

	xdmac_rx_cfg.mbr_bc =  0;
	xdmac_tx_cfg.mbr_ds =  0;
	xdmac_rx_cfg.mbr_sus = 0;
	xdmac_rx_cfg.mbr_dus = 0;

	xdmac_configure_transfer(XDMAC, ENDAT_XDMAC_RX_CH, &xdmac_rx_cfg);

	xdmac_channel_set_descriptor_control(XDMAC, ENDAT_XDMAC_RX_CH, 0);

	xdmac_channel_enable_interrupt(XDMAC, ENDAT_XDMAC_RX_CH, xdmaint);
	//xdmac_channel_enable(XDMAC, ENDAT_XDMAC_RX_CH);
	XDMAC->XDMAC_GE = (XDMAC_GE_EN0 << ENDAT_XDMAC_RX_CH);
	xdmac_enable_interrupt(XDMAC, ENDAT_XDMAC_RX_CH);

	/*Enable XDMAC interrupt */
	//NVIC_ClearPendingIRQ(XDMAC_IRQn);
	//NVIC_SetPriority( XDMAC_IRQn ,1);
	//NVIC_EnableIRQ(XDMAC_IRQn);

}
	
void endat_init()
{
	const usart_spi_opt_t usart2Temp_opt =
	{
		1000000, //baudrate
		US_MR_CHRL_8_BIT, //char length
		SPI_MODE_3,
		US_MR_CHMODE_NORMAL // channel mode
	};
	// Endat encoder serial link
	pio_set_output(PIOD, PIO_PD18|PIO_PD19, LOW, DISABLE, ENABLE);
	//SCK, RXD, and TXD
	pio_set_peripheral(PIOD, PIO_PERIPH_B, PIO_PD15);
	pio_set_peripheral(PIOD, PIO_PERIPH_B, PIO_PD16);
	pio_set_peripheral(PIOD, PIO_PERIPH_B, PIO_PD17);
	
	NVIC_ClearPendingIRQ(USART2_IRQn);
	
 	pmc_enable_periph_clk(ID_USART2);
	usart_drive_RTS_pin_low(USART2);
	usart_spi_force_chip_select(USART2);
	
 	usart_init_spi_master(USART2, &usart2Temp_opt, MAIN_PERIF_FREQ);
	usart_lin_disable_pdc_mode(USART2);
 	usart_enable_tx(USART2);
	usart_enable_rx(USART2);	

	//NVIC_SetPriorityGrouping(7);    //4 bits for preemp priority 0 bit for sub priority
	
	//NVIC_SetPriority(AFEC0_IRQn, 5);
	NVIC_ClearPendingIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);	
}

uint32_t endat_f_trials = 0;
void endat_set_faultTrial(void)
{
	if (endat_f_trials < ENDAT_MAXIMUM_COMM_TRIALS)
	{
		++endat_f_trials;
	}
	else
	{
		endat_err_set;
	}
}

void endat_clear_faultTrial(void)
{
	endat_f_trials = 0;
}



uint32_t endat_write_pda_packet(char *line, uint32_t lenght)
{
	if(!(USART2->US_CSR & US_CSR_TXRDY)) return 0;
	
	usart_drive_RTS_pin_low(USART2);
	
	spi_disable_xdmac();
	spi_xdmac_configure(USART2, lenght);
	
	return 1;
}
void invertline()
{
	char i;
	for (i=0; i<ENDAT_BUFFER_SIZE; i++)
	{
		test_line[i] = invertbyte(test_line[i]);
	}
}
void inverMSG(char start)
{
	char i;
	for (i=start; ((i<ENDAT_BUFFER_SIZE) && (i<start + 4)); i++)
	{
		test_line[i] = invertbyte(test_line[i]);
	}
}

uint32_t endat_posR, endat_crc, calc_crc, endat_prev_pos;
char endat_f_bits;

void endat_frame_decode(void)
{
	uint32_t *pos_crc_frame_p;
	uint32_t temp_pos;
	uint32_t temp_crc;
	char i,j, shifter = 0;
	for (i=0; i<ENDAT_BUFFER_SIZE; i++)
	{
		if (test_line[i])
		{
			//start bit detected
			break;
		}
	}
	test_line[i] &= 0x3F;
	if((ENDAT_BUFFER_SIZE - i) > 4 )
	{
		inverMSG(i); // invert data from MSB first to LSB first
		//there is valid data to analyze
		pos_crc_frame_p = (uint32_t) &test_line[i];
		temp_pos = *pos_crc_frame_p;
		*pos_crc_frame_p = 0;
		if (temp_pos)
		{
			while((temp_pos&1) == 0) temp_pos = (temp_pos >> 1);
		}
		temp_pos = (temp_pos >> 1);
		temp_pos &= 0xFFFFF;
		temp_crc = temp_pos >> 15;
		temp_pos &=0x7FFF;
		if (makeCRCPos(15, temp_pos) == temp_crc)
		{
			//crc correct
			endat_f_bits = temp_pos & 3;
			temp_pos = temp_pos >> 2;
			endat_posR = temp_pos;
			//dacc_interface_writeData(0, endat_posR, ENDAT_MAX_VAL, 0);
			endat_clear_faultTrial();
// 			if (!(endat_f_bits&1))// && (abs(endat_prev_pos - temp_pos) < 50))
// 			{
// 				endat_clear_faultTrial();
// 			}
// 			else endat_set_faultTrial(); //endat_err_set;
			//endat_prev_pos = temp_pos;
			
		}
		else
		{
			// error in CRC calc
			endat_set_faultTrial();
		}
		
	}
	else
	{
		//frame error no data recieved
		endat_set_faultTrial();
	}
}

uint32_t endat_stage = 0, endat_stage_timer = 0;
void endat_handler(void)
{
	if(++endat_stage_timer > 10) 
	{
		endat_stage_timer = 0;
		endat_stage = 0;
	}
 	if(usart_is_tx_empty(USART2) && usart_is_tx_ready(USART2) && (endat_stage == 0))
	{
		
		uint64_t *endat_buffer = (uint64_t)test_line;
		endat_clkena(1);
		endat_txena(1);
		
		//data recieved
		endat_frame_decode();
		*endat_buffer = 0;
		test_line[0] = invertbyte((0x7 << 2));

		endat_stage = 1;
 		endat_write_pda_packet(test_line, 1);

		//endat_write_pda_packet(test_line, ENDAT_BUFFER_SIZE);
		usart_enable_interrupt(USART2, US_IER_TXEMPTY);// generate interrupt after sending the first char
	}
	//encoder_endat_filter();	
}

void USART2_Handler(void)
{
	
	if(usart_is_tx_empty(USART2))
	{
		//first char of the Endat frame sent
		endat_txena(0); //enable recieve
		usart_disable_interrupt(USART2, US_IER_TXEMPTY);
		endat_write_pda_packet(test_line, ENDAT_BUFFER_SIZE);
		endat_stage = 0;
		endat_stage_timer = 0;
	}
}

#define endat_pos endat_posR
uint16_t endat_getpos(void)
{
	return endat_pos;	
}

float endat_getangle(void)
{
	float ret_val;
	ret_val = (endat_pos * 360);
	ret_val /= ENDAT_MAX_VAL;
	return ret_val;
}

float endat_getangle_elec(uint32_t pole_pair)
{
	float ret_val;
	ret_val = ((endat_pos * pole_pair) % ENDAT_MAX_VAL) * 360;
	ret_val /= ENDAT_MAX_VAL;
	return ret_val;
}

char endat_getfbits(void)
{
	return endat_f_bits;
}

uint32_t makeCRCPos(uint16_t clocks, uint16_t AlmPos)
{
	uint8_t ff[5]; //state of the 5 flip flops
	uint8_t code[20]; //data-bit array
	uint8_t ex; //auxiliary variable
	uint32_t crc = 0;  //determined CRC code
	signed int i;
	
	for(i=0; i<5; i++) //set all flip-flops to 1
		ff[i] = 1; 
	
	for (i=0; i<15; i++) // transfer pos bits into code array
	{
		code[i] = (AlmPos & 1) ? 1 : 0;
		AlmPos >>= 1;
	}
	
	for(i=0; i<clocks; i++) // calculate the CRC
	{
		ex = ff[4] ^ code[i];
		ff[4] = ff[3];
		ff[3] = ff[2] ^ ex;
		ff[2] = ff[1];
		ff[1] = ff[0] ^ ex;
		ff[0] = ex;
	}
	
	for(i=0; i<5; i++)
	{
		ff[i] = ff[i] ? 0 : 1;
		crc <<= 1;
		crc |= ff[i];
	}
	
	return crc;
}