/*
 * menue_basic.c
 *
 * Created: 24/11/2015 12:04:03 ص
 *  Author: ahmed
 */ 

#include <menue_basic.h>
#include <graphical_menues.h>
#include <menue_data_manager.h>
#include <floorDisplay.h>

//__________________________________________________
//basic list
menue_design_t menue_basic1;
#define OPT_BASIC_MAX 6
option_t opt_basic1[OPT_BASIC_MAX];

#define door_count          (opt_basic1[0])
#define display_type        (opt_basic1[1])
#define floor_disp          (opt_basic1[2])
#define door_type           (opt_basic1[3])
#define collective          (opt_basic1[4])
#define lift_direction      (opt_basic1[5])

option_val_t door_count_option_val;

//__________________________________________________
//floor_disp types list
menue_design_t menue_basic1_floor_disp;
#define OPT_BASIC_FLOOR_DISP_MAX 2
option_t opt_basic1_floor_disp[OPT_BASIC_FLOOR_DISP_MAX];

#define floor_disp_floorNumber			   (opt_basic1_floor_disp[0])
#define floor_disp_floorDisp			   (opt_basic1_floor_disp[1])

option_val_t floor_disp_option_val;
//__________________________________________________
//lift direction types list
menue_design_t menue_basic1_lift_directions;
#define OPT_BASIC_LIFT_DIRECTIONS_MAX 2
option_t opt_basic1_lift_directions[OPT_BASIC_LIFT_DIRECTIONS_MAX];

#define lift_direction_fwd			   (opt_basic1_lift_directions[0])
#define lift_direction_rev			   (opt_basic1_lift_directions[1])

//__________________________________________________
//collective types list
menue_design_t menue_basic1_collective_types;
#define OPT_BASIC_COLLECTIVE_TYPES_MAX 3
option_t opt_basic1_collective_types[OPT_BASIC_COLLECTIVE_TYPES_MAX];

#define collective_type_down24			   (opt_basic1_collective_types[0])
#define collective_type_down			   (opt_basic1_collective_types[1])
#define collective_type_updown			   (opt_basic1_collective_types[2])

//__________________________________________________
//display types list
menue_design_t menue_basic1_display_types;
#define OPT_BASIC_DISPLAY_TYPES_MAX 4
option_t opt_basic1_display_types[OPT_BASIC_DISPLAY_TYPES_MAX];

#define display_type_7seg			   (opt_basic1_display_types[0])
#define display_type_binary			   (opt_basic1_display_types[1])
#define display_type_decoder	       (opt_basic1_display_types[2])
#define display_type_binarypos         (opt_basic1_display_types[3])

//__________________________________________________
//door type list
menue_design_t menue_basic1_door_types;
#define OPT_BASIC_DOOR_TYPES_MAX 4
option_t opt_basic1_door_types[OPT_BASIC_DOOR_TYPES_MAX];

#define door_type_manual            (opt_basic1_door_types[0])
#define door_type_internal_auto     (opt_basic1_door_types[1])
#define door_type_auto_closed       (opt_basic1_door_types[2])
#define door_type_auto_open         (opt_basic1_door_types[3])
//__________________________________________________

void door_count_enter(void)
{
		
}
char door_count_exe(char enter, char esc, char up, char down, char left, char right)
{
	if(esc) return 0;
	return 1;
}
//struct menue_design test_list;

