/*
 * incremental.c
 *
 * Created: 24/03/2017 06:58:53 م
 *  Author: ahmed
 */ 

#include <asf.h>
#include <fixedptc.h>
#include <encoder_interface.h>
#include <pwm_interface.h>
#include <motor_data.h>
#include <trig.h>
#include <speed_regulator.h>
#include <DACC_interface.h>
#include <distance_regulator.h>

#define ENC_COUNT_REV (ENCODER_COUNTREV)//ENCODER_COUNTREV
#define AB_MULa (1.2)
#define AB_MULb (1.2)
#define ENC_ALPHA ((0.85) * AB_MULa)
#define ENC_BETA  (80 * AB_MULb)
#define ENC_FILTERATION_TIME (1.0f) //ms
#define ENC_F_SPEED_WEIGHT (f_speed_weight)
#define ENC_SPEED_WEIGHT (speed_weight)

float speed_weight = 1, f_speed_weight = 0;
//uint8_t enc_filteration_time = 1;
int64_t enc_theta_hat = 0;
float f_enc_omega_hat = 0, filtered_enc_omega_hat = 0;
float f_enc_acc_hat = 0, filtered_enc_acc_hat = 0;
float f_enc_theta_hat_frac = 0;

void inc_init()
{
	//set TC0 as qdec unit
	pmc_enable_periph_clk(ID_TC0);
	//set TIOA0 periph
	pio_set_peripheral(PIOA, PIO_PERIPH_B, PIO_PA0);
	//set TIOB1 periph
	pio_set_peripheral(PIOA,PIO_PERIPH_B,PIO_PA16);
	//set TIOB0 periph
	pio_set_peripheral(PIOA,PIO_PERIPH_B,PIO_PA1);
	
	tc_init(TC0,0,TC_CMR_TCCLKS_XC0); //
	tc_init(TC0,1,TC_CMR_TCCLKS_XC0); //
	//tc_init(TC0,2,TC_CMR_TCCLKS_XC0|TC_CMR_WAVSEL_UP); //
	
	
	if (menue_drive_data_setup.m_pg_direction == 1)
	{
		tc_set_block_mode(TC0, TC_BMR_QDEN|TC_BMR_POSEN|TC_BMR_SWAP);
	} 
	else
	{
		tc_set_block_mode(TC0, TC_BMR_QDEN|TC_BMR_POSEN/*|TC_BMR_EDGPHA*/);
	}
	
	
	tc_start(TC0,0);
	tc_start(TC0,1);
	tc_start(TC0,2);
}

uint16_t sincos_phase_adc_reading[2];
void sincos_store_phs_adc_val(uint16_t *adc_reading)
{
	sincos_phase_adc_reading[0] = adc_reading[0];
	sincos_phase_adc_reading[1] = adc_reading[1];
}

#define prev_enc_val enc_count[0]
uint16_t enc_count[4] = {0, 0, 0, 0};
int64_t enc_quad_offset = 0;
fixedpt enc_theta_frac = 0;
#define SINVALMAX /*3822*/3980
// hold a decmal val of less than 1 enc count
fixedpt enc_phase = 0;	
void sincos_store_val(void)
{
	uint32_t i;
	fixedpt tmp_enc_phase = 0;
	
	if( sincos_phase_adc_reading[0] > SINVALMAX) sincos_phase_adc_reading[0] = SINVALMAX;
	if( sincos_phase_adc_reading[1] > SINVALMAX) sincos_phase_adc_reading[1] = SINVALMAX;
	fixedpt fsin, fcos;
	
	if (menue_drive_data_applied.m_pg_direction == 1)
	{
		fsin = sincos_phase_adc_reading[0];
		fcos = sincos_phase_adc_reading[1];
	}
	else
	{
		fsin = sincos_phase_adc_reading[1];
		fcos = sincos_phase_adc_reading[0]; 
	}
	
	fsin -= SINVALMAX/2;
	fsin *= FIXEDPT_ONE;
	
	
	fcos -= SINVALMAX/2;
	fcos *= FIXEDPT_ONE;
	

	fsin /= (SINVALMAX/2);
	fcos /= (SINVALMAX/2);
	
	tmp_enc_phase = arctan2(-fcos, fsin); // get sincos phase val
	fixedpt tmp_frac;
	// convert the phase val from 360 deg/1 count to 180 deg/1 count
 	if(tmp_enc_phase > fixedpt_rconst(180)) tmp_frac = tmp_enc_phase - fixedpt_rconst(180);
	else tmp_frac = tmp_enc_phase;
	// convert the phase val from deg to frac of 1 enc count
 	tmp_frac /= 180;
	//store the frac part
 	enc_phase = tmp_frac;
	//dacc_interface_writeData(0, enc_phase, FIXEDPT_ONE, 0);
	//enc_phase = 0;
}
fixedpt enc_get_phase (void)
{
	return enc_phase;
	if (enc_get_theta() < 0)
	{
		return FIXEDPT_ONE - enc_phase;
	} 
	else
	{
		return enc_phase;
	}
}

