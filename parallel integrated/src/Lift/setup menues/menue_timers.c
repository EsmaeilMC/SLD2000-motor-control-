/*
 * menue_timers.c
 *
 * Created: 25/11/2015 12:08:42 م
 *  Author: ahmed
 */ 
/*
 * menue_basic.c
 *
 * Created: 24/11/2015 12:04:03 ص
 *  Author: ahmed
 */ 

#include <menue_timers.h>
#include <menue_data_manager.h>


//timers list
menue_design_t menue_timers;
#define OPT_TIMERS_MAX 12
option_t opt_timers[OPT_TIMERS_MAX];

#define ltimers_slow_timer	          (opt_timers[0])
#define ltimers_fast_timer	          (opt_timers[1])
#define ltimers_cam_timer	          (opt_timers[2])
#define ltimers_stop_timer		      (opt_timers[3])
#define ltimers_light_timer			  (opt_timers[4])
#define ltimers_parking_timer         (opt_timers[5])
#define ltimers_upstop_timer          (opt_timers[6])
#define ltimers_downstop_timer        (opt_timers[7])
#define ltimers_ers_toff_timer        (opt_timers[8])
#define ltimers_ers_stop_timer        (opt_timers[9])
#define ltimers_ers_ldir_delay        (opt_timers[10])
#define ltimers_ers_ldir_waitTime     (opt_timers[11])

option_val_t oval_ltimers_slow_timer;
option_val_t oval_ltimers_fast_timer;
option_val_t oval_ltimers_cam_timer;
option_val_t oval_ltimers_stop_timer;
option_val_t oval_ltimers_light_timer;
option_val_t oval_ltimers_parking_timer;
option_val_t oval_ltimers_upstop_timer;
option_val_t oval_ltimers_downstop_timer;
option_val_t oval_ltimers_toff_timer;
option_val_t oval_ltimers_ers_stop_timer;
option_val_t oval_ltimers_toff_timer;
option_val_t oval_ltimers_ers_stop_timer;
option_val_t oval_ltimers_ers_ldir_delay;
option_val_t oval_ltimers_ers_ldir_waitTime;

