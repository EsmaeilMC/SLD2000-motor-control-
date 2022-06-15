/*
 * menue_util.c
 *
 * Created: 03/05/2017 04:29:57 م
 *  Author: ahmed
 */ 
#include <menue_util.h>
#include <menue_data_manager.h>
#include <menue_util_io.h>
#include <menue_util_date_time.h>
#include <requests.h>

menue_design_t menue_util;
#define OPT_UTIL_MAX 6
option_t opt_util[OPT_UTIL_MAX];

#define util_ioset				(opt_util[0])
#define util_time_date			(opt_util[1])
#define utii_pass				(opt_util[2])
#define util_restore			(opt_util[3])
#define util_save				(opt_util[4])
#define util_req				(opt_util[5])

option_val_t oval_set_password;
option_val_t oval_req;

uint8_t req = 0;
void make_i_call(void)
{
	insert_irequest(req);
	req = 0;
}
void menue_util_init(menue_design_image_t *imgae_menue)
{
	fill_menue_image(imgae_menue, &menue_util, advanced_p, "Utilities (U)", &menue_lift_data_applied.set_password);
	
	util_ioset.text = "IO addressing";
	util_ioset.child = menue_util_io_init();
//////////////////////////////////////////////////////////////////////////	
	util_time_date.text = "Date/Time";
	util_time_date.child = menue_util_date_time_init();
//////////////////////////////////////////////////////////////////////////
	utii_pass.text = "Setup Password";
	oval_set_password.data_types = uint16_dt;
	oval_set_password.mantissa_count = 0;
	oval_set_password.real_count = 4;
	oval_set_password.max_value = 9999;
	oval_set_password.min_value = 0;
	oval_set_password.new_val_flag = &menue_lift_data_newdata_flag;
	//oval_set_password.unit = "A";
	//oval_set_password.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_set_password.val = &menue_lift_data_setup.set_password;
	utii_pass.option_save_val = &oval_set_password;
//////////////////////////////////////////////////////////////////////////
	util_restore.text = "Restore factory set";
	util_restore.confirm_msg = "Are you sure?";
	util_restore.confirmed_sub = &menue_data_manager_restore;	
	util_save.text = "Save all data";
	util_save.confirm_msg = "Are you sure?";
	util_save.confirmed_sub = &menue_data_manager_save;
	
	fill_menue_list(&menue_util, opt_util, OPT_UTIL_MAX, imgae_menue, 0);
	
	//////////////////////////////////////////////////////////////////////////
	util_req.text = "internal call";
	oval_req.data_types = uint8_dt;
	oval_req.mantissa_count = 0;
	oval_req.real_count = 2;
	oval_req.max_value = 31;
	oval_req.min_value = 0;
	oval_req.new_val_flag = &menue_lift_data_newdata_flag;
	//oval_req.unit = "A";
	//oval_req.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_req.val = &req;
	util_req.option_save_val = &oval_req;
	util_req.exit_menue = &make_i_call;
}