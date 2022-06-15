/*
 * Iinverter.h
 *
 * Created: 31/01/2017 04:07:27 م
 *  Author: ahmed
 */ 


#ifndef IINVERTER_H_
#define IINVERTER_H_

#include <fixedptc.h>

#define PM_VOLTAGE_DROP (fixedpt_rconst(2.5))
fixedpt dc_bus_v_reading;
extern float i_readout[3];
#define dc_bus_voltage dc_bus_v_f_reading//iverter_get_dc_v()

extern char demand_sw_reset_oc;
void inverter_fan_timer_init(void);
void inverter_fan_timer_reset(void);
void inverter_out_polar(fixedpt alfa, fixedpt volts);
void inverter_out_rect(fixedpt alfa, fixedpt beta);
void inverter_out_single_axis(float volts);
void inverter_out_single_axis_b(float volts);
void inverter_out_single_axis_c(float volts);
void inverter_out_0(void);
void inverter_out_7(void);

void inverter_update_dcbus_val(uint16_t adc_reading);
void inverter_check_dcbus_val_handler(void);
void inverter_rewind_dcbus_f_val(void );
void inverter_update_currents_val(uint16_t *adc_reading);

void inverter_ignore_uv (uint8_t ignore);

void inverter_set_dead_comp(uint32_t val);

void inverter_set_override(void);
void inverter_clear_override(void);
//void inverter_set_brake_duty(uint32_t b_duty);
void inverter_brake_chopper_handler(void);

void inverter_get_creadings(float* ria, float* rib, float* ric);
int32_t inverter_get_current_sign(uint32_t axis);

void IGBT_temp_handler(void);

extern fixedpt NTC_readout ;
fixedpt iverter_get_igbt_temp(void);
fixedpt iverter_get_dc_v(void);
void inverter_set_manual_dcVal(fixedpt dc_val);

uint32_t inverter_getADCcurrent(void);
fixedpt iverter_get_dc_v2(void);
void inverter_init_dc_filter(void);
#endif /* IINVERTER_H_ */