/*
 * e_faults.h
 *
 * Created: 20/11/2015 02:00:13 م
 *  Author: ahmed
 */ 


#ifndef E_FAULTS_H_
#define E_FAULTS_H_

#include <def.h>
#include <asf.h>
#include <sph_faults.h>

#define pos_errmax  4
#define errmax      5

#define err_max     32

#define  pos     0
#define  postmr  1
#define  errtmr  2
#define  errtf   3

extern uint_t postmrerr[];
extern uint_t tmrerr[];
extern uint_t err,poserr;


uByte_t check_err(uByte_t etype);



//________________________________________________

#define  fastset        poserr|=1

#define  fastpre        ((poserr&1)==1)
#define  fireset        poserr|=2;\
if((err3&0x2)==0){\
	save_err(3,1);\
err3|=0x2;}
#define  fire_present   ((poserr&2)==2)
#define  dlmtset        poserr|=4

#define  dlmtset_eep    eep_poserr|=4
//________________________________________________

#define  fastsetsi      err|=1



						

#define  slowpre        ((err&2)==2)
//__________________________________________________


uint8_t s_i_err(void);

extern uint8_t fault_ph_fail_idx, fault_e_stop_idx, fault_lock_idx, fault_fork_idx;

//#define  ph_fail        sph_faults_setfault(fault_ph_fail_idx)


//#define  ph_fail_pres   (tmrerr[1]>0)

#define  e_stop_set  sph_faults_setfault(fault_e_stop_idx)

//#define  e_stop_pres    (tmrerr[2]>0)


#define  lock_err_set   sph_faults_setfault(fault_lock_idx)
#define  shk_err_set    sph_faults_setfault(fault_fork_idx)
//________________________________________________

extern uint8_t fault_slow_idx, fault_fast_idx;

#define  slowset        sph_faults_setfault(fault_slow_idx)
#define  fastset        sph_faults_setfault(fault_fast_idx)

//////////////////////////////Faults and Direction limits////////////////////////////////////////////
extern uint8_t fault_dnfdl0_err_idx, fault_upfdl0_err_idx, fault_dnfl_err_idx, fault_upfl_err_idx, fault_fl_err_idx;
#define  dnfdl0_err						sph_faults_setfault(fault_dnfdl0_err_idx)
#define  upfdl0_err						sph_faults_setfault(fault_upfdl0_err_idx)
#define  dnfl_err						sph_faults_setfault(fault_dnfl_err_idx)
#define  upfl_err						sph_faults_setfault(fault_upfl_err_idx)
#define  Final_limit_err				sph_faults_setfault(fault_fl_err_idx)

//////////////////////////////Faults of Direct stop////////////////////////////////////////////
extern uint8_t fault_LRN_err_idx, fault_LRN_req_err_idx;
#define  LRN_err_set			sph_faults_setfault(fault_LRN_err_idx)
#define  LRN_req_err_set        sph_faults_setfault(fault_LRN_req_err_idx)
//////////////////////////////Faults of serial comm////////////////////////////////////////////
extern uint8_t fault_ctb_idx, fault_ccb0_idx, fault_ccb1_idx;
#define  CTB_err_set			sph_faults_setfault(fault_ctb_idx)
#define  CCB0_err_set        sph_faults_setfault(fault_ccb0_idx)
#define  CCB1_err_set			sph_faults_setfault(fault_ccb1_idx)
//////////////////////////////Faults of door machines////////////////////////////////////////////
extern uint8_t fault_fdoorab_idx, fault_bdoorab_idx;
#define  fdoor_ab_err_set			sph_faults_setfault(fault_fdoorab_idx)
#define  bdoor_ab_err_set        sph_faults_setfault(fault_bdoorab_idx)

void e_faults_inti(void);

#endif /* E_FAULTS_H_ */