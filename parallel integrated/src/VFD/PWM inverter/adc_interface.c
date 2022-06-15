/*
 * adc_interface.c
 *
 * Created: 06/03/2017 06:43:29 م
 *  Author: ahmed
 */ 

#include <asf.h>
#include <adc_interface.h>


void adc_interface_init(void)
{
	//AFEC0 Adc initialization routines
	const enum afec_channel_num chan_list[] = {AFEC_CHANNEL_0, AFEC_CHANNEL_1, AFEC_CHANNEL_2}; //channel sequence
	
	pmc_enable_periph_clk(ID_AFEC0);
	
	afec_enable(AFEC0);
	
	struct afec_config afec_cfg;
    afec_get_config_defaults(&afec_cfg); /*AFEC0 defaults settings changed in "afec.c"*/
    afec_init(AFEC0, &afec_cfg);
    afec_set_trigger(AFEC0, AFEC_TRIG_PWM_EVENT_LINE_0); /*PWM EVENT LINE0 Trigger*/

    struct afec_ch_config afec_ch_cfg;
    afec_ch_get_config_defaults(&afec_ch_cfg);
	afec_ch_cfg.gain = AFEC_GAINVALUE_0;
	afec_ch_cfg.diff = 0;
    afec_ch_set_config(AFEC0, AFEC_CHANNEL_0, &afec_ch_cfg);
    afec_ch_set_config(AFEC0, AFEC_CHANNEL_3, &afec_ch_cfg);
    //afec_ch_set_config(AFEC0, AFEC_CHANNEL_2, &afec_ch_cfg);
    afec_ch_set_config(AFEC0, AFEC_CHANNEL_6, &afec_ch_cfg);
    afec_ch_set_config(AFEC0, AFEC_CHANNEL_9, &afec_ch_cfg);
    //afec_ch_set_config(AFEC0, AFEC_CHANNEL_8, &afec_ch_cfg);
		
    /*Enable all associatd channels*/
    afec_channel_enable(AFEC0, AFEC_CHANNEL_0);
	afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_0, 0x200);

    afec_channel_enable(AFEC0, AFEC_CHANNEL_3);
	afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_3, 0x200);

    //afec_channel_enable(AFEC0, AFEC_CHANNEL_2);
	//afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_2, 0x200);

	//afec_channel_enable(AFEC0, AFEC_CHANNEL_6);
	afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_6, 0x200);

	//afec_channel_enable(AFEC0, AFEC_CHANNEL_9);
	afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_9, 0x200);

	//afec_channel_enable(AFEC0, AFEC_CHANNEL_8);
	//afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_8, 0x200);

	afec_set_sample_hold_mode(AFEC0, AFEC_CHANNEL_ALL, AFEC_SAMPLE_HOLD_MODE_1); //set dual sample and hold mode for all channels
    /*Set user sequence for AFEC*/
    //afec_configure_sequence(AFEC0, chan_list,3);
	
	//afec_enable_interrupt(AFEC0, AFEC_INTERRUPT_EOC_2);
	
	//afec_set_trigger(AFEC0, AFEC_TRIG_PWM_EVENT_LINE_0);
	//NVIC_EnableIRQ(AFEC0_IRQn);

    /*SET AFEC CALLBACK FUNCTION*/
    afec_set_callback(AFEC0, AFEC_INTERRUPT_EOC_9, afec_EOC_handler, 1);
	
////////////////////////////////////////////////////////////////////////////////////////////////////

	//AFEC1 Adc initialization routines
	const enum afec_channel_num chan_list1[] = {AFEC_CHANNEL_0, AFEC_CHANNEL_1}; //channel sequence
	
	pmc_enable_periph_clk(ID_AFEC1);
	
	afec_enable(AFEC1);
	
	struct afec_config afec_cfg1;
	afec_get_config_defaults(&afec_cfg1); /*AFEC1 defaults settings changed in "afec.c"*/
	afec_cfg1.afec_clock = 1000000UL;
	//afec_cfg1.tracktim = 1;
	//afec_cfg1.transfer = 0;
	//afec_cfg1.startup_time = AFEC_STARTUP_TIME_0;
	
	afec_init(AFEC1, &afec_cfg1);
	afec_set_trigger(AFEC1, AFEC_TRIG_SW); /*SW Trigger*/

	struct afec_ch_config afec_ch_cfg1;
	afec_ch_get_config_defaults(&afec_ch_cfg1);
	afec_ch_cfg1.gain = AFEC_GAINVALUE_0;
	afec_ch_cfg1.diff = 0;
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_1, &afec_ch_cfg1);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_2, &afec_ch_cfg1);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_3, &afec_ch_cfg1);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_4, &afec_ch_cfg1);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_7, &afec_ch_cfg1);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_8, &afec_ch_cfg1);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_9, &afec_ch_cfg1);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_10, &afec_ch_cfg1);
	
	/*Enable all associatd channels*/
	afec_channel_enable(AFEC1, AFEC_CHANNEL_1);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_1, 0x200);

	afec_channel_enable(AFEC1, AFEC_CHANNEL_2);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_2, 0x200);

	afec_channel_enable(AFEC1, AFEC_CHANNEL_3);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_3, 0x200);

	afec_channel_enable(AFEC1, AFEC_CHANNEL_4);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_4, 0x200);
	
	//afec_channel_enable(AFEC1, AFEC_CHANNEL_7);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_7, 0x200);

	//afec_channel_enable(AFEC1, AFEC_CHANNEL_8);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_8, 0x200);

	//afec_channel_enable(AFEC1, AFEC_CHANNEL_9);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_9, 0x200);

	//afec_channel_enable(AFEC1, AFEC_CHANNEL_10);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_10, 0x200);
	
	afec_set_sample_hold_mode(AFEC1, AFEC_CHANNEL_ALL, AFEC_SAMPLE_HOLD_MODE_1); //set dual sample and hold mode for all channels
	/*Set user sequence for AFEC*/
	//afec_configure_sequence(AFEC1, chan_list,3);
	
	//afec_enable_interrupt(AFEC1, AFEC_INTERRUPT_EOC_7);
	
	//afec_set_trigger(AFEC1, AFEC_TRIG_PWM_EVENT_LINE_0);
	//NVIC_EnableIRQ(AFEC1_IRQn);

	/*SET AFEC CALLBACK FUNCTION*/
	afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_10, afec1_EOC_handler, 2);	
	//NVIC_DisableIRQ(AFEC1_IRQn);
	//NVIC_ClearPendingIRQ(AFEC1_IRQn);
	//NVIC_SetPriority(AFEC1_IRQn, 0);
	//NVIC_EnableIRQ(AFEC1_IRQn);
}