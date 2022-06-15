/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#define  BOARD_AT24C_TWI_INSTANCE (TWI0)

#define F_CPU (300000000ul)

#include <asf.h>

#include <SCH.h>
#include <vfd.h>

#include <VF.h>
#include <fixedptc.h>

#include <Inverter.h>
#include <vfdstates.h>
#include <encoder_interface.h>
#include <vectorIM.h>
#include <vectorPM.h>

#include <axistransform.h>
#include <io_interface.h>

#include <elevator_controller.h>
#include <u_glcd.h>

#include <dynamic_twi.h>
#include <AT24Cxx_dynamic.h>

#include <menue_data_manager.h>
#include <sam_modbus.h>
#include <sph_timers.h>
#include <sph_faults.h>
#include <d_util.h>
#include <SEDACIO.h>
#include <d_faults.h>
#include <vfd_enable.h>
#include <phasefail.h>
#include <overload.h>
#include <encoder_interface.h>
#include <arm_math.h>
#include <speed_regulator.h>
#include <HoistInfo.h>
#include <EMCM.h>
#include <PM_param_tune1.h>
#include <pop_ups.h>
#include <con_toff.h>
#include <motor_excitation.h>
#include <req_io_mb_reader.h>

void TC8_Handler ( void )
{
	uint32_t tc_status = tc_get_status(TC2, 2);
	if (tc_status&TC_SR_CPCS)
	{
		//TC2.1 compare RA event
		sincos_store_val();
		encoder_inc_handler();
		EMCM_decel_point_watch();
		vfd_speed_loop_handler();
	}
}

void TC7_Handler ( void )
{
	uint32_t tc_status = tc_get_status(TC2, 1);
	if(tc_status&TC_SR_CPCS)
	{
		//encoder_handler();
		//TC2.2 compare RA event
		//pm_param_tune_handler();
		
		sch_update();
		//EMCM_decel_point_watch();
	} 	
}

uint8_t wd_rst_delay = 0;
uint8_t rswd_rst_delay = 0;
void WDT_Handler ( void )
{
	uint32_t status = wdt_get_status(WDT);
	return;
	if (status & WDT_SR_WDUNF)
	{
		wd_set;
		if(wd_rst_delay > 0) rstc_start_software_reset(RSTC);
		wd_rst_delay++;
	}
}

void RSWDT_Handler ( void )
{
	uint32_t status = rswdt_get_status(RSWDT);
	return;
	if (status & RSWDT_SR_WDUNF)
	{
		rswd_set;
		if(rswd_rst_delay > 0) rstc_start_software_reset(RSTC);
		rswd_rst_delay++;
	}
}

fixedpt NTC_readout = fixedpt_rconst(0xfff);

void afec1_EOC_handler (void)
{
	// read all ADC values
	uint16_t adc_readings[8];
	
	adc_readings[0] = afec_channel_get_value(AFEC1, 2);
	adc_readings[1] = afec_channel_get_value(AFEC1, 8);
	adc_readings[2] = afec_channel_get_value(AFEC1, 1);
	adc_readings[3] = afec_channel_get_value(AFEC1, 7);
	adc_readings[4] = afec_channel_get_value(AFEC1, 3);
	adc_readings[5] = afec_channel_get_value(AFEC1, 9);
	adc_readings[6] = afec_channel_get_value(AFEC1, 4);
	adc_readings[7] = afec_channel_get_value(AFEC1, 10);
	
	if(menue_drive_data_applied.m_encoder_type == 2) //endat encoder
	{
		sincos_store_phs_adc_val(&adc_readings[0]);
	}
	else if(menue_drive_data_applied.m_encoder_type == 1) //sincos encoder
	{
		sincos_store_abs_adc_val(&adc_readings[0]);
		sincos_store_phs_adc_val(&adc_readings[2]);
		sincos_store_val_abs();
		//vfd_current_loop_handler();
	}
	
	//update the dcbus voltage value
	inverter_update_dcbus_val(adc_readings[4]);
}

void afec_EOC_handler (void)
{
	// read all ADC values
	uint16_t adc_readings[4];
	float32_t ialpha, ibeta, isqr;
	
	afec_start_software_conversion(AFEC1);

	adc_readings[0] = afec_channel_get_value(AFEC0, 0);
	adc_readings[1] = afec_channel_get_value(AFEC0, 6);
	adc_readings[2] = afec_channel_get_value(AFEC0, 3);
	adc_readings[3] = afec_channel_get_value(AFEC0, 9);

	encoder_handler();
	
	//update 3 ph currents
	inverter_update_currents_val(&adc_readings[0]);
	
	//NTC reading filteration.
	NTC_readout += fixedpt_fromint(adc_readings[3]);
	NTC_readout = NTC_readout >>1; //divide by 2 for filteration
	
	axt_clark_exe();

	ialpha = axt_get_ialpha();
	ibeta = axt_get_ibeta();
	
	isqr = (ialpha * ialpha) + (ibeta * ibeta);
 	//isqr = (i_readout[0] * i_readout[0]);
	//if(menue_drive_data_applied.m_encoder_type != 1) // not sincos encoder
	vfd_current_loop_handler();
	
	overload_check(i_readout[0], i_readout[1], i_readout[2]);
	//overload_checkIm(isqr);
	axt_integrate_current(isqr);
	
	
}

