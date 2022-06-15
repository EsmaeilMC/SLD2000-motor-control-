/*
 * auto_state.h
 *
 * Created: 19/11/2015 03:09:22 م
 *  Author: Ahmed
 */ 



#ifndef AUTO_STATE_H_
#define AUTO_STATE_H_

#include <asf.h>
#include <def.h>
#include <elevator_controller.h>

#define auto_idle                5
#define auto_hspd                1
#define auto_lspd                2
#define auto_stop                3
#define auto_camtest             4
#define auto_stopping            7
#define auto_checkInvReady       6
#define auto_prespd              8

extern unsigned int fast_cntr, fast_cntr_pos;
#define FAST_TIME_OUT (menue_lift_data_applied.m_fast_time * AUTO_STATE_UPDATE_SEC)

void auto_enter(void);
uByte_t auto_instate(uByte_t state);

void auto_update(void);
void state_check(void);
void auto_update_elevator_initilal_values(void);

//Stopping state subs
void auto_stopping_enter(void);
void auto_stopping_exe(void);
void auto_stopping_exit(void);

//Idle Chekc inverter ready subs
void auto_checkInvReady_enter(void);
void auto_checkInvReady_exe(void);
void auto_checkInvReady_exit(void);

//Idle state subs
void auto_idle_enter(void);
void auto_idle_exe(void);
void auto_idle_exit(void);

//hspd state subs
void auto_hspd_enter(void);
void auto_hspd_exe(void);
void auto_hspd_exit(void);

//lspd state subs
void auto_lspd_enter(void);
void auto_lspd_exe(void);
void auto_lspd_exit(void);

//stop state subs
void auto_stop_enter(void);
void auto_stop_exe(void);
void auto_stop_exit(void);

//Cam state subs
void auto_camtest_enter(void);
void auto_camtest_exe(void);
void auto_camtest_exit(void);

void auto_lspd_creep(void);

void auto_prespd_enter(void);
void auto_prespd_exe(void);
void auto_prespd_exit(void);

void auto_set_mstp_met(void);

uint8_t auto_get_e_state(void);

void auto_reopen_door(void);

#define HIGH_SPEED (fixedpt_mul(D_MOTOR_SPEED_HZ, menue_lift_data_applied.m_high_speed) / 100)
#define CREEP_SPEED (fixedpt_mul(D_MOTOR_SPEED_HZ, menue_lift_data_applied.m_creep_speed) / 100)
#define RELEVEL_SPEED (fixedpt_mul(D_MOTOR_SPEED_HZ, menue_lift_data_applied.m_relevel_speed) / 100)
#define INS_SPEED (fixedpt_mul(D_MOTOR_SPEED_HZ, menue_lift_data_applied.m_ins_speed) / 100)

#endif /* AUTO STATE_H_ */