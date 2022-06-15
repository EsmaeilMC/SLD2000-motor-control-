/*
 * io_interface.h
 *
 * Created: 05/04/2017 01:09:40 م
 *  Author: ahmed
 */ 


#ifndef IO_INTERFACE_H_
#define IO_INTERFACE_H_

#include <def.h>

void io_interface_init(void);

void io_interface_add_out(uint32_t bit_add);
void io_interface_handler(void);
void io_interface_critical_handler(void);

uint32_t io_interface_get_door_message(uint32_t door_add);

#define IO_I_A_GR_POS 24
#define IO_I_A_GR_MSK 0xFF000000
uint32_t io_interface_update(uint32_t address, uint32_t set_val, uint32_t default_ret_val);

void scan_inputs_handler(void);
void scan_buttons_handler(void);

uByte_t get_enter_button(void);
uByte_t get_esc_button(void);
uByte_t get_up_button(void);
uByte_t get_down_button(void);
uByte_t get_left_button(void);
uByte_t get_right_button(void);
void drop_all_buttons(void);
#endif /* IO_INTERFACE_H_ */