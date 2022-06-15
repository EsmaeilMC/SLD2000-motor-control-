/*
 * Inverter.c
 *
 * Created: 31/01/2017 04:07:50 م
 *  Author: ahmed
 */ 

#include <asf.h>
#include <Inverter.h>
#include <fixedptc.h>
#include <SVM.h>
#include <d_faults.h>
#include <vfd.h>
#include <scCheck.h>
#include <arm_math.h>
#include <DACC_interface.h>
#include <Sph_timers.h>
#include <drive_interface_data.h>

fixedpt dc_bus_v_reading = 0, dc_bus_v_f_reading = 0;// dc_bus_v_f_reading2 = 0;
#define DC_BUS_MAX			fixedpt_rconst(760)
#define DC_BUS_MIN			fixedpt_rconst(350)
#define DC_BUS_ARD_MIN		fixedpt_rconst(150)

char demand_sw_reset_oc = 0;
void PWM0_Handler(void)
{
 	Pwm *p_pwm = PWM0;
 	uint32_t pwm_i_status, pwm_fault_status; 
	pwm_i_status = p_pwm->PWM_IMR1;
 	pwm_i_status = pwm_channel_get_interrupt_status(PWM0);
 	pwm_i_status = pwm_i_status >> 16; 
 	pwm_fault_status = pwm_fault_get_status(PWM0);
 	if (pwm_i_status & 0x07)
 	{
 		// fault for either pwm0, 1, or 2
 		if( pwm_fault_status & (1 << 1))
 		{
 			//PWM_PWMFI1 occured
			if (vfd_states_is_sctest_mode())
			{
				sc_err_set;
				vfd_trip();
				pwm_fault_clear_status(PWM0, (1 << 1));
			} 
			else
			{
				if (vfd_get_accelerating_stage() == 0) oc_err_set;
				else if (vfd_get_accelerating_stage() == 1) acc_oc_err_set;
				else if (vfd_get_accelerating_stage() == 2) dec_oc_err_set;
				
				vfd_trip();
				pwm_fault_clear_status(PWM0, (1 << 1));
				//demand_sw_reset_oc = 1;
			}
 		}
 	}
	if (pwm_i_status & 0x08)
	{
		// fault for pwm3 braking
 		if( pwm_fault_status & (1 << 2))
 		{
 			//PWM_PWMFI2 occured
 			brk_err_set;
 			vfd_trip();
 			pwm_fault_clear_status(PWM0, (1 << 2));
 		}
	}
	
}

uint8_t inverter_uv_ignore = 0;
void inverter_ignore_uv (uint8_t ignore)
{
	if (ignore) inverter_uv_ignore = 1;
	else inverter_uv_ignore = 0;
}


void inverter_rewind_dcbus_f_val(void )
{
		dc_bus_v_f_reading = dc_bus_v_reading;
		//dc_bus_v_f_reading2 = dc_bus_v_reading;
}
#define DC_FILTER_TIME (0.00334f) //sec
fixedpt vin_filter_weight = FIXEDPT_ONE, vout_filter_weight = 0;

void inverter_init_dc_filter(void)
{
	float t_ov_ts = PWM_FREQUENCY *	DC_FILTER_TIME;
	float tmp = 1 / (t_ov_ts + 1);
	
	tmp *= FIXEDPT_ONE;
	vin_filter_weight = tmp;
	
	t_ov_ts = 1 / t_ov_ts;
	tmp = 1 / (t_ov_ts + 1);
	
	tmp *= FIXEDPT_ONE;
	vout_filter_weight = tmp;
}

void inverter_update_dcbus_val(uint16_t adc_reading)
{
	fixedpt temp_dcbus = 0;
	
	
	//volatile float dummy;
	//convert the readings to volts.
	temp_dcbus = fixedpt_div( fixedpt_mul(fixedpt_rconst (adc_reading), fixedpt_rconst(3) ), fixedpt_rconst(4095));
	
	//dummy = fixedpt_tofloat(temp_dcbus);
	//amplify the volts to the DC bus level
	temp_dcbus = fixedpt_mul(temp_dcbus, fixedpt_rconst(10.31*3.8));
	
	//add diode drop from secondary transfomer perspective
	temp_dcbus += fixedpt_rconst(0.8);
	
	temp_dcbus = fixedpt_mul(temp_dcbus, fixedpt_rconst(10.26));
	dc_bus_v_reading = temp_dcbus;
	
	dc_bus_v_f_reading = fixedpt_mul(dc_bus_v_f_reading, vout_filter_weight) + fixedpt_mul(dc_bus_v_reading, vin_filter_weight);
	
	//dc_bus_v_f_reading = (dc_bus_v_f_reading >> 1) +  (dc_bus_v_reading >> 1);
	//dacc_interface_writeData(0, dc_bus_v_f_reading, fixedpt_rconst(600), 0);
	//tmp = (dc_bus_v_reading - dc_bus_v_f_reading2);
	//tmp *= TSAMPLE / DC_FILTER_TIME;
	//dc_bus_v_f_reading2 +=  (tmp);
	//dc_bus_v_f_reading = fixedpt_mul(dc_bus_v_f_reading, fixedpt_rconst(0.99)) +  (dc_bus_v_reading / 100);
}

