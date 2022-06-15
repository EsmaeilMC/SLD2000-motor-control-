/*
 * req_io_mb_reader.c
 *
 * Created: 22/05/2017 11:05:37 ص
 *  Author: ahmed
 */ 
#include <asf.h>
#include <sam_modbus.h>
#include <req_io_mb_reader.h>
#include <drive_interface_data.h>
#include <e_setup.h>
#include <e_faults.h>
#include <SEDACIO.h>

uint16_t mb_req_readout[7], mb_req_wrpin[6];

uint16_t mb_Holding_reg_car[7];
uint16_t mb_Holding_reg_hall[10];
uint16_t mb_Input_reg_car[1];

uint8_t req_mb_stage = 0, req_mb_hall_stage = 0; 

#define mb_display_manip *((uint8_t *)&mb_Holding_reg_car[5] + 1)
#define mb_fault_code *((uint8_t *)&mb_Holding_reg_car[6])
void req_mb_setUpMotion(void)
{
	uint8_t tmp_manip = mb_display_manip;
	tmp_manip &= 0xF8;
	tmp_manip |= 5;
	mb_display_manip = tmp_manip;
}

void req_mb_setDnMotion(void)
{
	uint8_t tmp_manip = mb_display_manip;
	tmp_manip &= 0xF8;
	tmp_manip |= 6;
	mb_display_manip = tmp_manip;
}
void req_mb_setStill(void)
{
	uint8_t tmp_manip = mb_display_manip;
	tmp_manip &= 0xF8;
	mb_display_manip = tmp_manip;
}
void req_mb_setINS(void)
{
	uint8_t tmp_manip = mb_display_manip;
	tmp_manip &= 0xF7;
	tmp_manip |= 8;
	mb_display_manip = tmp_manip;
}
void req_mb_clrINS(void)
{
	uint8_t tmp_manip = mb_display_manip;
	tmp_manip &= 0xF7;
	mb_display_manip = tmp_manip;
}

void req_mb_setFault(uint8_t fault_prefix, uint8_t fault_code)
{
	mb_fault_code = fault_code;
	
	uint8_t tmp_manip = mb_display_manip;
	tmp_manip &= 0xCF;
	tmp_manip |= (fault_prefix & 0x03) << 4;
	mb_display_manip = tmp_manip;
}

#ifndef _SERIAL_DRIVE
uint32_t mb_ireq_led_port, mb_ereq_led_port;
void req_io_mb_scan_handler(void)
{
	modbus_transaction_t req_transaction;
	req_transaction.slave_add = 0x11;
	
	if ( req_mb_stage == 0 )
	{
		req_transaction.start_add = 0;
		req_transaction.entry_type = RD_IREG;
		req_transaction.buff_size = 5;
		req_transaction.data_buff = mb_req_readout;
		//read req inputs
		if (mb_add_transaction(0, &req_transaction) == MB_ADD_TRANSACTION_SUCCESS) req_mb_stage++;
		//return;
	}
	
	//send new requests
	else if ( req_mb_stage == 1 )
	{
		req_transaction.start_add = 0;
		mb_req_led_port = mb_ireq_led_port | mb_ereq_led_port;
		req_transaction.entry_type = WR_HREG;
		req_transaction.buff_size = 5;
		req_transaction.data_buff = mb_req_wrpin;
		if (mb_add_transaction(0, &req_transaction) == MB_ADD_TRANSACTION_SUCCESS) req_mb_stage++;
	}
	else if ( req_mb_stage > 1 )
	{
		req_transaction.start_add = 5;
		req_transaction.entry_type = WR_HREG;
		req_transaction.buff_size = 1;
		req_transaction.data_buff = &mb_req_wrpin[5];
		if (mb_add_transaction(0, &req_transaction) == MB_ADD_TRANSACTION_SUCCESS) req_mb_stage = 0;
	}
}

#else

#define INTERNAL_COMM_TIMEOUT (40000 / 80) //8sec delay
uint32_t ctb_counter = 0, ccb0_counter = 0, ccb1_counter = 0;
void comm_loss_monitor(void)
{
	if (ctb_counter < INTERNAL_COMM_TIMEOUT)
	{
		ctb_counter++;
	}
	else
	{
		if(mnt == 0) CTB_err_set;
		mb_CTB_in = 0;
	}
	
	if (ccb0_counter < (2 *INTERNAL_COMM_TIMEOUT))
	{
		ccb0_counter++;
	}
	else
	{
		if(mnt == 0) CCB0_err_set;
	}
	
	if (ccb1_counter < INTERNAL_COMM_TIMEOUT)
	{
		ccb1_counter++;
	}
	else
	{
		if(mnt == 0); //CCB1_err_set;
	}
}

