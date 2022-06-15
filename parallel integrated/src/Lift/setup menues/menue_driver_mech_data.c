/*
 * menue_driver_mech_data.c
 *
 * Created: 10/01/2020 04:45:41 م
 *  Author: ahmed
 */ 

#include <menue_driver_mech_data.h>
#include <menue_data_manager.h>
#include <fixedptc.h>

menue_design_t menue_driver_mech_data;
#define OPT_DRIVER_MECH_MAX 8
option_t opt_driver_mech[OPT_DRIVER_MECH_MAX];

#define dmotor_mech_trachtion_sheave_dia		(opt_driver_mech[0])
#define dmotor_mech_motor_inertia				(opt_driver_mech[1])
#define dmotor_mech_sus_ratio					(opt_driver_mech[2])
#define dmotor_mech_counter_weight				(opt_driver_mech[3])
#define dmotor_mech_cabin_weight				(opt_driver_mech[4])
#define dmotor_mech_payload_weight				(opt_driver_mech[5])
#define dmotor_mech_estimate_inertia			(opt_driver_mech[6])
#define dmotor_mech_inertia						(opt_driver_mech[7])

option_val_t oval_dmech_trachtion_sheave_dia;
option_val_t oval_dmech_motor_inertia;
option_val_t oval_dmech_sus_ratio;
option_val_t oval_dmech_counter_weight;
option_val_t oval_dmech_cabin_weight;
option_val_t oval_dmech_payload_weight;
option_val_t oval_dmech_inertia;

void estimate_inertia(void)
{
	float r2 ,tmp;
	
	r2 = menue_drive_data_setup.m_mech_sheave_dia / 2;
	r2 /= 1000;
	r2 *= r2;
	
	tmp = menue_drive_data_setup.m_mech_payload_weight / 2;
	tmp += menue_drive_data_setup.m_mech_cabin_weight;
	tmp += menue_drive_data_setup.m_mech_counter_weight;
	
	tmp *= r2;
	tmp /= menue_drive_data_setup.m_mech_sus_ratio;
	tmp += menue_drive_data_setup.m_mech_motor_inertia;
	
	if(tmp < 1) tmp = 1;
	if (tmp > 500) tmp = 500;
	
	menue_drive_data_setup.m_mech_inertia = tmp;
}

