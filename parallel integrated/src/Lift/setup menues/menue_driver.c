/*
 * menue_driver.c
 *
 * Created: 02/05/2017 12:11:12 ص
 *  Author: ahmed
 */ 
#include <menue_driver.h>
#include <menue_driver_drive.h>
#include <menue_driver_motor_parameters.h>
#include <menue_driver_encoder.h>
#include <menue_driver_vector.h>
#include <menue_data_manager.h>
#include <menue_driver_vf.h>
#include <menue_drive_brake.h>
#include <menue_driver_overload.h>
#include <menue_drive_ARD.h>
#include <menue_driver_mech_data.h>

menue_design_t menue_drive;
#define OPT_DRIVE_MAX 11
option_t opt_drive[OPT_DRIVE_MAX];

#define drive_driver			(opt_drive[0])
#define drive_motor				(opt_drive[1])
#define drive_vector			(opt_drive[2])
#define drive_encoder			(opt_drive[3])
#define drive_vf				(opt_drive[4])
#define drive_overload			(opt_drive[5])
#define drive_brake				(opt_drive[6])
#define drive_mech				(opt_drive[7])
#define drive_ARD				(opt_drive[8])
#define drive_restore			(opt_drive[9])
#define drive_save				(opt_drive[10])

void menue_drive_init(menue_design_image_t *imgae_menue)
{
	fill_menue_image(imgae_menue, &menue_drive, timing, "Drive param setup (D)", &menue_lift_data_applied.set_password);
	
	drive_driver.text = "Driver";
	drive_driver.child = menue_driver_drive_init();
	drive_motor.text = "Motor Parameters";
	drive_motor.child = menue_driver_motor_parameters_init();
	drive_vector.text = "Vector param";
	drive_vector.child = menue_driver_vector_init();
	drive_mech.text = "Mechanical data";
	drive_mech.child = menue_driver_mech_data_init();
	drive_encoder.text = "Encoder param";
	drive_encoder.child = menue_driver_encoder_init();
	drive_vf.text = "V/F parameters";
	drive_vf.child = menue_driver_vf_init();
	drive_overload.text = "Overload";
	drive_overload.child = menue_driver_overload_init();
	drive_brake.text = "Motor brake";
	drive_brake.child = menue_driver_brake_init();
	drive_ARD.text = "ARD";
	drive_ARD.child = menue_drive_ARD_init();
	drive_restore.text = "Restore Factory set";
	drive_restore.confirm_msg = "Are you sure?";
	drive_restore.confirmed_sub = &menue_data_manager_drive_restore;
	drive_save.text = "Save drive param";
	drive_save.confirm_msg = "Are you sure?";
	drive_save.confirmed_sub = &menue_data_manager_saveDrive;
	
	fill_menue_list(&menue_drive, opt_drive, OPT_DRIVE_MAX, imgae_menue, 0);
}