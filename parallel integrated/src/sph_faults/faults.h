/*
 * faults.h
 *
 * Created: 25/05/2017 06:48:17 ص
 *  Author: ahmed
 */ 


#ifndef FAULTS_H_
#define FAULTS_H_

#ifndef SPH_FAULT_COUNT
#define SPH_FAULT_COUNT 45
#endif

#define SPH_FAULT_DISPLAY_TIMEOUT 3000
#include <asf.h>
#include <sph_faults_history.h>

typedef enum {STOP_IMMEDIATE, T_STOP_IMMEDIATE, POS_FAULT, T_POS_FAULT} fault_type_t;

void sph_faults_handler(void);
void sph_faults_init(void);
uint32_t sph_faults_add_fault(fault_type_t fault_type, uint32_t fault_timeout, uint8_t fault_prefix, uint8_t fault_code, const char *e_string);
void sph_faults_setfault(uint32_t fault_index);
uint32_t sph_faults_sie_check(void);
uint32_t sph_faults_pose_check(void);
uint8_t dont_record_fault();

uint32_t sph_faults_get_fault_to_display(const char **str, char *fault_prefix, char *fault_code);
uint32_t sph_faults_is_present(uint32_t fault_index);
const char *sph_faults_get_fault_string(uint8_t fault_id);
#endif /* FAULTS_H_ */