void store_enc_count(void)
{
	uint16_t en_cv = tc_read_cv(TC0,0);
	int32_t difference = en_cv - prev_enc_val;
	int64_t *eenc_val = (int64_t)&enc_count[0];
	prev_enc_val = en_cv;
	if(abs(difference) > 20000)
	{
		// encoder overflow
		if(difference < 0) (*eenc_val) += 0x10000;
		else (*eenc_val) -= 0x10000;
	}
	
	//enc_count[4] = 0;
}

void encoder_alpha_beta_filter_handler(void)
{
	volatile float theta_error;
	float temp_v;
	int32_t i_tmp;
	
	//get the projected theta value
	f_enc_theta_hat_frac += f_enc_omega_hat/(1000) /*sampling frequency*/;
	i_tmp = f_enc_theta_hat_frac;
	enc_theta_hat += i_tmp;
	f_enc_theta_hat_frac -= i_tmp;
	
	//calculate the error
	theta_error = enc_get_lin_theta();
	//dacc_interface_writeData(0, theta_error , 2, 0);
	theta_error -= enc_theta_hat;
	temp_v = enc_phase;
	temp_v /= FIXEDPT_ONE;
	theta_error += (temp_v - f_enc_theta_hat_frac);
	
	f_enc_theta_hat_frac += (ENC_ALPHA * theta_error);
	
	temp_v = (ENC_BETA * theta_error);
	//temp_v = fixedpt_mul(temp_v, fixedpt_rconst(PWM_FREQUENCY));
	f_enc_omega_hat += temp_v;
	
	//filtered_enc_omega_hat += (f_enc_omega_hat - filtered_enc_omega_hat) / (ENC_FILTERATION_TIME);
	filtered_enc_omega_hat = f_enc_omega_hat;
	//dacc_interface_writeData(0, filtered_enc_omega_hat / ENC_COUNT_REV, 1.0, 1);
}
#define	ENCODER_MAX_PERIOD (100)

fixedpt enc_prev_phase = 0, enc_prev_phase2 = 0;
int64_t enc_prev_pos = 0, enc_prev_pos2 = 0;
int8_t inc_period = 1;
float direct_speed_hat = 0;
void encoder_direct_speed_measure(void)
{
	int64_t enc_inc_val;
	fixedpt enc_temp_phase = enc_phase;
	fixedpt tmp;
	float displacement;
	
	
	enc_inc_val = enc_get_theta();
	
	if (1)//fixedpt_abs(enc_inc_val - enc_prev_pos) >= 1)
	{
		displacement = enc_temp_phase - enc_prev_phase;
		displacement /= FIXEDPT_ONE;
		displacement += (enc_inc_val - enc_prev_pos);
		displacement *= 1000; //sampling frequency
		//displacement /= inc_period;
		f_enc_omega_hat = displacement;
		//direct_speed_hat = displacement;
		
		tmp = enc_temp_phase - (2 * enc_prev_phase) + enc_prev_phase2;
		//tmp *= 100;
		//tmp /= FIXEDPT_ONE;
		displacement = tmp;
		displacement /= FIXEDPT_ONE;
		//displacement /= 100;
		displacement += (enc_inc_val - (2 * enc_prev_pos) + enc_prev_pos2);
		displacement *= 1000000; //sampling frequency2
		//displacement /= inc_period;
		f_enc_acc_hat = displacement;
		
		//inc_period = 1;
		
		enc_prev_phase2 = enc_prev_phase;
		enc_prev_pos2 = enc_prev_pos;
		
		enc_prev_phase = enc_temp_phase;
		enc_prev_pos = enc_inc_val;
	} 
	else
	{
		if (++inc_period > ENCODER_MAX_PERIOD)
		{
			inc_period = ENCODER_MAX_PERIOD;
			f_enc_omega_hat = 0;
			//direct_speed_hat = 0;
		}
	}
	filtered_enc_omega_hat = (f_enc_omega_hat * ENC_SPEED_WEIGHT + filtered_enc_omega_hat * ENC_F_SPEED_WEIGHT);
	filtered_enc_acc_hat = (f_enc_acc_hat * ENC_SPEED_WEIGHT + filtered_enc_acc_hat * ENC_F_SPEED_WEIGHT);
}
uint32_t sec_sample = 0;
int64_t prev_test_e_count;
int32_t enc_count_sample;
void encoder_inc_handler(void)
{
	store_enc_count();
	int64_t enc_inc_val = enc_get_theta();
	if (menue_drive_data_applied.m_encoder_type == 0) encoder_alpha_beta_filter_handler(); //incremental encoder
	else encoder_direct_speed_measure(); //sin/cos encoder
}