void inverter_check_dcbus_val_handler(void)
{
	if(!vfd_states_initialized()) return;
	
	if(dc_bus_v_f_reading > DC_BUS_MAX)
	{
		ov_err_set;
		vfd_trip();
	}
	else if ((dc_bus_v_f_reading < DC_BUS_MIN) && (inverter_uv_ignore != 1) && (1/*vfd_states_allowedtomove()*/))
	{
		vfd_trip();
		uv_err_set;
	}
	else if ((dc_bus_v_f_reading < DC_BUS_ARD_MIN) && (inverter_uv_ignore == 1) && (1/*vfd_states_allowedtomove()*/))
	{
		vfd_trip();
		uv_ard_err_set;
	}		
}

// Iin = I_RI_OV_RF * I_I_OV_V ( I_V_OFFSET - ADC reading )
#define INVERTER3

//averag parameters for the current offset values
#define  I_N (100) //averaging weight
char start_avg = 1;
float32_t i_adc_v_offset[] = {(2048),(2048),(2048)};


#define I_ADC_REF (3) //the ref value of the ADC
#define I_RI_OV_RF (4)	// amplification ratio of the conditioning circuit
#define I_V_OFFSET0 (1.5f) //offset voltage at 0 current for ADC input0
#define I_V_OFFSET1 (1.5f) //offset voltage at 0 current for ADC input1
#define I_V_OFFSET2 (1.5f) //offset voltage at 0 current for ADC input2
//const fixedpt i_adc_v_offset[] = {fixedpt_rconst(I_V_OFFSET0 * 4095.0 / I_ADC_REF),fixedpt_rconst(I_V_OFFSET1 * 4095.0 / I_ADC_REF),fixedpt_rconst(I_V_OFFSET2 * 4095.0 / I_ADC_REF)};


//#define I_ADC_V_OFFSET(x) (i_v_offset[x] * 4095.0 / I_ADC_REF) //offset voltage at 0 current for ADC input integer 0xfff representation (12 bits)

float32_t i_readout[3]={0,0,0};
#define  C_READOUT_AVG_FILTER (0.2f)
void inverter_update_currents_val(uint16_t *adc_reading)
{
	uint32_t i;
	float32_t floatadc;
	for (i=0; i<3; i++)
	{
		floatadc = (*adc_reading);
		if(start_avg)
		{
			i_adc_v_offset[i]   += ((floatadc - i_adc_v_offset[i]) * (TSAMPLE / C_READOUT_AVG_FILTER));
		}
		float32_t current_read_out = floatadc - i_adc_v_offset[i];
		current_read_out = (current_read_out * ( I_RI_OV_RF));
		current_read_out = (current_read_out / (4095));
		current_read_out = (current_read_out * (I_I_OV_V * I_ADC_REF));
		
		i_readout[i] = current_read_out;
		
		
		adc_reading++;
	}
	//i_readout[0] = 0;
	//i_readout[1] = 0;
	//i_readout[2] = 0;
	
	scCheck_current_handler(i_readout[0], i_readout[1], i_readout[2]);
}

uint32_t inverter_getADCcurrent(void)
{
	return &i_adc_v_offset[0];
}

void inverter_out_single_axis(float volts)
{
	volatile uint16_t pwm_a ,pwm_b, pwm_c;
	pwm_channel_t pwm_channel_instance;
	float m = 0;
	int32_t dead_time_comp;
	
	m = dc_bus_v_f_reading;
	m /= FIXEDPT_ONE;
	m =  volts / m;
	
	m = fixedpt_abs(m);
	
	
	if (m > 1) m = 1;
	
	
	
	
	if(volts >= 0)
	{
		pwm_a = (m * 4095);
		pwm_b = 0;
		pwm_c = 0;
	}
	else
	{
		pwm_a = ((1.0f - m) * 4095);
		pwm_b = 4095;
		pwm_c = 4095;
	}
	
	dead_time_comp = pwm_a;
	dead_time_comp += inverter_get_current_sign(0) * DEAD_TIME_INTERVAL;
	if (dead_time_comp > (4095))
	{
		dead_time_comp = 4095;
	}
	else if (dead_time_comp < 0)
	{
		dead_time_comp = 0;
	}
	pwm_a = dead_time_comp;
	
	pwm_channel_instance.ul_period = 0xfff;
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_a);
	//pwm_channel_update_duty(PWM, &pwm_channel_instance, 4059);
	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_b);
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_c);
	
	pwm_sync_unlock_update(PWM0);
}

