/*
 * menue_driver_encoder.c
 *
 * Created: 02/05/2017 05:48:51 م
 *  Author: ahmed
 */ 
#include <menue_driver_encoder.h>
#include <menue_data_manager.h>
#include <fixedptc.h>
#include <drive_interface_data.h>

//encoder list

menue_design_t menue_driver_enc;
 
#ifdef PMSM_MODEL
#define OPT_DRIVER_ENC_MAX 7
#else
#define OPT_DRIVER_ENC_MAX 4
#endif

option_t opt_driver_enc[OPT_DRIVER_ENC_MAX];

#define dmotor_enc_type								(opt_driver_enc[0])
#define dmotor_enc_pulse_count						(opt_driver_enc[1])
#define dmotor_enc_direction						(opt_driver_enc[2])
#define dmotor_enc_filter							(opt_driver_enc[3])
#define dmotor_enc_search_curr						(opt_driver_enc[4])
#define dmotor_enc_tune								(opt_driver_enc[5])
#define dmotor_enc_pole_angle						(opt_driver_enc[6])

option_val_t oval_deparam_ppr;
option_val_t oval_deparam_ppos;
option_val_t oval_deparam_scurrent;
option_val_t oval_deparam_filter;
//__________________________________________________
//encoder dir list
menue_design_t menue_eparam_enc_dir;
#define OPT_EPARAM_ENC_DIR_MAX 2
option_t opt_eparam_enc_dir[OPT_EPARAM_ENC_DIR_MAX];

#define enc_dir_alead				(opt_eparam_enc_dir[0])
#define enc_dir_blead				(opt_eparam_enc_dir[1])
//__________________________________________________
//encoder type list
menue_design_t menue_eparam_enc_type;

#ifdef PMSM_MODEL
#define OPT_EPARAM_ENC_TYPE_MAX 3
#else
#define OPT_EPARAM_ENC_TYPE_MAX 1
#endif

option_t opt_eparam_enc_type[OPT_EPARAM_ENC_TYPE_MAX];

#define enc_type_incr				(opt_eparam_enc_type[0])
#define enc_type_siccos				(opt_eparam_enc_type[1])
#define enc_type_endat				(opt_eparam_enc_type[2])
//__________________________________________________
//encoder tune list
menue_design_t menue_eparam_enc_tune;
#define OPT_EPARAM_ENC_TUNE_MAX 3
option_t opt_eparam_enc_tune[OPT_EPARAM_ENC_TUNE_MAX];

#define enc_tune_noAction					(opt_eparam_enc_tune[0])
#define enc_tune_rotational					(opt_eparam_enc_tune[1])
#define enc_tune_stand_still1				(opt_eparam_enc_tune[2])


