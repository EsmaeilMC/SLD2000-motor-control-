/*
 * mb_general.c
 *
 * Created: 10/05/2017 05:39:53 م
 *  Author: ahmed
 */ 
#include <mb_general.h>
#include <sam_modbus.h>
#include <port.h>
#include <mb_wrhreg.h>
#include <mb_wrmhreg.h>
#include <mb_rdhreg.h>
#include <mb_rdireg.h>
#include <mb_frame.h>
#include <modbus_channel_transaction_manager.h>
#include <mb_states.h>


mb_operation_t mb_instance[MB_CHANNEL_COUNT];

void mb_handler(void)
{
	mb_dispatch();
	mb_states_master_waiting_for_reply_handler();
	mb_states_master_process_reply_handler();
	mb_states_master_process_noreply_handler();
	mb_states_master_delay_to_ready();
	mb_load_handler();
}
void mb_dispatch(void)
{
	uint32_t i;
	for (i=0; i<MB_CHANNEL_COUNT; i++)
	{
		if(mb_instance[i].buff_size > 0)
		{
			if (mb_instance[i].mb_machine_state == MB_MACHINE_STATE_IDLE)
			if (!(mb_instance[i].mb_channel_status&MB_CHANNEL_STATUS_DSENT) && (mb_instance[i].mb_channel_status&MB_CHANNEL_STATUS_OK))//data ready to be sent
			{
				//data ready to be sent
				//send the data
				if (mb_instance[i].entry_type == WR_HREG)
				{
					//sending holding register
					//check the number of registers
					if (mb_instance[i].buff_size == 1)
					{
						//write single holding register
						mb_write_hreg(&mb_instance[i].mb_channel_frame_buff[1], &mb_instance[i].mb_frame_buff_count, mb_instance[i].mb_start_add, mb_instance[i].mb_buff[0]);
						mb_instance[i].mb_frame_buff_count++;
						mb_add_crc(mb_instance[i].mb_channel_frame_buff, &mb_instance[i].mb_frame_buff_count);
					}
					else if (mb_instance[i].buff_size > 1)
					{
						//write multiple holding register
						mb_write_mhreg(&mb_instance[i].mb_channel_frame_buff[1], &mb_instance[i].mb_frame_buff_count, mb_instance[i].mb_start_add, mb_instance[i].buff_size, &mb_instance[i].mb_buff[0]);
						mb_instance[i].mb_frame_buff_count++;
						mb_add_crc(mb_instance[i].mb_channel_frame_buff, &mb_instance[i].mb_frame_buff_count);

					}
				}
				else if (mb_instance[i].entry_type == RD_HREG)
				{
					//read holding register(s)
					mb_read_hreg(&mb_instance[i].mb_channel_frame_buff[1], &mb_instance[i].mb_frame_buff_count, mb_instance[i].mb_start_add, mb_instance[i].buff_size);
					mb_instance[i].mb_frame_buff_count++;
					mb_add_crc(mb_instance[i].mb_channel_frame_buff, &mb_instance[i].mb_frame_buff_count);
				}
				else if (mb_instance[i].entry_type == RD_IREG)
				{
					//read input register(s)
					mb_read_ireg(&mb_instance[i].mb_channel_frame_buff[1], &mb_instance[i].mb_frame_buff_count, mb_instance[i].mb_start_add, mb_instance[i].buff_size);
					mb_instance[i].mb_frame_buff_count++;
					mb_add_crc(mb_instance[i].mb_channel_frame_buff, &mb_instance[i].mb_frame_buff_count);
				}
				port_usart_write_pda_line(mb_instance[i].mb_usart, mb_instance[i].mb_channel_frame_buff, mb_instance[i].mb_frame_buff_count);
				//usart_disable_rx(mb_instance[i].mb_usart);
				port_usart_set_rx_pda_buff(mb_instance[i].mb_usart, mb_instance[i].mb_channel_read_frame_buff, MB_FRAME_BUFF_MAX + 3);
				usart_start_rx_timeout(mb_instance[i].mb_usart); //start timeout after receiving the first char
				mb_instance[i].mb_channel_status |= MB_CHANNEL_STATUS_DSENT; //send the data
				mb_instance[i].mb_machine_state = MB_MACHINE_STATE_REPLY_WAIT;
				mb_instance[i].mb_timer_counter = mb_instance[i].mb_wait_reply_timer_value; //load timer with timeout reply val.
			}
		}
	}
}

void mb_init(void)
{
	uint32_t i;
	for (i=0; i<MB_CHANNEL_COUNT; i++)
	{
		mb_instance[i].entry_type = MB_NONE;
		mb_instance[i].channel_enabled = 0; //disable all channels
		mb_instance[i].mb_usart = 0; // assign empty usart
		mb_instance[i].mb_channel_status = 0;
		mb_instance[i].mb_machine_state = MB_MACHINE_STATE_READY_TO_LOAD;
	}
	mb_transaction_init();
}

void mb_channel_init(uint32_t mb_channel_no, Usart *mb_usart, sam_usart_opt_t *usart_setting, uint32_t usart_id, uint32_t ch_master, uint16_t turn_around_val, uint32_t reply_timeout_val)
{
	if(mb_channel_no >= MB_CHANNEL_COUNT) return 0;//error
	
	mb_instance[mb_channel_no].mb_usart = mb_usart;
	mb_instance[mb_channel_no].channel_enabled = 1;
	mb_instance[mb_channel_no].buff_size = 0;
	mb_instance[mb_channel_no].mb_buff = 0;
	mb_instance[mb_channel_no].mb_channel_status = MB_CHANNEL_STATUS_OK; //set status ok
	mb_instance[mb_channel_no].mb_turnaround_timer_value = turn_around_val;
	mb_instance[mb_channel_no].mb_wait_reply_timer_value = reply_timeout_val;
	mb_instance[mb_channel_no].reply_sub = 0;
	if(ch_master) mb_instance[mb_channel_no].mb_channel_status |= MB_CHANNEL_STATUS_Master; //set status ok
	mb_port_init(usart_id, usart_setting, mb_usart);
}

// a function to check the status of a channel, as if the channel isn't master you can't perform master operations on it
uint32_t mb_is_channel_master(uint32_t mb_channel_no)
{
	if(mb_instance[mb_channel_no].mb_channel_status | MB_CHANNEL_STATUS_Master) return 1;
	else return 0;
}

void mb_chanel_add_reply_sub(uint32_t mb_channel_no, void (*reply_sub)(uint32_t slave_add))
{
	mb_instance[mb_channel_no].reply_sub = reply_sub;
}