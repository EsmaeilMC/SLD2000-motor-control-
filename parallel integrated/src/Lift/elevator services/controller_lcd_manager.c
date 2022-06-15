/*
 * controller_lcd_manager.c
 *
 * Created: 20/11/2015 01:09:16 م
 *  Author: ahmed
 */
#include <controller_lcd_manager.h>
#include <u_glcd.h>
#include <speed_regulator.h>
#include <auto_state.h>
#include <e_setup.h>
#include <service_priority_organizer.h>
#include <control_status_register.h>
#include <requests.h>
#include <SEDACIO.h>
#include <io_interface.h>
#include <floor_counter.h>
#include <menue_basic.h>
#include <sph_faults.h>
#include <vfd.h>
#include <VectorPM.h>
#include <axistransform.h>
#include <Inverter.h>
#include <encoder_interface.h>
#include <fastmath.h>
#include <SPMstatic_tune.h>
#include <EMCM.h>
#include <selfLearn.h>
#include <PM_param_tune1.h>
#include <IM_param_tune1.h>
#include <IM_param_tune2.h>
#include <pop_ups.h>
#include <overload.h>
#include <con_toff.h>
#include <req_io_mb_reader.h>
//#include <AT24Cxx_dynamic.h>

#define IDF_SCREEN_MAIN			0
#define IDF_SCREEN_PROGRAMMING  1
#define IDF_SCREEN_MONITOR		2
#define IDF_SCREEN_ENCODER		3
#define IDF_SCREEN_TORQUE		4

uByte_t active_screen = IDF_SCREEN_MAIN;
void controller_lcd_manager_handler(void)
{
	if (pop_ups_showMessage()) 
	{
		lcd_update();
		return;
	}
	if (active_screen == IDF_SCREEN_MAIN)
	{
		main_screen_play();
	} 
	else if (active_screen == IDF_SCREEN_PROGRAMMING)
	{
		programming_screen_play();
	}
	else if (active_screen == IDF_SCREEN_MONITOR)
	{
		monitor_screen_play();
	}
	else if (active_screen == IDF_SCREEN_ENCODER)
	{
		encoder_screen_play();
	}
	else if (active_screen == IDF_SCREEN_TORQUE)
	{
		torque_screen_play();
	}
	lcd_update();	
}

uByte_t check_elevator_inmotion(void)
{
	if(auto_instate(auto_stop)||auto_instate(auto_idle)||auto_instate(auto_camtest)) return 0;
	else return 1;
}

