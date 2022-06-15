/*
 * menue_data_manager.c
 *
 * Created: 29/04/2017 10:45:16 ص
 *  Author: ahmed
 */ 
#include <menue_data_manager.h>
#include <AT24Cxx_dynamic.h>
#include <HoistInfo.h>
#include <pop_ups.h>

uint8_t counter_info[9];

void menue_data_manager_update(void)
{
	menue_data_manager_lift_update();
	menue_data_manager_drive_update();
}

void menue_data_manager_CounterDataLoad (void)
{	
	at24cxx_dynamic_sch_read_array(0, 0, 9, &counter_info[0]);
}

uint8_t* menue_data_manager_get_CounterData (void)
{
	return &counter_info[0];
}
void menue_data_manager_load (void)
{
	menue_data_manager_lift_Hinfo_load(EEPROM_MEM_HINFO_SAVE_OFFSET);
	uint32_t men_data_add = menue_data_manager_lift_load(EEPROM_MEM_SAVE_OFFSET);
	men_data_add = men_data_add = menue_data_manager_drive_load(men_data_add);
	menue_data_manager_CounterDataLoad();
	//men_data_add++;
}

void menue_data_manager_save (void)
{
	menue_data_manager_lift_Hinfo_save(EEPROM_MEM_HINFO_SAVE_OFFSET);
	uint32_t men_data_add = menue_data_manager_lift_save(EEPROM_MEM_SAVE_OFFSET);
	men_data_add = menue_data_manager_drive_save(men_data_add);
	pop_ups_add("Data saved", 1000);
}

void menue_data_manager_hoist_save(void)
{
	menue_data_manager_lift_Hinfo_save(EEPROM_MEM_HINFO_SAVE_OFFSET);
	pop_ups_add("Hoist data saved!", 1000);
}

void menue_data_manager_saveLift (void)
{
	uint32_t men_data_add = menue_data_manager_lift_save(EEPROM_MEM_SAVE_OFFSET);
	pop_ups_add("Lift data saved!", 1000);
}

void menue_data_manager_saveCounterData (uint8_t Selector_data, int32_t encoder_value, uint32_t s_counter)
{
	counter_info[0] = Selector_data;
	int32_t *enc_pointer = (int32_t)&counter_info[1];
	uint32_t *p_s_counter = (uint32_t)&counter_info[5];
	*enc_pointer = encoder_value;
	*p_s_counter = s_counter;
	at24cxx_dynamic_sch_write_array(0, 0, 9, &counter_info[0]);
}

void menue_data_manager_saveDrive (void)
{
	uint32_t men_data_add = menue_data_manager_lift_getSize(EEPROM_MEM_SAVE_OFFSET);
	men_data_add = menue_data_manager_drive_save(men_data_add);
	pop_ups_add("Drive data saved!", 1000);
}

void menue_data_manager_restore (void)
{
	menue_data_manager_lift_restore();
	menue_data_manager_drive_restore();
	Hoist_init();
	pop_ups_add("Factory set restored!", 1000);
}