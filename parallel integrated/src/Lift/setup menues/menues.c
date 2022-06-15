/*
 * menues.c
 *
 * Created: 25/11/2015 08:22:16 ص
 *  Author: ahmed
 */ 

#include <menues.h>

//Basic graphical
menue_design_image_t menue_lift_graphical;

//Util graphical
menue_design_image_t menue_timers_graphical;

//Driver graphical
menue_design_image_t menue_driver_graphical;

//Faults graphical
menue_design_image_t menue_faults_graphical;

//access control graphical
menue_design_image_t menue_util_graphical;


void init_menues(void)
{
	connect_graphical_menues(&menue_lift_graphical, &menue_driver_graphical);
	connect_graphical_menues(&menue_driver_graphical, &menue_util_graphical);
	connect_graphical_menues(&menue_util_graphical, &menue_faults_graphical);
	connect_graphical_menues(&menue_faults_graphical, &menue_lift_graphical);
	
	menue_lift_init(&menue_lift_graphical);	//init the menue images and other sub_lists
	menue_drive_init(&menue_driver_graphical); //init the driver menue images and other sub_lists
	menue_util_init(&menue_util_graphical);
	menue_faults_init(&menue_faults_graphical);
	
	set_current_list(&menue_lift_graphical); //set the default active menue	
}
