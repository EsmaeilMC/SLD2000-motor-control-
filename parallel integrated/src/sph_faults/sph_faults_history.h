/*
 * sph_faults_history.h
 *
 * Created: 07/06/2017 03:26:28 م
 *  Author: ahmed
 */ 


#ifndef SPH_FAULTS_HISTORY_H_
#define SPH_FAULTS_HISTORY_H_

#include <asf.h>
#include <faults.h>

typedef struct
{
	uint8_t   next_f_ptr;
	uint8_t   prev_f_ptr;
	uint8_t   f_minute;
	uint8_t   f_hour;
	uint8_t   f_day;
	uint8_t   f_month;
	uint8_t   f_year; 
	uint16_t  occurance_count;
	uint8_t states;
	uint8_t floor;
	float id;
	float iq;
}fault_history_t;

extern fault_history_t faults_history_list[SPH_FAULT_COUNT];
extern uint8_t fault_history_newDataFlag[SPH_FAULT_COUNT];


void fault_history_init();
void fault_history_eraseAll();
void fault_history_saveAll();
void fault_history_eraseAndSaveAll();
void fault_history_loadAll();
void fault_history_setFault(uint8_t faultId);

fault_history_t *get_first_fault(uint8_t *fault_ptr, uint8_t *fault_indx);
fault_history_t *get_next_fault(uint8_t *fault_ptr, uint8_t *fault_indx);

fault_history_t *get_previous_fault(uint8_t *fault_ptr, uint8_t *fault_indx);

#endif /* SPH_FAULTS_HISTORY_H_ */