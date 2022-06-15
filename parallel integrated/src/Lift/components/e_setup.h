/*
 * e_setup.h
 *
 * Created: 20/11/2015 05:48:53 ص
 *  Author: ahmed
 */ 


#ifndef E_SETUP_H_
#define E_SETUP_H_

#include <menue_data_manager.h>

#define f_max (menue_lift_data_applied.m_floor_count)

extern char d_setup[];
extern char wait_t,s2dlta;
extern char e_hyd,ph_s,collective_opt;
extern char maintenance_speed,p1function,mtron,cpasson;
extern unsigned int floor_dis;
extern char max_req_count,req_pins,prk_floor,prk_tmr,fire_floor;
extern char password[];
extern char cpasseep[];
extern char stele[];
extern char to_type,eep_poserr,eon7sd,ee_lang;
extern char error_history[];
extern char serv;
extern unsigned int s_n[3];

void init_elevator_values(void);

#endif /* E SETUP_H_ */