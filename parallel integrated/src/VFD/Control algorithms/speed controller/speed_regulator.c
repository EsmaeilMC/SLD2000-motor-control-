/*
 * speed_regulator.c
 *
 * Created: 4/29/2018 2:58:44 AM
 *  Author: Ahmed
 */ 

#include <regulators.h>
#include <speed_regulator.h>
#include <vfdstates.h>
#include <menue_data_manager.h>
#include <encoder_interface.h>
#include <d_faults.h>
#include <ersdstates.h>
#include <motor_data.h>
#include <arm_math.h>
#include <VectorPM.h>
#include <ARB ESO.h>
#include <NLEF.h>
#include <SPT.h>
#include <drive_interface_data.h>
#include <vfd.h>
#include <motor_data.h>
#include <distance_regulator.h>


#define SPD_BAND (menue_drive_data_applied.m_ASR_speed_band)

#define PKP_ZEROSERVO (menue_drive_data_applied.m_APR_zs_p)
#define SKP_ZEROSERVO (menue_drive_data_applied.m_ASR_zs_p)
#define SKI_ZEROSERVO (menue_drive_data_applied.m_ASR_zs_i)

#define MOTOR_RATED_TORQUE (menue_drive_data_applied.m_motor_rated_current * KT)

float ski_ls_var = 1;
float ski_hs_var = 1;
#define SKP_LS (menue_drive_data_applied.m_ASR_lows_p)
#define SKI_LS (ski_ls_var)

#define SKP_HS (menue_drive_data_applied.m_ASR_highs_p)
#define SKI_HS (ski_hs_var)

#define ASR_SW_FREQLS menue_drive_data_applied.m_ASR_sw_lw_point
#define ASR_SW_FREQHS menue_drive_data_applied.m_ASR_sw_hi_point


#define UPS_UPPER_CURRENT_LIMIT (menue_drive_data_applied.m_ard_current_max) //user_settings
float current_limit = DRIVE_UPPER_CURRENT_LIMIT;

float Friction_coef = 0;


#define B_FRICTION (pi_s.kic / SPD_BAND)

uint8_t pos_zs_shutdown = 1;
float pos_zs_contribution = 0, phase_to_keep;
int64_t pos_to_keep = 0;
float pos_err;


void pos_regulator_reset_keeping_pos(void)
{
	pos_to_keep = pos_err + enc_get_lin_theta();
}

void pos_zs_regulate(void)
{
	if (pos_zs_shutdown) return;
	float tmp;
	
	pos_err = pos_to_keep - enc_get_lin_theta(); //generate error signal
	tmp = -enc_get_phase();
	tmp /= FIXEDPT_ONE;
	tmp += phase_to_keep;
	pos_err += tmp;
	
	tmp = PKP_ZEROSERVO * pos_err;
	
	pos_zs_contribution = tmp;
}

volatile float load_torque = 0;
volatile float reg_temp_torque = 0, spt_tourque = 0;
#define ENC_HSPEED_FILTER_TIME (menue_drive_data_applied.m_enc_filter_high)
#define ENC_LSPEED_FILTER_TIME (menue_drive_data_applied.m_enc_filter_low)
struct pi_t pi_s; 

float get_zs_displacement(void)
{	
	return -pos_err;
}

float KT = 1;
float speed_regulator_setKt(float kt)
{
	KT = kt;
}

#define TORQUE_FILTER_TIME (0) //(menue_drive_data_applied.m_motor_noload_current)

float trq_in_weignt = 1, trq_memory_weight = 0;
void calc_torque_filter_constants(void)
{
	if (TORQUE_FILTER_TIME < 0.01)
	{
		trq_in_weignt = 1;
		trq_memory_weight = 0;
	}
	else
	{
		trq_in_weignt = 1 / (1 + TORQUE_FILTER_TIME);
		trq_memory_weight = 1 + (1 / TORQUE_FILTER_TIME);
		trq_memory_weight = 1 / trq_memory_weight;
	}
}

