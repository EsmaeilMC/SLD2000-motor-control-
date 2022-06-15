/*
 * mb_states.c
 *
 * Created: 12/05/2017 06:03:02 م
 *  Author: ahmed
 */ 
#include <mb_states.h>
#include <mb_general.h>
#include <sam_modbus.h>
#include <port.h>
#include <mb_frame.h>
#include <mb_functions.h>

#define REPLY_TO_READY_TOUT (0)

void mb_states_master_waiting_for_reply_handler(void)
{
	uint32_t i;
	for (i=0; i<MB_CHANNEL_COUNT; i++)
	{
		if (mb_instance[i].mb_machine_state == MB_MACHINE_STATE_REPLY_WAIT)
		{
			if(mb_instance[i].mb_usart->US_CSR & US_CSR_TIMEOUT)
			{
				//timeout passed frame received before reply timeout passes!
				if (mb_instance[i].mb_channel_frame_buff[0] == mb_instance[i].mb_channel_read_frame_buff[0])
				{
					//expected slave response
					mb_instance[i].mb_machine_state = MB_MACHINE_STATE_PROCESS_REPLY;
					//mb_instance[i].mb_rx_buffer_count = (MB_FRAME_BUFF_MAX + 3 + 2 - XDMAC_CBC_BLEN(XDMAC->XDMAC_CHID[5].XDMAC_CUBC));
					return;
				}
				else
				{
					//unexpected slave response
					//ignore the frame and continue waiting
					port_usart_set_rx_pda_buff(mb_instance[i].mb_usart, mb_instance[i].mb_channel_read_frame_buff, MB_FRAME_BUFF_MAX + 3); //reset pda buffer
					usart_start_rx_timeout(mb_instance[i].mb_usart); //start timeout after receiving the first char
				}
			}
			if(--mb_instance[i].mb_timer_counter == 0)
			{
				//timeout has passed and no response
				mb_instance[i].mb_machine_state = MB_MACHINE_STATE_NOREPLY_ERROR;
				return;
			}
		}
	}
	
}

void mb_states_master_process_reply_handler(void)
{
	uint32_t i;
	for (i=0; i<MB_CHANNEL_COUNT; i++)
	{
		if (mb_instance[i].mb_machine_state == MB_MACHINE_STATE_PROCESS_REPLY)
		{
			//check for crc_error
			mb_instance[i].mb_rx_buffer_count = mb_instance[i].mb_channel_read_frame_buff[2] + 3;
			if(mb_check_crc(mb_instance[i].mb_channel_read_frame_buff, &mb_instance[i].mb_rx_buffer_count))
			{
				mb_functions_master_check_res(&mb_instance[i]);
			}
			mb_instance[i].reply_to_ready_counter = REPLY_TO_READY_TOUT;
			mb_instance[i].mb_machine_state = MB_MACHINE_STATE_WAIT_TO_READY;
		}
	}
	
}

void mb_states_master_process_noreply_handler(void)
{
	uint32_t i;
	for (i=0; i<MB_CHANNEL_COUNT; i++)
	{
		if (mb_instance[i].mb_machine_state == MB_MACHINE_STATE_NOREPLY_ERROR)
		{
			mb_instance[i].reply_to_ready_counter = REPLY_TO_READY_TOUT;
			mb_instance[i].mb_machine_state = MB_MACHINE_STATE_WAIT_TO_READY;
		}
	}
	
}

void mb_states_master_delay_to_ready(void)
{
	uint32_t i;
	for (i=0; i<MB_CHANNEL_COUNT; i++)
	{
		if (mb_instance[i].mb_machine_state == MB_MACHINE_STATE_WAIT_TO_READY)
		{
			if (mb_instance[i].reply_to_ready_counter > 0) mb_instance[i].reply_to_ready_counter--;
			else mb_instance[i].mb_machine_state = MB_MACHINE_STATE_READY_TO_LOAD;
		}
	}
}