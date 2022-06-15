/*
 * req_io_mb_reader.h
 *
 * Created: 22/05/2017 11:05:49 ص
 *  Author: ahmed
 */ 


#ifndef REQ_IO_MB_READER_H_
#define REQ_IO_MB_READER_H_

#include <drive_interface_data.h>

extern uint16_t mb_Holding_reg_car[7];
extern uint16_t mb_Input_reg_car[1];
extern uint16_t mb_Holding_reg_hall[10];

#define mb_CTB_out	(mb_Holding_reg_car[0])
#define mb_CTB_in	(mb_Input_reg_car[0])

void req_mb_setUpMotion(void);
void req_mb_setDnMotion(void);
void req_mb_setStill(void);
void req_mb_setINS(void);
void req_mb_clrINS(void);
void req_mb_setFault(uint8_t fault_prefix, uint8_t fault_code);

#ifndef _SERIAL_DRIVE

	extern uint16_t mb_req_readout[5], mb_req_wrpin[6];
	extern uint32_t mb_ireq_led_port, mb_ereq_led_port;

	#define mb_ireq (*((uint32_t *)&mb_req_readout[1]))
	#define mb_ereq (*((uint32_t *)&mb_req_readout[3]))

	#define mb_display_type(x) *(((char *)&mb_req_wrpin[3])+1) = (x & 0x0F)
	#define mb_display_data *((char *)&mb_req_wrpin[3])

	#define mb_req_led_port (*((uint32_t *)&mb_req_wrpin[1]))
	#define mb_req_led_disable_port (*((uint32_t *)&mb_req_wrpin[4]))

#else

	extern uint16_t mb_req_readout[7], mb_req_wrpin[6];
	
	#define mb_ireq (*((uint32_t *)&mb_req_readout[1]))
	#define mb_ereq (*((uint32_t *)&mb_req_readout[3]))
	#define mb_eereq (*((uint32_t *)&mb_req_readout[5]))

	#define mb_display_type(x) *(((uint8_t *)&mb_req_wrpin[3])+1) = (x & 0x0F)
	#define mb_display_data *((uint8_t *)&mb_Holding_reg_car[5])

	#define mb_ireq_led_port (*((uint32_t *)&mb_Holding_reg_car[1]))
	#define mb_ereq_led_port (*((uint32_t *)&mb_Holding_reg_hall[0]))
	#define mb_eereq_led_port (*((uint32_t *)&mb_Holding_reg_hall[6]))
	#define mb_req_led_disable_port (*((uint32_t *)&mb_Holding_reg_car[3]))
	#define mb_ereq_led_disable_port (*((uint32_t *)&mb_Holding_reg_hall[2]))
	#define mb_eereq_led_disable_port (*((uint32_t *)&mb_Holding_reg_hall[8]))
#endif
void req_io_mb_scan_handler(void);

void req_io_internal_reply_sub(uint32_t slave_add);

#endif /* REQ_IO_MB_READER_H_ */