void inverter_out_single_axis_b(float volts)
{
	volatile uint16_t pwm_a ,pwm_b, pwm_c;
	pwm_channel_t pwm_channel_instance;
	float m = 0;
	
	m = dc_bus_v_f_reading;
	m /= FIXEDPT_ONE;
	m =  volts / m;
	
	m = fixedpt_abs(m);
	
	
	if (m > 1) m = 1;
	
	
	
	
	if(volts >= 0)
	{
		pwm_b = (m * 4095);
		pwm_a = 0;
		pwm_c = 0;
	}
	else
	{
		pwm_b = ((1.0f - m) * 4095);
		pwm_a = 4095;
		pwm_c = 4095;
	}
	
	pwm_channel_instance.ul_period = 0xfff;
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_a);
	//pwm_channel_update_duty(PWM, &pwm_channel_instance, 4059);
	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_b);
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_c);
	
	pwm_sync_unlock_update(PWM0);
}

void inverter_out_single_axis_c(float volts)
{
	volatile uint16_t pwm_a ,pwm_b, pwm_c;
	pwm_channel_t pwm_channel_instance;
	float m = 0;
	
	m = dc_bus_v_f_reading;
	m /= FIXEDPT_ONE;
	m =  volts / m;
	
	m = fixedpt_abs(m);
	
	
	if (m > 1) m = 1;
	
	
	
	
	if(volts >= 0)
	{
		pwm_c = (m * 4095);
		pwm_b = 0;
		pwm_a = 0;
	}
	else
	{
		pwm_c = ((1.0f - m) * 4095);
		pwm_b = 4095;
		pwm_a = 4095;
	}
	
	pwm_channel_instance.ul_period = 0xfff;
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_a);
	//pwm_channel_update_duty(PWM, &pwm_channel_instance, 4059);
	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_b);
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_c);
	
	pwm_sync_unlock_update(PWM0);
}
void inverter_out_polar(fixedpt alfa, fixedpt volts)
{
	volatile uint16_t pwm_a ,pwm_b, pwm_c;
	pwm_channel_t pwm_channel_instance;
	fixedpt m = 0;
	
	
	m = fixedpt_div( volts , dc_bus_v_f_reading);
	
	m = fixedpt_abs(m);
	
	//m = fixedpt_mul(m, fixedpt_rconst(1.1547)); // compensate for the limit on the SVM technique
	
	if (m > fixedpt_rconst(1)) m = fixedpt_rconst(1);
	//m = fixedpt_rconst(1);
	//m = fixedpt_rconst(1);
	//alfa = 0;
	
	//volatile float dumm = fixedpt_tofloat(m) ;
	
	svm_polar_to_ABC(&pwm_a, &pwm_b, &pwm_c, alfa ,m);
	
	pwm_channel_instance.ul_period = 0xfff;
	
// 	pwm_a = 0xfff /2;
// 	pwm_b = 0xfff /2;
// 	pwm_c = 0xfff /2;

	//if (pwm_a > 0xfff)
	{
		//pwm_a = 0xfff;
	}
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_a);
	//pwm_channel_update_duty(PWM, &pwm_channel_instance, 4059);
	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_b);
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_c);
	
	pwm_sync_unlock_update(PWM0);
}

void inverter_out_7(void)
{
	uint16_t pwm_a ,pwm_b, pwm_c;
	pwm_channel_t pwm_channel_instance;
		
	pwm_a = 0;
	pwm_b = 0;
	pwm_c = 0;
	
	pwm_channel_instance.ul_period = 0xfff;
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_a);
	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_b);
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_c);
	
	pwm_sync_unlock_update(PWM0);
}

void inverter_out_0(void)
{
	uint16_t pwm_a ,pwm_b, pwm_c;
	pwm_channel_t pwm_channel_instance;
	
	pwm_a = 4095;
	pwm_b = 4095;
	pwm_c = 4095;
	
	pwm_channel_instance.ul_period = 0xfff;
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_a);
	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_b);
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_c);
	
	pwm_sync_unlock_update(PWM0);
}