uByte_t heart_beat_flasher_index;

void speedLoop_init_timer(void)
{
	//timer counter TC2 initialization for use with the schedular

	pmc_enable_periph_clk(ID_TC8);
	
	tc_init(TC2, 2, TC_CMR_TCCLKS_TIMER_CLOCK2|TC_CMR_WAVSEL_UP_RC); //TC8 clk = MCK/8 = 18750000
	tc_write_rc(TC2, 2, (18750000 / 1000) - 1);
	tc_enable_interrupt(TC2, 2, TC_IER_CPCS);
	NVIC_SetPriority(TC8_IRQn,2);
	NVIC_EnableIRQ(TC8_IRQn);
	tc_start(TC2, 2);
}


void poff_sleep_monitor(void)
{
	//rstc_start_software_reset(RSTC);
	//return;
	if (ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 5)) == 0)
	{
		//deep sleep event
		//rstc_start_software_reset(RSTC);
		//return;
		vfd_trip();
		sleep_err_set;
		pio_set_output(PIOA, 0xFFFFFFFF & (~PIO_PA5), LOW, DISABLE, ENABLE);
		pio_set_output(PIOB, 0xFFFFFFFF, LOW, DISABLE, ENABLE);
		pio_set_output(PIOC, 0xFFFFFFFF & (~PIO_PC28), LOW, DISABLE, ENABLE);
		pio_set_output(PIOD, 0xFFFFFFFF, LOW, DISABLE, ENABLE);
		pio_set_output(PIOE, 0xFFFFFFFF, LOW, DISABLE, ENABLE);
		pio_set_input(PIOA, PIO_PA5, 0);
		pio_set_input(PIOC, PIO_PC28, 0);
		//pmc_enable_backupmode(); //go to deep sleep
		supc_backup_sram_off(SUPC);
		supc_enable_backup_mode(SUPC);
			
	}
}
void reset_wdt(void)
{
	wdt_restart(WDT);
	rswdt_restart(RSWDT);
}

void main_reinit(void)
{
	overload_init();
}

char brake_val = 0;
void brake_toggle(void)
{
	//m_mainOutput(brake_val);
	io_interface_update(3 | (5 << IO_I_A_GR_POS), brake_val, 2);
	
	if(brake_val) brake_val = 0;
	else brake_val = 1;
}

char oc_confirm = 0;
void over_current_reset(void)
{
	if (demand_sw_reset_oc == 0) return;
	
	if (oc_confirm == 0)
	{
		oc_confirm = 1;
	} 
	else
	{
		rstc_start_software_reset(RSTC);
	}
	
}

void init_with_delay(void)
{
	encoder_init();
// 	moc_err_set;
// 	bk_err_set;
// 	fx_err_set;
}

void enable_pio_writeProtect(void)
{
	pio_set_lc_writeProtect();
	pio_set_writeprotect(PIOA, 1);
	pio_set_writeprotect(PIOB, 1);
	pio_set_writeprotect(PIOC, 1);
	pio_set_writeprotect(PIOD, 1);
}

#define WDT_RESET_TIME (2)
#define WDT_RESET_HANDLER_DELAY (300 * WDT_RESET_TIME)
#define WDT_RESET_HANDLER_TIME  (300 * WDT_RESET_TIME)