void enc_set_theta(int64_t pos)
{
	int64_t *tmppos = &pos;
	int64_t *eencval = (int64_t)&enc_count[0];
	int64_t enc_offset = pos - enc_get_theta();
	
	enc_quad_offset = (*tmppos);
	enc_quad_offset -= prev_enc_val;
	*eencval = pos - enc_quad_offset;
	
	enc_prev_pos += enc_offset;
	distance_regulator_correct_offset(enc_offset);
	
	enc_theta_hat = pos;
	pos_regulator_reset_keeping_pos();
}

int64_t enc_get_theta(void)
{
	int64_t *temp = (uint64_t)&enc_count[0];
	return (*temp + enc_quad_offset);
}

int64_t theta_tm1 = 0;
fixedpt phase_tm1 = 0;
int64_t theta_t;
#define sign(x) (x<0)?-1:1
int64_t enc_get_lin_theta(void)
{
	theta_t = enc_get_theta();
	
	if ( fixedpt_abs(theta_t - theta_tm1) < 2)
	{
		if ( fixedpt_abs(enc_phase - phase_tm1) > fixedpt_rconst(0.5)) //phase transition of more than 0.5
		{
			// enc pulse_cross_over
			theta_tm1 -= sign(enc_phase - phase_tm1);
			
		}
	} 
	else
	{
		theta_tm1 = theta_t;
	}
	phase_tm1 = enc_phase;
	return theta_tm1;
}

int64_t enc_get_theta_hat(void)
{
	return enc_theta_hat;
}

fixedpt enc_get_omega_hat(void)
{
	//return enc_count_sample * FIXEDPT_ONE;
	return filtered_enc_omega_hat * FIXEDPT_ONE;
}

fixedpt enc_get_omega_rev_hat(void)
{
	float tmp = filtered_enc_omega_hat / ENC_COUNT_REV;
	tmp *= FIXEDPT_ONE;
	return tmp;
	//return (filtered_enc_omega_hat * FIXEDPT_ONE) / ENC_COUNT_REV;
}

float enc_get_acc_shaft_rad_hat(void)
{
	return (filtered_enc_acc_hat *  M_TWO_PI) / ENC_COUNT_REV ;
}

float enc_get_omega_shaft_rad_hat(void)
{
	return (filtered_enc_omega_hat *  M_TWO_PI) / ENC_COUNT_REV ;
}

fixedpt enc_get_omega_hz_hat(uint32_t poles)
{
	return enc_get_omega_rev_hat() * poles;
}

fixedpt enc_get_omega_rad_hat(uint32_t poles)
{
	return fixedpt_mul(enc_get_omega_hz_hat(poles), FIXEDPT_TWO_PI) ;
}

fixedpt enc_get_omega_rev_hat_d(void)
{
	return (direct_speed_hat * FIXEDPT_ONE) / ENC_COUNT_REV;
}

fixedpt enc_get_omega_hz_hat_d(uint32_t poles)
{
	return enc_get_omega_rev_hat_d() * poles;
}

fixedpt enc_get_omega_rad_hat_d(uint32_t poles)
{
	return fixedpt_mul(enc_get_omega_hz_hat_d(poles), FIXEDPT_TWO_PI) ;
}

fixedpt enc_get_angle_deg(uint32_t poles)
{
	uint32_t tmp = tc_read_cv(TC0,0);
	//tmp = 1000;
	tmp %= ENC_COUNT_REV;
	tmp *= poles;
	fixedpt ang = fixedpt_div(fixedpt_fromint(tmp), fixedpt_fromint(ENC_COUNT_REV));
	ang = fixedpt_mul(ang, fixedpt_rconst(360));
	
	while (ang >= fixedpt_rconst(360)) ang -= fixedpt_rconst(360);
	while (ang < 0) ang += fixedpt_rconst(360);
	return -ang ;
}
float enc_filteration_time = 0;
void enc_set_filter_timeconst(float t_cons)
{
	float temp;
	if (enc_filteration_time != t_cons)
	{
		enc_filteration_time = t_cons;
		if (t_cons == 0)
		{
			speed_weight = 1;
			f_speed_weight = 0;
		}
		else if(t_cons > 0)
		{
			temp = t_cons + 1;
			temp = 1 / temp;
			speed_weight = temp;
			
			temp = 1 / t_cons;
			temp += 1;
			temp = 1 / temp;
			f_speed_weight = temp;
		}
	}
}