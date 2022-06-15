/*
 * PM_param_tune1.c
 *
 * Created: 28/08/2018 01:39:57 م
 *  Author: ahmed
 */ 
#include <asf.h>
#include <fixedptc.h>
#include <menue_data_manager.h>
#include <fastmath.h>
#include <SCH.h>
#include <PM_param_tune1.h>
#include <encoder_interface.h>
#include <motor_data.h>
#include <vfd.h>
#include <Inverter.h>
#include <axistransform.h>
#include <regulators.h>
#include <arm_math.h>
#include <DACC_interface.h>
#include <pop_ups.h>

struct pi_t pi_d;

float motor_test_current;

#define PM_TST_RS_CURRENT (motor_test_current)
#define PM_mul (10)
#define SAMPLE_FREQ (1000 * PM_mul)
#define STG_01_PER (200 * PM_mul)
#define STG_12_PER (500 * PM_mul)
#define STG_23_PER (300 * PM_mul)
#define STG_34_PER (5000 * PM_mul)

#define TUNE_VMIN		(10)
#define TUNE_VMAX		(300)
#define TUNE_VSTEPS		(50)
#define TUNE_FMIN		(100)
#define TUNE_FMAX		(500)
#define TUNE_FSTEPS		(100)

uint32_t stg_timeout_counter = 0, sin_wave_timer = 0;
uint8_t pm_param_tune_stage = 0;
float vd, id, p_vd, p_id;
float vms, ims, com_v;
float rs, ls;
float test_sig_freq, test_sig_reset_val;

void calc_ms(uint32_t weight)
{
	p_vd = (vd - p_vd)/2;
	p_vd += vd;
	
	p_id = (id - p_id)/2;
	p_id += id;
	
	vms += (p_vd * p_vd) / weight;
	ims += (p_id * p_id) / weight;
	
	p_vd = vd;
	p_id = id;
}

void pm_param_t_setFreq(float freq)
{
	
	test_sig_freq = freq / (SAMPLE_FREQ); //divide by sample freq
	test_sig_freq *= (2 * PI);
	test_sig_reset_val = 5 * (SAMPLE_FREQ) / freq;
}

void pm_param_t_setVrms(float vrms)
{
	com_v = vrms * sqrt(2);
}

void pm_param_t_outSin(void)
{
	vd = com_v * (arm_sin_f32(test_sig_freq * sin_wave_timer) + 0);
	sin_wave_timer++;
	if (sin_wave_timer > test_sig_reset_val) sin_wave_timer -= test_sig_reset_val;
	//dacc_interface_writeData(0, vd, com_v, 1);
	//dacc_interface_writeData(0, vd, com_v, 1);
}

void PM_param_tune1_current_handler(void)
{
	axt_park_exe();
	
	id = axt_get_id();

	//dacc_interface_writeData(1, id, 8, 1);
	if ( (pm_param_tune_stage == 0) || (pm_param_tune_stage == 1) )
	{
		//out fixed current stage
		vd = (PM_TST_RS_CURRENT - id); //error signal.
		vd = reg_pi_exe(537.4f, vd, 0, TSAMPLE, &pi_d);
		//vd *= PM_TUNE_CURRENT_P;
	} 
	else if ( (pm_param_tune_stage == 2) || (pm_param_tune_stage == 3) )
	{
		//search for fixed frequency and voltage level to measure impedance
	} 
	else
	{
		inverter_out_rect(0, 0);
		return;
	}
	
	float valpha, vbeta;

	axt_ipark_exe( vd, 0, &valpha, &vbeta );
	inverter_out_rect((valpha * FIXEDPT_ONE), (vbeta * FIXEDPT_ONE));
}

uint8_t pm_tune_comm = 0, start_pm_tune = 1;
void pm_param_tune1_init(void)
{
	reg_pi_init(menue_drive_data_applied.m_ACRD_p, menue_drive_data_applied.m_ACRD_i, &pi_d);
	
	motor_test_current = fixedpt_tofloat(menue_drive_data_applied.m_motor_rated_current / sqrt(8));
	pm_tune_comm = menue_drive_motor_tune;
	pm_param_tune_stage = 0;
	stg_timeout_counter = 0;
	vms = 0;
	ims = 0;
	float flux_angle = encoder_getangle_elec_abs(MOTOR_POLEPAIR) + (menue_drive_data_applied.m_pole_pos_offset);
	start_pm_tune = 0;
	axt_set_theta(flux_angle);
}



uint8_t test_voltage_step, test_freq_step;
void enter_3rd_stage()
{
	vms = 0;
	ims = 0;
	vd = 0;
	
	stg_timeout_counter = 0;
	pm_param_tune_stage++;
	sin_wave_timer = 0;
	
	pm_param_t_setVrms(TUNE_VMIN + ((TUNE_VMAX - TUNE_VMIN) / TUNE_VSTEPS) * test_voltage_step);
	pm_param_t_setFreq(TUNE_FMAX - ((TUNE_FMAX - TUNE_FMIN) / TUNE_FSTEPS) * test_freq_step);
}

