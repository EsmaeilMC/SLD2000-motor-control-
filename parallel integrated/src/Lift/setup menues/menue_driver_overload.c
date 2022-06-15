/*
 * menue_driver_overload.c
 *
 * Created: 27/07/2017 06:03:26 م
 *  Author: ahmed
 */ 


#include <menue_driver_overload.h>
#include <menue_data_manager.h>
#include <fixedptc.h>

//encoder list

menue_design_t menue_driver_overload;
#define OPT_DRIVER_OVERLOAD_MAX 3
option_t opt_driver_overload[OPT_DRIVER_OVERLOAD_MAX];

#define dmotor_overload_type					(opt_driver_overload[0])
#define dmotor_overload_val						(opt_driver_overload[1])
#define dmotor_overload_time					(opt_driver_overload[2])


option_val_t oval_dover_val;
option_val_t oval_dover_time;
//__________________________________________________
//overload type list
menue_design_t menue_overload_types;
#define OPT_OVERLOAD_TYPES_MAX 3
option_t opt_overload_types[OPT_OVERLOAD_TYPES_MAX];

#define overload_vfd				(opt_overload_types[0])
#define overload_normal				(opt_overload_types[1])
#define overload_off				(opt_overload_types[2])



menue_design_t *menue_driver_overload_init(void)
{
	dmotor_overload_type.text = "Settings";
	overload_vfd.text = "VFD motor";
	overload_normal.text = "Normal motor";
	overload_off.text = "Off";
	
	fill_menue_list(&menue_overload_types, &opt_overload_types, OPT_OVERLOAD_TYPES_MAX, 0, &menue_drive_data_setup.m_overload_type);

	// set the 'option ladvanced_req_pins' child

	dmotor_overload_type.child = &menue_overload_types;
//___________________________________________________________

	dmotor_overload_val.text = "Value";
	oval_dover_val.data_types = fixedpt_dt;
	oval_dover_val.mantissa_count = 1;
	oval_dover_val.real_count = 3;
	oval_dover_val.max_value = 120;
	oval_dover_val.min_value = 20;
	oval_dover_val.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dover_val.unit = "%";
	//oval_dover_val.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dover_val.val = &menue_drive_data_setup.m_overload_value;
	dmotor_overload_val.option_save_val = &oval_dover_val;
//___________________________________________________________

	dmotor_overload_time.text = "Time";
	oval_dover_time.data_types = uint8_dt;
	oval_dover_time.mantissa_count = 0;
	oval_dover_time.real_count = 2;
	oval_dover_time.max_value = 60;
	oval_dover_time.min_value = 1;
	oval_dover_time.new_val_flag = &menue_drive_data_newdata_flag;
	oval_dover_time.unit = "Sec";
	//oval_dover_time.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_dover_time.val = &menue_drive_data_setup.m_overload_time;
	dmotor_overload_time.option_save_val = &oval_dover_time;
	
//__________________________________________________
	fill_menue_list(&menue_driver_overload, opt_driver_overload, OPT_OVERLOAD_TYPES_MAX, 0, 0);
	return &menue_driver_overload;	
}