float torque_filter(float com_torque, float prev_torque)
{
	return (com_torque * trq_in_weignt) + (prev_torque * trq_memory_weight);
}

#define ZS_TORQUE_TC (15.0f)
#define ZS_TORQUE_MEM_GAIN (1 / (1 + (1 / ZS_TORQUE_TC)))
#define ZS_TORQUE_NEW_GAIN (1 / (1 + ZS_TORQUE_TC))

float prev_com_torque = 0 , pit = 0;
float speed_regulate(float cmd_speed, float current_speed, uint32_t accel)
{
	
	volatile float tmp, cmd_speed_ele_hz = cmd_speed * MOTOR_POLEPAIR / M_TWO_PI;
	if (MOTOR_IM)
	{
		cmd_speed_ele_hz *= GB_REDUCTION_RATIO;
	}
	
	uint32_t cmd_tourque_filter = 1;
	float s_ff;
	float tmp_torque_lmt ;
	
	if (vfd_is_motor_regen()) tmp_torque_lmt = DRIVE_UPPER_CURRENT_LIMIT * KT;
	else tmp_torque_lmt = current_limit * KT;
	
	if(vfd_states_allowedtomove())
	{
		if(fixedpt_abs(cmd_speed_ele_hz) <= ASR_SW_FREQLS) 
		{
			//Set LS PI controller coef
			reg_pi_change_kp(SKP_LS, &pi_s);
			reg_pi_change_ki(SKI_LS, &pi_s);
			enc_set_filter_timeconst(ENC_LSPEED_FILTER_TIME);
		}
		else if((fixedpt_abs(cmd_speed_ele_hz) > ASR_SW_FREQLS) && (fixedpt_abs(cmd_speed_ele_hz) <= ASR_SW_FREQHS))
		{
			//gradually increase PI controller coef
			tmp = (fixedpt_abs(cmd_speed_ele_hz) - ASR_SW_FREQLS) / (ASR_SW_FREQHS - ASR_SW_FREQLS);
			reg_pi_change_kp(SKP_LS + tmp * (SKP_HS - SKP_LS), &pi_s);
			reg_pi_change_ki(SKI_LS + tmp * (SKI_HS - SKI_LS), &pi_s);
		}
		else if(fixedpt_abs(cmd_speed_ele_hz) > ASR_SW_FREQHS)
		{
			reg_pi_change_kp(SKP_HS, &pi_s);
			reg_pi_change_ki(SKI_HS, &pi_s);
			//if (accel == 0) cmd_tourque_filter = 100;//ENC_HSPEED_FILTER_TIME;
			enc_set_filter_timeconst(ENC_LSPEED_FILTER_TIME);
		}
	}
	

	//current_speed /= (MOTOR_POLEPAIR); //convert to shaft speed in rad/sec
	//current_speed *= 2 * PI;
	if (pos_zs_shutdown)
	{
		tmp = cmd_speed - current_speed; //generate error signal 
		tmp += distance_regulator_get_ffspeed(); //add distance control speed feed forward
		if (MOTOR_PM_CVC) s_ff = load_torque ;//- (current_speed * B_FRICTION)/*actuve damping*/;
		else s_ff = 0;
		tmp = reg_pi_exe(tmp_torque_lmt, tmp, s_ff, 0.001, &pi_s);
		
		if (0)//accel == 0)
		{
			tmp = torque_filter(tmp, prev_com_torque);
		}
		else
		{
			prev_com_torque = 0;
		}
		pit = tmp;
	}
	else
	{
		if (spt_permission_brake_open())
		{
			tmp = 0 - current_speed; //generate error signal
			s_ff = nlef_exe(tmp, 0, SKP_ZEROSERVO);
			reg_temp_torque = arb_eso_exe(s_ff + reg_temp_torque, current_speed);
			s_ff += reg_temp_torque;
			
			//load_torque = (load_torque * ZS_TORQUE_MEM_GAIN) + (s_ff * ZS_TORQUE_NEW_GAIN);
			
			tmp = s_ff;
			pos_zs_regulate();
			tmp += pos_zs_contribution;
			tmp += spt_tourque;
			tmp = nlef_saturation_trim(tmp_torque_lmt, tmp);
			
			load_torque = (load_torque * ZS_TORQUE_MEM_GAIN) + (tmp * ZS_TORQUE_NEW_GAIN);
			
			if (fixedpt_abs(tmp) >= (DRIVE_UPPER_CURRENT_LIMIT * KT))
			{
				zs_servo_err_set;
			}
		}
		else
		{
			tmp = spt_perform();
			//arb_eso_exe(tmp, 0);
			spt_tourque = tmp;
		}
	}

	return tmp / KT; //convert torque to current
}

