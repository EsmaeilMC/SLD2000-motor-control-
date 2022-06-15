/*
 * mb_rdhreg.c
 *
 * Created: 11/05/2017 01:46:03 م
 *  Author: ahmed
 */ 
#include <mb_rdhreg.h>


void mb_read_hreg(uint8_t *buff, uint8_t *buff_count, uint16_t start_add, uint16_t data_count)
{
	//add the function code read H register
	*buff++ = 0x03;
	*buff++ = *(((uint8_t *)&start_add)+1);
	*buff++ = *((uint8_t *)&start_add);
	*buff++ = *(((uint8_t *)&data_count)+1);
	*buff++ = *((uint8_t *)&data_count);
	*buff_count = 5;
}

uint32_t mb_read_hreg_master_slave_res(mb_operation_t *mb_passed_instance)
{
	uint8_t *buffer = &mb_passed_instance->mb_channel_read_frame_buff[1] , count = mb_passed_instance->mb_rx_buffer_count;
	uint16_t *store_data_buff = mb_passed_instance->mb_buff;
	uint8_t save_buffer_data_count = mb_passed_instance->buff_size;
	if (*buffer++ & 0x80)
	{
		//exception response
		return 0;
	}
	else
	{
		//normal response
		if(store_data_buff)
		{
			//there is a buffer to store data to
			if(((*buffer)/2) < save_buffer_data_count) save_buffer_data_count = (*buffer) / 2; //get the least buffer count as not to go beyond the buffer boundries
			buffer++;
			while(save_buffer_data_count--)
			{
				//starting saving data
				*store_data_buff = (*buffer++) << 8;
				(*store_data_buff++) += *buffer++;
			}
		}
		return 1;
	}
	
}