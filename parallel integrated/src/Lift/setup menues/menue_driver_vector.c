/*
 * menue_driver_vector.c
 *
 * Created: 02/05/2017 05:51:01 م
 *  Author: ahmed
 */ 

#include <menue_driver_vector.h>
#include <menue_data_manager.h>
#include <fixedptc.h>

//vector control list

menue_design_t menue_driver_vector;
#define OPT_DRIVER_VECTOR_MAX 18
option_t opt_driver_vector[OPT_DRIVER_VECTOR_MAX];

#define dmotor_vector_ASR_ls_p						(opt_driver_vector[0])
#define dmotor_vector_ASR_ls_i						(opt_driver_vector[1])
//#define dmotor_vector_enc_low						(opt_driver_vector[2])
#define dmotor_vector_sw_lw_point					(opt_driver_vector[2])

#define dmotor_vector_ASR_hs_p						(opt_driver_vector[3])
#define dmotor_vector_ASR_hs_i						(opt_driver_vector[4])
//#define dmotor_vector_enc_high						(opt_driver_vector[5])
#define dmotor_vector_sw_hi_point					(opt_driver_vector[5])
//#define dmotor_vector_speed_band					(opt_driver_vector[8])
#define dmotor_vector_APR_LEVEL_P					(opt_driver_vector[6])

#define dmotor_vector_ASR_ZS_P						(opt_driver_vector[7])
#define dmotor_vector_ASR_ZS_ESO					(opt_driver_vector[8])
#define dmotor_vector_APR_ZS_P						(opt_driver_vector[9])

#define dmotor_vector_ACRD_p						(opt_driver_vector[10])
#define dmotor_vector_ACRD_i						(opt_driver_vector[11])
#define dmotor_vector_ACRQ_p						(opt_driver_vector[12])
#define dmotor_vector_ACRQ_i						(opt_driver_vector[13])
#define dmotor_vector_ACR_bandwidth					(opt_driver_vector[14])
#define dmotor_vector_ACR_bandwidth_zs				(opt_driver_vector[15])
#define dmotor_vector_t_decay						(opt_driver_vector[16])
#define dmotor_vector_speed_dev						(opt_driver_vector[17])



option_val_t oval_dmvector_ASR_ls_p;
option_val_t oval_dmvector_ASR_ls_i;
option_val_t oval_dmvector_ASR_hs_p;
option_val_t oval_dmvector_ASR_hs_i;
option_val_t oval_dmvector_ASR_ZS_P;
option_val_t oval_dmvector_ASR_ZS_I;
option_val_t oval_dmvector_ASR_ZS_ESO;
option_val_t oval_dmvector_sw_lw_point;
option_val_t oval_dmvector_sw_hi_point;
option_val_t oval_dmvector_speed_band;
option_val_t oval_dmvector_ACRD_p;
option_val_t oval_dmvector_ACRD_i;
option_val_t oval_dmvector_ACRQ_p;
option_val_t oval_dmvector_ACRQ_i;
option_val_t oval_dmvector_ACR_bandwidth;
option_val_t oval_dmvector_ACR_bandwidth_zs;
option_val_t oval_dmvector_t_decay;
option_val_t oval_dmvector_APR_ZS_P;
option_val_t oval_dmvector_speed_dev;
option_val_t oval_dmvector_lspeed_filter;
option_val_t oval_dmvector_hspeed_filter;
option_val_t oval_dmvector_apr_level_p;

