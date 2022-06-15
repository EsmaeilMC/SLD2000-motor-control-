/*
 * menue_driver_motor_parameters.c
 *
 * Created: 02/05/2017 04:59:08 م
 *  Author: ahmed
 */ 
#include <menue_driver_motor_parameters.h>
#include <menue_data_manager.h>
#include <fixedptc.h>
#include <drive_interface_data.h>

//motor parameters list

menue_design_t menue_driver_motor_param;
#define OPT_DRIVER_MOTOR_PARAM_MAX 17
option_t opt_driver_motor_param[OPT_DRIVER_MOTOR_PARAM_MAX];

#define dmotor_param_m_type								(opt_driver_motor_param[0])
#define dmotor_param_rated_power						(opt_driver_motor_param[1])
#define dmotor_param_rated_freq							(opt_driver_motor_param[2])
#define dmotor_param_rated_speed						(opt_driver_motor_param[3])
#define dmotor_param_rated_voltage						(opt_driver_motor_param[4])
#define dmotor_param_rated_current						(opt_driver_motor_param[5])
#define dmotor_param_rated_pf							(opt_driver_motor_param[6])
#define dmotor_param_pole_pair							(opt_driver_motor_param[7])
#define dmotor_param_auto_tune							(opt_driver_motor_param[8])
#define dmotor_param_stator_res							(opt_driver_motor_param[9])
#define dmotor_param_rotor_res							(opt_driver_motor_param[10])
#define dmotor_param_leakage_induct						(opt_driver_motor_param[11])
#define dmotor_param_magnetizing_induct					(opt_driver_motor_param[12])
#define dmotor_param_no_load_current					(opt_driver_motor_param[13])
#define dmotor_param_LD									(opt_driver_motor_param[14])
#define dmotor_param_LQ									(opt_driver_motor_param[15])
#define dmotor_param_BEMF								(opt_driver_motor_param[16])

option_val_t oval_dmparam_rated_power;
option_val_t oval_dmparam_rated_freq;
option_val_t oval_dmparam_rated_speed;
option_val_t oval_dmparam_rated_voltage;
option_val_t oval_dmparam_rated_current;
option_val_t oval_dmparam_rated_pf;
option_val_t oval_dmparam_stator_res;
option_val_t oval_dmparam_rotor_res;
option_val_t oval_dmparam_leakage_induct;
option_val_t oval_dmparam_LD;
option_val_t oval_dmparam_LQ;
option_val_t oval_dmparam_magnetizing_induct;
option_val_t oval_dmparam_no_load_current;
option_val_t oval_dmparam_pole_pair;
option_val_t oval_dmparam_BEMF;
option_val_t oval_dmparam_auto_tune;
//__________________________________________________
//motor type list
menue_design_t menue_dmparam_m_type;

#ifdef PMSM_MODEL
#define OPT_DMPARAM_M_TYPE_MAX 2
#else
#define OPT_DMPARAM_M_TYPE_MAX 1
#endif

option_t opt_dmparam_m_type[OPT_DMPARAM_M_TYPE_MAX];
#define m_type_induct				(opt_dmparam_m_type[0])
#define m_type_sync					(opt_dmparam_m_type[1])

void calc_syn_motor_nPoles(void)
{
	fixedpt temp = menue_drive_data_setup.m_motor_rated_speed;
	if (menue_drive_data_setup.m_motor_type == 1) //Syn motor
	{
		temp /= 60;
		temp = fixedpt_div(menue_drive_data_setup.m_motor_rated_freq, temp);
		menue_drive_data_setup.m_motor_pole_pair_count = fixedpt_toint(temp);
	}
}

