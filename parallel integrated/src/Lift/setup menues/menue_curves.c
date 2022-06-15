/*
 * menue_curves.c
 *
 * Created: 30/05/2017 03:56:15 ص
 *  Author: ahmed
 */ 
#include <menue_curves.h>
#include <menue_data_manager.h>


//timers list
menue_design_t menue_curves;
#define OPT_CURVES_MAX 19
option_t opt_curves[OPT_CURVES_MAX];

#define lcurves_high_speed				(opt_curves[0])
#define lcurves_medium_speed			(opt_curves[1])
#define lcurves_creep_speed				(opt_curves[2])
#define lcurves_relevel_speed			(opt_curves[3])
#define lcurves_ins_speed				(opt_curves[4])
#define lcurves_acc						(opt_curves[5])
#define lcurves_dec						(opt_curves[6])
#define lcurves_ins_acc					(opt_curves[7])
#define lcurves_ins_dec					(opt_curves[8])
#define lcurves_ins_corner				(opt_curves[9])
#define lcurves_corner1					(opt_curves[10])
#define lcurves_corner2					(opt_curves[11])
#define lcurves_corner3					(opt_curves[12])
#define lcurves_prespeed				(opt_curves[13])
#define lcurves_prespd_time				(opt_curves[14])
#define lcurves_ins_stopping_mode       (opt_curves[15])
#define lcurves_ARDspeed				(opt_curves[16])
#define lcurves_ARDAccDec				(opt_curves[17])
#define lcurves_ARDjerk				    (opt_curves[18])
//__________________________________________________
//lift inspection stopping types list
menue_design_t menue_curves_ins_stopping;
#define OPT_CURVES_INS_STOPPING_MAX 2
option_t opt_curves_ins_stopping[OPT_CURVES_INS_STOPPING_MAX];

#define ins_stopping_abrupt			(opt_curves_ins_stopping[0])
#define ins_stopping_decelerate		(opt_curves_ins_stopping[1])

//__________________________________________________
option_val_t oval_high_speed;
option_val_t oval_medium_speed;
option_val_t oval_creep_speed;
option_val_t oval_relevel_speed;
option_val_t oval_ins_speed;
option_val_t oval_acc;
option_val_t oval_dec;
option_val_t oval_ins_acc;
option_val_t oval_ins_dec;
option_val_t oval_ins_corner;
option_val_t oval_corner1;
option_val_t oval_corner2;
option_val_t oval_corner3;
option_val_t oval_pre_spd;
option_val_t oval_pre_spd_time;
option_val_t oval_ARDspeed;
option_val_t oval_ARDaccdec;
option_val_t oval_ARDjerk;

