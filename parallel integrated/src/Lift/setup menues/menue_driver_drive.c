/*
 * menue_driver_drive.c
 *
 * Created: 02/05/2017 04:11:04 م
 *  Author: ahmed
 */ 

#include <menue_advanced.h>
#include <menue_data_manager.h>

//drive list

menue_design_t menue_driver_drive;
#define OPT_DRIVER_DRIVE_MAX 4
option_t opt_driver_drive[OPT_DRIVER_DRIVE_MAX];

#define ddrive_max_freq					(opt_driver_drive[0])
#define ddrive_carier_freq				(opt_driver_drive[1])
#define ddrive_control_mode				(opt_driver_drive[2])
#define ddrive_inp_ph_fail				(opt_driver_drive[3])

option_val_t oval_ddrive_max_freq;
option_val_t oval_ddrive_carier_freq;

//__________________________________________________
//control mode list
menue_design_t menue_driver_drive_control_mode;
#define OPT_DRIVER_DRIVE_CONTROL_MODE_MAX 3
option_t opt_driver_drive_control_mode[OPT_DRIVER_DRIVE_CONTROL_MODE_MAX];

#define control_mode_vf						(opt_driver_drive_control_mode[0])
#define control_mode_cl_vector				(opt_driver_drive_control_mode[1])
#define control_mode_ol_vector				(opt_driver_drive_control_mode[2])

//__________________________________________________
//phase fail list
menue_design_t menue_driver_drive_phase_fail;
#define OPT_DRIVER_DRIVE_PHASE_FAIL_MAX 2
option_t opt_driver_drive_phase_fail[OPT_DRIVER_DRIVE_PHASE_FAIL_MAX];

#define phase_fail_on						(opt_driver_drive_phase_fail[0])
#define phase_fail_off						(opt_driver_drive_phase_fail[1])

menue_design_t *menue_driver_drive_init(void)
{
	ddrive_max_freq.text = "Maximum Frequency";
	oval_ddrive_max_freq.data_types = fixedpt_dt;
	oval_ddrive_max_freq.mantissa_count = 2;
	oval_ddrive_max_freq.real_count = 3;
	oval_ddrive_max_freq.max_value = 100;
	oval_ddrive_max_freq.min_value = 1;
	oval_ddrive_max_freq.new_val_flag = &menue_drive_data_newdata_flag;
	oval_ddrive_max_freq.unit = "Hz";
	//oval_ddrive_max_freq.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_ddrive_max_freq.val = &menue_drive_data_setup.m_max_freq;
	ddrive_max_freq.option_save_val = &oval_ddrive_max_freq;
//__________________________________________________

	ddrive_carier_freq.text = "Carrier Frequency";
	oval_ddrive_carier_freq.data_types = uint8_dt;
	oval_ddrive_carier_freq.mantissa_count = 0;
	oval_ddrive_carier_freq.real_count = 2;
	oval_ddrive_carier_freq.max_value = 8;
	oval_ddrive_carier_freq.min_value = 2;
	oval_ddrive_carier_freq.new_val_flag = &menue_drive_data_newdata_flag;
	oval_ddrive_carier_freq.unit = "KHz";
	//oval_ddrive_max_freq.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_ddrive_carier_freq.val = &menue_drive_data_setup.m_carier_freq;
	ddrive_carier_freq.option_save_val = &oval_ddrive_carier_freq;
//__________________________________________________
	
	ddrive_control_mode.text = "Control mode";
	control_mode_vf.text = "v/f";
	control_mode_cl_vector.text = "Closed loop vector";
	control_mode_ol_vector.text = "Open loop vector";


	fill_menue_list(&menue_driver_drive_control_mode, &opt_driver_drive_control_mode, OPT_DRIVER_DRIVE_CONTROL_MODE_MAX, 0, &menue_drive_data_setup.m_control_mode);

	// set the 'option ladvanced_req_pins' child

	ddrive_control_mode.child = &menue_driver_drive_control_mode;
//__________________________________________________
	
	ddrive_inp_ph_fail.text = "Input phase fail";
	phase_fail_on.text = "On";
	phase_fail_off.text = "off";

	fill_menue_list(&menue_driver_drive_phase_fail, &opt_driver_drive_phase_fail, OPT_DRIVER_DRIVE_PHASE_FAIL_MAX, 0, &menue_drive_data_setup.m_input_phase_fail);

	// set the 'option ladvanced_req_pins' child

	ddrive_inp_ph_fail.child = &menue_driver_drive_phase_fail;
//___________________________________________________________
	fill_menue_list(&menue_driver_drive, opt_driver_drive, OPT_DRIVER_DRIVE_MAX, 0, 0);
	return &menue_driver_drive;
}
