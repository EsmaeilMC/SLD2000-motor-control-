/*
 * sph_faults_history.c
 *
 * Created: 07/06/2017 03:27:43 م
 *  Author: ahmed
 */ 
#include <faults.h>
#include <menue_data_manager.h>
#include <AT24Cxx_dynamic.h>
#include <floor_counter.h>
#include <vfd.h>
#include <SEDACIO.h>
#include <auto_state.h>
#include <vfdstates.h>

fault_history_t faults_history_list[SPH_FAULT_COUNT];
uint8_t fault_history_newDataFlag[SPH_FAULT_COUNT];
uint32_t fault_history_entry_pointer;
void fault_history_init()
{
	uint32_t i;
	fault_history_entry_pointer = 0xFF;
	for (i=0; i<SPH_FAULT_COUNT; i++)
	{
		//set newDataFlag to 0;
		fault_history_newDataFlag[i] = 0;
		faults_history_list[i].next_f_ptr = 0xFF; //set pointer to null
		faults_history_list[i].prev_f_ptr = 0xFF; //set pointer to null
		faults_history_list[i].occurance_count = 0; //set count to zero
	}
}

void fault_history_eraseAll()
{
	uint32_t i;
	fault_history_entry_pointer = 0xFF;
	for (i=0; i<SPH_FAULT_COUNT; i++)
	{
		//set newDataFlag to 1;
		fault_history_newDataFlag[i] = 1;
		faults_history_list[i].next_f_ptr = 0xFF; //set pointer to null
		faults_history_list[i].prev_f_ptr = 0xFF; //set pointer to null
		faults_history_list[i].occurance_count = 0;
	}
}

void fault_history_saveAll()
{
	uint32_t i;
	//return;
	for (i=0; i<SPH_FAULT_COUNT; i++)
	{
		//set newDataFlag to 0;
		fault_history_newDataFlag[i] = 0;
	}
	//save fault history to EEPROM
	at24cxx_dynamic_sch_write_array(0, EEPROM_MEN_SAVE_END_ADD, sizeof(fault_history_entry_pointer), &fault_history_entry_pointer);
	at24cxx_dynamic_sch_write_array(0, EEPROM_MEN_SAVE_END_ADD + sizeof(fault_history_entry_pointer), sizeof(fault_history_t) * SPH_FAULT_COUNT, faults_history_list);
}

void fault_history_eraseAndSaveAll()
{
	fault_history_eraseAll();
	fault_history_saveAll();
}

void fault_history_loadAll()
{
	//load fault history to RAM
 	at24cxx_dynamic_sch_read_array(0, EEPROM_MEN_SAVE_END_ADD, sizeof(fault_history_entry_pointer), &fault_history_entry_pointer);
 	at24cxx_dynamic_sch_read_array(0, EEPROM_MEN_SAVE_END_ADD + sizeof(fault_history_entry_pointer), sizeof(fault_history_t) * SPH_FAULT_COUNT, faults_history_list);
}

void fault_history_setFault(uint8_t faultId)
{
	uint32_t year, month, day, hour, minute;
	if (faultId < SPH_FAULT_COUNT)
	{
		//we are within the faults range
		
		if (faultId != fault_history_entry_pointer)
		{
			//faultId != entry fult pointer
			if (faults_history_list[faultId].prev_f_ptr < SPH_FAULT_COUNT)
			{
				//this fault points to another chained fault
				//so set it's next fault pointer to this fault's next pointer
				faults_history_list[faults_history_list[faultId].prev_f_ptr].next_f_ptr = faults_history_list[faultId].next_f_ptr;
			}
		
			if (faults_history_list[faultId].next_f_ptr < SPH_FAULT_COUNT)
			{
				//this fault points to another chained fault
				//so set it's prev fault pointer to this fauilt's prev pointer
				faults_history_list[faults_history_list[faultId].next_f_ptr].prev_f_ptr = faults_history_list[faultId].prev_f_ptr;
			}
			//set faultId.pointer to point to entrypointer
			faults_history_list[faultId].next_f_ptr = fault_history_entry_pointer;
			faults_history_list[faultId].prev_f_ptr = 0xFF;
			if (fault_history_entry_pointer < SPH_FAULT_COUNT)
			{
				//set fault_history_entry_pointer.pointer to point to faultId
				faults_history_list[fault_history_entry_pointer].prev_f_ptr = faultId;
			}
		
			//set faultId.prevPointer to point to Null
			faults_history_list[faultId].prev_f_ptr = 0xFF;
			//set entrypoint to point to faultId
			fault_history_entry_pointer = faultId;
		}

		
		if (faults_history_list[faultId].next_f_ptr == faultId)
		{
			//if fault_history_entry_pointer originally points to faultId make next pointer point to nothing
			//to prevent infinite loop
			faults_history_list[faultId].next_f_ptr = 0xFF;
		} 
		
		
		
		//increment faultId.ocurrance count
		faults_history_list[faultId].occurance_count++;
		
		// save last occurance time and date
		rtc_get_date(RTC, &year, &month, &day, 0);
		rtc_get_time(RTC, &hour, &minute, 0);
		faults_history_list[faultId].f_day = day;
		faults_history_list[faultId].f_month = month;
		faults_history_list[faultId].f_hour = hour;
		faults_history_list[faultId].f_minute = minute;
		if (year > 2012) year -= 2012;
		else year = 0;
		faults_history_list[faultId].f_year = year;
		faults_history_list[faultId].floor = get_current_floor() & 0x7F;
		if(mstop == 0) faults_history_list[faultId].floor |= 0x80;
		faults_history_list[faultId].states = vfdstates_get_d_state();
		faults_history_list[faultId].id = cid;
		faults_history_list[faultId].iq = ciq;
		fault_history_saveAll();
	}
}

uint8_t current_fault_view;
fault_history_t *get_first_fault(uint8_t *fault_ptr, uint8_t *fault_indx)
{
	current_fault_view = 0xff;
	*fault_indx = 0;
	if (fault_history_entry_pointer >= SPH_FAULT_COUNT)
	{
		return 0;
	} 
	else
	{
		*fault_ptr = fault_history_entry_pointer;
		current_fault_view = fault_history_entry_pointer;
		return &faults_history_list[current_fault_view];
	}
}

fault_history_t *get_next_fault(uint8_t *fault_ptr, uint8_t *fault_indx)
{
	if (current_fault_view >= SPH_FAULT_COUNT)
	{
		return 0;
	}
	else
	{
		if (faults_history_list[current_fault_view].next_f_ptr >= SPH_FAULT_COUNT)
		{
			// no next fault to view
			return &faults_history_list[current_fault_view];
		}
		(*fault_indx)++;
		current_fault_view = faults_history_list[current_fault_view].next_f_ptr;
		*fault_ptr = current_fault_view;
		return &faults_history_list[current_fault_view];
	}
}

fault_history_t *get_previous_fault(uint8_t *fault_ptr, uint8_t *fault_indx)
{
	if (current_fault_view >= SPH_FAULT_COUNT)
	{
		return 0;
	}
	else
	{
		if (faults_history_list[current_fault_view].prev_f_ptr >= SPH_FAULT_COUNT)
		{
			// no next fault to view
			return &faults_history_list[current_fault_view];
		}
		if(*fault_indx > 0) (*fault_indx)--;
		current_fault_view = faults_history_list[current_fault_view].prev_f_ptr;
		*fault_ptr = current_fault_view;
		return &faults_history_list[current_fault_view];
	}
}