#define M_S_FLASH_MAX (7)
uint32_t main_screen_flsh_counter = 0;
void main_screen_play(void)
{
	uint8_t shiftchar = 0;
	
	if (++main_screen_flsh_counter > 2 * M_S_FLASH_MAX) main_screen_flsh_counter = 0;
	
	char i,selc;
	static char scr_stg=0;
	const char *str;
	char fault_code, fault_prefix;	
	int res;
	char *tmp;
	
	selc=selector_counter;
	glcd_clear();
	glcd_gotoxy(0,0);
	
	
	if (menue_lift_data_applied.m_direct_stop == 0)
	{
		shiftchar = 0;
		putIt('S');
		glcd_puts(itos(selc),0);
	}
	else
	{
		shiftchar = 20;
		int64_t lift_speed = fixedpt_abs(enc_get_omega_hat());// fixedpt_abs(vfd_get_desired_speed());
		//lift_speed *= menue_drive_data_applied.m_encoder_p_count * 2;
		//lift_speed /= menue_drive_data_applied.m_motor_pole_pair_count;
		lift_speed /= hoist_info.enc_count_per_meter;
		
		glcd_puts(fixedpt_cstr(lift_speed, 2),0);
		glcd_putsf("M/s",0);
	}
	
	
	glcd_gotoxy(24 + shiftchar,0);
	if((mstop==0)&&((selc&1)==0)) putIt('=');
	putIt('F');

	glcd_puts(itos(get_current_floor()), 0);
		
	glcd_gotoxy(46 + shiftchar, 0);
	if(check_elevator_inmotion())
	{
		//motion state
		if(direction_up)
		{
			putIt(0);
			req_mb_setUpMotion();
		}
		else
		{
			putIt(1);
			req_mb_setDnMotion();
		}
	}
	else
	{
		req_mb_setStill();
	}
	if(mnt==0) req_mb_clrINS();
	if (selfLearn_isLearning()) glcd_putsf("LRN", 0);
	else if (vfd_states_isTuning()) glcd_putsf("TUN", 0);
	else if(mnt==1) 
	{
		glcd_putsf("INS",0);
		req_mb_setINS();
	}
	else if(ersd_isOff()==0) 
	{
		glcd_putsf("ARD",0);
	}
	else
	{
		putIt('t');
		res=get_scheduled_floor();
		if(res==-1) glcd_putsf("__",0);
		else glcd_puts(itos(res),0);
	}
	glcd_gotoxy(92,0);
	if (device_off) putIt('O');
	else if (main_screen_flsh_counter < M_S_FLASH_MAX)
	{
		if (menue_lift_data_applied.m_cont_dis_technique == 1) putIt('S');
		else if (menue_lift_data_applied.m_cont_dis_technique == 2) putIt('T');
	}
	
		
	glcd_gotoxy(101,0);
	if(Estop) putIt('_');
	else putIt('P');
	glcd_gotoxy(108,0);
	if(lock) putIt('_');
	else putIt('L');
	glcd_gotoxy(115,0);
	if(vfd_enable_isEnabled()) putIt('_');
	else putIt('E');
		
	glcd_gotoxy(122,0);
	if(shk==1) putIt('_');
	else putIt('F');
	
	
	glcd_gotoxy(0,9);
	for(i=0;i<16;i++)
	{
		if(collect == 2)
		{
			//All collective mode
			if((ireq&msk(i))!=0)
			{
				putIt(6);
			}
			else if((ereq&msk(i)) && (eereq&msk(i)))
			{
				//putIt(5);
				putIt(10);
			}
			else if((ereq&msk(i)))
			{
				//putIt(5);
				putIt(9);
			}
			else if((eereq&msk(i)))
			{
				//putIt(5);
				putIt(8);
			}
			else putIt(7);
		}
		else
		{
			//up down collective mode
			if((ireq&msk(i))!=0)
			{
				putIt(6);
			}
			else if((ereq&msk(i)))
			{
				putIt(5);
			}
			else putIt(7);
		}
			
	}
	
	
	glcd_gotoxy(0,18);
	for(i=0;i<8;i++)
	{
		if(collect == 2)
		{
			//All collective mode
			if((ireq&msk(i+16))!=0)
			{
				putIt(6);
			}
			else if((ereq&msk(i+16)) && (eereq&msk(i+16)))
			{
				//putIt(5);
				putIt(10);
			}
			else if((ereq&msk(i+16)))
			{
				//putIt(5);
				putIt(9);
			}
			else if((eereq&msk(i+16)))
			{
				//putIt(5);
				putIt(8);
			}
			else putIt(7);
		}
		else
		{
			//up down collective mode
			if((ireq&msk(i+16))!=0)
			{
				putIt(6);
			}
			else if((ereq&msk(i+16)))
			{
				putIt(5);
			}
			else putIt(7);
		}
		
	}
	glcd_gotoxy(66, 18);
	uint32_t hour, minute, seconds, PM = 0;
	rtc_get_time(RTC, &hour, &minute, &seconds);
	if (hour > 12)
	{
		PM = 1;
		hour -= 12;
	}
	if (hour == 0) hour = 12;
	glcd_puts(itos(hour), 0);
	putIt(':');
	glcd_puts(itos(minute), 0);
	putIt(':');
	glcd_puts(itos(seconds), 0);
	
	glcd_gotoxy(128 - 15, 18);
	if (PM) glcd_putsf(" PM", 0);
	else glcd_putsf(" AM", 0);
	
	glcd_gotoxy(0, 27);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(sqrt(axt_get_current_avg()/2)), 2),0);
	glcd_putsf("A",0);
	
	glcd_gotoxy(42,27);

	glcd_puts(fixedpt_cstr(fixedpt_abs(vfd_get_desired_speed()), 2),0);
	glcd_putsf("Hz",0);
	
	
	
	glcd_gotoxy(96,27);
	if (auto_instate(auto_stop)==1)
	{
		write_r_text("Stop",0);
	}
	else if(auto_instate(auto_idle)==1)
	{
		write_r_text("Idle",0);
	}
	else if(auto_instate(auto_camtest)==1)
	{
		write_r_text("Door",0);
	}
	else if(auto_instate(auto_hspd)==1)
	{
		write_r_text("Hspd",0);
	}
	else if(auto_instate(auto_lspd)==1)
	{
		write_r_text("Lspd",0);
	}
	else if(auto_instate(auto_stopping)==1)
	{
		write_r_text("Ariv",0);
	}
	
	if(get_enter_button())
	{
		active_screen = IDF_SCREEN_PROGRAMMING;
	}
	else if(get_up_button())
	{
		active_screen = IDF_SCREEN_MONITOR;
	}
	else if(get_down_button())
	{
		active_screen = IDF_SCREEN_ENCODER;
	}
	else if(get_right_button())
	{
		active_screen = IDF_SCREEN_TORQUE;
	}