menue_design_t *menue_curves_init(void)
{
	//fill menue lift timers
	
	lcurves_high_speed.text = "High speed";
	oval_high_speed.data_types = fixedpt_dt;
	oval_high_speed.mantissa_count = 1;
	oval_high_speed.real_count = 3;
	oval_high_speed.max_value = 100.0;
	oval_high_speed.min_value = 1;
	oval_high_speed.new_val_flag = &menue_lift_data_newdata_flag;
	oval_high_speed.unit = "%";
	//oval_high_speed.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_high_speed.val = &menue_lift_data_setup.m_high_speed;
	lcurves_high_speed.option_save_val = &oval_high_speed;
	//__________________________________________________

	lcurves_medium_speed.text = "Medium speed";
	oval_medium_speed.data_types = fixedpt_dt;
	oval_medium_speed.mantissa_count = 1;
	oval_medium_speed.real_count = 3;
	oval_medium_speed.max_value = 100;
	oval_medium_speed.min_value = 1;
	oval_medium_speed.new_val_flag = &menue_lift_data_newdata_flag;
	oval_medium_speed.unit = "%";
	//oval_medium_speed.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_medium_speed.val = &menue_lift_data_setup.m_medium_speed;
	lcurves_medium_speed.option_save_val = &oval_medium_speed;
	//__________________________________________________
	lcurves_creep_speed.text = "Creep speed";
	oval_creep_speed.data_types = fixedpt_dt;
	oval_creep_speed.mantissa_count = 1;
	oval_creep_speed.real_count = 3;
	oval_creep_speed.max_value = 100;
	oval_creep_speed.min_value = 0;
	oval_creep_speed.new_val_flag = &menue_lift_data_newdata_flag;
	oval_creep_speed.unit = "%";
	//oval_creep_speed.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_creep_speed.val = &menue_lift_data_setup.m_creep_speed;
	lcurves_creep_speed.option_save_val = &oval_creep_speed;
	//__________________________________________________
	lcurves_relevel_speed.text = "Relevel speed";
	oval_relevel_speed.data_types = fixedpt_dt;
	oval_relevel_speed.mantissa_count = 1;
	oval_relevel_speed.real_count = 3;
	oval_relevel_speed.max_value = 100;
	oval_relevel_speed.min_value = 0;
	oval_relevel_speed.new_val_flag = &menue_lift_data_newdata_flag;
	oval_relevel_speed.unit = "%";
	//oval_relevel_speed.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_relevel_speed.val = &menue_lift_data_setup.m_relevel_speed;
	lcurves_relevel_speed.option_save_val = &oval_relevel_speed;
	//__________________________________________________
	lcurves_ins_speed.text = "Ins speed";
	oval_ins_speed.data_types = fixedpt_dt;
	oval_ins_speed.mantissa_count = 1;
	oval_ins_speed.real_count = 3;
	oval_ins_speed.max_value = 100;
	oval_ins_speed.min_value = 0;
	oval_ins_speed.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ins_speed.unit = "%";
	//oval_ins_speed.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ins_speed.val = &menue_lift_data_setup.m_ins_speed;
	lcurves_ins_speed.option_save_val = &oval_ins_speed;
	//__________________________________________________
	
	lcurves_acc.text = "Acceleration";
	oval_acc.data_types = fixedpt_dt;
	oval_acc.mantissa_count = 2;
	oval_acc.real_count = 2;
	oval_acc.max_value = 20.99;
	oval_acc.min_value = 0.1;
	oval_acc.new_val_flag = &menue_lift_data_newdata_flag;
	oval_acc.unit = "Sec";
	//oval_acc.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_acc.val = &menue_lift_data_setup.m_acc;
	lcurves_acc.option_save_val = &oval_acc;
	
	//__________________________________________________

	lcurves_dec.text = "Deceleration";
	oval_dec.data_types = fixedpt_dt;
	oval_dec.mantissa_count = 2;
	oval_dec.real_count = 2;
	oval_dec.max_value = 20.99;
	oval_dec.min_value = 0.1;
	oval_dec.new_val_flag = &menue_lift_data_newdata_flag;
	oval_dec.unit = "Sec";
	//oval_dec.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_dec.val = &menue_lift_data_setup.m_dec;
	lcurves_dec.option_save_val = &oval_dec;
	//__________________________________________________

	lcurves_ins_acc.text = "Ins Aceleration";
	oval_ins_acc.data_types = fixedpt_dt;
	oval_ins_acc.mantissa_count = 2;
	oval_ins_acc.real_count = 2;
	oval_ins_acc.max_value = 20.0;
	oval_ins_acc.min_value = 0.1;
	oval_ins_acc.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ins_acc.unit = "Sec";
	//oval_ins_acc.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ins_acc.val = &menue_lift_data_setup.m_ins_acc;
	lcurves_ins_acc.option_save_val = &oval_ins_acc;
	//__________________________________________________

	lcurves_ins_dec.text = "Ins Deceleration";
	oval_ins_dec.data_types = fixedpt_dt;
	oval_ins_dec.mantissa_count = 2;
	oval_ins_dec.real_count = 2;
	oval_ins_dec.max_value = 20.0;
	oval_ins_dec.min_value = 0.1;
	oval_ins_dec.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ins_dec.unit = "Sec";
	//oval_ins_dec.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ins_dec.val = &menue_lift_data_setup.m_ins_dec;
	lcurves_ins_dec.option_save_val = &oval_ins_dec;	
	//__________________________________________________
	lcurves_ins_corner.text = "Ins corner";
	oval_ins_corner.data_types = fixedpt_dt;
	oval_ins_corner.mantissa_count = 2;
	oval_ins_corner.real_count = 2;
	oval_ins_corner.max_value = 99.99;
	oval_ins_corner.min_value = 1;
	oval_ins_corner.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ins_corner.unit = "%";
	//oval_ins_corner.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ins_corner.val = &menue_lift_data_setup.m_ins_corner;
	lcurves_ins_corner.option_save_val = &oval_ins_corner;
	//__________________________________________________
	
	lcurves_corner1.text = "Corner 1";
	oval_corner1.data_types = fixedpt_dt;
	oval_corner1.mantissa_count = 2;
	oval_corner1.real_count = 2;
	oval_corner1.max_value = 99.99;
	oval_corner1.min_value = 1;
	oval_corner1.new_val_flag = &menue_lift_data_newdata_flag;
	oval_corner1.unit = "%";
	//oval_corner1.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_corner1.val = &menue_lift_data_setup.m_corner1;
	lcurves_corner1.option_save_val = &oval_corner1;
	//__________________________________________________

	lcurves_corner2.text = "Corner 2";
	oval_corner2.data_types = fixedpt_dt;
	oval_corner2.mantissa_count = 2;
	oval_corner2.real_count = 2;
	oval_corner2.max_value = 99.99;
	oval_corner2.min_value = 1;
	oval_corner2.new_val_flag = &menue_lift_data_newdata_flag;
	oval_corner2.unit = "%";
	//oval_corner2.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_corner2.val = &menue_lift_data_setup.m_corner2;
	lcurves_corner2.option_save_val = &oval_corner2;
	//__________________________________________________
	
	lcurves_corner3.text = "Corner 3";
	oval_corner3.data_types = fixedpt_dt;
	oval_corner3.mantissa_count = 2;
	oval_corner3.real_count = 2;
	oval_corner3.max_value = 99.99;
	oval_corner3.min_value = 1;
	oval_corner3.new_val_flag = &menue_lift_data_newdata_flag;
	oval_corner3.unit = "%";
	//oval_corner3.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_corner3.val = &menue_lift_data_setup.m_corner3;
	lcurves_corner3.option_save_val = &oval_corner3;
	//__________________________________________________


	lcurves_ins_stopping_mode.text = "Ins stop mode";
	ins_stopping_abrupt.text = "Abrupt";
	ins_stopping_decelerate.text = "Decelerate";

	fill_menue_list(&menue_curves_ins_stopping, opt_curves_ins_stopping, OPT_CURVES_INS_STOPPING_MAX, 0, &menue_lift_data_setup.m_ins_stopping_mode);

	//Set the child to enter
	lcurves_ins_stopping_mode.child = &menue_curves_ins_stopping;
	//__________________________________________________
	
	lcurves_ARDspeed.text = "ARD speed";
	oval_ARDspeed.data_types = fixedpt_dt;
	oval_ARDspeed.mantissa_count = 2;
	oval_ARDspeed.real_count = 2;
	oval_ARDspeed.max_value = 99.99;
	oval_ARDspeed.min_value = 0.5;
	oval_ARDspeed.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ARDspeed.unit = "Hz";
	//oval_corner1.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ARDspeed.val = &menue_lift_data_setup.m_ARDspeed;
	lcurves_ARDspeed.option_save_val = &oval_ARDspeed;
	//__________________________________________________

	lcurves_ARDAccDec.text = "ARD Acc/Dec";
	oval_ARDaccdec.data_types = fixedpt_dt;
	oval_ARDaccdec.mantissa_count = 2;
	oval_ARDaccdec.real_count = 2;
	oval_ARDaccdec.max_value = 99.99;
	oval_ARDaccdec.min_value = 0.5;
	oval_ARDaccdec.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ARDaccdec.unit = "Sec";
	//oval_ARDaccdec.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ARDaccdec.val = &menue_lift_data_setup.m_ARDAccDec;
	lcurves_ARDAccDec.option_save_val = &oval_ARDaccdec;
	//__________________________________________________
	
	lcurves_ARDjerk.text = "ARD Corner";
	oval_ARDjerk.data_types = fixedpt_dt;
	oval_ARDjerk.mantissa_count = 2;
	oval_ARDjerk.real_count = 2;
	oval_ARDjerk.max_value = 99.99;
	oval_ARDjerk.min_value = 0;
	oval_ARDjerk.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ARDjerk.unit = "%";
	//oval_ARDjerk.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ARDjerk.val = &menue_lift_data_setup.m_ARDJerk;
	lcurves_ARDjerk.option_save_val = &oval_ARDjerk;
	//__________________________________________________
	lcurves_prespeed.text = "Soft start speed";
	oval_pre_spd.data_types = fixedpt_dt;
	oval_pre_spd.mantissa_count = 2;
	oval_pre_spd.real_count = 2;
	oval_pre_spd.max_value = 99.99;
	oval_pre_spd.min_value = 0;
	oval_pre_spd.new_val_flag = &menue_lift_data_newdata_flag;
	oval_pre_spd.unit = "Hz";
	//oval_pre_spd.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_pre_spd.val = &menue_lift_data_setup.m_pre_speed;
	lcurves_prespeed.option_save_val = &oval_pre_spd;	
	//__________________________________________________
	lcurves_prespd_time.text = "Soft speed time";
	oval_pre_spd_time.data_types = uint16_dt;
	oval_pre_spd_time.mantissa_count = 0;
	oval_pre_spd_time.real_count = 4;
	oval_pre_spd_time.max_value = 5000;
	oval_pre_spd_time.min_value = 0;
	oval_pre_spd_time.new_val_flag = &menue_lift_data_newdata_flag;
	oval_pre_spd_time.unit = "ms";
	//oval_pre_spd_time.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_pre_spd_time.val = &menue_lift_data_setup.m_pre_spd_time;
	lcurves_prespd_time.option_save_val = &oval_pre_spd_time;
	//__________________________________________________
	fill_menue_list(&menue_curves, opt_curves, OPT_CURVES_MAX, 0, 0);
	
	return &menue_curves;
}