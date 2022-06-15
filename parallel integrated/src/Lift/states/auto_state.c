
#include <asf.h>
#include <fixedptc.h>
#include <encoder_interface.h>
#include <auto_state.h>
#include <SEDACIO.h>
#include <timers.h>
#include <control_status_register.h>
#include <SCH.h>
#include <e_faults.h>
#include <service_priority_organizer.h>
#include <requests.h>
#include <floor_counter.h>
#include <menue_data_manager.h>
#include <EMCM.h>
#include <selfLearn.h>
#include <con_toff.h>
#include <distance_regulator.h>

#include <vfd.h>


uint8_t auto_state = 0;
uint8_t prev_auto_state = 0;

#define ARD (((auto_state & 2) == 2) && !ARDINS)
#define INS ( ((auto_state & 1) == 1) && 1)
#define ARDINS ((auto_state & 3) == 3)



uByte_t onfloor = 0;
uByte_t auto_counter = auto_stop;

uint8_t auto_get_e_state(void)
{
	return auto_counter;
}
int32_t get_speed_sign()
{
	int32_t sign = 1;
	if (!direction_up)
	{
		sign *= -1;
	}
	if (menue_lift_data_applied.m_up_dir)
	{
		sign *= -1;
	}
	return sign;
}

void state_check(void)
{
	auto_state = 0;
	if(mnt == 1) auto_state |= 1;
	if(ersin == 1) auto_state |= 2;
	
	if((prev_auto_state != auto_state) || ARDINS) auto_stop_enter();
	
	prev_auto_state = auto_state;
}
void auto_lift_states_monitor()
{
	if(INS) ctb_ignore_door_limits(1);
	else ctb_ignore_door_limits(0);
	
	if (INS)
	{
		req_enable(0);
		drop_all_req();
	}
	else if (ARD)
	{
		req_enable(0);
		drop_all_req();
	}
	else if(dont_record_fault()) req_enable(0);
	else req_enable(1);
}

void ctb_automatic_doors_handler(void)
{
	if (external_auto)
	{
		ctb_activate_door1(1);
		ctb_activate_door2(0);
	}
	
	if (INS)
	{
		ctb_light_curtain1_ignore(1);
		ctb_light_curtain2_ignore(1);
	}
	h_open_set(0);
	h_close_set(0);
	if (ctb_door1_state_closing || ctb_door2_state_closing)
	{
		h_open_set(0);
		h_close_set(1);
	} 
	else if (ctb_door1_state_opening || ctb_door2_state_opening)
	{
		h_open_set(1);
		h_close_set(0);
	}
	if (INS == 0)
	{
		//door feedback error
		if (ctb_door1_state_abnormal_feedback)
		{
			fdoor_ab_err_set;
		}
		if (ctb_door2_state_abnormal_feedback)
		{
			bdoor_ab_err_set;
		}
	}
}

void auto_update(void)
{
	selector_count();
	switch (auto_counter)
	{
		case auto_idle:
		auto_idle_exe();
		break;
		case auto_stop:
		auto_stop_exe();
		break;
		case auto_hspd:
		auto_hspd_exe();
		break;
		case auto_lspd:
		auto_lspd_exe();
		break;
		case auto_camtest:
		auto_camtest_exe();
		break;
		case auto_checkInvReady:
		auto_checkInvReady_exe();
		break;
		case auto_stopping:
		auto_stopping_exe();
		break;
		case auto_prespd:
		auto_prespd_exe();
		break;
	}
	state_check();
	direction_limits_check();
	auto_lift_states_monitor();
	
	ctb_automatic_doors_handler();
}

void auto_enter(void){}
uByte_t auto_instate(uByte_t state)
{
	if(state==auto_counter)
		return 1;
	else
		return 0;
}

void auto_update_elevator_initilal_values(void)
{
	menue_data_manager_update();
	uint8_t *temp_data_ptr = menue_data_manager_get_CounterData();
	sel_set(temp_data_ptr[0]);
	enc_set_theta(*((int32_t*)&temp_data_ptr[1]));
	toff_counter = (*((int32_t*)&temp_data_ptr[5]));
}

void open_doors(void)
{
	m_d1open(1);
	m_d1close(0);
	m_d2open(1);
	m_d2close(0);
	m_d3open(1);
	m_d3close(0);
	ctb_close_door1(0);
	ctb_close_door2(0);
}

