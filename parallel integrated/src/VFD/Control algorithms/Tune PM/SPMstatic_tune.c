/*
 * SPMstatic_tune.c
 *
 * Created: 09/07/2018 05:16:44 م
 *  Author: ahmed
 */ 

#include <SPMstatic_tune.h>
#include <fixedptc.h>
#include <motor_data.h>
#include <axistransform.h>
#include <Inverter.h>
#include <regulators.h>
#include <encoder_interface.h>
#include <d_faults.h>
#include <pop_ups.h>

uint32_t dummy_test = 0;

#define TEST_CURRENT fixedpt_tofloat((menue_drive_data_applied.m_motor_rated_current * menue_drive_data_applied.m_encoder_search_current)/100)
#define TEST_VECTOR_DELAY_VAL (8000 / 8)
#define TEST_VOLTAGE (200)
#define VECTOR_F_POWER (4 - 1)
#define SEC_COUNT (10000)
#define FILTER_WIGHT (0.1F)

float vector_f_time, id_f = 0;
float test_vectors[12];
void gen_test_vectors(float first_angle, float sec_angle)
{
	int i;
	float ang_offset = sec_angle - first_angle;
	while(ang_offset >= 360) ang_offset -= (360);
	while(ang_offset < 0) ang_offset += (360);
	ang_offset /= 11;
	
	for (i=0; i<12; i++)
	{
		test_vectors[i] = first_angle + i * ang_offset;
		while(test_vectors[i] >= 360) test_vectors[i] -= (360);
		while(test_vectors[i] < 0) test_vectors[i] += (360);
	}
}

uint16_t vector_index = 0,  chosen_vector_index = 0;
uint32_t tune_vector_time_counter; 
float vector_min_time = 0xFFFFFFFF;
uint32_t tune_vector_out(float flux_angle)
{
 	float id;
 	float tmp, com_vd;
 	
 	while(flux_angle > 360) flux_angle -= (360);
 	while(flux_angle < 0) flux_angle += (360);
 	axt_set_theta(flux_angle);
 	axt_park_exe();
 	
 	id = axt_get_id();
	id_f = (id_f * (1-FILTER_WIGHT) + id * FILTER_WIGHT);

	com_vd = TEST_VOLTAGE;
 	float valpha, vbeta;
	
	//return 1;
	if (id_f >= TEST_CURRENT)
	{
		inverter_out_rect(0, 0);
		return 1;
	}
 	axt_ipark_exe(com_vd, 0, &valpha, &vbeta );
 	inverter_out_rect(fixedpt_rconst(valpha), fixedpt_rconst(vbeta));
	tune_vector_time_counter++;
	if (tune_vector_time_counter > SEC_COUNT) tune1_err_set;
	return 0;
}


uint32_t test12vectors_delay = 0;
uint8_t test_vector_f_timer = 0;
uint32_t test12vectors(void)
{
	float tmp = 0;
	if (test12vectors_delay > 0)
	{
		//delay for current decay
		test12vectors_delay--;
		inverter_set_override();
		tune_vector_time_counter = 0;
		id_f = 0;
		return 0;
	} 
	else
	{
		//start testing for a vector
		inverter_clear_override();
	}
	
	if(vector_index > 11) 
	{
		// all 12 vectors are tested
		inverter_set_override();
		return 1;
	}
	if (tune_vector_out(test_vectors[vector_index]))
	{
		//The vector has been tested
		tmp = tune_vector_time_counter;
		tmp = tmp / (VECTOR_F_POWER + 1);
		vector_f_time += tmp;
		dummy_test = vector_f_time;
		if (test_vector_f_timer >= VECTOR_F_POWER)
		{
			if (vector_f_time <= vector_min_time)
			{
				//found a closer vector to rotor angel
				vector_min_time = vector_f_time;
				chosen_vector_index = vector_index;
			}
			vector_index++;
			test_vector_f_timer = 0; //reset the vector avarage testing  time
			vector_f_time = 0; //reset filtered time
		} 
		else
		{
			//repeat testing the same vecotor
			test_vector_f_timer++;
		}
		
		test12vectors_delay = TEST_VECTOR_DELAY_VAL;
		tune_vector_time_counter = 0; //reset the vector testing  time
		id_f = 0;
	}
	return 0;
}

uint8_t spm_stage_counter = 0;
float tune_enc_angel;
void SPMtune_init(void)
{
	spm_stage_counter = 0;
	tune_enc_angel = encoder_getangle_elec_abs(MOTOR_POLEPAIR);
	
	gen_test_vectors(0, 330);
	vector_index = 0;
	test12vectors_delay = TEST_VECTOR_DELAY_VAL;
	vector_min_time = 0xFFFFFFFF;
	vector_f_time = 0;
}

void SPMstatic_tune_current_handler(void)
{
	uint8_t v1, v2;
	if (spm_stage_counter == 0)
	{
		//stage 0 test 30 deg vect
		if (test12vectors())
		{
			spm_stage_counter++;
			v2 = chosen_vector_index + 1;
			if(v2 > 11) v2 = 0; // overflow val is 11
			if(chosen_vector_index == 0) v1 = 11;
			else v1 = chosen_vector_index - 1;

			gen_test_vectors(test_vectors[v1], test_vectors[v2]);
			vector_index = 0;
			test12vectors_delay = TEST_VECTOR_DELAY_VAL;
			vector_min_time = 0xFFFFFFFF;
			vector_f_time = 0;
		}
	}
	else if (spm_stage_counter == 1)
	{
		if (test12vectors())
		{
			spm_stage_counter++;
			v2 = chosen_vector_index + 1;
			if(v2 > 11) v2 = 0; // overflow val is 11
			if(chosen_vector_index == 0) v1 = 11;
			else v1 = chosen_vector_index - 1;

			gen_test_vectors(test_vectors[v1], test_vectors[v2]);
			vector_index = 0;
			test12vectors_delay = TEST_VECTOR_DELAY_VAL;
			vector_min_time = 0xFFFFFFFF;
			vector_f_time = 0;
		}
	}
	else if (spm_stage_counter == 2)
	{
		if (test12vectors())
		{
			spm_stage_counter++;
			
			float ret_angle = test_vectors[chosen_vector_index] - tune_enc_angel;
			while(ret_angle > 360) ret_angle -= (360);
			while(ret_angle < 0) ret_angle += (360);
			menue_drive_data_setup.m_pole_pos_offset = ret_angle;
			pop_ups_add("ENC_Tune Successful", 1500);
		}
	} 
	else
	{
		return;
	}
	
}
uint32_t SPMstatic_getchosenvec(void)
{
	//return chosen_vector_index;
	float ret_angle = test_vectors[chosen_vector_index] - tune_enc_angel;
	while(ret_angle > 360) ret_angle -= (360);
	while(ret_angle < 0) ret_angle += (360);
	return ret_angle;
}