/*
 * SPT.h
 *
 * Created: 14/04/2020 09:01:50 ص
 *  Author: ahmed
 */ 


#ifndef SPT_H_
#define SPT_H_

#include <asf.h>

void spt_init(float r_torque);
uint8_t spt_permission_brake_open(void);
void spt_vfd_set_ready_to_open_brake(void);
float spt_perform(void);

#endif /* SPT_H_ */