menue_design_t *menue_timers_init(void)
{
	//fill menue lift timers
	
	ltimers_slow_timer.text = "Slow time-out";
	oval_ltimers_slow_timer.data_types = uint8_dt;
	oval_ltimers_slow_timer.mantissa_count = 0;
	oval_ltimers_slow_timer.real_count = 2;
	oval_ltimers_slow_timer.max_value = 50;
	oval_ltimers_slow_timer.min_value = 1;
	oval_ltimers_slow_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_slow_timer.unit = "Sec";
	oval_ltimers_slow_timer.text = "When this time elapse if the lift couldn't find the door level signal it will stop flaging an error";
	oval_ltimers_slow_timer.val = &menue_lift_data_setup.m_slow_time;
	ltimers_slow_timer.option_save_val = &oval_ltimers_slow_timer;
//__________________________________________________

	ltimers_fast_timer.text = "Fast time-out";
	oval_ltimers_fast_timer.data_types = uint8_dt;
	oval_ltimers_fast_timer.mantissa_count = 0;
	oval_ltimers_fast_timer.real_count = 2;
	oval_ltimers_fast_timer.max_value = 50;
	oval_ltimers_fast_timer.min_value = 1;
	oval_ltimers_fast_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_fast_timer.unit = "Sec";
	oval_ltimers_fast_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_fast_timer.val = &menue_lift_data_setup.m_fast_time;
	ltimers_fast_timer.option_save_val = &oval_ltimers_fast_timer;
//__________________________________________________
	ltimers_cam_timer.text = "CAM time-out";
	oval_ltimers_cam_timer.data_types = uint8_dt;
	oval_ltimers_cam_timer.mantissa_count = 0;
	oval_ltimers_cam_timer.real_count = 2;
	oval_ltimers_cam_timer.max_value = 50;
	oval_ltimers_cam_timer.min_value = 1;
	oval_ltimers_cam_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_cam_timer.unit = "Sec";
	//oval_ltimers_cam_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_cam_timer.val = &menue_lift_data_setup.m_cam_time;
	ltimers_cam_timer.option_save_val = &oval_ltimers_cam_timer;
//__________________________________________________
	ltimers_stop_timer.text = "Stop wait time";
	oval_ltimers_stop_timer.data_types = uint8_dt;
	oval_ltimers_stop_timer.mantissa_count = 0;
	oval_ltimers_stop_timer.real_count = 2;
	oval_ltimers_stop_timer.max_value = 10;
	oval_ltimers_stop_timer.min_value = 1;
	oval_ltimers_stop_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_stop_timer.unit = "Sec";
	//oval_ltimers_cam_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_stop_timer.val = &menue_lift_data_setup.m_stop_wait_time;
	ltimers_stop_timer.option_save_val = &oval_ltimers_stop_timer;
//__________________________________________________
	ltimers_light_timer.text = "Light-on time";
	oval_ltimers_light_timer.data_types = uint8_dt;
	oval_ltimers_light_timer.mantissa_count = 0;
	oval_ltimers_light_timer.real_count = 3;
	oval_ltimers_light_timer.max_value = 180;
	oval_ltimers_light_timer.min_value = 1;
	oval_ltimers_light_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_light_timer.unit = "Sec";
	//oval_ltimers_cam_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_light_timer.val = &menue_lift_data_setup.m_light_time;
	ltimers_light_timer.option_save_val = &oval_ltimers_light_timer;
	
//__________________________________________________

	ltimers_parking_timer.text = "Parking delay";
	oval_ltimers_parking_timer.data_types = uint16_dt;
	oval_ltimers_parking_timer.mantissa_count = 0;
	oval_ltimers_parking_timer.real_count = 4;
	oval_ltimers_parking_timer.max_value = 1000;
	oval_ltimers_parking_timer.min_value = 0;
	oval_ltimers_parking_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_parking_timer.unit = "Sec";
	//oval_ltimers_parking_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_parking_timer.val = &menue_lift_data_setup.m_parking_time;
	ltimers_parking_timer.option_save_val = &oval_ltimers_parking_timer;	
//__________________________________________________

	ltimers_upstop_timer.text = "Up_stop delay";
	oval_ltimers_upstop_timer.data_types = uint16_dt;
	oval_ltimers_upstop_timer.mantissa_count = 0;
	oval_ltimers_upstop_timer.real_count = 4;
	oval_ltimers_upstop_timer.max_value = 9999;
	oval_ltimers_upstop_timer.min_value = 1;
	oval_ltimers_upstop_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_upstop_timer.unit = "40ms";
	//oval_ltimers_upstop_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_upstop_timer.val = &menue_lift_data_setup.m_up_stop_delay;
	ltimers_upstop_timer.option_save_val = &oval_ltimers_upstop_timer;
	
//__________________________________________________
	ltimers_downstop_timer.text = "Down-stop delay";
	oval_ltimers_downstop_timer.data_types = uint16_dt;
	oval_ltimers_downstop_timer.mantissa_count = 0;
	oval_ltimers_downstop_timer.real_count = 4;
	oval_ltimers_downstop_timer.max_value = 9999;
	oval_ltimers_downstop_timer.min_value = 1;
	oval_ltimers_downstop_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_downstop_timer.unit = "40ms";
	//oval_ltimers_downstop_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_downstop_timer.val = &menue_lift_data_setup.m_down_stop_delay;
	ltimers_downstop_timer.option_save_val = &oval_ltimers_downstop_timer;
//__________________________________________________
	
	ltimers_ers_toff_timer.text = "ARD T-Off delay";
	oval_ltimers_toff_timer.data_types = uint16_dt;
	oval_ltimers_toff_timer.mantissa_count = 0;
	oval_ltimers_toff_timer.real_count = 3;
	oval_ltimers_toff_timer.max_value = 999;
	oval_ltimers_toff_timer.min_value = 1;
	oval_ltimers_toff_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_toff_timer.unit = "Sec";
	//oval_ltimers_toff_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_toff_timer.val = &menue_lift_data_setup.m_ers_toff_delay;
	ltimers_ers_toff_timer.option_save_val = &oval_ltimers_toff_timer;
//__________________________________________________

	ltimers_ers_stop_timer.text = "ARD stop time-out";
	oval_ltimers_ers_stop_timer.data_types = uint16_dt;
	oval_ltimers_ers_stop_timer.mantissa_count = 0;
	oval_ltimers_ers_stop_timer.real_count = 3;
	oval_ltimers_ers_stop_timer.max_value = 999;
	oval_ltimers_ers_stop_timer.min_value = 1;
	oval_ltimers_ers_stop_timer.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_ers_stop_timer.unit = "Sec";
	//oval_ltimers_downstop_timer.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_ers_stop_timer.val = &menue_lift_data_setup.m_ers_stop_time;
	ltimers_ers_stop_timer.option_save_val = &oval_ltimers_ers_stop_timer;
//__________________________________________________

	ltimers_ers_ldir_delay.text = "ARD L-Dir delay";
	oval_ltimers_ers_ldir_delay.data_types = uint16_dt;
	oval_ltimers_ers_ldir_delay.mantissa_count = 0;
	oval_ltimers_ers_ldir_delay.real_count = 3;
	oval_ltimers_ers_ldir_delay.max_value = 999;
	oval_ltimers_ers_ldir_delay.min_value = 1;
	oval_ltimers_ers_ldir_delay.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_ers_ldir_delay.unit = "Sec";
	//oval_ltimers_ers_ldir_delay.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_ers_ldir_delay.val = &menue_lift_data_setup.m_ers_ldir_delay;
	ltimers_ers_ldir_delay.option_save_val = &oval_ltimers_ers_ldir_delay;
	//__________________________________________________

	ltimers_ers_ldir_waitTime.text = "ARD L_dir test time";
	oval_ltimers_ers_ldir_waitTime.data_types = uint16_dt;
	oval_ltimers_ers_ldir_waitTime.mantissa_count = 0;
	oval_ltimers_ers_ldir_waitTime.real_count = 3;
	oval_ltimers_ers_ldir_waitTime.max_value = 999;
	oval_ltimers_ers_ldir_waitTime.min_value = 1;
	oval_ltimers_ers_ldir_waitTime.new_val_flag = &menue_lift_data_newdata_flag;
	oval_ltimers_ers_ldir_waitTime.unit = "Sec";
	//oval_ltimers_ers_ldir_waitTime.text = "When this time elapses if the lift couldn't find selector signal, it will stop flaging an error";
	oval_ltimers_ers_ldir_waitTime.val = &menue_lift_data_setup.m_ers_ldir_test_time;
	ltimers_ers_ldir_waitTime.option_save_val = &oval_ltimers_ers_ldir_waitTime;
	//__________________________________________________
		
	fill_menue_list(&menue_timers,opt_timers,OPT_TIMERS_MAX, 0, 0);
	
	return &menue_timers;
}