menue_design_t *menue_driver_vector_init(void)
{
	dmotor_vector_ASR_ls_p.text = "ASR low P";
	oval_dmvector_ASR_ls_p.data_types = float_dt;
	oval_dmvector_ASR_ls_p.mantissa_count = 1;
	oval_dmvector_ASR_ls_p.real_count = 3;
	oval_dmvector_ASR_ls_p.max_value = 500;
	oval_dmvector_ASR_ls_p.min_value = 0.01;
	oval_dmvector_ASR_ls_p.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ASR_ls_p.unit = "KW";
	oval_dmvector_ASR_ls_p.text = "Low speed P gain";
	oval_dmvector_ASR_ls_p.val = &menue_drive_data_setup.m_ASR_lows_p;
	dmotor_vector_ASR_ls_p.option_save_val = &oval_dmvector_ASR_ls_p;
//__________________________________________________
	dmotor_vector_ASR_ls_i.text = "ASR low I";
	oval_dmvector_ASR_ls_i.data_types = float_dt;
	oval_dmvector_ASR_ls_i.mantissa_count = 3;
	oval_dmvector_ASR_ls_i.real_count = 3;
	oval_dmvector_ASR_ls_i.max_value = 50;
	oval_dmvector_ASR_ls_i.min_value = 0.01;
	oval_dmvector_ASR_ls_i.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvector_ASR_ls_i.unit = "Sec";
	oval_dmvector_ASR_ls_i.text = "Low speed integ time";
	oval_dmvector_ASR_ls_i.val = &menue_drive_data_setup.m_ASR_lows_i;
	dmotor_vector_ASR_ls_i.option_save_val = &oval_dmvector_ASR_ls_i;
//__________________________________________________
	dmotor_vector_ASR_hs_p.text = "ASR high P";
	oval_dmvector_ASR_hs_p.data_types = float_dt;
	oval_dmvector_ASR_hs_p.mantissa_count = 3;
	oval_dmvector_ASR_hs_p.real_count = 3;
	oval_dmvector_ASR_hs_p.max_value = 500;
	oval_dmvector_ASR_hs_p.min_value = 0.01;
	oval_dmvector_ASR_hs_p.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ASR_hs_p.unit = "KW";
	oval_dmvector_ASR_hs_p.text = "High speed P gain";
	oval_dmvector_ASR_hs_p.val = &menue_drive_data_setup.m_ASR_highs_p;
	dmotor_vector_ASR_hs_p.option_save_val = &oval_dmvector_ASR_hs_p;
//__________________________________________________
	dmotor_vector_ASR_hs_i.text = "ASR high I";
	oval_dmvector_ASR_hs_i.data_types = float_dt;
	oval_dmvector_ASR_hs_i.mantissa_count = 3;
	oval_dmvector_ASR_hs_i.real_count = 3;
	oval_dmvector_ASR_hs_i.max_value = 50;
	oval_dmvector_ASR_hs_i.min_value = 0.01;
	oval_dmvector_ASR_hs_i.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvector_ASR_hs_i.unit = "Sec";
	oval_dmvector_ASR_hs_i.text = "High speed integ time";
	oval_dmvector_ASR_hs_i.val = &menue_drive_data_setup.m_ASR_highs_i;
	dmotor_vector_ASR_hs_i.option_save_val = &oval_dmvector_ASR_hs_i;
//__________________________________________________
	dmotor_vector_APR_LEVEL_P.text = "APR Level P";
	oval_dmvector_apr_level_p.data_types = float_dt;
	oval_dmvector_apr_level_p.mantissa_count = 2;
	oval_dmvector_apr_level_p.real_count = 2;
	oval_dmvector_apr_level_p.max_value = 50;
	oval_dmvector_apr_level_p.min_value = 0.0;
	oval_dmvector_apr_level_p.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_apr_level_p.unit = "Sec";
	oval_dmvector_apr_level_p.text = "positioning gain";
	oval_dmvector_apr_level_p.val = &menue_drive_data_setup.m_APR_level_p;
	dmotor_vector_APR_LEVEL_P.option_save_val = &oval_dmvector_apr_level_p;
//__________________________________________________
// 	dmotor_vector_speed_band.text = "Speed bandwidth";
// 	oval_dmvector_speed_band.data_types = float_dt;
// 	oval_dmvector_speed_band.mantissa_count = 2;
// 	oval_dmvector_speed_band.real_count = 2;
// 	oval_dmvector_speed_band.max_value = 99;
// 	oval_dmvector_speed_band.min_value = 0.01;
// 	oval_dmvector_speed_band.new_val_flag = &menue_drive_data_newdata_flag;
// 	oval_dmvector_speed_band.unit = "rad/sec";
// 	//oval_dmvector_speed_band.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
// 	oval_dmvector_speed_band.val = &menue_drive_data_setup.m_ASR_speed_band;
// 	dmotor_vector_speed_band.option_save_val = &oval_dmvector_speed_band;
//__________________________________________________
	dmotor_vector_ASR_ZS_P.text = "ASR zero speed P";
	oval_dmvector_ASR_ZS_P.data_types = float_dt;
	oval_dmvector_ASR_ZS_P.mantissa_count = 3;
	oval_dmvector_ASR_ZS_P.real_count = 3;
	oval_dmvector_ASR_ZS_P.max_value = 999;
	oval_dmvector_ASR_ZS_P.min_value = 0.1;
	oval_dmvector_ASR_ZS_P.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ASR_ZS_P.unit = "KW";
	//oval_dmvector_ASR_ZS_P.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_ASR_ZS_P.val = &menue_drive_data_setup.m_ASR_zs_p;
	dmotor_vector_ASR_ZS_P.option_save_val = &oval_dmvector_ASR_ZS_P;
//__________________________________________________
	dmotor_vector_ASR_ZS_ESO.text = "ASR zero speed ESO";
	oval_dmvector_ASR_ZS_ESO.data_types = float_dt;
	oval_dmvector_ASR_ZS_ESO.mantissa_count = 3;
	oval_dmvector_ASR_ZS_ESO.real_count = 3;
	oval_dmvector_ASR_ZS_ESO.max_value = 999;
	oval_dmvector_ASR_ZS_ESO.min_value = 0.0;
	oval_dmvector_ASR_ZS_ESO.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ASR_ZS_ESO.unit = "KW";
	//oval_dmvector_ASR_ZS_ESO.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_ASR_ZS_ESO.val = &menue_drive_data_setup.m_APR_zs_eso;
	dmotor_vector_ASR_ZS_ESO.option_save_val = &oval_dmvector_ASR_ZS_ESO;
//__________________________________________________
// 	dmotor_vector_ASR_ZS_I.text = "ASR zero speed I";
// 	oval_dmvector_ASR_ZS_I.data_types = float_dt;
// 	oval_dmvector_ASR_ZS_I.mantissa_count = 3;
// 	oval_dmvector_ASR_ZS_I.real_count = 3;
// 	oval_dmvector_ASR_ZS_I.max_value = 999;
// 	oval_dmvector_ASR_ZS_I.min_value = 0.1;
// 	oval_dmvector_ASR_ZS_I.new_val_flag = &menue_drive_data_newdata_flag;
// 	//oval_dmvector_ASR_ZS_I.unit = "KW";
// 	//oval_dmvector_ASR_ZS_I.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
// 	oval_dmvector_ASR_ZS_I.val = &menue_drive_data_setup.m_ASR_zs_i;
// 	dmotor_vector_ASR_ZS_I.option_save_val = &oval_dmvector_ASR_ZS_I;
//__________________________________________________
	dmotor_vector_APR_ZS_P.text = "APR zero speed P";
	oval_dmvector_APR_ZS_P.data_types = float_dt;
	oval_dmvector_APR_ZS_P.mantissa_count = 3;
	oval_dmvector_APR_ZS_P.real_count = 3;
	oval_dmvector_APR_ZS_P.max_value = 999;
	oval_dmvector_APR_ZS_P.min_value = 0.0;
	oval_dmvector_APR_ZS_P.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvector_APR_ZS_P.val = &menue_drive_data_setup.m_APR_zs_p;
	dmotor_vector_APR_ZS_P.option_save_val = &oval_dmvector_APR_ZS_P;
//__________________________________________________
	dmotor_vector_sw_lw_point.text = "Switch LS point";
	oval_dmvector_sw_lw_point.data_types = float_dt;
	oval_dmvector_sw_lw_point.mantissa_count = 3;
	oval_dmvector_sw_lw_point.real_count = 3;
	oval_dmvector_sw_lw_point.max_value = 100;
	oval_dmvector_sw_lw_point.min_value = 0.05;
	oval_dmvector_sw_lw_point.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvector_sw_lw_point.unit = "Hz";
	//oval_dmvector_sw_lw_point.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_sw_lw_point.val = &menue_drive_data_setup.m_ASR_sw_lw_point;
	dmotor_vector_sw_lw_point.option_save_val = &oval_dmvector_sw_lw_point;
//__________________________________________________
	dmotor_vector_sw_hi_point.text = "Switch HS point";
	oval_dmvector_sw_hi_point.data_types = float_dt;
	oval_dmvector_sw_hi_point.mantissa_count = 3;
	oval_dmvector_sw_hi_point.real_count = 3;
	oval_dmvector_sw_hi_point.max_value = 100;
	oval_dmvector_sw_hi_point.min_value = 0.05;
	oval_dmvector_sw_hi_point.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvector_sw_hi_point.unit = "Hz";
	//oval_dmvector_sw_hi_point.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_sw_hi_point.val = &menue_drive_data_setup.m_ASR_sw_hi_point;
	dmotor_vector_sw_hi_point.option_save_val = &oval_dmvector_sw_hi_point;
//__________________________________________________
	dmotor_vector_ACRD_p.text = "ACR-D P";
	oval_dmvector_ACRD_p.data_types = float_dt;
	oval_dmvector_ACRD_p.mantissa_count = 3;
	oval_dmvector_ACRD_p.real_count = 5;
	oval_dmvector_ACRD_p.max_value = 99999;
	oval_dmvector_ACRD_p.min_value = 0.01;
	oval_dmvector_ACRD_p.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ACRD_p.unit = "KW";
	//oval_dmvector_ACRD_p.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_ACRD_p.val = &menue_drive_data_setup.m_ACRD_p;
	dmotor_vector_ACRD_p.option_save_val = &oval_dmvector_ACRD_p;
//__________________________________________________
	dmotor_vector_ACRD_i.text = "ACR-D I";
	oval_dmvector_ACRD_i.data_types = float_dt;
	oval_dmvector_ACRD_i.mantissa_count = 3;
	oval_dmvector_ACRD_i.real_count = 5;
	oval_dmvector_ACRD_i.max_value = 99999;
	oval_dmvector_ACRD_i.min_value = 0.01;
	oval_dmvector_ACRD_i.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ACRD_i.unit = "KW";
	//oval_dmvector_ACRD_i.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_ACRD_i.val = &menue_drive_data_setup.m_ACRD_i;
	dmotor_vector_ACRD_i.option_save_val = &oval_dmvector_ACRD_i;
	
//__________________________________________________
	dmotor_vector_ACRQ_p.text = "ACR-Q P";
	oval_dmvector_ACRQ_p.data_types = float_dt;
	oval_dmvector_ACRQ_p.mantissa_count = 3;
	oval_dmvector_ACRQ_p.real_count = 5;
	oval_dmvector_ACRQ_p.max_value = 99999;
	oval_dmvector_ACRQ_p.min_value = 0.01;
	oval_dmvector_ACRQ_p.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ACRQ_p.unit = "KW";
	//oval_dmvector_ACRQ_p.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_ACRQ_p.val = &menue_drive_data_setup.m_ACRQ_p;
	dmotor_vector_ACRQ_p.option_save_val = &oval_dmvector_ACRQ_p;
//__________________________________________________
	dmotor_vector_ACRQ_i.text = "ACR-Q I";
	oval_dmvector_ACRQ_i.data_types = float_dt;
	oval_dmvector_ACRQ_i.mantissa_count = 3;
	oval_dmvector_ACRQ_i.real_count = 5;
	oval_dmvector_ACRQ_i.max_value = 99999;
	oval_dmvector_ACRQ_i.min_value = 0.01;
	oval_dmvector_ACRQ_i.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ACRQ_i.unit = "KW";
	//oval_dmvector_ACRQ_i.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_ACRQ_i.val = &menue_drive_data_setup.m_ACRQ_i;
	dmotor_vector_ACRQ_i.option_save_val = &oval_dmvector_ACRQ_i;
//__________________________________________________
	dmotor_vector_ACR_bandwidth.text = "ACR bandwidth";
	oval_dmvector_ACR_bandwidth.data_types = float_dt;
	oval_dmvector_ACR_bandwidth.mantissa_count = 2;
	oval_dmvector_ACR_bandwidth.real_count = 3;
	oval_dmvector_ACR_bandwidth.max_value = 999.99;
	oval_dmvector_ACR_bandwidth.min_value = 0.01;
	oval_dmvector_ACR_bandwidth.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ACR_bandwidth.unit = "KW";
	//oval_dmvector_ACR_bandwidth.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_ACR_bandwidth.val = &menue_drive_data_setup.m_ACR_bandwidth;
	dmotor_vector_ACR_bandwidth.option_save_val = &oval_dmvector_ACR_bandwidth;
//__________________________________________________
	dmotor_vector_ACR_bandwidth_zs.text = "ACR bandwidth ZS";
	oval_dmvector_ACR_bandwidth_zs.data_types = float_dt;
	oval_dmvector_ACR_bandwidth_zs.mantissa_count = 2;
	oval_dmvector_ACR_bandwidth_zs.real_count = 3;
	oval_dmvector_ACR_bandwidth_zs.max_value = 999.99;
	oval_dmvector_ACR_bandwidth_zs.min_value = 0.01;
	oval_dmvector_ACR_bandwidth_zs.new_val_flag = &menue_drive_data_newdata_flag;
	//oval_dmvector_ACR_bandwidth_zs.unit = "KW";
	//oval_dmvector_ACR_bandwidth_zs.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_ACR_bandwidth_zs.val = &menue_drive_data_setup.m_ACR_bandwidth_zs;
	dmotor_vector_ACR_bandwidth_zs.option_save_val = &oval_dmvector_ACR_bandwidth_zs;
//__________________________________________________
	dmotor_vector_t_decay.text = "Tourque decay";
	oval_dmvector_t_decay.data_types = uint16_dt;
	oval_dmvector_t_decay.mantissa_count = 0;
	oval_dmvector_t_decay.real_count = 4;
	oval_dmvector_t_decay.max_value = 3000;
	oval_dmvector_t_decay.min_value = 10;
	oval_dmvector_t_decay.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvector_t_decay.unit = "ms";
	//oval_dmvector_t_decay.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_t_decay.val = &menue_drive_data_setup.m_tourque_decay;
	dmotor_vector_t_decay.option_save_val = &oval_dmvector_t_decay;
//__________________________________________________
	dmotor_vector_speed_dev.text = "Allowed speed dev";
	oval_dmvector_speed_dev.data_types = uint16_dt;
	oval_dmvector_speed_dev.mantissa_count = 0;
	oval_dmvector_speed_dev.real_count = 3;
	oval_dmvector_speed_dev.max_value = 100;
	oval_dmvector_speed_dev.min_value = 0;
	oval_dmvector_speed_dev.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dmvector_speed_dev.unit = "%";
	//oval_dmvector_speed_dev.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dmvector_speed_dev.val = &menue_drive_data_setup.m_speed_dev;
	dmotor_vector_speed_dev.option_save_val = &oval_dmvector_speed_dev;
//__________________________________________________
	
	fill_menue_list(&menue_driver_vector, opt_driver_vector, OPT_DRIVER_VECTOR_MAX, 0, 0);
	return &menue_driver_vector;	
}