/*
 * requests.h
 *
 * Created: 19/11/2015 04:33:35 م
 *  Author: ahmed
 */ 


#ifndef REQUESTS_H_
#define REQUESTS_H_

#include <def.h>
#include <asf.h>

extern ulong_t ireq,ereq,eereq;


void drop_all_req(void);
void drop_req(char req_num);
void req_scan(void);
void pos_next_floor();
void init_requests(uint32_t task_delay);
void req_enable(uint8_t command);
void insert_irequest(uint32_t request);
#endif /* REQUESTS_H_ */