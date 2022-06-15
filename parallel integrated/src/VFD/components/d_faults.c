/*
 * d_faults.c
 *
 * Created: 25/05/2017 02:51:03 م
 *  Author: ahmed
 */ 

#include <d_faults.h>

uint8_t fault_ov_idx, fault_uv_idx, fault_oc_idx, fault_ol_idx, fault_sc_idx, fault_ot_idx, fault_en_idx, 
		fault_phfail_idx, fault_endat_idx, fault_controlNotSupportted, fault_speedDev_idx, fault_uv_ard_idx,
		fault_zs_servo_idx, fault_tune1_idx, fault_oc_idx, fault_brk_idx, fault_uv_ard_idx, fault_sincos_err_idx,
		ofault_phfail_idx, sleep_f_idx, rswdrst_f_idx, wdrst_f_idx, init_fault, mCFb_f_idx, bCFb_f_idx, FxCFb_f_idx,
		fault_accoc_idx, fault_decoc_idx, fault_dis_dev_idx;

void d_faults_init(void)
{
	fault_ov_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 1, 1, "Over voltage");
	fault_uv_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 1, 2, "Under voltage");
	fault_oc_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 1, 3, "Over current");
	fault_ol_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 1, 4, "Overload");
	fault_sc_idx = sph_faults_add_fault(STOP_IMMEDIATE, 5000, 1, 5, "output short circuit");
	fault_ot_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 1, 6, "IGBT over temperature");
	fault_en_idx = sph_faults_add_fault(STOP_IMMEDIATE, 5000, 1, 7, "Drive En error");
	fault_phfail_idx = sph_faults_add_fault(T_POS_FAULT, 5000, 1, 8, "Phase fail");
	ofault_phfail_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 1, 9, "Motor Phase fail");
	fault_endat_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 800, 1, 10, "PG COMM");
	fault_controlNotSupportted = sph_faults_add_fault(T_STOP_IMMEDIATE, 1000, 1, 11, "Control mode not supp");
	fault_speedDev_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 8000, 1, 12, "Speed deviation");
	fault_uv_ard_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 10000, 1, 13, "ARD Under voltage");
	fault_zs_servo_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 4000, 1, 14, "ZS servo out of range");
	fault_tune1_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 10000, 1, 15, "Tune1 Error");
	//fault_oc_idx = sph_faults_add_fault(STOP_IMMEDIATE, 10000, "Over Current");
	fault_brk_idx = sph_faults_add_fault(STOP_IMMEDIATE, 10000, 1, 16, "Brake short ct");
	fault_uv_ard_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 8000, 1, 17, "Under ARD voltage");
	fault_sincos_err_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 8000, 1, 18, "SinCos sig missing");
	sleep_f_idx = sph_faults_add_fault(STOP_IMMEDIATE, 8000, 1, 19, "Reset needed");
	wdrst_f_idx  = sph_faults_add_fault(STOP_IMMEDIATE, 8000, 1, 20, "WDReset");
	rswdrst_f_idx = sph_faults_add_fault(STOP_IMMEDIATE, 8000, 1, 21, "RSWDReset"); 
	init_fault = sph_faults_add_fault(STOP_IMMEDIATE, 8000, 1, 22, "init fault");
	mCFb_f_idx  = sph_faults_add_fault(T_STOP_IMMEDIATE, 8000, 1, 23, "MOC FB fault");
	bCFb_f_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 8000, 1, 24, "Brk FB fault");
	FxCFb_f_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 50000, 1, 25, "Fx FB fault");
	fault_accoc_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 1, 26, "Acc OverCurrent");
	fault_decoc_idx = sph_faults_add_fault(T_STOP_IMMEDIATE, 5000, 1, 27, "Dec OverCurrent");
	fault_dis_dev_idx = sph_faults_add_fault(T_POS_FAULT, 5000, 1, 28, "Distance dev");
}