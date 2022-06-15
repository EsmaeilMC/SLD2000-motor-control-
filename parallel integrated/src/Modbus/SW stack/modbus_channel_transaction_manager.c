/*
 * modbus_channel_transaction_manager.c
 *
 * Created: 12/05/2017 12:23:34 م
 *  Author: ahmed
 */ 

#include <sam_modbus.h>
#include <mb_general.h>
#include <modbus_channel_transaction_manager.h>

modbus_transaction_buffer_data_t transactions_buffers[MODBUS_TRANSACTION_BUFFER_MAX];
uint8_t mb_channel_transaction_pending_count[MB_CHANNEL_COUNT];
void mb_transaction_init(void)
{
	uint32_t i;
	for(i=0; i<MODBUS_TRANSACTION_BUFFER_MAX; i++)
	{
		transactions_buffers[i].transaction_empty = 1;
		//empty all transaction buffer	
	}
	for (i=0; i<MB_CHANNEL_COUNT; i++)
	{
		mb_channel_transaction_pending_count[i]=0;
		//empty all transaction buffer counter.
	}
}



uint32_t mb_add_transaction(uint32_t mb_channel, modbus_transaction_t *transaction)
{
	uint32_t i;
	if(mb_channel >= MB_CHANNEL_COUNT) return MB_ADD_TRANSACTION_NO_SUCH_CHANNEL;
	else if(mb_instance[mb_channel].channel_enabled == 0) return MB_ADD_TRANSACTION_CHANNEL_IS_NOT_INITIALIZED;
	if (mb_channel_transaction_pending_count[mb_channel] >= (MODBUS_TRANSACTION_BUFFER_MAX / MB_CHANNEL_COUNT) ) return MB_ADD_TRANSACTION_BUFF_FULL;
	
	for(i=0; i<MODBUS_TRANSACTION_BUFFER_MAX; i++)
	{
		if(transactions_buffers[i].transaction_empty == 1)
		{
			transactions_buffers[i].transaction_empty = 0;
			transactions_buffers[i].mb_channel = mb_channel;
			transactions_buffers[i].entry_type = transaction->entry_type;
			transactions_buffers[i].buff_size = transaction->buff_size;
			transactions_buffers[i].mb_data_buff = transaction->data_buff;
			transactions_buffers[i].mb_start_add = transaction->start_add;
			transactions_buffers[i].slave_add = transaction->slave_add;
			transactions_buffers[i].time_lapse = 0;
			mb_channel_transaction_pending_count[mb_channel]++; //increment the channel buffer count
			return MB_ADD_TRANSACTION_SUCCESS;
		}
	}
	
	return MB_ADD_TRANSACTION_BUFF_FULL;		
}
void mb_load_handler(void)
{
	uint32_t i,j, selected_buffer, largest_buffer_time_lapse;
	
	for (j=0; j<MODBUS_TRANSACTION_BUFFER_MAX; j++)
	{
		if (!(transactions_buffers[j].transaction_empty))
		{
			//increment time lapse
			if(transactions_buffers[j].time_lapse < 0xFFFFFFFF) transactions_buffers[j].time_lapse++;
		}
	}
	
	for (i=0; i<MB_CHANNEL_COUNT; i++)
	{
		// a loop to search for empty channel
		if(mb_channel_transaction_pending_count[i] == 0) continue; //if there is no data pending exit
		if (mb_instance[i].mb_machine_state == MB_MACHINE_STATE_READY_TO_LOAD)
		if (mb_instance[i].channel_enabled == 1)//modbus channel is enable
		{
			//channel is ready to load data
			largest_buffer_time_lapse = 0; //set time lapse to smallest
			selected_buffer = 0xFFFFFFFF; // set selected channel to none
			for (j=0; j<MODBUS_TRANSACTION_BUFFER_MAX; j++)
			{
				if (transactions_buffers[j].buff_size > 0)//there is data in this buffer space
				if (transactions_buffers[j].mb_channel == i) //same channel data found
				if (!(transactions_buffers[j].transaction_empty))
				{
					if(transactions_buffers[j].time_lapse >= largest_buffer_time_lapse)
					{
						largest_buffer_time_lapse = transactions_buffers[j].time_lapse;
						selected_buffer = j;
					}
				}
			}
			if (selected_buffer != 0xFFFFFFFF)
			{
				//there is a channel in this buffer waiting to be processed
				mb_instance[i].entry_type = transactions_buffers[selected_buffer].entry_type;
				mb_instance[i].buff_size = transactions_buffers[selected_buffer].buff_size;
				mb_instance[i].mb_start_add = transactions_buffers[selected_buffer].mb_start_add;
				mb_instance[i].mb_channel_frame_buff[0] = transactions_buffers[selected_buffer].slave_add;
				mb_instance[i].mb_buff = transactions_buffers[selected_buffer].mb_data_buff;
				mb_instance[i].mb_machine_state = MB_MACHINE_STATE_IDLE;
				transactions_buffers[selected_buffer].transaction_empty = 1;
				mb_instance[i].mb_channel_status &= MB_CHANNEL_STATUS_OK|MB_CHANNEL_STATUS_Master;
				mb_channel_transaction_pending_count[i]--; //decrement the channel buffer count
			}
		} 
	}
		
}