void req_io_internal_reply_sub(uint32_t slave_add)
{
	if (slave_add == 240)
	{
		// rec ctb message
		ctb_counter = 0;
	} 
	if (slave_add == 241)
	{
		// rec ccb0 message
		ccb0_counter = 0;
	}
	if (slave_add == 242)
	{
		// rec ccb1 message
		ccb1_counter = 0;
	}
}

void req_io_mb_cabin_handler(void)
{
	modbus_transaction_t req_transaction;
	
	
	if ( req_mb_stage == 0 )
	{
		req_transaction.slave_add = 240;
		req_transaction.start_add = 0;
		req_transaction.entry_type = RD_IREG;
		req_transaction.buff_size = 1;
		req_transaction.data_buff = mb_Input_reg_car;
		//read req inputs
		if (mb_add_transaction(0, &req_transaction) == MB_ADD_TRANSACTION_SUCCESS)
			req_mb_stage++;
	}
	//send new requests
	else if ( req_mb_stage == 1 )
	{
		req_transaction.slave_add = 0;
		req_transaction.start_add = 0;;
		req_transaction.entry_type = WR_HREG;
		req_transaction.buff_size = 7;
		//mb_display_data = 5;
		req_transaction.data_buff = mb_Holding_reg_car;
		if (mb_add_transaction(0, &req_transaction) == MB_ADD_TRANSACTION_SUCCESS)
			req_mb_stage++;
	}
	else if ( req_mb_stage > 1 )
	{
		req_transaction.slave_add = 241;
		req_transaction.start_add = 0;
		req_transaction.entry_type = RD_IREG;
		req_transaction.buff_size = 1;
		req_transaction.data_buff = &mb_req_readout[1];
		if (mb_add_transaction(0, &req_transaction) == MB_ADD_TRANSACTION_SUCCESS)
			req_mb_stage = 0;
	}
	
}

uint16_t ext_req_mb_hall_readout[28];

void req_io_mb_get_ereq(void)
{
	uint32_t i;
	uint16_t tmp;
	for (i=0; i<=f_max; i++)
	{
		tmp = ext_req_mb_hall_readout[i] >> 1;
		if(tmp & 1)
		{
			mb_ereq |= (1ul << i);
		} 
		else
		{
			mb_ereq &= ~(1ul << i);
		}
		
		if(tmp & 2)
		{
			mb_eereq |= (1ul << i);
		}
		else
		{
			mb_eereq &= ~(1ul << i);
		}
		
		ext_req_mb_hall_readout[i] = 0;
	}
}
uint8_t hall_broadcast = 0;
void req_io_mb_hall_handler(void)
{
	modbus_transaction_t req_transaction;
	
	if ( ((req_mb_hall_stage % 5) == 0 ) && (hall_broadcast) )
	{
		req_io_mb_get_ereq();
		
		mb_Holding_reg_hall[4] = mb_Holding_reg_car[5];
		mb_Holding_reg_hall[5] = mb_Holding_reg_car[6];
		
		req_transaction.slave_add = 0;
		req_transaction.start_add = 1;;
		req_transaction.entry_type = WR_HREG;
		req_transaction.buff_size = 10;
		//mb_display_data = 5;
		req_transaction.data_buff = mb_Holding_reg_hall;
		if (mb_add_transaction(1, &req_transaction) == MB_ADD_TRANSACTION_SUCCESS) hall_broadcast = 0;
	}
	else if (req_mb_hall_stage <= f_max )
	{
		req_transaction.slave_add = req_mb_hall_stage + 1;
		req_transaction.start_add = 0;
		req_transaction.entry_type = RD_IREG;
		req_transaction.buff_size = 1;
		//req_transaction.data_buff = &mb_req_readout[1];
		req_transaction.data_buff = &ext_req_mb_hall_readout[req_mb_hall_stage];
		//read req inputs
		if (mb_add_transaction(1, &req_transaction) == MB_ADD_TRANSACTION_SUCCESS)
		{
			req_mb_hall_stage++;
			hall_broadcast = 1;
		}
	}
	else
	{
		req_mb_hall_stage = 0;
	}
	
}

void req_io_mb_scan_handler(void)
{
	req_io_mb_cabin_handler();
	req_io_mb_hall_handler();	
	comm_loss_monitor();
}
#endif