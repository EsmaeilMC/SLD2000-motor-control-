/*
 * sam_modbus.h
 *
 * Created: 10/05/2017 05:41:03 م
 *  Author: ahmed
 */ 


#ifndef SAM_MODBUS_H_
#define SAM_MODBUS_H_

#define MB_CHANNEL_COUNT 2

#include <asf.h>
typedef enum {MB_NONE, WR_HREG, RD_HREG, RD_IREG}mb_opt_type_t;


#define MB_ADD_TRANSACTION_SUCCESS 1
#define MB_ADD_TRANSACTION_BUFF_FULL 2
#define MB_ADD_TRANSACTION_NO_SUCH_CHANNEL 3
#define MB_ADD_TRANSACTION_CHANNEL_IS_NOT_INITIALIZED 4


typedef struct
{
	mb_opt_type_t entry_type;
	uint16_t *data_buff;
	uint8_t start_add;
	uint8_t buff_size;
	uint8_t slave_add;
}modbus_transaction_t;

void mb_handler(void);
void mb_init(void);
void mb_channel_init(uint32_t mb_channel_no, Usart *mb_usart, sam_usart_opt_t *usart_setting, uint32_t usart_id, uint32_t ch_master, uint16_t turn_around_val, uint32_t reply_timeout_val);
void mb_chanel_add_reply_sub(uint32_t mb_channel_no, void (*reply_sub)(uint32_t slave_add));

uint32_t mb_add_transaction(uint32_t mb_channel, modbus_transaction_t *transaction);
#endif /* SAM_MODBUS_H_ */