void close_doors(void)
{
	m_d1open(0);
	m_d1close(1);
	m_d2open(0);
	m_d2close(1);
	m_d3open(0);
	m_d3close(1);
	ctb_close_door1(1);
	ctb_close_door2(1);
}

//stop state subs
uByte_t dr_opened=0; //signal door opened and closed
uByte_t auto_stop_guard(void);
uByte_t auto_stop_guard(void)
{
	if(shk==0) return 1;
	if(s_i_err()) return 1; //if there is error preventing elevator moving don't exit stop_state
	if(h_open==1) return 1; //if |<>| pressed open door and don't exit
	return 0; //else exit stop_state
}
uint8_t floor_counter_save = 1;
void auto_stop_enter(void)
{
	auto_counter = auto_stop;
	vfd_estop();
	ersd_stop();
	selfLearn_terminateLearning();
	m_gong(0);
	if (floor_counter_save)
	{
		if(mnt == 0) toff_counter++;
		menue_data_manager_saveCounterData(selector_counter, enc_get_theta(), toff_counter);
		//menue_data_manager_saveCounterData(10, 1000);
		floor_counter_save = 0;
	}
	
	if(!ARDINS) 
	{
		IO_YARD(0);
		if(ARD && (mstop == 1) ) ersd_allowTo_turn_on();
	}
	
	menue_data_manager_update();
	set_stop_flash_rate;
	if((!INS) && (!ARD)) stop_idle_reset();
	
	if(INS)
	{
		ar_up(0);
		ar_dwn(0);
	}
	ctb_force_door_close(0);
	march_cam(0);
	ctb_light_curtain1_ignore(0);
	ctb_light_curtain2_ignore(0);
}
void auto_stop_exe(void)
{
	
	if ((INS)||(ARD)) light_timer_reset();
	if(ARDINS) 
	{
		IO_YARD(1);
		ersd_force_turn_off();
	}
	else if(ARD)
	{
		if (mstop == 0) ersd_force_turn_off();
		else ersd_allowTo_turn_on();
	}
	if((h_close==1)&&(external_auto)&&!(s_i_err()==1)&&(ctb_light_curtain_fb==0))
	{
		light_timer_reset();
		auto_idle_enter();// if |<>| is pressed then goto idle to close auto door
	}
	else
	{

		if((shk==0))//&&!(external_auto))//the door is open now
		{
			//reset light timer
			light_timer_reset();
			if(!(external_auto))
			{
				stop_idle_reset();
				dr_opened=1;//signal door opened flag
			}
		}
		else if((shk==1)&&(ctb_light_curtain_fb == 0))//||external_auto) //the door is closed or automatic doors
		{
			auto_stop_exit();
		}
		if(1)//door_is_auto)
		{
			if(!onfloor)
			{
				if((mstop==0) && (INS==0))
				{
					onfloor=1;
					open_doors();
				}
				else if(mstop == 1)
				{
					close_doors();
				}
			}
			else
				{
					open_doors();
				}
		}
	}

	auto_stop_exit();
}
void auto_stop_exit(void)
{
	if(auto_stop_guard())
	{
		return;
	}
	if((dr_opened && !external_auto )||(INS)||(ARD))
	{
		dr_opened=0;
	}
	if( (INS) && ((mup == 1) || (mdwn == 1)) ) auto_idle_enter();
	else if ( (ARD) && (ersd_isReady()) ) auto_idle_enter();
	
	//else
	//stop_idle_reset();
}


//Idle Check inverter ready subs
void auto_checkInvReady_enter(void){}
void auto_checkInvReady_exe(void){}
void auto_checkInvReady_exit(void){}

uint8_t cf_press = 0;