#define TUNE_CONTROLLER_BANDWIDTH (0.00007)
#define TUNE_OMEGA (8 * 2 * PI)
void pm_tune_calc_PI(void)
{
	menue_drive_data_setup.m_ACRD_i = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_stator_resistance * 10000;
	menue_drive_data_setup.m_ACRD_p = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_leakage_inductance * 1000;
	menue_drive_data_setup.m_ACRQ_i = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_stator_resistance * 10000;
	menue_drive_data_setup.m_ACRQ_p = TUNE_OMEGA * TUNE_CONTROLLER_BANDWIDTH * menue_drive_data_setup.m_motor_leakage_inductance * 1000;
}

void pm_tune_calc_ke(void)
{
	float ke = menue_drive_data_applied.m_motor_rated_power * 1000;
	ke /= sqrt(6) * PI * fixedpt_tofloat(fixedpt_mul(menue_drive_data_applied.m_motor_rated_freq, menue_drive_data_applied.m_motor_rated_current));
	menue_drive_data_setup.m_motor_BEMF = ke;
}
 
void pm_tune_calc_data(void)
{
	//vms /= 3; //vrms = voffrms/sqrt(3)
	//float tmp = 2 * vms / rs; //= irms ^ 2
	//rs = sqrt(rs);
	//rs -= 1.5;
	//rs *= rs;
	ls = vms / ims;
	ls -= rs;
	ls = sqrt(ls);
	//ls /= SAMPLE_FREQ;
	ls /= (test_sig_freq * SAMPLE_FREQ);
	menue_drive_data_setup.m_motor_stator_resistance = sqrt(rs);
	menue_drive_data_setup.m_motor_leakage_inductance = (ls) * 1000; // inductance in mH
	
	if (pm_tune_comm > 1) pm_tune_calc_PI();
	if (pm_tune_comm > 2) pm_tune_calc_ke();
	pop_ups_add("Tune Successful", 1500);
}
void pm_param_tune_handler(void)
{
	if (start_pm_tune)
	{
		return;
	}
	if (!PM_PARAM_TUNE)
	{
		//sch_delete_task(pm_tune_index);
		start_pm_tune = 1;
		return;
	}
	
	if (pm_param_tune_stage == 0)
	{
		//out fixed current stage
		stg_timeout_counter++;
		if (stg_timeout_counter == STG_01_PER)
		{
			stg_timeout_counter = 0;
			pm_param_tune_stage++;
			vms = 0;
			ims = 0;
		}
	}
	else if (pm_param_tune_stage == 1)
	{
		//measure Rs
		vms += (vd * vd) / STG_12_PER;
		ims += (id * id) / STG_12_PER;
		stg_timeout_counter++;
		if (stg_timeout_counter == STG_12_PER)
		{
			rs = vms / ims;
			
			test_voltage_step = 0;
			test_freq_step = 0;
			
			vms = 0;
			ims = 0;
			vd = 0;
			
			stg_timeout_counter = 0;
			pm_param_tune_stage++;
			sin_wave_timer = 0;
			pm_param_t_setVrms(TUNE_VMIN);
			pm_param_t_setFreq(TUNE_FMAX);
		}
	}
	else if (pm_param_tune_stage == 2)
	{
		//search for fixxed frequency and voltage level to measure impedance
		stg_timeout_counter++;
		pm_param_t_outSin();
		vms += (vd * vd) / STG_23_PER;
		ims += (id * id) / STG_23_PER;
/*		calc_ms(STG_23_PER);*/
		if (stg_timeout_counter == STG_23_PER)
		{
			stg_timeout_counter = 0;
			if (ims >= (PM_TST_RS_CURRENT * PM_TST_RS_CURRENT))
			{
				enter_3rd_stage();
			}
			
			if (test_voltage_step < TUNE_VSTEPS)
			{
				//we still incrementing voltage
				test_voltage_step ++;
				
				vms = 0;
				ims = 0;
				vd = 0;
				
				stg_timeout_counter = 0;
				sin_wave_timer = 0;
				pm_param_t_setVrms(TUNE_VMIN + ((TUNE_VMAX - TUNE_VMIN) / TUNE_VSTEPS) * test_voltage_step);
				pm_param_t_setFreq(TUNE_FMAX);
			} 
			else
			{
				//voltage limit reached
				if (test_freq_step < TUNE_FSTEPS)
				{
					//we still decrementing freq
					test_freq_step ++;
					
					vms = 0;
					ims = 0;
					vd = 0;
					
					stg_timeout_counter = 0;
					sin_wave_timer = 0;
					pm_param_t_setVrms(TUNE_VMAX);
					pm_param_t_setFreq(TUNE_FMAX - ((TUNE_FMAX - TUNE_FMIN) / TUNE_FSTEPS) * test_freq_step);
				} 
				else
				{
					//freq limit reached
					enter_3rd_stage();
				}
			}
			
		}
	}
	else if (pm_param_tune_stage == 3)
	{
		//measure Ls
		stg_timeout_counter++;
		pm_param_t_outSin();
 		vms += (vd * vd) / STG_34_PER;
 		ims += (id * id) / STG_34_PER;
//		calc_ms(STG_34_PER);
		if (stg_timeout_counter == STG_34_PER)
		{
			stg_timeout_counter = 0;
			pm_param_tune_stage++;
		}
	}
	else
	{
		start_pm_tune = 1;
		sch_add_task_ms(&pm_tune_calc_data, 100, 0);
		vfd_estop();
	}
}

float pm_param_getdata1(void)
{
	//return sqrt(ims);
	//return (test_sig_freq * SAMPLE_FREQ) / (2 * PI);
	return (ls * 1000);// / sqrt(3);
}