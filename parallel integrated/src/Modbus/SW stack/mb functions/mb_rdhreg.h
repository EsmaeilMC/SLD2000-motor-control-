/*
 * mb_rdhreg.h
 *
 * Created: 11/05/2017 01:46:16 م
 *  Author: ahmed
 */ 


#ifndef MB_RDHREG_H_
#define MB_RDHREG_H_

#include <asf.h>
#include <mb_general.h>

void mb_read_hreg(uint8_t *buff, uint8_t *buff_count, uint16_t start_add, uint16_t data_count);
uint32_t mb_read_hreg_master_slave_res(mb_operation_t *mb_passed_instance);




#endif /* MB_RDHREG_H_ */