void auto_reopen_door(void)
{
	if (auto_instate(auto_idle) == 1)
	{
		cf_press = 1;
	}
}
//Idle state subs
#define ERSC_TOFF_DELAY 250
uint32_t ersc_toff_counter;
void auto_idle_enter(void)
{
	auto_counter=auto_idle;
	idle_camt_reset();
	ctb_force_door_close(0);
	IO_YARD(0);
	m_gong(0);
	if(ARD && (mstop == 1) ) ersd_allowTo_turn_on();
	ersc_toff_counter = 0;
	cf_press = 0;
	set_stop_flash_rate;
	vfd_estop();
	march_cam(0);
	if(!door_auto_close)
	{
		if(onfloor)
		{
			open_doors();
		}
	}
	else //if the door is auto_close then close at ideal
	{
		close_doors();
	}
		
	
	if(INS)
	{
		ar_up(0);
		ar_dwn(0);
	}
	//req_drop_cntr=0;
	ctb_light_curtain1_ignore(0);
	ctb_light_curtain2_ignore(0);
}
void auto_idle_exe(void)
{
	if(((device_off == 1)&&(INS==0))||(s_i_err()==1))
	{
		auto_stop_enter();
		return;
	}
	if ( (INS) || (ARD) ) light_timer_reset();
	if ( (ARD) || (ARDINS))
	{
		//stop if emergency unit is not ready
		if( !ersd_isReady() )
		{
			auto_stop_enter();
			return;
		}
		else if((mstop == 0) || (INS))
		{
			//signal turning off ARD device
			if(++ersc_toff_counter > ERSC_TOFF_DELAY)
			{
				IO_YARD(1);
				ersd_force_turn_off();
			}
		}
	}
	if(!door_auto_close)
	{
		if(onfloor)
		{
			open_doors();
		}
	}
	else //if the door is auto_close then close at ideal
	{
		close_doors();
	}
	if(((h_open==1)||(ctb_light_curtain_fb)||((cf_press==1)&&(h_close==0)))&&(door_is_auto))
	{
		auto_stop_enter();// if |<>| or current floor ex button is pressed then goto stop to open auto door
		light_timer_reset();
	}
	else
	{
		if((shk==0))//&&(!external_auto))
		{
			auto_idle_exit();
		}
		else if((shk==1))//||(external_auto))
		{
			if((ARD) && (ersd_isReady() == 1))
			{
				if((mstop == 1))//not on floor
				{
					idle_camt_start(); //auto_idle_exit();
				}
			}
			else if((INS) && ((mup==1)||(mdwn==1)))
			{
				if(!(((mup == 1) && (upfdl0 == 0) && (((mstop == 0) && (upfl == 2)) || (upfl == 0)) ) || ((mdwn ==1 ) && (dnfdl0 == 0) && (((mstop == 0) && (dnfl == 2)) || (dnfl == 0)))))//don't move further than final limits
					idle_camt_start();//auto_idle_exit();//if mup or mdn then enter maintenance move mode
			}
			else if((get_scheduled_floor()!=-1)&&(INS==0)&&(ARD==0)&&(ersd_isOff()))// if there is a service in auto mode
			{
				idle_camt_start();//start cam transition timer
			}
		}
	}
}
void auto_idle_exit(void)
{
	if((shk==0) || (!Estop))//&&(!external_auto))//door is opened
	{
		//enter stop if door opened in normal and internal auto door
		auto_stop_enter();
	}
	else if((shk==1))//||(external_auto))//door is closed or automatic doors
	{
		if( (INS) && ((mup == 0) && (mdwn == 0)) ) auto_stop_enter();
		else if(ow==0)
			auto_camtest_enter();
		else
			auto_idle_enter();
	}
}


