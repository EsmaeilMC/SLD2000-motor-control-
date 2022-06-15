/*
 * EMCM.h
 *
 * Created: 13/07/2018 07:25:34 م
 *  Author: ahmed
 */ 


#ifndef EMCM_H_
#define EMCM_H_

#include <asf.h>

//EMCM stands for encoder motion control module

void EMCM_handler(void);
uint32_t set_Hspeed_profile(void);
uint32_t exit_hspd(void);
uint32_t exit_lspd(void);
void EMCM_decel_point_watch(void);
int8_t EMCM_get_travel_speed_sign(void);
int32_t EMCM_get_enc_flash(void);
int32_t EMCM_get_floorErr(void);
void upl0decel(void);
void dnl0decel(void);
void mstp_met(void);
void set_correct_atStop(void);
void set_Stop_afterCorrection(void);

#endif /* EMCM_H_ */