/*
 * menue_drive_ARD.c
 *
 * Created: 09/07/2018 12:14:24 م
 *  Author: ahmed
 */  

#include <menue_drive_ARD.h>
#include <menue_data_manager.h>

//drive ARD list

menue_design_t menue_drive_ARD;
#define OPT_DRIVE_ARD_MAX 2
option_t opt_ard[OPT_DRIVE_ARD_MAX];

#define dard_ard_type					(opt_ard[0])
#define dard_ard_max_current			(opt_ard[1])


//__________________________________________________
//ARD types list
menue_design_t menue_drive_ard_types;
#define OPT_ARD_TYPES_MAX 2
option_t opt_ard_types[OPT_ARD_TYPES_MAX];

#define ard_type_Battery				(opt_ard_types[0])
#define ard_type_UPS					(opt_ard_types[1])

option_val_t oval_ard_max_current;
//__________________________________________________


menue_design_t *menue_drive_ARD_init(void)
{

	dard_ard_type.text = "ARD type";
	ard_type_Battery.text = "Battery";
	ard_type_UPS.text = "UPS";


	fill_menue_list(&menue_drive_ard_types, &opt_ard_types, OPT_ARD_TYPES_MAX, 0, &menue_drive_data_setup.m_ard_type);

	// set the 'option dard_ard_type' child

	dard_ard_type.child = &menue_drive_ard_types;

//__________________________________________________
	dard_ard_max_current.text = "ARD max current";
	oval_ard_max_current.data_types = float_dt;
	oval_ard_max_current.mantissa_count = 1;
	oval_ard_max_current.real_count = 2;
	oval_ard_max_current.max_value = 25;
	oval_ard_max_current.min_value = 0.0;
	oval_ard_max_current.new_val_flag = &menue_drive_data_newdata_flag;
	oval_ard_max_current.unit = "A";
	//oval_ard_max_current.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_ard_max_current.val = &menue_drive_data_setup.m_ard_current_max;
	dard_ard_max_current.option_save_val = &oval_ard_max_current;	
//__________________________________________________	
	fill_menue_list(&menue_drive_ARD, opt_ard, OPT_DRIVE_ARD_MAX, 0, 0);
	return &menue_drive_ARD;
}