menue_design_t *menue_driver_motor_parameters_init(void)
{
	dmotor_param_m_type.text = "Motor type";
	m_type_induct.text = "Induction";
	#ifdef PMSM_MODEL
	m_type_sync.text = "Synchronous";
	#endif
	
	fill_menue_list(&menue_dmparam_m_type, &opt_dmparam_m_type, OPT_DMPARAM_M_TYPE_MAX, 0, &menue_drive_data_setup.m_motor_type);

	// set the 'option ladvanced_req_pins' child

	dmotor_param_m_type.child = &menue_dmparam_m_type;
//___________________________________________________________
	dmotor_param_rated_power.text = "Rated power";
	oval_dmparam_rated_power.data_types = float_dt;
	oval_dmparam_rated_power.mantissa_count = 3;
	oval_dmparam_rated_power.real_count = 2;
	oval_dmparam_rated_power.max_value = 15;
	oval_dmparam_rated_power.min_value = 0.75;
	oval_dmparam_rated_power.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_rated_power.unit = "KW";
	//oval_dmparam_rated_power.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_rated_power.val = &menue_drive_data_setup.m_motor_rated_power;
	dmotor_param_rated_power.option_save_val = &oval_dmparam_rated_power;
//__________________________________________________
	dmotor_param_rated_freq.text = "Rated frequency";
	oval_dmparam_rated_freq.data_types = fixedpt_dt;
	oval_dmparam_rated_freq.mantissa_count = 2;
	oval_dmparam_rated_freq.real_count = 2;
	oval_dmparam_rated_freq.max_value = 100;
	oval_dmparam_rated_freq.min_value = 1;
	oval_dmparam_rated_freq.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_rated_freq.unit = "Hz";
	//oval_dmparam_rated_freq.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_rated_freq.val = &menue_drive_data_setup.m_motor_rated_freq;
	dmotor_param_rated_freq.option_save_val = &oval_dmparam_rated_freq;
//__________________________________________________
	dmotor_param_rated_speed.text = "Rated speed";
	oval_dmparam_rated_speed.data_types = fixedpt_dt;
	oval_dmparam_rated_speed.mantissa_count = 2;
	oval_dmparam_rated_speed.real_count = 4;
	oval_dmparam_rated_speed.max_value = 3000;
	oval_dmparam_rated_speed.min_value = 1;
	oval_dmparam_rated_speed.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_rated_speed.unit = "RPM";
	//oval_dmparam_rated_speed.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_rated_speed.val = &menue_drive_data_setup.m_motor_rated_speed;
	dmotor_param_rated_speed.option_save_val = &oval_dmparam_rated_speed;
	dmotor_param_rated_speed.exit_menue = &calc_syn_motor_nPoles;
//__________________________________________________
	dmotor_param_rated_voltage.text = "Rated voltage";
	oval_dmparam_rated_voltage.data_types = fixedpt_dt;
	oval_dmparam_rated_voltage.mantissa_count = 2;
	oval_dmparam_rated_voltage.real_count = 3;
	oval_dmparam_rated_voltage.max_value = 450;
	oval_dmparam_rated_voltage.min_value = 1;
	oval_dmparam_rated_voltage.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_rated_voltage.unit = "V";
	//oval_dmparam_rated_voltage.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_rated_voltage.val = &menue_drive_data_setup.m_motor_rated_voltage;
	dmotor_param_rated_voltage.option_save_val = &oval_dmparam_rated_voltage;
//__________________________________________________
	dmotor_param_rated_current.text = "Rated current";
	oval_dmparam_rated_current.data_types = fixedpt_dt;
	oval_dmparam_rated_current.mantissa_count = 2;
	oval_dmparam_rated_current.real_count = 2;
	oval_dmparam_rated_current.max_value = 50;
	oval_dmparam_rated_current.min_value = 1;
	oval_dmparam_rated_current.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_rated_current.unit = "A";
	//oval_dmparam_rated_current.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_rated_current.val = &menue_drive_data_setup.m_motor_rated_current;
	dmotor_param_rated_current.option_save_val = &oval_dmparam_rated_current;
//__________________________________________________
	dmotor_param_rated_pf.text = "Power factor";
	oval_dmparam_rated_pf.data_types = float_dt;
	oval_dmparam_rated_pf.mantissa_count = 2;
	oval_dmparam_rated_pf.real_count = 2;
	oval_dmparam_rated_pf.max_value = 99.99;
	oval_dmparam_rated_pf.min_value = 60;
	oval_dmparam_rated_pf.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_rated_pf.unit = "%";
	//oval_dmparam_rated_pf.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_rated_pf.val = &menue_drive_data_setup.m_motor_rated_pfactor;
	dmotor_param_rated_pf.option_save_val = &oval_dmparam_rated_pf;
//__________________________________________________
	dmotor_param_auto_tune.text = "Auto tune";
	oval_dmparam_auto_tune.data_types = uint8_dt;
	oval_dmparam_auto_tune.mantissa_count = 0;
	oval_dmparam_auto_tune.real_count = 1;
	oval_dmparam_auto_tune.max_value = 3;
	oval_dmparam_auto_tune.min_value = 0;
	//oval_dmparam_auto_tune.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmparam_auto_tune.unit = "%";
	//oval_dmparam_auto_tune.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_auto_tune.val = &menue_drive_motor_tune;
	dmotor_param_auto_tune.option_save_val = &oval_dmparam_auto_tune;
//__________________________________________________
	dmotor_param_stator_res.text = "Stator resistance";
	oval_dmparam_stator_res.data_types = float_dt;
	oval_dmparam_stator_res.mantissa_count = 3;
	oval_dmparam_stator_res.real_count = 3;
	oval_dmparam_stator_res.max_value = 999;
	oval_dmparam_stator_res.min_value = 0.001;
	oval_dmparam_stator_res.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_stator_res.unit = "Ohm";
	//oval_dmparam_stator_res.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_stator_res.val = &menue_drive_data_setup.m_motor_stator_resistance;
	dmotor_param_stator_res.option_save_val = &oval_dmparam_stator_res;
//__________________________________________________
	dmotor_param_rotor_res.text = "Rotor resistance";
	oval_dmparam_rotor_res.data_types = float_dt;
	oval_dmparam_rotor_res.mantissa_count = 3;
	oval_dmparam_rotor_res.real_count = 3;
	oval_dmparam_rotor_res.max_value = 999;
	oval_dmparam_rotor_res.min_value = 0.001;
	oval_dmparam_rotor_res.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_rotor_res.unit = "Ohm";
	//oval_dmparam_rotor_res.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_rotor_res.val = &menue_drive_data_setup.m_motor_rotor_resistance;
	dmotor_param_rotor_res.option_save_val = &oval_dmparam_rotor_res;
//__________________________________________________
	dmotor_param_leakage_induct.text = "Leakage induct";
	oval_dmparam_leakage_induct.data_types = float_dt;
	oval_dmparam_leakage_induct.mantissa_count = 2;
	oval_dmparam_leakage_induct.real_count = 3;
	oval_dmparam_leakage_induct.max_value = 999;
	oval_dmparam_leakage_induct.min_value = 0.01;
	oval_dmparam_leakage_induct.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_leakage_induct.unit = "mH";
	//oval_dmparam_leakage_induct.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_leakage_induct.val = &menue_drive_data_setup.m_motor_leakage_inductance;
	dmotor_param_leakage_induct.option_save_val = &oval_dmparam_leakage_induct;
//__________________________________________________
	dmotor_param_magnetizing_induct.text = "Magnetizing induct";
	oval_dmparam_magnetizing_induct.data_types = float_dt;
	oval_dmparam_magnetizing_induct.mantissa_count = 2;
	oval_dmparam_magnetizing_induct.real_count = 4;
	oval_dmparam_magnetizing_induct.max_value = 9999;
	oval_dmparam_magnetizing_induct.min_value = 0.01;
	oval_dmparam_magnetizing_induct.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_magnetizing_induct.unit = "mH";
	//oval_dmparam_magnetizing_induct.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_magnetizing_induct.val = &menue_drive_data_setup.m_motor_magnetizing_inductance;
	dmotor_param_magnetizing_induct.option_save_val = &oval_dmparam_magnetizing_induct;
//__________________________________________________	
	dmotor_param_no_load_current.text = "No load current";
	oval_dmparam_no_load_current.data_types = float_dt;
	oval_dmparam_no_load_current.mantissa_count = 2;
	oval_dmparam_no_load_current.real_count = 2;
	oval_dmparam_no_load_current.max_value = 99;
	oval_dmparam_no_load_current.min_value = 0;
	oval_dmparam_no_load_current.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_no_load_current.unit = "A";
	//oval_dmparam_no_load_current.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_no_load_current.val = &menue_drive_data_setup.m_motor_noload_current;
	dmotor_param_no_load_current.option_save_val = &oval_dmparam_no_load_current;
//__________________________________________________
	dmotor_param_LD.text = "LD";
	oval_dmparam_LD.data_types = float_dt;
	oval_dmparam_LD.mantissa_count = 2;
	oval_dmparam_LD.real_count = 3;
	oval_dmparam_LD.max_value = 999;
	oval_dmparam_LD.min_value = 0.01;
	oval_dmparam_LD.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_LD.unit = "mH";
	//oval_dmparam_LD.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_LD.val = &menue_drive_data_setup.m_motor_LD;
	dmotor_param_LD.option_save_val = &oval_dmparam_LD;
//__________________________________________________
	dmotor_param_LQ.text = "LQ";
	oval_dmparam_LQ.data_types = float_dt;
	oval_dmparam_LQ.mantissa_count = 2;
	oval_dmparam_LQ.real_count = 3;
	oval_dmparam_LQ.max_value = 999;
	oval_dmparam_LQ.min_value = 0.01;
	oval_dmparam_LQ.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_LQ.unit = "mH";
	//oval_dmparam_LQ.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_LQ.val = &menue_drive_data_setup.m_motor_LQ;
	dmotor_param_LQ.option_save_val = &oval_dmparam_LQ;
//__________________________________________________	
	dmotor_param_BEMF.text = "Back EMF";
	oval_dmparam_BEMF.data_types = float_dt;
	oval_dmparam_BEMF.mantissa_count = 3;
	oval_dmparam_BEMF.real_count = 2;
	oval_dmparam_BEMF.max_value = 99;
	oval_dmparam_BEMF.min_value = 0.01;
	oval_dmparam_BEMF.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmparam_BEMF.unit = "A";
	//oval_dmparam_BEMF.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_BEMF.val = &menue_drive_data_setup.m_motor_BEMF;
	dmotor_param_BEMF.option_save_val = &oval_dmparam_BEMF;
//__________________________________________________	
	dmotor_param_pole_pair.text = "Pole pair count";
	oval_dmparam_pole_pair.data_types = uint8_dt;
	oval_dmparam_pole_pair.mantissa_count = 0;
	oval_dmparam_pole_pair.real_count = 2;
	oval_dmparam_pole_pair.max_value = 40;
	oval_dmparam_pole_pair.min_value = 1;
	oval_dmparam_pole_pair.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmparam_pole_pair.unit = "";
	//oval_dmparam_pole_pair.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmparam_pole_pair.val = &menue_drive_data_setup.m_motor_pole_pair_count;
	dmotor_param_pole_pair.option_save_val = &oval_dmparam_pole_pair;
//___________________________________________________________
	fill_menue_list(&menue_driver_motor_param, opt_driver_motor_param, OPT_DRIVER_MOTOR_PARAM_MAX, 0, 0);
	return &menue_driver_motor_param;	
}