/*
 * VF.h
 *
 * Created: 31/01/2017 04:59:49 م
 *  Author: ahmed
 */ 


#ifndef VF_H_
#define VF_H_

#include <fixedptc.h>
#include <drive_interface_data.h>

#define VM				(menue_drive_data_applied.m_vm)
#define FM				(menue_drive_data_applied.m_fm)

#define VMIN			(menue_drive_data_applied.m_vmin)
#define FMIN			(menue_drive_data_applied.m_fmin)

#define TBOOST			(menue_drive_data_applied.m_tboost)

#define BREAK_FREQ		(menue_drive_data_applied.m_break_freq)
#define BREAK_VOLTAGE	(menue_drive_data_applied.m_break_voltage)

#define sl1 (fixedpt_div(VM - VMIN, FM - FMIN))
#define sl2 (fixedpt_div(vbase - VM, fbase - FM))





void vf_speed_out_handler(fixedpt speed, uint32_t breakactive);

#endif /* VF_H_ */