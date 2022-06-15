/*
 * e_faults.c
 *
 * Created: 20/11/2015 01:58:47 م
 *  Author: ahmed
 */ 
#include <e_faults.h>

#define err_preset 200

uint_t postmrerr[pos_errmax];
uint_t tmrerr[errmax];
uint_t err=0,poserr=0;



uByte_t check_err(uByte_t etype)
{
	char i;
	switch (etype)
	{
		case postmr:
		for(i=0;i<pos_errmax;i++)
		{
			if(postmrerr[i]>0)
			return 1;
		}
		break;
		case errtmr:
		for(i=0;i<errmax;i++)
		{
			if(tmrerr[i]>0)
			return 1;
		}
		break;
		case pos:
		if(poserr!=0)
		return 1;
		break;
		case errtf:
		if(err!=0)
		return 1;
		break;
	}
	return 0;
}



//_________________________________________________________
//replacement
uint8_t fault_slow_idx, fault_fast_idx;
uint8_t fault_ph_fail_idx, fault_e_stop_idx, fault_lock_idx, fault_fork_idx;
uint8_t fault_dnfdl0_err_idx, fault_upfdl0_err_idx, fault_dnfl_err_idx, fault_upfl_err_idx, fault_fl_err_idx;
uint8_t fault_LRN_err_idx, fault_LRN_req_err_idx;
uint8_t fault_ctb_idx, fault_ccb0_idx, fault_ccb1_idx;
uint8_t fault_fdoorab_idx, fault_bdoorab_idx;
void e_faults_inti(void)
{
	fault_ph_fail_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 1, "Phase failure");
	fault_e_stop_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 2, "Emergency stop");
	fault_lock_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 3, "Lock fail");
	fault_fork_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 4, "Fork cont fail");
//______________________________________
	fault_fast_idx = sph_faults_add_fault(T_POS_FAULT, 5000, 2, 5, "Fast timeout");
	fault_slow_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 6, "Slow timeout");
//////////////////////////////////////////////////////////////////////////
	fault_dnfdl0_err_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 7, "Down fd limit0");
	fault_upfdl0_err_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 8, "Up fd limit0");
	fault_dnfl_err_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 14, "Down final lim");
	fault_upfl_err_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 15, "Up final lim");
	fault_fl_err_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 2, 16, "Final limits check");
//////////////////////////////////////////////////////////////////////////
	fault_LRN_err_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 10000, 2, 9, "Hoist LRN error");
	fault_LRN_req_err_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 6000, 2, 10, "HoistWay LRN required");
//////////////////////////////////////////////////////////////////////////	
	fault_ctb_idx = sph_faults_add_fault(T_POS_FAULT, 2500, 2, 9, "CTB Comm loss");
	fault_ccb0_idx = sph_faults_add_fault(T_POS_FAULT, 2500, 2, 10, "CCB0 Comm loss");
	fault_ccb1_idx = sph_faults_add_fault(T_POS_FAULT, 2500, 2, 11, "CCB1 Comm loss");
//////////////////////////////////////////////////////////////////////////	
	fault_fdoorab_idx = sph_faults_add_fault(T_POS_FAULT, 5000, 2, 12, "ab Front door");
	fault_bdoorab_idx = sph_faults_add_fault(T_POS_FAULT, 5000, 2, 13, "ab back door");

}

uint8_t s_i_err(void)
{
	return sph_faults_sie_check();
}
