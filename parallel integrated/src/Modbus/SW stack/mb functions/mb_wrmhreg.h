/*
 * mb_wrmhreg.h
 *
 * Created: 11/05/2017 01:45:42 م
 *  Author: ahmed
 */ 


#ifndef MB_WRMHREG_H_
#define MB_WRMHREG_H_

#include <asf.h>
#include <mb_general.h>

void mb_write_mhreg(uint8_t *buff, uint8_t *buff_count,uint16_t start_add,uint16_t data_count, uint16_t *data);
uint32_t mb_write_mhreg_master_slave_res(mb_operation_t *mb_check_instance);


#endif /* MB_WRMHREG_H_ */