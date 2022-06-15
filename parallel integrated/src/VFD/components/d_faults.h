/*
 * d_faults.h
 *
 * Created: 25/05/2017 02:50:54 م
 *  Author: ahmed
 */ 


#ifndef D_FAULTS_H_
#define D_FAULTS_H_

#include <asf.h>
#include <sph_faults.h>

extern uint8_t fault_ov_idx, fault_uv_idx, fault_oc_idx, fault_ol_idx, fault_sc_idx, fault_ot_idx, fault_en_idx, 
				fault_phfail_idx, fault_endat_idx, fault_controlNotSupportted, fault_speedDev_idx, fault_uv_ard_idx,
				fault_zs_servo_idx, fault_tune1_idx, fault_oc_idx, fault_brk_idx, fault_uv_ard_idx, fault_sincos_err_idx,
				ofault_phfail_idx, sleep_f_idx, rswdrst_f_idx, wdrst_f_idx, init_fault, mCFb_f_idx, bCFb_f_idx, FxCFb_f_idx,
				fault_accoc_idx, fault_decoc_idx, fault_dis_dev_idx;

#define  ov_err_set				sph_faults_setfault(fault_ov_idx)
#define  uv_err_set				sph_faults_setfault(fault_uv_idx)
#define  oc_err_set				sph_faults_setfault(fault_oc_idx)
#define  ol_err_set				sph_faults_setfault(fault_ol_idx)
#define  sc_err_set				sph_faults_setfault(fault_sc_idx)
#define  ot_err_set				sph_faults_setfault(fault_ot_idx)
#define  en_err_set				sph_faults_setfault(fault_en_idx)
#define  phfail_err_set			sph_faults_setfault(fault_phfail_idx)
#define  endat_err_set			sph_faults_setfault(fault_endat_idx)
#define  speedDev_set			sph_faults_setfault(fault_speedDev_idx)
#define  control_fault_set		sph_faults_setfault(fault_controlNotSupportted)
#define  uvARD_err_set			sph_faults_setfault(fault_uv_ard_idx)
#define  zs_servo_err_set			sph_faults_setfault(fault_zs_servo_idx)
#define  tune1_err_set			sph_faults_setfault(fault_tune1_idx)
#define  brk_err_set			sph_faults_setfault(fault_brk_idx)
#define  uv_ard_err_set				sph_faults_setfault(fault_uv_ard_idx)
#define  sincos_err_set				sph_faults_setfault(fault_sincos_err_idx)
#define  ophfail_err_set			sph_faults_setfault(ofault_phfail_idx)
#define  sleep_err_set			sph_faults_setfault(sleep_f_idx)
#define  wd_set			sph_faults_setfault(wdrst_f_idx)
#define  rswd_set			sph_faults_setfault(rswdrst_f_idx)
#define  init_f_set			sph_faults_setfault(init_fault)
#define  moc_err_set			sph_faults_setfault(mCFb_f_idx)
#define  bk_err_set			sph_faults_setfault(bCFb_f_idx)
#define  fx_err_set			sph_faults_setfault(FxCFb_f_idx)
#define  acc_oc_err_set				sph_faults_setfault(fault_accoc_idx)
#define  dec_oc_err_set				sph_faults_setfault(fault_decoc_idx)
#define  dev_dis_err_set				sph_faults_setfault(fault_dis_dev_idx)

#define  ov_err_pre   sph_faults_is_present(fault_ov_idx)
#define  uv_err_pre   sph_faults_is_present(fault_uv_idx)
#define  oc_err_pre   sph_faults_is_present(fault_oc_idx)
#define  ol_err_pre   sph_faults_is_present(fault_ol_idx)
#define  sc_err_pre   sph_faults_is_present(fault_sc_idx)
#define  ot_err_pre   sph_faults_is_present(fault_ot_idx)
#define  en_err_pre   sph_faults_is_present(fault_en_idx)
#define  endat_err_pre   sph_faults_is_present(fault_endat_idx)
#define  speedDev_pre   sph_faults_is_present(fault_speedDev_idx)
#define  control_fault_pre   sph_faults_is_present(fault_controlNotSupportted)
#define  uvARD_err_pre			sph_faults_is_present(fault_uv_ard_idx)
#define  zs_servo_err_pre			sph_faults_is_present(fault_zs_servo_idx)
#define  tune1_err_pre			sph_faults_is_present(fault_tune1_idx)
#define  brk_err_pre			sph_faults_is_present(fault_brk_idx)
#define  uv_ard_err_pre   sph_faults_is_present(fault_uv_ard_idx)
#define  sincos_err_pre   sph_faults_is_present(fault_sincos_err_idx)
#define  sleep_err_pre   sph_faults_is_present(sleep_f_idx)
#define  wd_pre   sph_faults_is_present(rswdrst_f_idx)
#define  rswd_pre   sph_faults_is_present(rswdrst_f_idx)
#define  init_f_pre			sph_faults_is_present(init_fault)
#define  moc_err_pre			sph_faults_is_present(mCFb_f_idx)
#define  bk_err_pre			sph_faults_is_present(bCFb_f_idx)
#define  fx_err_pre			sph_faults_is_present(FxCFb_f_idx)
#define  acc_oc_err_pre				sph_faults_is_present(fault_accoc_idx)
#define  dec_oc_err_pre				sph_faults_is_present(fault_decoc_idx)
#define  dev_dis_err_pre				sph_faults_is_present(fault_dis_dev_idx)

void d_faults_init(void);

#endif /* D_FAULTS_H_ */