/*
 * e_setup.c
 *
 * Created: 20/11/2015 11:57:18 ص
 *  Author: ahmed
 */
#include <e_setup.h>
#include <timers.h>
#include <elevator_controller.h>




char d_setup[32]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
char cam_t=4,wait_t=4,light_t=7,d_type=0,s2dlta=3;
char e_hyd=0,ph_s=0,eep_slwtmr=4,eep_fsttmr=5;
char cam_trils=2,maintenance_speed=0,p1function=0,mtron=0,cpasson=0;
unsigned int floor_dis=0xffff;
char max_req_count=0,req_pins=0,prk_floor=0,prk_tmr=0,fire_floor=0;
char password[]={0,0,0,0,0};
char cpasseep[]={0,1};
char stele[]={0,1,0,6,8,9,5,5,0,9,9};
char to_type=0,eep_poserr=0,eon7sd=0,ee_lang=0;
char error_history[]={0x0,0x0,0x0,0x0,0,0,0,0,0,0,0,0,0,0,0,0,0x0,0x0,0x0,0x0,0,0,0,0,0,0,0,0,0,0,0,0};
char serv=0;
unsigned int s_n[3];

