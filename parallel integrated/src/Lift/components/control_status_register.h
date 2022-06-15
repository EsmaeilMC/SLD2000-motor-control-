/*
 * control_status_register.h
 *
 * Created: 20/11/2015 06:23:22 ص
 *  Author: ahmed
 */ 


#ifndef CONTROL_STATUS_REGISTER_H_
#define CONTROL_STATUS_REGISTER_H_

#include <e_setup.h>
#include <menue_data_manager.h>

extern  uByte_t heart_beat_flasher_index ; //to hold the index of flasher handler in the schedular
#define set_heart_beat_flasher_rate(f_period) sch_edit_task_period_ms(heart_beat_flasher_index,f_period)
#define set_stop_flash_rate set_heart_beat_flasher_rate(1000)
#define set_cam_flash_rate set_heart_beat_flasher_rate(750)
#define set_fast_flash_rate set_heart_beat_flasher_rate(250)
#define set_slow_flash_rate set_heart_beat_flasher_rate(500)

#define msk(x) (1ul << x)

#define public_lang 0
#define manual_door (menue_lift_data_applied.m_door_type == 0)
#define external_auto (menue_lift_data_applied.m_door_type == 2 || menue_lift_data_applied.m_door_type == 3)
#define door_auto_close (menue_lift_data_applied.m_door_type == 2)
#define door_auto_open (menue_lift_data_applied.m_door_type == 3)
#define door_internal_auto (menue_lift_data_applied.m_door_type == 1)
#define door_is_auto (menue_lift_data_applied.m_door_type != 0)

extern char direction_up;

#endif /* CONTROL STATUS REGISTER_H_ */