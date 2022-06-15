/*
 * mb_functions.c
 *
 * Created: 13/05/2017 03:47:13 م
 *  Author: ahmed
 */ 

#include <mb_functions.h>
#include <mb_wrhreg.h>
#include <mb_wrmhreg.h>
#include <mb_rdhreg.h>
#include <mb_rdireg.h>



uint32_t mb_functions_master_check_res(mb_operation_t *mb_passed_instance)
{
	uint8_t *buffer = &mb_passed_instance->mb_channel_read_frame_buff[1] , count = mb_passed_instance->mb_rx_buffer_count;
	
	if (mb_passed_instance->reply_sub)
	{
		mb_passed_instance->reply_sub(mb_passed_instance->mb_channel_read_frame_buff[0]);
	}
	
	if ((*buffer & 0x7F) == 0x06)
	{
		//response to write holding reg
		return mb_write_hreg_master_slave_res(mb_passed_instance);
	}
	else if ((*buffer & 0x7F) == 0x10)
	{
		//response to write multiple holding reg
		return mb_write_mhreg_master_slave_res(mb_passed_instance);
	}
	else if ((*buffer & 0x7F) == 0x03)
	{
		//response to read holding reg
		return mb_read_hreg_master_slave_res(mb_passed_instance);
	}
	else if ((*buffer & 0x7F) == 0x04)
	{
		//response to read input registers
		return mb_read_ireg_master_slave_res(mb_passed_instance);
	}
}