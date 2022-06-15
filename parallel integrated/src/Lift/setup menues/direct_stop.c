/*
 * direct_stop.c
 *
 * Created: 07/08/2018 03:21:59 م
 *  Author: ahmed
 */ 

#include <direct_stop.h>
#include <graphical_menues.h>
#include <menue_data_manager.h>


uint8_t floor_number = 0;
//__________________________________________________
//direct_stop list
menue_design_t menue_direct_stop;
#define OPT_DIRECT_STOP_MAX 6

option_t optl_direct_stop[OPT_DIRECT_STOP_MAX];

#define opt_direct_stop         (optl_direct_stop[0])
#define opt_lift_speed	        (optl_direct_stop[1])
#define opt_plate_length        (optl_direct_stop[2])
#define opt_creep_distance      (optl_direct_stop[3])
#define opt_self_learn          (optl_direct_stop[4])
#define opt_floor_level         (optl_direct_stop[5])

option_val_t lift_speed_option_val;
option_val_t plate_length_option_val;
option_val_t creep_distance_option_val;

//__________________________________________________
//self_learn types list
menue_design_t menue_direct_stop_self_learn;
#define OPT_DIRECT_STOP_SELF_LEARN_MAX 2
option_t opt_direct_stop_self_learn[OPT_DIRECT_STOP_SELF_LEARN_MAX];

#define self_learn_on			   (opt_direct_stop_self_learn[1])
#define self_learn_off			   (opt_direct_stop_self_learn[0])

//__________________________________________________
//direct_stop types list
menue_design_t menue_direct_stop_activation;
#define OPT_DIRECT_STOP_ACTIVATION_MAX 2
option_t opt_direct_stop_activation[OPT_DIRECT_STOP_ACTIVATION_MAX];
//__________________________________________________
//floor levels types list
menue_design_t menue_direct_stop_floor_leves;
#define OPT_DIRECT_STOP_FLOOR_LEVEL_MAX 2
option_t opt_direct_stop_floor_levels[OPT_DIRECT_STOP_FLOOR_LEVEL_MAX];

#define floor_level_floorNumber			   (opt_direct_stop_floor_levels[0])
#define floor_level_floorLevel			   (opt_direct_stop_floor_levels[1])

option_val_t floor_number_option_val;
option_val_t floor_level_option_val;
//__________________________________________________

#define direct_stop_inactive			  (opt_direct_stop_activation[0])
#define direct_stop_active				  (opt_direct_stop_activation[1])

void floor_level_floorLevel_enter(void)
{
	floor_level_option_val.val = &hoist_info.level_position_correction[floor_number];
}