#define WDT_WDVAL (256 * WDT_RESET_TIME)
int main (void)
{
	board_init();
	

	//wdt_disable(WDT);
	wdt_init(WDT, WDT_MR_WDRSTEN | WDT_MR_WDDBGHLT /*| WDT_MR_WDFIEN*/, WDT_WDVAL, (WDT_WDVAL - WDT_WDVAL/5));
	//NVIC_EnableIRQ(WDT_IRQn);
	rswdt_init(RSWDT, /*RSWDT_MR_WDFIEN |*/ RSWDT_MR_WDDBGHLT /*| RSWDT_MR_WDIDLEHLT*/ | RSWDT_MR_ALLONES_Msk | RSWDT_MR_WDRSTEN, WDT_WDVAL, (WDT_WDVAL - WDT_WDVAL/5));
	//NVIC_EnableIRQ(RSWDT_IRQn);
	
	sch_init();
 	sph_faults_init();
 	sph_timer_init();
 	
 	io_interface_init();
 	elevator_init();
 	d_util_init();
 	fault_history_init();
 	fault_history_loadAll();
	
	
	#define VFD_INIT_DELAY 2000

	sch_add_task_ms(&speedLoop_init_timer,0 + VFD_INIT_DELAY, 0);
	sch_add_task_ms_noOD(&inverter_check_dcbus_val_handler,VFD_INIT_DELAY + 1247, 2);
	
	sch_add_task_ms(&vfd_states_handler, 5 + VFD_INIT_DELAY, 1);
	sch_add_task_ms(&ersdstates_handler, 5 + VFD_INIT_DELAY + 1500, 1);
	sch_add_task_ms_noOD(&io_interface_handler, 20, 1);
 	heart_beat_flasher_index = sch_add_task_ms(&ST1_LED, 0, 500); //heart beat flash
	sch_add_task_ms(&IGBT_temp_handler,50,100);
	sch_add_task_ms(&d_twi_handler, 1 , 2);
	sch_add_task_ms(&at24cxx_dynamic_handler,2 ,4);	//sch_add_task_ms(&read_memory, 3000, 5000);
	sch_add_task_ms(&mb_handler, 100, 1);
	sch_add_task_ms(&sph_timer_handler, 100, 1);
	sch_add_task_ms(&sph_faults_handler, 370, 1);
	sch_add_task_ms_noOD(&reset_wdt, WDT_RESET_HANDLER_DELAY, WDT_RESET_HANDLER_TIME);
	sch_add_task_ms_noOD(&u_glcd_screen_updater, 0, 20);
	sch_add_task_ms(&phasefail_handler, 1000, 2);
	sch_add_task_ms(&EMCM_handler, 571 + VFD_INIT_DELAY, 40);
	sch_add_task_ms(&init_with_delay, 1000 + VFD_INIT_DELAY, 0);
	//sch_add_task_ms(&EMCM_decel_point_watch, 122 + VFD_INIT_DELAY, 1);
	
	sch_add_task_ms(&main_reinit, 1043, 1000);
	sch_add_task_ms(&overload_update, 2343, 1000);
	sch_add_task_ms(&auto_update_elevator_initilal_values, 1000, 0);
	sch_add_task_ms(&vfd_enable_handler, 2530, 1);
	sch_add_task_ms(&pop_ups_handler, 0xFF, 1);
	sch_add_task_ms(&con_toff_monitor_handler, 204, 1000);
	sch_add_task_100us(&poff_sleep_monitor, 1, 1);
	//sch_add_task_ms(&brake_toggle, 15000, 1700);
	sch_add_task_ms(&enable_pio_writeProtect, VFD_INIT_DELAY + 4000, 0);
	sch_add_task_ms(&over_current_reset, 3000, 3000);
	sch_add_task_ms(&sedacio_output_handler, 3000, 5);
	sch_add_task_ms(&motor_excitation_handler, VFD_INIT_DELAY + 53, 1);
	
	dtwi_init();
	vector_IM_init();
	vector_PM_init();
	speed_PI_init();
	mb_init();
	inverter_fan_timer_init();
	// USART1 initialization.
	#define	MB0_USART_SERIAL                 USART1
	#define MB0_USART_SERIAL_ID              ID_USART1  //USART0 for same70
	#define MB0_USART_SERIAL_BAUDRATE        19200
	#define MB0_USART_SERIAL_CHAR_LENGTH     US_MR_CHRL_8_BIT
	#define MB0_USART_SERIAL_PARITY          US_MR_PAR_NO
	#define MB0_USART_SERIAL_STOP_BIT        US_MR_NBSTOP_2_BIT

	const sam_usart_opt_t mb0_usart_console_settings = {
		MB0_USART_SERIAL_BAUDRATE,
		MB0_USART_SERIAL_CHAR_LENGTH,
		MB0_USART_SERIAL_PARITY,
		MB0_USART_SERIAL_STOP_BIT,
		US_MR_CHMODE_NORMAL
	};
	mb_channel_init(0, MB0_USART_SERIAL, &mb0_usart_console_settings, MB0_USART_SERIAL_ID, 1, 5, 25);
	mb_chanel_add_reply_sub(0, &req_io_internal_reply_sub);
	
	// USART1 initialization.
	#define	MB1_USART_SERIAL                 USART0
	#define MB1_USART_SERIAL_ID              ID_USART0  //USART1 for same70
	#define MB1_USART_SERIAL_BAUDRATE        19200
	#define MB1_USART_SERIAL_CHAR_LENGTH     US_MR_CHRL_8_BIT
	#define MB1_USART_SERIAL_PARITY          US_MR_PAR_NO
	#define MB1_USART_SERIAL_STOP_BIT        US_MR_NBSTOP_2_BIT

	const sam_usart_opt_t mb1_usart_console_settings = {
		MB1_USART_SERIAL_BAUDRATE,
		MB1_USART_SERIAL_CHAR_LENGTH,
		MB1_USART_SERIAL_PARITY,
		MB1_USART_SERIAL_STOP_BIT,
		US_MR_CHMODE_NORMAL
	};
	mb_channel_init(1, MB1_USART_SERIAL, &mb1_usart_console_settings, MB1_USART_SERIAL_ID, 1, 5, 30);
	
	//Hoist_init();	
	menue_data_manager_load();
	//sch_add_task_ms(&test_faults, 600, 8000);
	
	while(1){
		sch_dispatch_tasks();
	}
}
