/*
 * mb_states.h
 *
 * Created: 12/05/2017 06:02:54 م
 *  Author: ahmed
 */ 


#ifndef MB_STATES_H_
#define MB_STATES_H_

#include <asf.h>

void mb_states_master_waiting_for_reply_handler(void);
void mb_states_master_process_reply_handler(void);
void mb_states_master_process_noreply_handler(void);
void mb_states_master_delay_to_ready(void);

#endif /* MB_STATES_H_ */