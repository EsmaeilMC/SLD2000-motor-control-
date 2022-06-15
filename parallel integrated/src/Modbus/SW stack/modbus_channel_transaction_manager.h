/*
 * modbus_channel_transaction_manager.h
 *
 * Created: 12/05/2017 12:23:44 م
 *  Author: ahmed
 */ 


#ifndef MODBUS_CHANNEL_TRANSACTION_MANAGER_H_
#define MODBUS_CHANNEL_TRANSACTION_MANAGER_H_
#include <mb_general.h>

#define MODBUS_TRANSACTION_BUFFER_MAX 4
typedef struct
{
	mb_opt_type_t entry_type;
	uint32_t time_lapse;
	uint16_t *mb_data_buff;	
	uint8_t mb_start_add;
	uint8_t buff_size;
	uint8_t mb_channel;
	uint8_t slave_add;
	uint8_t transaction_empty;
}modbus_transaction_buffer_data_t;

void mb_transaction_init(void);
void mb_load_handler(void);


#endif /* MODBUS_CHANNEL_TRANSACTION_MANAGER_H_ */