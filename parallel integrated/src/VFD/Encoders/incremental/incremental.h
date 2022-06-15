/*
 * incremental.h
 *
 * Created: 24/03/2017 06:58:38 م
 *  Author: ahmed
 */ 


#ifndef INCREMENTAL_H_
#define INCREMENTAL_H_


void encoder_inc_handler(void);

void inc_init();

int64_t enc_get_theta(void);
int64_t enc_get_lin_theta(void);
int64_t enc_get_theta_hat(void);
fixedpt enc_get_omega_hat(void);
fixedpt enc_get_omega_rev_hat(void);
float enc_get_omega_shaft_rad_hat(void);
float enc_get_acc_shaft_rad_hat(void);
fixedpt enc_get_omega_hz_hat(uint32_t poles);
fixedpt enc_get_angle_deg(uint32_t poles);
fixedpt enc_get_omega_rad_hat(uint32_t poles);
void sincos_store_val(void);
void sincos_store_phs_adc_val(uint16_t *adc_reading);
fixedpt enc_get_phase (void);
void enc_set_theta(int64_t pos);
void enc_set_filter_timeconst(float t_cons);

fixedpt enc_get_omega_rev_hat_d(void);
fixedpt enc_get_omega_hz_hat_d(uint32_t poles);
fixedpt enc_get_omega_rad_hat_d(uint32_t poles);
#endif /* INCREMENTAL_H_ */