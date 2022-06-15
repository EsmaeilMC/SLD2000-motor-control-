/*
 * mb_general.h
 *
 * Created: 10/05/2017 05:40:30 م
 *  Author: ahmed
 */ 


#ifndef MB_GENERAL_H_
#define MB_GENERAL_H_
#include <asf.h>
#include <sam_modbus.h>
	
#define MB_FRAME_BUFF_MAX 30

#define MB_CHANNEL_STATUS_OK 1ul
#define MB_CHANNEL_STATUS_Master 2ul
#define MB_CHANNEL_STATUS_DSENT 4ul


#define MB_MACHINE_STATE_READY_TO_LOAD (0)
#define MB_MACHINE_STATE_IDLE (1)
#define MB_MACHINE_STATE_TURNAROUND (2)
#define MB_MACHINE_STATE_REPLY_WAIT (3)
#define MB_MACHINE_STATE_NOREPLY_ERROR (4)
#define MB_MACHINE_STATE_FRAME_ERROR (5)
#define MB_MACHINE_STATE_REPLY_EXCEPTION (6)
#define MB_MACHINE_STATE_PROCESS_REPLY (7)
#define MB_MACHINE_STATE_WAIT_TO_READY (8)

typedef struct
{
	mb_opt_type_t entry_type;
	uint16_t *mb_buff;
	uint16_t mb_timer_counter;
	uint16_t mb_turnaround_timer_value;
	uint16_t mb_wait_reply_timer_value;
	uint8_t mb_machine_state;
	uint8_t mb_start_add;
	uint8_t mb_frame_buff_count;
	uint8_t buff_size;
	uint8_t channel_enabled;
	uint8_t mb_rx_buffer_count;
	uint8_t mb_channel_frame_buff[MB_FRAME_BUFF_MAX + 3];
	uint8_t mb_channel_read_frame_buff[MB_FRAME_BUFF_MAX + 3];
	Usart *mb_usart;
	uint32_t mb_channel_status;
	uint32_t reply_to_ready_counter;
	void (*reply_sub)(uint32_t slave_add);
}mb_operation_t;

extern mb_operation_t mb_instance[MB_CHANNEL_COUNT];

void mb_dispatch(void);
uint32_t mb_is_channel_master(uint32_t mb_channel_no);

#endif /* MB_GENERAL_H_ */