//hspd state subs
int8_t lspd_mstp_met = 0, Dstop_speed_sign = 1;
void march_set(void)
{
	march_cam(1);
	close_doors();
	if(INS)
	{
		ar_up(direction_up);
		ar_dwn(!direction_up);
	}

}
unsigned int fast_cntr=0,fast_cntr_pos=0;
void auto_hspd_enter(void)
{
	auto_counter = auto_hspd;
	//s_move_flag;
	
	ctb_force_door_close(0);
	set_fast_flash_rate;
	fast_cntr=0;
	fast_cntr_pos = FAST_TIME_OUT;
	march_set();
	m_gong(0);
	lspd_mstp_met = 0; //reset mstop met signal
	if (DIRECT_STOP)
	{
		if(!set_Hspeed_profile()) 
		{
			auto_stop_enter();
			drop_all_req();
		}
		else
		{
			Dstop_speed_sign = EMCM_get_travel_speed_sign();
		}
	} 
	else
	{
		vfd_set_motion_acc(menue_lift_data_applied.m_acc);
		vfd_set_motion_j1(menue_lift_data_applied.m_corner1/100);
		vfd_set_motion_j2(menue_lift_data_applied.m_corner2/100);
		
		vfd_set_ref_speed(get_speed_sign() * HIGH_SPEED);
	}
	ctb_light_curtain1_ignore(1);
	ctb_light_curtain2_ignore(1);
}
void auto_hspd_exe(void)
{
	if(! vfd_enable_isEnabled()) auto_stop_enter();
	
	light_timer_reset();
	IO_YARD(0);
	ersd_allowTo_turn_on();
	if(!Estop) e_stop_set;
	if(!shk) shk_err_set;
	if(!lock) lock_err_set;
	
	if(direction_up)
	{
		if(upfl == 0)
		{
			auto_stop_enter();
			drop_all_req();
			upfl_err;
			return;
		}
	}
	else
	{
		if(dnfl == 0)
		{
			auto_stop_enter();
			drop_all_req();
			dnfl_err;
			return;
		}
	}
	
	if((s_i_err()==1))// if there is an immediate stop error then stop
	{
		auto_stop_enter();
		drop_all_req();
		return;
	}
	if((shk==0)||(lock==0))
	{
		auto_stop_enter();
		return;
	}
	if(INS==0)
	{
		//check if the car arrived to destination
		auto_hspd_exit();
	}
	else if ( (INS) || (ARD) )
	{
		auto_stop_enter();
	}

	if((fast_cntr > FAST_TIME_OUT)&&(INS==0))
	{
		// todo fast time error
		fastset;
		drop_all_req();
		fast_cntr_pos++;
		if(fast_cntr_pos > FAST_TIME_OUT)
		{
			slowset;
			auto_stop_enter();
		}
	}
	else
		fast_cntr++;
	if(0/*ol_err>0*/)
		pos_next_floor();
}
char    next_floor;// to get destination floor during states
void auto_hspd_exit(void)
{
		
	int temp;
	temp = get_scheduled_floor();
	if((temp != -1))
	{
		next_floor = temp;
		if(((temp != f_max) && (temp != 0)) || (DIRECT_STOP))
		{
			if(get_current_floor() == next_floor)//we're in the destination floor
			{
				if(infloorzone())
				{
					if (DIRECT_STOP)
					{
						if(exit_hspd()) auto_lspd_enter();
					}
					else auto_lspd_enter();
				}
			}
		}
	}
	else
		pos_next_floor();	
}

//creep after meeting dirlimit subs
char prev_lspd_creep = 0;
void auto_lspd_creep(void)
{
	if (prev_lspd_creep == 0)
	{
		vfd_set_ref_speed(get_speed_sign() * CREEP_SPEED);
		prev_lspd_creep = 1;
	}
}
//lspd state subs
#define SLOW_TIME_OUT (menue_lift_data_applied.m_slow_time * AUTO_STATE_UPDATE_SEC)
unsigned int  slow_cntr=0;
uint32_t stop_delay_counter;
#define RELEVEL_DELAY_DSTOP (25 * 5)
uint8_t relevel_counter = 0;

void auto_set_mstp_met(void)
{
	lspd_mstp_met = 1; //w've met mstp
}

