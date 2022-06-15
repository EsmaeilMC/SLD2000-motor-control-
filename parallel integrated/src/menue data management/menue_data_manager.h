/*
 * menue_data_manager.h
 *
 * Created: 29/04/2017 10:45:28 ص
 *  Author: ahmed
 */ 


#ifndef MENUE_DATA_MANAGER_H_
#define MENUE_DATA_MANAGER_H_

#include <menue_data_manager_lift.h>
#include <menue_data_manager_drive.h>
#include <HoistInfo.h>

void menue_data_manager_update(void);
#define EEPROM_MEM_SAVE_OFFSET (9 + sizeof(hoist_info_t))
#define EEPROM_MEM_HINFO_SAVE_OFFSET (9)
#define EEPROM_MEN_SAVE_END_ADD (EEPROM_MEM_SAVE_OFFSET + sizeof(menue_lift_data_t) + sizeof(menue_drive_data_t))

void menue_data_manager_load (void);
void menue_data_manager_save (void);
void menue_data_manager_restore (void);
void menue_data_manager_saveLift (void);
void menue_data_manager_saveDrive (void);
void menue_data_manager_hoist_save(void);
uint8_t* menue_data_manager_get_CounterData (void);
void menue_data_manager_saveCounterData (uint8_t Selector_data, int32_t encoder_value, uint32_t s_counter);

#endif /* MENUE_DATA_MANAGER_H_ */