void start_zero_servo(void)
{
	if(pos_zs_shutdown != 0)
	{
		float tmp;
		tmp = MOTOR_RATED_TORQUE;
		tmp /= FIXEDPT_ONE;
		reg_pi_init(SKP_ZEROSERVO, SKI_ZEROSERVO, &pi_s);
		arb_eso_init();
		spt_init(tmp);
		load_torque = 0;
		reg_temp_torque = 0;
		spt_tourque = 0;
		pos_zs_contribution = 0;
		distance_regulator_disable();
	}
		
	pos_zs_shutdown = 0;
}

void stop_zero_servo(void)
{
	pos_zs_shutdown = 1;
}

void speed_PI_init(void)
{
	if ( (ersd_isOff()) || (UPS_UPPER_CURRENT_LIMIT == 0) )
	{
		current_limit = DRIVE_UPPER_CURRENT_LIMIT;
	} 
	else
	{
		current_limit = UPS_UPPER_CURRENT_LIMIT;
	}
	pos_to_keep = enc_get_lin_theta();
	phase_to_keep = enc_get_phase();
	phase_to_keep /= FIXEDPT_ONE;
	
	calc_torque_filter_constants();
	prev_com_torque = 0;
	distance_regulator_disable();
	if(menue_drive_data_applied.m_motor_type == 0)
	{
		//IM motor
		if(menue_drive_data_applied.m_ASR_lows_i != 0) ski_ls_var = (menue_drive_data_applied.m_ASR_lows_p / menue_drive_data_applied.m_ASR_lows_i);
		if(menue_drive_data_applied.m_ASR_highs_i != 0) ski_hs_var = (menue_drive_data_applied.m_ASR_highs_p / menue_drive_data_applied.m_ASR_highs_i);
		reg_pi_init(SKP_LS, SKI_LS, &pi_s);
	}
}

void speed_PI_changeLSGains(void)
{
	pos_zs_shutdown = 1;
	if(menue_drive_data_applied.m_ASR_lows_i != 0) ski_ls_var = (menue_drive_data_applied.m_ASR_lows_p / menue_drive_data_applied.m_ASR_lows_i);
	if(menue_drive_data_applied.m_ASR_highs_i != 0) ski_hs_var = (menue_drive_data_applied.m_ASR_highs_p / menue_drive_data_applied.m_ASR_highs_i);
	//reg_pi_change_gains(SKP_LS, SKI_LS, &pi_s);
	//reg_pi_add_i_memory_val(pos_zs_contribution, &pi_s);
	//load_torque +=  pos_zs_contribution + spt_tourque;
	if(menue_drive_data_applied.m_motor_type == 1) reg_pi_init(SKP_LS, SKI_LS, &pi_s); //PM motor
	
	//enc_set_filter_timeconst(ENC_LSPEED_FILTER_TIME);
	//current_limit = DRIVE_UPPER_CURRENT_LIMIT;
	//pos_zs_contribution = 0;
}
float get_spt(void)
{
	return spt_tourque;
}

float get_pzs(void)
{
	return pos_zs_contribution;
}

float get_loadtorque(void)
{
	return load_torque;
}

float get_pit(void)
{
	return pit;
}