void auto_lspd_enter(void)
{
	auto_counter = auto_lspd;
	IO_YARD(0);
	m_gong(0);
	ersd_allowTo_turn_on();
	relevel_counter = 0;
	prev_lspd_creep = 0;
	set_slow_flash_rate;
	slow_cntr=0;
	ctb_force_door_close(0);
	
	//      run_read_tmr =  run_read_tmr_val;
	//      ers_slow_tmr = 60 * 25 * char_ers_slow_tmr;
	//      ers_slow_cntr=0;

	onfloor = 0;
	if(direction_up) stop_delay_counter = menue_lift_data_applied.m_up_stop_delay;
	else stop_delay_counter = menue_lift_data_applied.m_down_stop_delay;
	//      if(floor_relevel==1) stop_delay_counter=1;
	march_set();
	if( (INS) && (ARD == 0) )
	{
		vfd_set_motion_acc(menue_lift_data_applied.m_ins_acc);
		vfd_set_motion_j1(menue_lift_data_applied.m_ins_corner/100);
		vfd_set_motion_j2(menue_lift_data_applied.m_ins_corner/100);
		
		if(enter_Hlearn(direction_up))
		{
			//selflearn mode
			//setlearn speed.
			vfd_set_ref_speed(get_speed_sign() * INS_SPEED / 2);
		}
		else
		{
			//normal INS mode
			vfd_set_ref_speed(get_speed_sign() * INS_SPEED);
		}
		
	}
	else if( (ARD) && (INS == 0) )
	{
		if(ersd_isReady)
		{
			ersd_move();
		}
		else
		{
			auto_stop_enter();
		}
	}
	else if(ARDINS)
	{
		auto_stop_enter();
		return;
	}
	else
	{
		if (DIRECT_STOP)
		{
			set_correct_atStop();
		}
		else
		{
			vfd_set_motion_acc(menue_lift_data_applied.m_dec);
			vfd_set_motion_j1(menue_lift_data_applied.m_corner1/100);
			vfd_set_motion_j2(menue_lift_data_applied.m_corner2/100);
			
			vfd_set_ref_speed(get_speed_sign() * CREEP_SPEED);	
		}
	}
	ctb_light_curtain1_ignore(1);
	ctb_light_curtain2_ignore(1);
}
void auto_lspd_exe()
{
	if(! vfd_enable_isEnabled()) auto_stop_enter();
	
	light_timer_reset();
	if(!Estop) e_stop_set;
	if(!shk) shk_err_set;
	if(!lock) lock_err_set;
	
	if(ARD)
	{
		if (mstop == 0)
		{
			//arrived
			auto_stopping_enter();
		}
	}
	else
	{
		if((slow_cntr > SLOW_TIME_OUT)&&(INS==0))
		{
			// slow timeout set
			slowset;
		}
		else
		slow_cntr++;
	}
	if(s_i_err())// if there is an immediate stop error then stop
	{
		auto_stop_enter();
		drop_all_req();
		return;
	}
	if((shk==0)||(lock==0))
	{
		auto_stop_enter();
		return;
	}
	if(INS==0)
	{
		if(direction_up)
		{
			if(upfl == 0)
			{
				auto_stop_enter();
				drop_all_req();
				upfl_err;
				return;
			}
		}
		else
		{
			if(dnfl == 0)
			{
				auto_stop_enter();
				drop_all_req();
				dnfl_err;
				return;
			}
		}
		
		if(shk==1)
		{
			//check if the car arrived exactly at floor
			auto_lspd_exit();
		}
	}
	else if(INS)
	{
		if(direction_up)
		{
			if(selfLearn_exe())
			{
				//lrn process
			}
			else if(mup==0)
			{
				if (menue_lift_data_applied.m_ins_stopping_mode == 0)
				{
					auto_stop_enter();
				}
				else
				{
					vfd_set_motion_acc(menue_lift_data_applied.m_ins_dec);
					auto_stopping_enter();
				}
			}
		}
		else
		{
			if(mdwn==0)
			{
				if (menue_lift_data_applied.m_ins_stopping_mode == 0)
				{
					auto_stop_enter();
				}
				else
				{
					vfd_set_motion_acc(menue_lift_data_applied.m_ins_dec);
					auto_stopping_enter();
				}
			}
		}
	}
}
void auto_lspd_exit(void)
{
	char temp;
	if(mstop == 0)
	{
		if (DIRECT_STOP)
		{
			drop_req(get_current_floor());
			auto_stopping_enter();
		}
		else if (stop_delay_counter > 0)
		{
			stop_delay_counter--;
			//delay before stopping
		}
		else
		{
			drop_req(get_current_floor());
			//inc_ser_tout();//if the service timeout enabled
			//if(floor_relevel==1) auto_stop_enter();
			//else
			auto_stopping_enter();
		}
	}
	else if (mstop == 1)
	{
		if (DIRECT_STOP)
		{
			if (vfd_get_ref_speed() == 0)
			{
				if (relevel_counter < RELEVEL_DELAY_DSTOP)
				{
					relevel_counter ++;
					drop_req(get_current_floor());
					set_correct_atStop();
					set_Stop_afterCorrection();
				}
				else
				{
					fixedpt relevel_speed = RELEVEL_SPEED * Dstop_speed_sign;
					if (lspd_mstp_met) relevel_speed *= -1;
					vfd_set_motion_acc(menue_lift_data_applied.m_ins_acc);
					vfd_set_motion_j1(menue_lift_data_applied.m_ins_corner/100);
					vfd_set_motion_j2(menue_lift_data_applied.m_ins_corner/100);
					vfd_set_ref_speed(relevel_speed);
				}
			}
			
		}
	}
}

