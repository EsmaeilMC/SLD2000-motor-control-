/*
 * mb_wrhreg.c
 *
 * Created: 11/05/2017 01:44:53 م
 *  Author: ahmed
 */ 
#include <mb_wrhreg.h>
#include <asf.h>

//volatile uint8_t tempdata;
void mb_write_hreg(uint8_t *buff, uint8_t *buff_count,uint16_t start_add, uint16_t data)
{
	//add the function code
	*buff++ = 0x06;
	*buff++ = *(((uint8_t *)&start_add)+1);
	*buff++ = *((uint8_t *)&start_add);
	*buff++ = *(((uint8_t *)&data)+1);
	*buff++ = *((uint8_t *)&data);
	*buff_count = 5;	
}

uint32_t mb_write_hreg_master_slave_res(mb_operation_t *mb_passed_instance)
{
	uint8_t *buffer = &mb_passed_instance->mb_channel_read_frame_buff[1] , count = mb_passed_instance->mb_rx_buffer_count;
	
	if (*buffer & 0x80)
	{
		//exception response
		return 0;
	}
	else
	{
		//normal response
		return 1;
	}
	
}