menue_design_t *menue_driver_encoder_init(void)
{
	dmotor_enc_type.text = "Encoder type";
	enc_type_incr.text = "Incremental";
	#ifdef PMSM_MODEL
	enc_type_siccos.text = "SinCos";
	enc_type_endat.text = "Endat";
	#endif
	
	fill_menue_list(&menue_eparam_enc_type, &opt_eparam_enc_type, OPT_EPARAM_ENC_TYPE_MAX, 0, &menue_drive_data_setup.m_encoder_type);

	// set the 'option ladvanced_req_pins' child

	dmotor_enc_type.child = &menue_eparam_enc_type;
//___________________________________________________________
	dmotor_enc_pulse_count.text = "Encoder pulse/rev";
	oval_deparam_ppr.data_types = uint16_dt;
	oval_deparam_ppr.mantissa_count = 0;
	oval_deparam_ppr.real_count = 5;
	oval_deparam_ppr.max_value = 10000;
	oval_deparam_ppr.min_value = 100;
	oval_deparam_ppr.new_val_flag = &menue_drive_data_newdata_flag;
	oval_deparam_ppr.unit = "P/R";
	//oval_deparam_ppr.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_deparam_ppr.val = &menue_drive_data_setup.m_encoder_p_count;
	dmotor_enc_pulse_count.option_save_val = &oval_deparam_ppr;
//__________________________________________________
	dmotor_enc_direction.text = "Enc direction";
	enc_dir_alead.text = "Phase A lead";
	enc_dir_blead.text = "Phase B lead";
	
	fill_menue_list(&menue_eparam_enc_dir, &opt_eparam_enc_dir, OPT_EPARAM_ENC_DIR_MAX, 0, &menue_drive_data_setup.m_pg_direction);

	// set the 'option ladvanced_req_pins' child

	dmotor_enc_direction.child = &menue_eparam_enc_dir;
#ifdef PMSM_MODEL
//___________________________________________________________
	dmotor_enc_search_curr.text = "Angle Srch intensity";
	oval_deparam_scurrent.data_types = uint8_dt;
	oval_deparam_scurrent.mantissa_count = 0;
	oval_deparam_scurrent.real_count = 3;
	oval_deparam_scurrent.max_value = 250;
	oval_deparam_scurrent.min_value = 50;
	oval_deparam_scurrent.new_val_flag = &menue_drive_data_newdata_flag;
	oval_deparam_scurrent.unit = "%";
	//oval_deparam_scurrent.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_deparam_scurrent.val = &menue_drive_data_setup.m_encoder_search_current;
	dmotor_enc_search_curr.option_save_val = &oval_deparam_scurrent;
//__________________________________________________
	dmotor_enc_tune.text = "Enc Phase tune";
	enc_tune_noAction.text = "No action";
	enc_tune_rotational.text = "Rotational";
	enc_tune_rotational.option_characteristics = OPTION_HIDDEN;
	enc_tune_stand_still1.text = "stationary";
	
	fill_menue_list(&menue_eparam_enc_tune, &opt_eparam_enc_tune, OPT_EPARAM_ENC_TUNE_MAX, 0, &menue_drive_encoder_tune);

	// set the 'option ladvanced_req_pins' child

	dmotor_enc_tune.child = &menue_eparam_enc_tune;
//__________________________________________________
	dmotor_enc_filter.text = "speed filter";
	oval_deparam_filter.data_types = float_dt;
	oval_deparam_filter.mantissa_count = 2;
	oval_deparam_filter.real_count = 2;
	oval_deparam_filter.max_value = 99;
	oval_deparam_filter.min_value = 0;
	oval_deparam_filter.new_val_flag = &menue_drive_data_newdata_flag;
	oval_deparam_filter.unit = "ms";
	//oval_deparam_filter.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_deparam_filter.val = &menue_drive_data_setup.m_enc_filter_low;
	dmotor_enc_filter.option_save_val = &oval_deparam_filter;
//__________________________________________________
// 	dmotor_vector_enc_high.text = "H speed filter";
// 	oval_dmvector_hspeed_filter.data_types = uint8_dt;
// 	oval_dmvector_hspeed_filter.mantissa_count = 0;
// 	oval_dmvector_hspeed_filter.real_count = 2;
// 	oval_dmvector_hspeed_filter.max_value = 99;
// 	oval_dmvector_hspeed_filter.min_value = 0;
// 	oval_dmvector_hspeed_filter.new_val_flag = &menue_drive_data_newdata_flag;
// 	oval_dmvector_hspeed_filter.unit = "ms";
// 	//oval_dmvector_hspeed_filter.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
// 	oval_dmvector_hspeed_filter.val = &menue_drive_data_setup.m_enc_filter_high;
// 	dmotor_vector_enc_high.option_save_val = &oval_dmvector_hspeed_filter;
//___________________________________________________________
	dmotor_enc_pole_angle.text = "Pole position offset";
	oval_deparam_ppos.data_types = float_dt;
	oval_deparam_ppos.mantissa_count = 2;
	oval_deparam_ppos.real_count = 3;
	oval_deparam_ppos.max_value = 360;
	oval_deparam_ppos.min_value = -360;
	oval_deparam_ppos.new_val_flag = &menue_drive_data_newdata_flag;
	oval_deparam_ppos.unit = "Deg";
	//oval_deparam_ppos.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_deparam_ppos.val = &menue_drive_data_setup.m_pole_pos_offset;
	dmotor_enc_pole_angle.option_save_val = &oval_deparam_ppos;
#endif
//__________________________________________________
	fill_menue_list(&menue_driver_enc, opt_driver_enc, OPT_DRIVER_ENC_MAX, 0, 0);
	return &menue_driver_enc;	
}