//Stopping state subs
uint32_t auto_stopping_timeout=0;
void auto_stopping_enter(void)
{
	auto_counter = auto_stopping;
	auto_stopping_timeout = 50 * 25;
	IO_YARD(0);
	m_gong(1);
	ersd_allowTo_turn_on();
	if ((INS == 0) && (DIRECT_STOP == 0) )
	{
		vfd_set_motion_acc(menue_lift_data_applied.m_dec);
		vfd_set_motion_j1(menue_lift_data_applied.m_corner1/100);
		vfd_set_motion_j2(menue_lift_data_applied.m_corner3/100);
	}
	ctb_force_door_close(0);
	if((DIRECT_STOP == 0) || (INS == 1)  || (ARD == 1)) vfd_set_ref_speed(0);
	//distance_regulator_enable();
	ctb_light_curtain1_ignore(1);
	ctb_light_curtain2_ignore(1);
}
void auto_stopping_exe(void)
{
	if(! vfd_enable_isEnabled()) auto_stop_enter();
	
	if(auto_stopping_timeout-- == 0)
	{
		auto_stop_enter();
	}
	light_timer_reset();
	if(!Estop) e_stop_set;
	if(!shk) shk_err_set;
	if(!lock) lock_err_set;
	
	if(ARD)
	{
		//stope if emergency unit is not ready
		if(0)//ers_instate(ers_idle)!=1)
		{
			auto_stop_enter();
			return;
		}
	}
	
	if (INS == 0)
	{
		if(direction_up)
		{
			if(upfl == 0)
			{
				auto_stop_enter();
				drop_all_req();
				upfl_err;
				return;
			}
		}
		else
		{
			if(dnfl == 0)
			{
				auto_stop_enter();
				drop_all_req();
				dnfl_err;
				return;
			}
		}
	}
	if((s_i_err()==1))// if there is an imediate stop error then stop
	{
		auto_stop_enter();
		drop_all_req();
		return;
	}
	if((shk==0)||(lock==0))
	{
		auto_stop_enter();
		return;
	}
	auto_stopping_exit();
	
}
void auto_stopping_exit(void)
{
	if (!vfd_states_isStopping())
	{
		auto_stop_enter();
	}
}

//Cam state subs
//sec_dev delay depends on timer1 frequency
#define CAM_TIME        (menue_lift_data_applied.m_cam_time )
#define CAM_TRIALS        (menue_lift_data_applied.m_cam_trials )
uint_t sec_dev_counter=0;
uByte_t cam_time_counter=0;
uByte_t cam_trial_counter;

uByte_t cam_lock_wait=0;

