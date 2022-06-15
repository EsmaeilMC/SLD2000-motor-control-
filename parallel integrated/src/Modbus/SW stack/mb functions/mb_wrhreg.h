/*
 * mb_wrhreg.h
 *
 * Created: 11/05/2017 01:45:01 م
 *  Author: ahmed
 */ 


#ifndef MB_WRHREG_H_
#define MB_WRHREG_H_

#include <asf.h>
#include <mb_general.h>

void mb_write_hreg(uint8_t *buff, uint8_t *buff_count,uint16_t start_add, uint16_t data);
uint32_t mb_write_hreg_master_slave_res(mb_operation_t *mb_passed_instance);


#endif /* MB_WRHREG_H_ */