#define sign(x) ((x<0)?-1:1)
void inverter_out_rect(fixedpt alfa, fixedpt beta)
{
	volatile uint16_t pwm_a ,pwm_b, pwm_c;
	pwm_channel_t pwm_channel_instance;
	fixedpt m = 0, pm_v_drop;
	
	
	alfa = fixedpt_div (alfa, dc_bus_v_f_reading);
	
	beta = fixedpt_div (beta, dc_bus_v_f_reading);
// 	pm_v_drop = fixedpt_div( PM_VOLTAGE_DROP, dc_bus_v_f_reading );
// 	pm_v_drop *= 0xfff;
	
	svm_rectangular_to_ABC(&pwm_a, &pwm_b, &pwm_c, alfa, beta);
	
/*	pwm_a +=*/
	pwm_channel_instance.ul_period = 0xfff;
	//pwm_a = 500;
	
	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_b);
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_c);
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_duty(PWM0, &pwm_channel_instance, pwm_a);
	//pwm_channel_update_duty(PWM0, &pwm_channel_instance, 500);
	
	pwm_sync_unlock_update(PWM0);
}

void inverter_set_override(void)
{
	
	pwm_channel_t pwm_channel_instance;
	pwm_output_t pwm_output_instance;
	
	pwm_output_instance.b_override_pwmh = true;
	pwm_output_instance.b_override_pwml = true;
	pwm_output_instance.override_level_pwmh = LOW;
	pwm_output_instance.override_level_pwml = LOW;
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_output(PWM0, &pwm_channel_instance, &pwm_output_instance, false);

	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_output(PWM0, &pwm_channel_instance, &pwm_output_instance, false);
	
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_output(PWM0, &pwm_channel_instance, &pwm_output_instance, false);	

	pwm_channel_instance.channel = PWM_CHANNEL_3;
	pwm_channel_update_output(PWM0, &pwm_channel_instance, &pwm_output_instance, false);
	start_avg = 1; //start avarraging the offset current
	SC_LEVEL_LOW;
}

void inverter_clear_override(void)
{
	start_avg = 0; //stop avarraging the offset current
	
	pwm_channel_t pwm_channel_instance;
	pwm_output_t pwm_output_instance;
	
	pwm_output_instance.b_override_pwmh = false;
	pwm_output_instance.b_override_pwml = false;
	pwm_output_instance.override_level_pwmh = LOW;
	pwm_output_instance.override_level_pwml = LOW;
	
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_update_output(PWM0, &pwm_channel_instance, &pwm_output_instance, false);

	pwm_channel_instance.channel = PWM_CHANNEL_1;
	pwm_channel_update_output(PWM0, &pwm_channel_instance, &pwm_output_instance, false);
	
	pwm_channel_instance.channel = PWM_CHANNEL_2;
	pwm_channel_update_output(PWM0, &pwm_channel_instance, &pwm_output_instance, false);
	
	pwm_channel_instance.channel = PWM_CHANNEL_3;
	pwm_channel_update_output(PWM0, &pwm_channel_instance, &pwm_output_instance, false);
}

void inverter_set_brake_duty(uint32_t b_duty)
{
		pwm_channel_t pwm_channel_instance;
		
		pwm_channel_instance.ul_period = 0xfff;
		
		pwm_channel_instance.channel = PWM_CHANNEL_3;
		pwm_channel_update_duty(PWM0, &pwm_channel_instance, b_duty);
		
		//pwm_sync_unlock_update(PWM);	
}

#define BRAKE_VTOP_MAX		fixedpt_rconst(730)
#define BRAKE_TON_VOLTAGE	fixedpt_rconst(650)
#define BRAKE_TOFF_VOLTAGE	fixedpt_rconst(610)
#define BRAKE_DUTY_MAX		fixedpt_rconst(0.9)

char inverter_braking_state = 0;

