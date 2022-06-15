/*
 * menue_driver_vf.c
 *
 * Created: 30/05/2017 03:45:59 م
 *  Author: ahmed
 */ 

#include <menue_driver_vf.h>
#include <menue_data_manager.h>
#include <fixedptc.h>

//vector control list

menue_design_t menue_driver_vf;
#define OPT_DRIVER_VF_MAX 7
option_t opt_driver_vf[OPT_DRIVER_VF_MAX];
		
#define dmotor_vf_m_vmin						(opt_driver_vf[0])
#define dmotor_vf_m_fmin						(opt_driver_vf[1])
#define dmotor_vf_m_vm							(opt_driver_vf[2])
#define dmotor_vf_m_fm							(opt_driver_vf[3])
#define dmotor_vf_m_tboost						(opt_driver_vf[4])
#define dmotor_vf_m_break_freq					(opt_driver_vf[5])
#define dmotor_vf_m_break_voltage				(opt_driver_vf[6])


option_val_t oval_dmvf_m_vmin;
option_val_t oval_dmvf_m_fmin;
option_val_t oval_dmvf_m_vm;
option_val_t oval_dmvf_m_fm;
option_val_t oval_dmvf_m_tboost;
option_val_t oval_dmvf_m_break_freq;
option_val_t oval_dmvf_m_break_voltage;


menue_design_t *menue_driver_vf_init(void)
{

	dmotor_vf_m_vmin.text = "Vmin";
	oval_dmvf_m_vmin.data_types = fixedpt_dt;
	oval_dmvf_m_vmin.mantissa_count = 2;
	oval_dmvf_m_vmin.real_count = 2;
	oval_dmvf_m_vmin.max_value = 99;
	oval_dmvf_m_vmin.min_value = 0.0;
	oval_dmvf_m_vmin.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvf_m_vmin.unit = "V";
	//oval_dmvf_m_vmin.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvf_m_vmin.val = &menue_drive_data_setup.m_vmin;
	dmotor_vf_m_vmin.option_save_val = &oval_dmvf_m_vmin;
	//__________________________________________________
	dmotor_vf_m_fmin.text = "Fmin";
	oval_dmvf_m_fmin.data_types = fixedpt_dt;
	oval_dmvf_m_fmin.mantissa_count = 2;
	oval_dmvf_m_fmin.real_count = 2;
	oval_dmvf_m_fmin.max_value = 99;
	oval_dmvf_m_fmin.min_value = 0.0;
	oval_dmvf_m_fmin.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvf_m_fmin.unit = "Hz";
	//oval_dmvf_m_fmin.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvf_m_fmin.val = &menue_drive_data_setup.m_fmin;
	dmotor_vf_m_fmin.option_save_val = &oval_dmvf_m_fmin;
	//__________________________________________________
	dmotor_vf_m_vm.text = "Vm";
	oval_dmvf_m_vm.data_types = fixedpt_dt;
	oval_dmvf_m_vm.mantissa_count = 2;
	oval_dmvf_m_vm.real_count = 3;
	oval_dmvf_m_vm.max_value = 400;
	oval_dmvf_m_vm.min_value = 0.0;
	oval_dmvf_m_vm.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvf_m_vm.unit = "V";
	//oval_dmvf_m_vm.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvf_m_vm.val = &menue_drive_data_setup.m_vm;
	dmotor_vf_m_vm.option_save_val = &oval_dmvf_m_vm;
	//__________________________________________________
	dmotor_vf_m_fm.text = "Fm";
	oval_dmvf_m_fm.data_types = fixedpt_dt;
	oval_dmvf_m_fm.mantissa_count = 2;
	oval_dmvf_m_fm.real_count = 2;
	oval_dmvf_m_fm.max_value = 50;
	oval_dmvf_m_fm.min_value = 0.0;
	oval_dmvf_m_fm.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvf_m_fm.unit = "Hz";
	//oval_dmvf_m_fm.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvf_m_fm.val = &menue_drive_data_setup.m_fm;
	dmotor_vf_m_fm.option_save_val = &oval_dmvf_m_fm;
	//__________________________________________________
	dmotor_vf_m_tboost.text = "Torque boost";
	oval_dmvf_m_tboost.data_types = fixedpt_dt;
	oval_dmvf_m_tboost.mantissa_count = 2;
	oval_dmvf_m_tboost.real_count = 3;
	oval_dmvf_m_tboost.max_value = 300;
	oval_dmvf_m_tboost.min_value = 0.0;
	oval_dmvf_m_tboost.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvf_m_tboost.unit = "Hz";
	//oval_dmvf_m_tboost.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvf_m_tboost.val = &menue_drive_data_setup.m_tboost;
	dmotor_vf_m_tboost.option_save_val = &oval_dmvf_m_tboost;
	//__________________________________________________
	dmotor_vf_m_break_freq.text = "Break frequency";
	oval_dmvf_m_break_freq.data_types = fixedpt_dt;
	oval_dmvf_m_break_freq.mantissa_count = 2;
	oval_dmvf_m_break_freq.real_count = 2;
	oval_dmvf_m_break_freq.max_value = 99;
	oval_dmvf_m_break_freq.min_value = 0.0;
	oval_dmvf_m_break_freq.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvf_m_break_freq.unit = "Hz";
	//oval_dmvf_m_break_freq.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvf_m_break_freq.val = &menue_drive_data_setup.m_break_freq;
	dmotor_vf_m_break_freq.option_save_val = &oval_dmvf_m_break_freq;
	//__________________________________________________
	dmotor_vf_m_break_voltage.text = "Break voltage";
	oval_dmvf_m_break_voltage.data_types = fixedpt_dt;
	oval_dmvf_m_break_voltage.mantissa_count = 2;
	oval_dmvf_m_break_voltage.real_count = 3;
	oval_dmvf_m_break_voltage.max_value = 380;
	oval_dmvf_m_break_voltage.min_value = 0.0;
	oval_dmvf_m_break_voltage.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvf_m_break_voltage.unit = "V";
	//oval_dmvf_m_break_voltage.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvf_m_break_voltage.val = &menue_drive_data_setup.m_break_voltage;
	dmotor_vf_m_break_voltage.option_save_val = &oval_dmvf_m_break_voltage;
	//__________________________________________________	
	fill_menue_list(&menue_driver_vf, opt_driver_vf, OPT_DRIVER_VF_MAX, 0, 0);
	return &menue_driver_vf;
}