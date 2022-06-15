/*
 * port.h
 *
 * Created: 09/05/2017 12:23:44 م
 *  Author: ahmed
 */ 


#ifndef PORT_H_
#define PORT_H_

void mb_port_init(uint32_t usart_id, const sam_usart_opt_t *c_opt_usart, Usart *p_usart);
uint32_t port_usart_write_pda_line(Usart *p_usart, char *line, uint32_t lenght);
uint32_t port_usart_set_rx_pda_buff(Usart *p_usart, char *line, uint32_t lenght);

#endif /* PORT_H_ */