//////////////////////////////////////////////////////////////////////////
	d_line(0,128,39,39);
		
	glcd_gotoxy(50,36);
		
	if(public_lang) write_r_text("أعطال",1);
	else write_r_text("Faults",0);
	
	if(sph_faults_get_fault_to_display(&str, &fault_prefix, &fault_code))
	{
		//there is a fault to display
		glcd_gotoxy(0,46);
		if(fault_prefix == 1)
		{
			glcd_putsf("D",0);
		}
		else if (fault_prefix == 2)
		{
			glcd_putsf("L",0);
		}
		glcd_puts(itos(fault_code), 0);
		glcd_putsf(" ",0);
		glcd_putsf(str, 0);
		
		req_mb_setFault(fault_prefix, fault_code);
	}
	else
	{
		req_mb_setFault(0, 0);
	}	
	glcd_gotoxy(0,55);
	//glcd_puts(sitos(EMCM_get_enc_flash()), 0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(get_zs_displacement()/2), 3), 0);
	
	//glcd_puts(fixedpt_cstr(fixedpt_rconst(/*sqrt*/(pm_param_getdata1())), 2),0);
	if (menue_lift_data_applied.m_direct_stop == 1)
	{
		if (check_elevator_inmotion() == 0)
		{
			glcd_putsf("  ", 0);
			int64_t floor_err = EMCM_get_floorErr() * FIXEDPT_ONE;
			floor_err /= hoist_info.enc_count_per_meter;
			
			if (fixedpt_abs(floor_err) >= FIXEDPT_ONE)
			{
				
				if (floor_err < fixedpt_rconst(-0.01))
				{
					putIt(1);
				} 
				else if (floor_err > fixedpt_rconst(0.01))
				{
					putIt(0);
				}
				floor_err = fixedpt_abs(floor_err);
				
				glcd_puts(fixedpt_cstr(floor_err, 2),0);
				glcd_putsf(" M", 0);
			}
			else if (fixedpt_abs(floor_err) >= fixedpt_rconst(0.01))
			{
				floor_err *= 100;
				
				if (floor_err < fixedpt_rconst(-0.01))
				{
					putIt(1);
				}
				else if (floor_err > fixedpt_rconst(0.01))
				{
					putIt(0);
				}
				floor_err = fixedpt_abs(floor_err);
				
				glcd_puts(fixedpt_cstr(floor_err, 2),0);
				glcd_putsf(" cm", 0);
			}
			else
			{
				floor_err *= 1000;
				
				if (floor_err < fixedpt_rconst(-0.01))
				{
					putIt(1);
				}
				else if (floor_err > fixedpt_rconst(0.01))
				{
					putIt(0);
				}
				floor_err = fixedpt_abs(floor_err);
				
				glcd_puts(fixedpt_cstr(floor_err, 2),0);
				glcd_putsf(" mm", 0);
			}
			//glcd_puts(sitos(EMCM_get_floorErr()),0);
		}
		//glcd_gotoxy(80,55);
		//glcd_puts(sitos(EMCM_get_enc_flash()),0);
		
	}
	
	
	drop_all_buttons();

}