menue_design_t *menue_basic_init(void)
{
	//fill menue door basic
	door_count.text = "Floors count";
	//door_count.enter = &door_count_enter;
	//door_count.exe = &door_count_exe;
	door_count_option_val.data_types = uint8_dt;
	door_count_option_val.mantissa_count	=  0;
	door_count_option_val.real_count		=  2;
	door_count_option_val.max_value			=  23;
	door_count_option_val.min_value			=  1;
	//door_count_option_val.unit				= "d";
	door_count_option_val.text = "Enter your building number of doors";
	
	door_count_option_val.val				= &menue_lift_data_setup.m_floor_count;
	door_count_option_val.new_val_flag		= &menue_lift_data_newdata_flag;
	door_count.option_save_val = &door_count_option_val;
//__________________________________________________

	display_type.text = "Display type";
	
	display_type_7seg.text = "7 Segments";
	display_type_binary.text = "Binary";
	display_type_binarypos.text = "Binary Pos";
	display_type_decoder.text = "Decoder";

	fill_menue_list(&menue_basic1_display_types, &opt_basic1_display_types, OPT_BASIC_DISPLAY_TYPES_MAX, 0, &menue_lift_data_setup.m_display_type);

	// set the 'option display_type' child to be menue_basic1_display_types
	//this way when you click display_type menue_basic1_door_types' items will appear!
	display_type.child = &menue_basic1_display_types;
//__________________________________________________
	
	floor_disp.text = "Floor display";
	
	floor_disp_floorNumber.text = "Enter floor number";
	
	floor_disp_option_val.data_types = uint8_dt;
	floor_disp_option_val.mantissa_count	=  0;
	floor_disp_option_val.real_count		=  2;
	floor_disp_option_val.max_value			=  24;
	floor_disp_option_val.min_value			=  0;
	//door_count_option_val.unit				= "d";
	floor_disp_option_val.text = "Enter floor number";
	
	floor_disp_option_val.val				= &start_floor_floor_display;
	//floor_disp_option_val.new_val_flag		= &menue_lift_data_newdata_flag;
	floor_disp_floorNumber.option_save_val = &floor_disp_option_val;
	
	floor_disp_floorDisp.text = "Floor Display";
	floor_disp_floorDisp.exe = &floor_dislplay_menue_exe;
	floor_disp_floorDisp.enter = &floor_dislplay_menue_enter;
	
	fill_menue_list(&menue_basic1_floor_disp, &opt_basic1_floor_disp, OPT_BASIC_FLOOR_DISP_MAX, 0, 0);

	// set the 'option display_type' child to be menue_basic1_floor display
	floor_disp.child = &menue_basic1_floor_disp;	

//__________________________________________________

	door_type.text = "door type";
	//fill menue door options
	door_type_manual.text = "Manual";
	door_type_internal_auto.text = "Internal automatic";
	door_type_auto_closed.text = "Automatic N closed";
	door_type_auto_open.text = "Automatic N open";
	
	fill_menue_list(&menue_basic1_door_types, opt_basic1_door_types, OPT_BASIC_DOOR_TYPES_MAX, 0, &menue_lift_data_setup.m_door_type);
	
	// set the 'option door_type' child to be menue_basic1_door_types
	//this way when you click door_type menue_basic1_door_types' items will appear!
	door_type.child = &menue_basic1_door_types;
//__________________________________________________	
	collective.text = "Registering system";
	collective_type_down24.text = "Down collect 24";
	collective_type_down.text = "Down collect";
	collective_type_updown.text = "Up down collect";
	
	fill_menue_list(&menue_basic1_collective_types, opt_basic1_collective_types, OPT_BASIC_COLLECTIVE_TYPES_MAX, 0, &menue_lift_data_setup.m_collection);
	// set the 'option collection_type' child to be menue_basic1_door_types
	collective.child = &menue_basic1_collective_types;
//__________________________________________________
	lift_direction.text = "Lift up direction";
	lift_direction_fwd.text = "Forward";
	lift_direction_rev.text = "Reverse";
	
	fill_menue_list(&menue_basic1_lift_directions, opt_basic1_lift_directions, OPT_BASIC_LIFT_DIRECTIONS_MAX, 0, &menue_lift_data_setup.m_up_dir);
	
	//Set the child to enter
	lift_direction.child = &menue_basic1_lift_directions;
//__________________________________________________
	fill_menue_list(&menue_basic1, opt_basic1, OPT_BASIC_MAX, 0, 0);

	return &menue_basic1;
}