uByte_t mnt_speed=1;
void auto_camtest_enter(void)
{
	auto_counter = auto_camtest;
	menue_data_manager_update();
	floor_counter_save = 1;
	//cam_lock_wait=0;
	light_timer_reset();
	IO_YARD(0);
	m_gong(0);
	ersd_allowTo_turn_on();
	
	set_cam_flash_rate;
	sec_dev_counter=0;
	cam_time_counter=0;
	
	vfd_estop();
	march_cam(1);
	close_doors();
	ctb_light_curtain1_ignore(0);
	ctb_light_curtain2_ignore(0);
	ctb_force_door_close(1);
}
void auto_camtest_exe(void)
{
	light_timer_reset();
	if((s_i_err()==1))// if there is an immediate stop error then stop
	{
		auto_stop_enter();
		drop_all_req();
		return;
	}
	if(INS)
	{
		if(!((mup==1)||(mdwn==1)))
		{
			cam_trial_counter=0;
			auto_stop_enter();
			return;
		}
	}
	if((shk==1))//||(external_auto))
	{
		if(++sec_dev_counter >= AUTO_STATE_UPDATE_SEC)
		{
			sec_dev_counter=0;
			//locker test
			if((lock==0)||(shk==0))
			{
				//locker closed
				ctb_force_door_close(1);
				if(++cam_time_counter>=(CAM_TIME))
				{
					//_____________________________________________
					cam_time_counter=0;
					//1st cam fail
					++cam_trial_counter;
					if(cam_trial_counter >= CAM_TRIALS)
					{
						//cam failed many times
						//drop all requests
						drop_all_req();
						cam_trial_counter=0;
						auto_stop_enter();
						//cam error set
						if(lock==0)
						{
							lock_err_set;
							//Automatic door check
						}
						if((shk==0)&&(external_auto))
						{
							shk_err_set;
						}
						return;
					}
					march_cam(0);
					auto_idle_enter();
					return;
				}
				else
				{
					//cam test1 fail
					//cam_time_counter++;
					return;
				}
				
			}
		}
		if((lock==0)||(shk==0))
		return;
	}
	else if((shk==0))//&&(!external_auto))
	{
		cam_time_counter=0;
		sec_dev_counter=0;
		auto_stop_enter();
		return;
	}
	auto_camtest_exit();
}
void auto_camtest_exit(void)
{
	int destination=-1;

	if((shk==1) && (Estop))//||(external_auto))
	{
		if(lock==1)
		{// all safety sensors are present
			cam_trial_counter=0;
			
			if(INS == 0) if(++cam_lock_wait<15)
			return;
			
			cam_lock_wait=0;
			if (ARD)
			{
				if(mstop == 1) auto_lspd_enter();
				else auto_stop_enter();
			}
			else if(INS == 0)
			{
				destination = get_scheduled_floor();
				if(destination != -1)
				{
					//next_floor=destination;
					
					if((infloorzone())&&(destination == get_current_floor()))//check if we're in the destination area
					{	
						auto_lspd_enter();
					}
					else
					{
						if (/*(menue_drive_data_applied.m_motor_type == 0) && */(menue_lift_data_applied.m_pre_spd_time > 0))
						{
							auto_prespd_enter();
						}
						else
						{
							auto_hspd_enter();
						}
					}
				}
				else
				{//there is no call to serve
					cam_time_counter=0;
					sec_dev_counter=0;
					cam_trial_counter=0;
					auto_stop_enter();
				}
			}
			else if(INS == 1)
			{
				if(mup==1)
				{
					direction_up = 1;
					//ent move state
					auto_lspd_enter();
				}
				else if(mdwn==1)
				{
					direction_up=0;
					//ent move state
					auto_lspd_enter();
				}
				else
					auto_stop_enter();
			}
		}
	}
	else auto_stop_enter();
}

volatile uint16_t pre_spd_counter = 0;
void auto_prespd_enter(void)
{
	auto_counter = auto_prespd;
	//s_move_flag;
	ctb_force_door_close(0);
	pre_spd_counter = menue_lift_data_applied.m_pre_spd_time / 40;
	set_fast_flash_rate;
	march_set();
	m_gong(0);
	lspd_mstp_met = 0; //reset mstop met signal
	
	vfd_set_motion_acc(menue_lift_data_applied.m_acc);
	vfd_set_motion_j1(menue_lift_data_applied.m_corner1/100);
	vfd_set_motion_j2(menue_lift_data_applied.m_corner2/100);
		
	vfd_set_ref_speed(get_speed_sign() * menue_lift_data_applied.m_pre_speed);
	ctb_light_curtain1_ignore(1);
	ctb_light_curtain2_ignore(1);
}
void auto_prespd_exe(void)
{
	if(! vfd_enable_isEnabled()) auto_stop_enter();
	
	light_timer_reset();
	IO_YARD(0);
	ersd_allowTo_turn_on();
	if(!Estop) e_stop_set;
	if(!shk) shk_err_set;
	if(!lock) lock_err_set;
	
	if((s_i_err() == 1))// if there is an immediate stop error then stop
	{
		auto_stop_enter();
		drop_all_req();
		return;
	}
	if((shk==0)||(lock==0))
	{
		auto_stop_enter();
		return;
	}
	if(INS==0)
	{
		//check if the car arrived to destination
		auto_prespd_exit();
	}
	else if ( (INS) || (ARD) )
	{
		auto_stop_enter();
	}

}
void auto_prespd_exit(void)
{
	if (pre_spd_counter > 0)
	{
		if(vfd_states_allowedtomove()) pre_spd_counter--;
	} 
	else
	{
		auto_hspd_enter();
	}
}