void programming_screen_play(void)
{
	if(menue_execute(get_enter_button(), get_esc_button(), get_up_button(), get_down_button(), get_left_button(), get_right_button())) active_screen = IDF_SCREEN_MAIN;	
	drop_all_buttons();
}

void monitor_screen_play(void)
{
	uint32_t *adc_reading;
	adc_reading = inverter_getADCcurrent();
	float ia,ib,ic;
	inverter_get_creadings(&ia, &ib, &ic);
	
	glcd_clear();
	glcd_gotoxy(0,0);

	glcd_putsf("I ",0);
	glcd_puts(fixedpt_cstr( fixedpt_rconst(sqrt(axt_get_current_avg())), 4),0);
	
	glcd_gotoxy(0, 45);
	glcd_putsf("OL ",0);
	glcd_puts(itos(get_olval()),0);
	
	glcd_gotoxy(0, 9);
	glcd_putsf("Ia ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(ia), 4),0);
	//glcd_puts(fixedpt_cstr((hoist_info.dirup0_decel), 4),0);
	//glcd_puts(sitos(get_olia()),0);
	
	glcd_gotoxy(0, 18);
	glcd_putsf("Ib ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(ib), 4),0);
	//glcd_puts(fixedpt_cstr((hoist_info.dirdwn0_decel), 4),0);
	//glcd_puts(sitos(get_olib()),0);

	glcd_gotoxy(0, 27);
	glcd_putsf("Ic ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(ic), 4),0);
	//glcd_puts(sitos(get_olic()),0);
	
	glcd_gotoxy(0, 36);
	glcd_putsf("Im ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(IM_param_tune1_getMdata()), 4),0);
	//glcd_puts(sitos(get_olim()),0);
	
	glcd_gotoxy(0, 45);
	glcd_putsf("T ",0);
	glcd_puts(fixedpt_cstr(iverter_get_igbt_temp(), 2), 0);
	
	glcd_gotoxy(0, 54);
	glcd_putsf("V ",0);
	glcd_puts(fixedpt_cstr(iverter_get_dc_v(), 2), 0);

	if(get_esc_button())
	{
		active_screen = IDF_SCREEN_MAIN;
	}
	
	drop_all_buttons();
}

void encoder_screen_play(void)
{
	glcd_clear();
	glcd_gotoxy(0,0);

	glcd_putsf("Encoder",0);
	
	glcd_gotoxy(0, 9);
	glcd_putsf("Inc ",0);
	int64_t tmp_theta = enc_get_lin_theta();
	uint32_t tmp_phs = (enc_get_phase() * 1000) / FIXEDPT_ONE;
	if (tmp_theta < 0)
	{
		tmp_theta += 1;
		if (tmp_theta == 0) putIt('-');
		tmp_phs = 1000 - tmp_phs;
	}
	glcd_puts(sitos(tmp_theta),0);
	
	putIt('.');
	if(tmp_phs < 10) putIt('0');
	if(tmp_phs < 100) putIt('0');
	glcd_puts(itos(tmp_phs),0);
	
// 	glcd_gotoxy(0, 18);
// 	glcd_putsf("ABS ",0);
// 	glcd_puts(fixedpt_cstr(fixedpt_rconst(encoder_getangle_abs()),4),0);

	glcd_gotoxy(0, 18);
	glcd_putsf("Rev ",0);
//	glcd_puts(fixedpt_cstr(fixedpt_abs(enc_get_omega_shaft_rad_hat()) * FIXEDPT_ONE, 8),0);
	
	glcd_puts(fixedpt_cstr(enc_get_omega_hz_hat(MOTOR_POLEPAIR), 8),0);
//	glcd_puts(fixedpt_cstr(enc_get_omega_hat(), 4),0);
	
//  glcd_gotoxy(0, 18);
//  glcd_putsf("phs ",0);
//  glcd_puts(fixedpt_cstr(enc_get_phase(), 4),0);	

	glcd_gotoxy(0, 27);
 	glcd_putsf("Vd ",0);
 	glcd_puts(fixedpt_cstr(fixedpt_rconst(cvd), 4),0);
	//glcd_puts(fixedpt_cstr((vfd_get_desired_distance() / MOTOR_POLEPAIR), 4),0);
	
	
	glcd_gotoxy(0, 36);
	glcd_putsf("Vq ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(cvq), 4),0);
	
	glcd_gotoxy(0, 45);
	glcd_putsf("Id ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(cid), 4),0);

	glcd_gotoxy(0, 54);
	glcd_putsf("Iq ",0);
	//glcd_puts(fixedpt_cstr(fixedpt_rconst(get_loadtorque()), 4),0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(ciq), 4),0);
	
// 	glcd_gotoxy(0, 54);
// 	glcd_putsf("vec ",0);
// 	glcd_puts(itos(SPMstatic_getchosenvec()),0);
	
	if(get_esc_button())
	{
		active_screen = IDF_SCREEN_MAIN;
	}
	
	drop_all_buttons();
}

void torque_screen_play(void)
{
	glcd_clear();
	glcd_gotoxy(0,0);

	glcd_putsf("torques",0);
	
	glcd_gotoxy(0, 9);
	glcd_putsf("Inc ",0);
	int64_t tmp_theta = enc_get_lin_theta();
	uint32_t tmp_phs = (enc_get_phase() * 1000) / FIXEDPT_ONE;
	if (tmp_theta < 0)
	{
		tmp_theta += 1;
		if (tmp_theta == 0) putIt('-');
		tmp_phs = 1000 - tmp_phs;
	}
	glcd_puts(sitos(tmp_theta),0);
	
	putIt('.');
	if(tmp_phs < 10) putIt('0');
	if(tmp_phs < 100) putIt('0');
	glcd_puts(itos(tmp_phs),0);
	

	glcd_gotoxy(0, 18);
	glcd_putsf("Rev ",0);
	//	glcd_puts(fixedpt_cstr(fixedpt_abs(enc_get_omega_shaft_rad_hat()) * FIXEDPT_ONE, 8),0);
	
	glcd_puts(fixedpt_cstr(enc_get_omega_hz_hat(MOTOR_POLEPAIR), 8),0);

	glcd_gotoxy(0, 27);
// 	glcd_putsf("SPT ",0);
// 	glcd_puts(fixedpt_cstr(fixedpt_rconst(get_spt()), 4),0);
	
	glcd_putsf("Acc ",0);
	glcd_puts(fixedpt_cstr(enc_get_acc_shaft_rad_hat() * FIXEDPT_ONE, 4),0);

	
	glcd_gotoxy(0, 36);
	glcd_putsf("PZS ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(get_pzs()), 4),0);
	
	glcd_gotoxy(0, 45);
	glcd_putsf("LT ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(get_loadtorque()), 4),0);

	glcd_gotoxy(0, 54);
	glcd_putsf("PIT ",0);
	glcd_puts(fixedpt_cstr(fixedpt_rconst(get_pit()), 4),0);

	
	if(get_esc_button())
	{
		active_screen = IDF_SCREEN_MAIN;
	}
	
	drop_all_buttons();
}