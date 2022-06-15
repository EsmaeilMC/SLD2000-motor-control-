/*
 * menue_lift.c
 *
 * Created: 01/05/2017 11:16:05 م
 *  Author: ahmed
 */ 
#include <menue_lift.h>
#include <menue_basic.h>
#include <menue_timers.h>
#include <menue_curves.h>
#include <menue_advanced.h>
#include <menue_data_manager.h>
#include <direct_stop.h>

menue_design_t menue_lift;
#define OPT_LIFT_MAX 8
option_t opt_lift[OPT_LIFT_MAX];

#define lift_basic				(opt_lift[0])
#define lift_advanced			(opt_lift[1])
#define lift_timers				(opt_lift[2])
#define lift_curve_param		(opt_lift[3])
#define Hoist_info				(opt_lift[4])
#define lift_restore_param		(opt_lift[5])
#define lift_save_param			(opt_lift[6])
#define lift_save_hoist			(opt_lift[7])

void menue_lift_init(menue_design_image_t *imgae_menue)
{
	fill_menue_image(imgae_menue, &menue_lift, basic_set, "Lift param setup (L)", &menue_lift_data_applied.set_password);
	
	lift_basic.text = "Basic setup";
	lift_basic.child = menue_basic_init();
	
	lift_advanced.text = "Advanced setup";
	lift_advanced.child = menue_advanced_init();
	
	lift_timers.text = "Timers & delays";
	lift_timers.child = menue_timers_init();
	
	lift_curve_param.text = "Speed curves";
	lift_curve_param.child = menue_curves_init();
	
	Hoist_info.text = "Hoist data";
	Hoist_info.child = menue_direct_stop_init();
	
	lift_save_param.text = "Save lift param";
	lift_save_param.confirm_msg = "Are you sure?";
	lift_save_param.confirmed_sub = &menue_data_manager_saveLift;
	//lift_save_param.option_characteristics |= OPTION_HIDDEN;
	
	lift_save_hoist.text = "Save Hoist data";
	lift_save_hoist.confirm_msg = "Are you sure?";
	lift_save_hoist.confirmed_sub = &menue_data_manager_hoist_save;
	
	lift_restore_param.text = "Restore factory set";
	lift_restore_param.confirm_msg = "Are you sure?";
	lift_restore_param.confirmed_sub = &menue_data_manager_lift_restore;
	//lift_restore_param.option_characteristics |= OPTION_HIDDEN;
	fill_menue_list(&menue_lift, opt_lift, OPT_LIFT_MAX, imgae_menue, 0);
}