menue_design_t *menue_driver_mech_data_init(void)
{
	dmotor_mech_trachtion_sheave_dia.text = "Traction sheave dia";
	oval_dmech_trachtion_sheave_dia.data_types = uint16_dt;
	oval_dmech_trachtion_sheave_dia.mantissa_count = 0;
	oval_dmech_trachtion_sheave_dia.real_count = 4;
	oval_dmech_trachtion_sheave_dia.max_value = 1500;
	oval_dmech_trachtion_sheave_dia.min_value = 20;
	oval_dmech_trachtion_sheave_dia.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmech_trachtion_sheave_dia.unit = "mm";
	//oval_dmech_trachtion_sheave_dia.text = "Low speed P gain";
	oval_dmech_trachtion_sheave_dia.val = &menue_drive_data_setup.m_mech_sheave_dia;
	dmotor_mech_trachtion_sheave_dia.option_save_val = &oval_dmech_trachtion_sheave_dia;
//__________________________________________________
	dmotor_mech_motor_inertia.text = "Motor inertia";
	oval_dmech_motor_inertia.data_types = float_dt;
	oval_dmech_motor_inertia.mantissa_count = 3;
	oval_dmech_motor_inertia.real_count = 2;
	oval_dmech_motor_inertia.max_value = 99;
	oval_dmech_motor_inertia.min_value = 0.01;
	oval_dmech_motor_inertia.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmech_motor_inertia.unit = "Kg.m2";
	//oval_dmech_motor_inertia.text = "Low speed P gain";
	oval_dmech_motor_inertia.val = &menue_drive_data_setup.m_mech_motor_inertia;
	dmotor_mech_motor_inertia.option_save_val = &oval_dmech_motor_inertia;
//__________________________________________________
	dmotor_mech_sus_ratio.text = "Suspension ratio";
	oval_dmech_sus_ratio.data_types = uint8_dt;
	oval_dmech_sus_ratio.mantissa_count = 0;
	oval_dmech_sus_ratio.real_count = 1;
	oval_dmech_sus_ratio.max_value = 8;
	oval_dmech_sus_ratio.min_value = 1;
	oval_dmech_sus_ratio.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmech_sus_ratio.unit = "KW";
	//oval_dmech_sus_ratio.text = "Low speed P gain";
	oval_dmech_sus_ratio.val = &menue_drive_data_setup.m_mech_sus_ratio;
	dmotor_mech_sus_ratio.option_save_val = &oval_dmech_sus_ratio;
//__________________________________________________
	dmotor_mech_counter_weight.text = "Counter weight";
	oval_dmech_counter_weight.data_types = float_dt;
	oval_dmech_counter_weight.mantissa_count = 3;
	oval_dmech_counter_weight.real_count = 4;
	oval_dmech_counter_weight.max_value = 8000;
	oval_dmech_counter_weight.min_value = 0;
	oval_dmech_counter_weight.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmech_counter_weight.unit = "Kg";
	//oval_dmech_counter_weight.text = "Low speed P gain";
	oval_dmech_counter_weight.val = &menue_drive_data_setup.m_mech_counter_weight;
	dmotor_mech_counter_weight.option_save_val = &oval_dmech_counter_weight;
	//__________________________________________________
	dmotor_mech_payload_weight.text = "PayLoad weight";
	oval_dmech_payload_weight.data_types = float_dt;
	oval_dmech_payload_weight.mantissa_count = 3;
	oval_dmech_payload_weight.real_count = 4;
	oval_dmech_payload_weight.max_value = 8000;
	oval_dmech_payload_weight.min_value = 0;
	oval_dmech_payload_weight.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmech_payload_weight.unit = "Kg";
	//oval_dmech_payload_weight.text = "Low speed P gain";
	oval_dmech_payload_weight.val = &menue_drive_data_setup.m_mech_payload_weight;
	dmotor_mech_payload_weight.option_save_val = &oval_dmech_payload_weight;
	//__________________________________________________
	dmotor_mech_cabin_weight.text = "Cabin weight";
	oval_dmech_cabin_weight.data_types = float_dt;
	oval_dmech_cabin_weight.mantissa_count = 3;
	oval_dmech_cabin_weight.real_count = 4;
	oval_dmech_cabin_weight.max_value = 8000;
	oval_dmech_cabin_weight.min_value = 0;
	oval_dmech_cabin_weight.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmech_cabin_weight.unit = "Kg";
	//oval_dmech_cabin_weight.text = "Low speed P gain";
	oval_dmech_cabin_weight.val = &menue_drive_data_setup.m_mech_cabin_weight;
	dmotor_mech_cabin_weight.option_save_val = &oval_dmech_cabin_weight;
	//__________________________________________________
	dmotor_mech_estimate_inertia.text = "Estimate inertia";
	dmotor_mech_estimate_inertia.enter = &estimate_inertia;
	
	//__________________________________________________
	dmotor_mech_inertia.text = "Total inertia";
	oval_dmech_inertia.data_types = float_dt;
	oval_dmech_inertia.mantissa_count = 3;
	oval_dmech_inertia.real_count = 3;
	oval_dmech_inertia.max_value = 400;
	oval_dmech_inertia.min_value = 1;
	oval_dmech_inertia.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmech_inertia.unit = "Kg.m2";
	///oval_dmech_inertia.text = "Low speed P gain";
	oval_dmech_inertia.val = &menue_drive_data_setup.m_mech_inertia;
	dmotor_mech_inertia.option_save_val = &oval_dmech_inertia;

//__________________________________________________

	fill_menue_list(&menue_driver_mech_data, opt_driver_mech, OPT_DRIVER_MECH_MAX, 0, 0);
	return &menue_driver_mech_data;
}