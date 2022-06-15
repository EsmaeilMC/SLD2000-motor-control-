/*
 * pwm_interface.c
 *
 * Created: 31/01/2017 03:43:40 م
 *  Author: ahmed
 */ 

#include <asf.h>
#include <pwm.h>
#include <pwm_interface.h>
#include <d_faults.h>

float pwm_real_inv_freq = 8000;//(pwm_inv_freq);

#define PWM_CLOCK_DIV_MAX  256
#define PWM_CLOCK_PRE_MAX  11

static uint32_t pwm_clocks_generate(uint32_t ul_frequency, uint32_t ul_mck)
{
	uint32_t ul_divisors[PWM_CLOCK_PRE_MAX] =
	{1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
	uint32_t ul_pre = 0;
	uint32_t ul_div;

	/* Find prescaler and divisor values */
	do {
		ul_div = (ul_mck / ul_divisors[ul_pre]) / ul_frequency;
		if (ul_div <= PWM_CLOCK_DIV_MAX) {
			break;
		}
		ul_pre++;
	} while (ul_pre < PWM_CLOCK_PRE_MAX);

	/* Return result */
	if (ul_pre < PWM_CLOCK_PRE_MAX) {
		return ul_div * (ul_divisors[ul_pre]);
		} else {
		return PWM_INVALID_ARGUMENT;
	}
}

uint32_t dead_time = 150;
uint32_t get_dead_time_interval(void)
{
	return dead_time;
}
void pwm_interface_init(void)
{
				
		//Config the PWMs
		// Period value of PWM output waveform.
		uint32_t ul_tmp = pwm_inv_freq;
		if(ul_tmp >= 9000) ul_tmp = 9000;
		else if(ul_tmp >= 7000) ul_tmp = 7000;
		
		// Initial duty cycle value.
		#define INIT_DUTY_VALUE (PERIOD_VALUE *(0.0))
		
		// Initial deadtimeh value.
		#define INIT_DEADTIMEH_VALUE dead_time
		// Initial deadtimel value.
		#define INIT_DEADTIMEL_VALUE dead_time
		
		pwm_channel_t pwm_channel_instance;
		pwm_fault_t pwm_fault_instance;
		pmc_enable_periph_clk(ID_PWM0);
		
		pwm_channel_disable(PWM0, PWM_CHANNEL_0);
		pwm_channel_disable(PWM0, PWM_CHANNEL_1);
		pwm_channel_disable(PWM0, PWM_CHANNEL_2);
		pwm_channel_disable(PWM0, PWM_CHANNEL_3);
		
		// Set PWM clock A as PWM_FREQUENCY * PERIOD_VALUE (clock B is not used).
		pwm_clock_t clock_setting = {
			.ul_clka = 2 * ul_tmp * PERIOD_VALUE,
			.ul_clkb = 1 * PWM_BRAKE_FREQUENCY * PERIOD_VALUE,
			.ul_mck = sysclk_get_peripheral_hz(),
		};
		pwm_real_inv_freq = sysclk_get_peripheral_hz();
		pwm_real_inv_freq /= PERIOD_VALUE;
		pwm_real_inv_freq /= pwm_clocks_generate(ul_tmp * PERIOD_VALUE, sysclk_get_peripheral_hz());
		
		dead_time = DEAD_TIME * (pwm_real_inv_freq * PERIOD_VALUE * 2);
		dead_time /= 1000000;
		if (dead_time > PERIOD_VALUE)
		{
			 dead_time = 150;
			 init_f_set;
		}
		if (dead_time < 50)
		{
			dead_time = 150;
			init_f_set;
		}
		//dead_time = 0xfff/1 -100;
		pwm_init(PWM0, &clock_setting);
		//delay_s(1);
		
		
		// Use PWM clock A as source clock.
		pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
		// channel output inverted
		pwm_channel_instance.b_pwmh_output_inverted = false;
		pwm_channel_instance.b_pwml_output_inverted = false;
		// enable deadtime generation for half bridge configuration
		pwm_channel_instance.b_deadtime_generator = true;
		//counter event generator for the channel
		pwm_channel_instance.counter_event = PWM_EVENT_PERIOD_HALF_END;
		//output polarity at the beginning of the period
		pwm_channel_instance.polarity = PWM_LOW;
		//waveform left or center aligned
		pwm_channel_instance.alignment = PWM_ALIGN_CENTER;
		// Period value of output waveform.
		pwm_channel_instance.ul_period = PERIOD_VALUE;
		// Duty cycle value of output waveform.
		pwm_channel_instance.ul_duty = INIT_DUTY_VALUE;
		//set deadtime high and low value
		pwm_channel_instance.us_deadtime_pwmh = INIT_DEADTIMEH_VALUE;
		pwm_channel_instance.us_deadtime_pwml = INIT_DEADTIMEL_VALUE;
		
		
		//set output selection override values
		pwm_channel_instance.output_selection.override_level_pwmh = PWM_LOW;
		pwm_channel_instance.output_selection.override_level_pwml = PWM_LOW;
		pwm_channel_instance.output_selection.b_override_pwml = true;
		pwm_channel_instance.output_selection.b_override_pwmh = true;


		//set fault output state
		pwm_channel_instance.ul_fault_output_pwml = PWM_LOW;
		pwm_channel_instance.ul_fault_output_pwmh = PWM_LOW;
		//set fault Input 1 as external fault trigger
		pwm_channel_instance.fault_id = (1 << 1); //PWM_FAULT_PWMFI1;
		
		pwm_channel_instance.ul_spread = 0; //disable spread spectrum mode
		
		//set channel Sync state
		pwm_channel_instance.b_sync_ch = true;
		
		pwm_channel_instance.channel = PWM_CHANNEL_0;
		pwm_channel_init(PWM0, &pwm_channel_instance);
		
		pwm_channel_instance.channel = PWM_CHANNEL_1;
		pwm_channel_init(PWM0, &pwm_channel_instance);

		pwm_channel_instance.channel = PWM_CHANNEL_2;
		pwm_channel_init(PWM0, &pwm_channel_instance);
		
		pwm_sync_init(PWM0,PWM_SYNC_UPDATE_MODE_0, 0);
		
		//_______________________________________________
		//Braking chopper
		
		// Use PWM clock B as source clock for braking.
		pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKB;
		// enable deadtime generation for half bridge configuration
		pwm_channel_instance.b_deadtime_generator = true;
		//output polarity at the beginning of the period
		pwm_channel_instance.polarity = PWM_LOW;
		//waveform left or center aligned
		pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
		// Period value of output waveform.
		pwm_channel_instance.ul_period = PERIOD_VALUE;
		// Duty cycle value of output waveform.
		pwm_channel_instance.ul_duty = (0);
		//set deadtime high and low value
		pwm_channel_instance.us_deadtime_pwmh = 0;
		pwm_channel_instance.us_deadtime_pwml = 0;		
		
		//set output selection override values
		pwm_channel_instance.output_selection.override_level_pwmh = PWM_LOW;
		pwm_channel_instance.output_selection.override_level_pwml = PWM_LOW;
		pwm_channel_instance.output_selection.b_override_pwml = true;
		pwm_channel_instance.output_selection.b_override_pwmh = true;

		//set fault output state
		pwm_channel_instance.ul_fault_output_pwml = PWM_LOW;
		pwm_channel_instance.ul_fault_output_pwmh = PWM_LOW;
		//set fault Input 2 as external fault trigger
		pwm_channel_instance.fault_id = (1 << 2); //PWM_FAULTI2
		
		
		//set channel Sync state
		pwm_channel_instance.b_sync_ch = false;
		
		pwm_channel_instance.channel = PWM_CHANNEL_3;
		pwm_channel_init(PWM0, &pwm_channel_instance);
		
		
		//config comparison units
		pwm_cmp_t pwm_cmp_instance;
		//select comparison unit 0
		pwm_cmp_instance.unit = 0;
		//comparison at incrementing state
		pwm_cmp_instance.b_is_decrementing = false;
		//set pulse to be on event line 0
		pwm_cmp_instance.b_pulse_on_line_0 = true;
		//set pulse to be on event line 1
		//pwm_cmp_instance.b_pulse_on_line_1 = true;
		//enable the comparison unit
		pwm_cmp_instance.b_enable = true;
		//set the value to be the pwm period
		pwm_cmp_instance.ul_value =  PERIOD_VALUE;
		//set the periodicity of the comparison unit
		pwm_cmp_instance.ul_period = 0;
		pwm_cmp_instance.ul_trigger = 0;
		pwm_cmp_instance.ul_update_period = 0;
		
		pwm_cmp_init(PWM0, &pwm_cmp_instance);
		
		//Config PIO
		//PWML0
		pio_set_output(PIOD, PIO_PD10, LOW, DISABLE, ENABLE);
		pio_set_peripheral(PIOD,PIO_PERIPH_B,PIO_PD10);
		//PWMH0
		pio_set_output(PIOD, PIO_PD11, LOW, DISABLE, ENABLE);
		pio_set_peripheral(PIOD, PIO_PERIPH_B, PIO_PD11);
		//PWML1
		//SET PB12 AS GPIO INSTEAD OF ERASE FUNCTION
		//matrix_set_system_io(CCFG_SYSIO_SYSIO12);
		pio_set_output(PIOB, PIO_PB12, LOW, DISABLE, ENABLE);
		pio_set_peripheral(PIOB,PIO_PERIPH_A,PIO_PB12);
		//PWMH1
		pio_set_output(PIOA, PIO_PA2, LOW, DISABLE, ENABLE);
		pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA2);
		//PWML2
		pio_set_output(PIOA, PIO_PA30, LOW, DISABLE, ENABLE);
		pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA30);
		//PWMH2
		pio_set_output(PIOC, PIO_PC19, LOW, DISABLE, ENABLE);
		pio_set_peripheral(PIOC,PIO_PERIPH_B,PIO_PC19);

		//PWML3
		pio_set_output(PIOC, PIO_PC22, LOW, DISABLE, ENABLE);
		pio_set_peripheral(PIOC,PIO_PERIPH_B,PIO_PC22);
		
		//configure PWMFI1
		//PWM0_FI1, FI2
		pio_set_input(PIOD, PIO_PD8, 0);
		pio_set_peripheral(PIOD, PIO_PERIPH_B, PIO_PD8);
		
		pio_set_input(PIOD, PIO_PD9, PIO_PULLUP);
		pio_set_peripheral(PIOD, PIO_PERIPH_B, PIO_PD9);
		
		pwm_fault_instance.fault_id = (1 << 1); //faultI 1
		pwm_fault_instance.b_clear = false;
		pwm_fault_instance.b_filtered = true;
		pwm_fault_instance.polarity = PWM_HIGH;
		pwm_fault_init(PWM0, &pwm_fault_instance);
		
		pwm_fault_instance.fault_id = (1 << 2);//faultI 2
		pwm_fault_instance.polarity = PWM_LOW;
		pwm_fault_instance.b_filtered = false;
		pwm_fault_init(PWM0, &pwm_fault_instance);
		
		((Pwm *)(PWM0))->PWM_IER1 = (0xF << 16); //enable 4 channels fault interrupt
		NVIC_EnableIRQ(PWM0_IRQn);
		
		pwm_enable_protect(PWM0, 0, 0);
		pwm_enable_protect(PWM0, 1, 0);
		pwm_enable_protect(PWM0, 2, 0);
		pwm_enable_protect(PWM0, 3, 0);
		pwm_enable_protect(PWM0, 4, 0);
		pwm_enable_protect(PWM0, 5, 0);
}

void pwm_interface_enablesynchchannels(void)
{
	pwm_channel_enable(PWM0, PWM_CHANNEL_0);
	pwm_channel_enable(PWM0, PWM_CHANNEL_3);
}
