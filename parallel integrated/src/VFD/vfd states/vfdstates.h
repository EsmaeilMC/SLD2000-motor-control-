/*
 * vfdstates.h
 *
 * Created: 05/03/2017 07:47:36 م
 *  Author: ahmed
 */ 


#ifndef VFDSTATES_H_
#define VFDSTATES_H_


void vfd_states_handler(void);
unsigned int vfd_states_allowedtomove(void);
void vfd_states_estop(void);
void vfd_states_trip(void);
uint8_t vfd_states_isStopping(void);
unsigned int vfd_states_isTuning(void);

unsigned int vfd_states_is_sctest_mode(void);
void scCheck_init(void);
void vfd_states_set_softcharge_mode(uint32_t mode);
unsigned int vfd_states_initialized(void);

uint8_t vfdstates_get_d_state(void);

#endif /* VFDSTATES_H_ */