void inverter_brake_chopper_handler(void)
{
	//inverter_set_brake_duty(0xfff - 0xfff/10);
	//return;
	//volatile float m = fixedpt_tofloat(dc_bus_voltage);
	if ((dc_bus_voltage >= BRAKE_TON_VOLTAGE))
	{
		inverter_braking_state = 1;
	} 
	else if((dc_bus_voltage < BRAKE_TOFF_VOLTAGE) || (inverter_braking_state == 0))
	{
		inverter_set_brake_duty(0);
		inverter_braking_state = 0;
		return;	
	}
	
	fixedpt D ;//= BRAKE_DUTY_MAX;
	//D = fixedpt_mul(D, dc_bus_voltage - BRAKE_TOFF_VOLTAGE);
	D = dc_bus_voltage - BRAKE_TOFF_VOLTAGE;
	D = fixedpt_div(D, (BRAKE_VTOP_MAX - BRAKE_TOFF_VOLTAGE));
	
	if (D > BRAKE_DUTY_MAX) D = BRAKE_DUTY_MAX;
	
	D = fixedpt_mul(D, fixedpt_rconst (0xfff));
	
	inverter_set_brake_duty(fixedpt_toint(D));
}

void inverter_get_creadings(float32_t* ria, float32_t* rib, float32_t* ric)
{
	*ria = i_readout[0];
	*rib = i_readout[1];
	*ric = i_readout[2];
	//*ric = -i_readout[0] - i_readout[1];
}

uint32_t inverter_dead_comp = 1;

void inverter_set_dead_comp(uint32_t val)
{
	inverter_dead_comp = 1;//val;
}

int32_t inverter_get_current_sign(uint32_t axis)
{
	//return 0;
	if(inverter_dead_comp == 0) return 0;
	if (axis < 3)
	{
		if (i_readout[axis] >= 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if ((-i_readout[0] - i_readout[1]) >= 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

#define Cth  fixedpt_rconst(0.1233090342) // a constant to convert from ADC reading to Resistance/10
#define NTC_R_INF_INV fixedpt_rconst(16.48766088) //a constant for temprature calculation
#define NTC_MULTIPLIER fixedpt_rconst(10) //a constant for temprature calculation
#define NTC_BETA  fixedpt_rconst(3375) //a constant from the NTC data sheet
#define NTC_KELVIN_TO_C  fixedpt_rconst(273.15) //a constant to convert to celisious

#define FAN_ON_TEMP   fixedpt_rconst(55.0) 
#define FAN_TMR_ON_TEMP   fixedpt_rconst(40.0)
#define FAN_OFF_TEMP  fixedpt_rconst(30.0) 
#define MAX_OP_TEMP   fixedpt_rconst(90) 



//#define NTC_TEST_R  fixedpt_rconst(49.5) //a constant to convert to celisious
fixedpt IGBT_temp = fixedpt_rconst(25);

uint32_t fan_timer_indx;
void fan_timer_ton(void)
{
	if (IGBT_temp >= FAN_TMR_ON_TEMP) Fan_On;
}
void fan_timer_toff(void)
{
	Fan_Off;
}
uint8_t fan_timer_val = 1;
void inverter_fan_timer_init(void)
{
	fan_timer_indx = sph_timer_add(&fan_timer_val, 60000, &fan_timer_ton, &fan_timer_toff);
}
void inverter_fan_timer_reset(void)
{
	sph_timer_reset(fan_timer_indx);
}
void fan_temp_control(void)
{
	if (IGBT_temp >= FAN_ON_TEMP)
	{
		//Fan_On;
		inverter_fan_timer_reset();
	}
	else if(IGBT_temp <= FAN_OFF_TEMP)
	{
		Fan_Off;
	}
}
void IGBT_temp_handler(void)
{
	fixedpt NTC_temp = fixedpt_mul( NTC_readout, Cth); // get NTC resistance in ohms
	
	//NTC_temp = NTC_TEST_R;
	NTC_temp = fixedpt_mul(NTC_R_INF_INV, NTC_temp);
	NTC_temp = fixedpt_ln( NTC_temp ) + fixedpt_ln( NTC_MULTIPLIER ); // multiply for precision keeping
	NTC_temp = fixedpt_div(NTC_BETA, NTC_temp) - NTC_KELVIN_TO_C;
	IGBT_temp = NTC_temp;
	
	fan_temp_control();
	if(IGBT_temp >= MAX_OP_TEMP)
	{
		ot_err_set;
	}
	//volatile float testlin = fixedpt_tofloat( NTC_temp);
	//testlin *= 1;

}

fixedpt iverter_get_igbt_temp(void)
{
	return IGBT_temp;
}
fixedpt dc_ers_manual_val = 0;
void inverter_set_manual_dcVal(fixedpt dc_val)
{
	dc_ers_manual_val = fixedpt_abs(dc_val);	
}

fixedpt iverter_get_dc_v(void)
{
	if(dc_ers_manual_val == 0) return dc_bus_v_f_reading;
	else return dc_ers_manual_val;
}

fixedpt iverter_get_dc_v2(void)
{
	return dc_bus_v_f_reading;
}