menue_design_t *menue_direct_stop_init(void)
{
	//fill menue lift speed
	opt_lift_speed.text = "Lift speed";
	lift_speed_option_val.data_types = fixedpt_dt;
	lift_speed_option_val.mantissa_count	=  1;
	lift_speed_option_val.real_count		=  1;
	lift_speed_option_val.max_value			=  8;
	lift_speed_option_val.min_value			=  0.1;
	lift_speed_option_val.unit				= "M/s";
	//lift_speed_option_val.text = "Enter your building number of doors";
	
	lift_speed_option_val.val				= &menue_lift_data_setup.m_lift_speed;
	lift_speed_option_val.new_val_flag		= &menue_lift_data_newdata_flag;
	opt_lift_speed.option_save_val = &lift_speed_option_val;
//__________________________________________________
	//fill menue plate length
	opt_plate_length.text = "Stop plate length";
	plate_length_option_val.data_types = uint8_dt;
	plate_length_option_val.mantissa_count	=  0;
	plate_length_option_val.real_count		=  3;
	plate_length_option_val.max_value			=  500;
	plate_length_option_val.min_value			=  10;
	plate_length_option_val.unit				= "mm";
	//plate_length_option_val.text = "Enter your building number of doors";
	
	plate_length_option_val.val				= &menue_lift_data_setup.m_stop_plate_length;
	plate_length_option_val.new_val_flag		= &menue_lift_data_newdata_flag;
	opt_plate_length.option_save_val = &plate_length_option_val;
//__________________________________________________
	//fill menue creep distance
	opt_creep_distance.text = "stopping distance";
	creep_distance_option_val.data_types = fixedpt_dt;
	creep_distance_option_val.mantissa_count	=  2;
	creep_distance_option_val.real_count		=  2;
	creep_distance_option_val.max_value			=  24;
	creep_distance_option_val.min_value			=  0.01;
	//creep_distance_option_val.unit				= "d";
	//creep_distance_option_val.text = "Enter your building number of doors";
	
	creep_distance_option_val.val				= &menue_lift_data_setup.m_creep_distance;
	creep_distance_option_val.new_val_flag		= &menue_lift_data_newdata_flag;
	opt_creep_distance.option_save_val = &creep_distance_option_val;
//__________________________________________________
	opt_direct_stop.text = "DirectStop Act";
	direct_stop_inactive.text = "Inactive";
	direct_stop_active.text = "Active";
	
	fill_menue_list(&menue_direct_stop_activation, opt_direct_stop_activation, OPT_DIRECT_STOP_ACTIVATION_MAX, 0, &menue_lift_data_setup.m_direct_stop);
	
	//Set the child to enter
	opt_direct_stop.child = &menue_direct_stop_activation;
//__________________________________________________
	opt_self_learn.text = "HoistWay learn";
	self_learn_on.text = "On";
	self_learn_off.text = "Off";
	
	fill_menue_list(&menue_direct_stop_self_learn, opt_direct_stop_self_learn, OPT_DIRECT_STOP_SELF_LEARN_MAX, 0, &menue_lift_data_startSelfLrn);
	
	//Set the child to enter
	opt_self_learn.child = &menue_direct_stop_self_learn;
//__________________________________________________
	
	opt_floor_level.text = "Floor Level";
	
	floor_level_floorNumber.text = "Enter floor number";
	
	floor_number_option_val.data_types = uint8_dt;
	floor_number_option_val.mantissa_count	=  0;
	floor_number_option_val.real_count		=  2;
	floor_number_option_val.max_value			=  24;
	floor_number_option_val.min_value			=  0;
	floor_number_option_val.text = "Enter floor number";
	
	floor_number_option_val.val	= &floor_number;
	floor_level_floorNumber.option_save_val = &floor_number_option_val;
	
	floor_level_floorLevel.text = "Floor Level";
	//floor_level_floorLevel.exe = &floor_dislplay_menue_exe;
	//floor_level_floorLevel.enter = &floor_dislplay_menue_enter;
	floor_level_option_val.data_types			=	fixedpt_dt;
	floor_level_option_val.mantissa_count		=	2;
	floor_level_option_val.real_count			=	3;
	floor_level_option_val.max_value			=	fixedpt_rconst(200);
	floor_level_option_val.min_value			=	-fixedpt_rconst(200);
	floor_level_option_val.unit					=	"mm";
	floor_level_option_val.val					=	&hoist_info.level_position_correction[floor_number];

	floor_level_floorLevel.option_save_val = &floor_level_option_val;
	floor_level_floorLevel.enter = &floor_level_floorLevel_enter;
	
	fill_menue_list(&menue_direct_stop_floor_leves, &opt_direct_stop_floor_levels, OPT_DIRECT_STOP_FLOOR_LEVEL_MAX, 0, 0);

	// set the 'option display_type' child to be menue_basic1_floor display
	opt_floor_level.child = &menue_direct_stop_floor_leves;	

//__________________________________________________
	fill_menue_list(&menue_direct_stop, optl_direct_stop, OPT_DIRECT_STOP_MAX, 0, 0);

	return &menue_direct_stop;
}