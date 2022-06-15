/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#include <adc_interface.h>
#include <DACC_interface.h>
#include <rtc_interface.h>


const twihs_options_t temp_tw_options = {.master_clk = (150000000), .speed = (50000), .chip = (0), .smbus = 0};
	
void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	delay_ms(20);
	ioport_init();
	
	//pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);   // enable external 32.768kHz crystal
	//while (!pmc_osc_is_ready_32kxtal());        // wait until oscillator is ready
	
	
	pio_set_input(PIOA, PIO_PA5, 0);
	pio_set_input(PIOC, PIO_PC28, 0);
	
	supc_set_wakeup_inputs(SUPC, SUPC_WUIR_WKUPEN4, SUPC_WUIR_WKUPT4);
	supc_set_wakeup_mode(SUPC, SUPC_WUMR_WKUPDBC_32768_SLCK);
	
	//supc_backup_sram_off(SUPC);
		
		//vfd_trip();
		//sleep_err_set;
	pio_set_output(PIOA, 0xFFFFFFFF & (~PIO_PA5), LOW, DISABLE, ENABLE);
	pio_set_output(PIOB, 0xFFFFFFFF, LOW, DISABLE, ENABLE);
	pio_set_output(PIOC, 0xFFFFFFFF & (~PIO_PC28), LOW, DISABLE, ENABLE);
	pio_set_output(PIOD, 0xFFFFFFFF, LOW, DISABLE, ENABLE);
	pio_set_output(PIOE, 0xFFFFFFFF, LOW, DISABLE, ENABLE);
	pio_set_input(PIOA, PIO_PA5, 0);
	pio_set_input(PIOC, PIO_PC28, 0);
	if (ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 5)) == 0)
	{
		//pmc_enable_backupmode(); //go to deep sleep
		supc_backup_sram_off(SUPC);
		supc_enable_backup_mode(SUPC);
		return;
	}
	
	
	
	delay_ms(250 / 25);
	sysclk_init();
	ioport_init();
	
	
	
	/* Setup Alarms */
	//set RTT alarm for 1 mintue later
	//REG_SUPC_WUMR |= SUPC_WUMR_RTTEN; //enable RTT alarm
	//REG_SUPC_WUMR |= SUPC_WUMR_LPDBCEN0; //set low power debouncer enable wkup0 for button input
	//REG_SUPC_WUIR |= SUPC_WUIR_WKUPEN4;
	//REG_SUPC_WUMR |= SUPC_WUMR_WKUPDBC_32_SLCK; //wkup4 debounce 32 clock cycles
	
	//SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF_STOP_VREG;
	//these both need configuring in their respective places
	/*
	 * ENABLE CLOCK FOR pio peripherals
	*/
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_PIOC);
	pmc_enable_periph_clk(ID_PIOD);
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOE);
	
	//Set soft charge relay pin out low
	pio_set_output(PIOC, PIO_PC25, LOW, DISABLE, ENABLE);

	//Set Fan pin out low
	pio_set_output(PIOD, PIO_PD31, LOW, DISABLE, ENABLE);

	//Set Status LEDs pins out low
	pio_set_output(PIOC, PIO_PC30, HIGH, DISABLE, ENABLE);
	pio_set_output(PIOC, PIO_PC31, HIGH, DISABLE, ENABLE);
	
	//disable all pullups for all ADC inputs
	pio_pull_up(PIOD, PIO_PD30,0);
	//pio_pull_up(PIOA, PIO_PA17,0);
	pio_pull_up(PIOE, PIO_PE5,0);
	pio_pull_up(PIOC, PIO_PC12,0);
	pio_pull_up(PIOA, PIO_PA20,0);
	pio_pull_up(PIOC, PIO_PC0,0);
	pio_pull_up(PIOC, PIO_PC15,0);
	pio_pull_up(PIOC, PIO_PC27,0);
	pio_pull_up(PIOC, PIO_PC13,0);
	pio_pull_up(PIOC, PIO_PC26,0);
	pio_pull_up(PIOC, PIO_PC29,0);
	pio_pull_up(PIOE, PIO_PE3,0);
	
	//LCD PORTS init
	pio_set_output(PIOD, (PIO_PD21|PIO_PD22|PIO_PD23), LOW, DISABLE, ENABLE);
	pio_set_output(PIOA, (PIO_PA25|PIO_PA26|PIO_PA11), LOW, DISABLE, ENABLE);
	pio_set_output(PIOC, (0xFFul << 1), LOW, DISABLE, ENABLE);

	/*
	 * ENABLE CLOCK FOR twi peripherals
	*/

	//twi0 periph A set
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA3);
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA4);
	
	//Set EEPROM Wr protect pins out low
	pio_set_output(PIOC, PIO_PC21, LOW, DISABLE, ENABLE);
	pmc_enable_periph_clk(ID_TWIHS0);
	
	twihs_master_init(TWIHS0, &temp_tw_options);
	
	//Enable XDMA controller
	pmc_enable_periph_clk(ID_XDMAC);
	
	dac_interface_init();
	//pio_set_output(PIOD, PIO_PD26|PIO_PD25, HIGH, DISABLE, ENABLE);
	//pmc_enable_periph_clk(ID_RTC);
	rtc_init();
	// wakeup pins
	pio_set_input(PIOC, PIO_PC28, 0);
	pio_set_input(PIOA, PIO_PA5, 0);
	
	matrix_set_system_io(CCFG_SYSIO_SYSIO4|CCFG_SYSIO_SYSIO12);
	//matrix_set_system_io(CCFG_SYSIO_SYSIO12);

	//set sc_level selector limit out high
	pio_set_output(PIOE, PIO_PE4, HIGH, DISABLE, ENABLE);	
	
	//Set unused Pins out low
  	pio_set_output(PIOA, PIO_PA6|PIO_PA7|PIO_PA8|PIO_PA9|PIO_PA10|PIO_PA28|PIO_PA29|PIO_PA31, LOW, DISABLE, ENABLE);
  	pio_set_output(PIOB, PIO_PB2|PIO_PB8|PIO_PB9, LOW, DISABLE, ENABLE);
  	pio_set_output(PIOC, PIO_PC0|PIO_PC11|PIO_PC14|PIO_PC16|PIO_PC17|PIO_PC18|PIO_PC20|PIO_PC23|PIO_PC24|PIO_PC29, LOW, DISABLE, ENABLE);
  	pio_set_output(PIOD, PIO_PD1|PIO_PD2|PIO_PD3|PIO_PD4|PIO_PD5|PIO_PD6|PIO_PD7|PIO_PD12|PIO_PD14|PIO_PD28|PIO_PD29, LOW, DISABLE, ENABLE);
  	pio_set_output(PIOE, PIO_PE2|PIO_PE0|PIO_PE1|PIO_PE2|PIO_PE3, LOW, DISABLE, ENABLE);
}
