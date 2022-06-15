/*
 * io_interface.c
 *
 * Created: 05/04/2017 01:09:58 م
 *  Author: ahmed
 */

#include <asf.h>
#include <io_interface.h>
#include <GIO.h>
#include <req_io_mb_reader.h>
#include <drive_interface_data.h>


#define HOIST0 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 13))
#define HOIST1 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 22))
#define HOIST2 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 18))

#define IO_X1 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOB, 3))
#define IO_X2 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOD, 24))
#define IO_X3 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 14))
#define IO_X4 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 15))
#define IO_X5 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOD, 27))
#define IO_X6 ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 23))

#define IO_PHR ioport_get_pin_level(IOPORT_CREATE_PIN(PIOA, 28))
#define IO_PHS ioport_get_pin_level(IOPORT_CREATE_PIN(PIOC, 18))
#define IO_PHT ioport_get_pin_level(IOPORT_CREATE_PIN(PIOD, 7))

#define IO_Y1(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOA, 12), x)
#define IO_Y2(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOD, 13), x)
#define IO_Y3(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOC, 10), x)
#define IO_Y4(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOD, 20), x)
#define IO_Y5(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOD, 26), x)
#define IO_Y6(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOC, 9), x)
#define IO_Y7(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOD, 25), x)
#define IO_Y8(x) ioport_set_pin_level(IOPORT_CREATE_PIN(PIOA, 27), x)


void io_interface_init(void)
{	
	//Set relay yERS pin out low
	pio_set_output(PIOB, PIO_PB2, LOW, DISABLE, ENABLE);
	//Set relay y0,y7,y4 pin out low
	pio_set_output(PIOA, PIO_PA12|PIO_PA27, LOW, DISABLE, ENABLE);
	//Set relay y1,y3,y4,y6 pin out low
	pio_set_output(PIOD, PIO_PD13|PIO_PD20|PIO_PD25|PIO_PD26, LOW, DISABLE, ENABLE);
	//Set relay Y2,Y4,Y5 pin out low
	pio_set_output(PIOC, PIO_PC10|PIO_PC9, LOW, DISABLE, ENABLE);
	//pio_set_output(PIOE, PIO_PE5, LOW, DISABLE, ENABLE);
	
	//set direct inputs pins
	pio_set_input(PIOA, PIO_PA14|PIO_PA15|PIO_PA23, PIO_PULLUP);
	pio_set_input(PIOB, PIO_PB3, PIO_PULLUP);
	pio_set_input(PIOD, PIO_PD24|PIO_PD27, PIO_PULLUP);
	
	//set direct phase failure pins
	pio_set_input(PIOD, PIO_PD7, PIO_PULLUP);
	pio_set_input(PIOA, PIO_PA28, PIO_PULLUP);
	pio_set_input(PIOC, PIO_PC18, PIO_PULLUP);
	
	//set shift registers interface pins
	// HOIST1, HOIST0, HOIST2
	pio_set_input(PIOA, PIO_PA13|PIO_PA18|PIO_PA22, PIO_PULLUP);
} 

void io_interface_add_out(uint32_t bit_add)
{
	pio_out_data(PIOC, 0x7, bit_add, 1);
}
#define IO_INTERFACE_FILTER_VAL 4
#define IO_INTERFACE_FILTER_TOP 2
#define IO_INTERFACE_FILTER_MAX (IO_INTERFACE_FILTER_TOP + IO_INTERFACE_FILTER_VAL)
#define IO_INTERFACE_INPUTOFFSET 4
#define IO_INTERFACE_INPUTCOUNT (24 + IO_INTERFACE_INPUTOFFSET)
char io_interface_filters[IO_INTERFACE_INPUTCOUNT + 5];

void io_interface_integrate_enableIputs(void)
{
	if (IO_X1)
	{
		if (io_interface_filters[28] > 0)
		{
			io_interface_filters[28]--;
		}
	}
	else
	{
		if (io_interface_filters[28] < IO_INTERFACE_FILTER_MAX)
		{
			io_interface_filters[28]++;
		}
	}
	if (IO_X2)
	{
		if (io_interface_filters[29] > 0)
		{
			io_interface_filters[29]--;
		}
	}
	else
	{
		if (io_interface_filters[29] < IO_INTERFACE_FILTER_MAX)
		{
			io_interface_filters[29]++;
		}
	}	
}

void io_interface_integrate_iputs(void)
{
	uint32_t i;	
	if (IO_X3)
	{
		if (io_interface_filters[0] > 0)
		{
			io_interface_filters[0]--;
		}
	}
	else
	{
		if (io_interface_filters[0] < IO_INTERFACE_FILTER_MAX)
		{
			io_interface_filters[0]++;
		}
	}
	
	if (IO_X4)
	{
		if (io_interface_filters[1] > 0)
		{
			io_interface_filters[1]--;
		}
	}

	else
	{
		if (io_interface_filters[1] < IO_INTERFACE_FILTER_MAX)
		{
			io_interface_filters[1]++;
		}
	}
	if (IO_X5)
	{
		if (io_interface_filters[2] > 0)
		{
			io_interface_filters[2]--;
		}
	}
	else
	{
		if (io_interface_filters[2] < IO_INTERFACE_FILTER_MAX)
		{
			io_interface_filters[2]++;
		}
	}
	if (IO_X6)
	{
		if (io_interface_filters[3] > 0)
		{
			io_interface_filters[3]--;
		}
	}
	else
	{
		if (io_interface_filters[3] < IO_INTERFACE_FILTER_MAX)
		{
			io_interface_filters[3]++;
		}
	}
	
	for (i=0; i<8; i++)
	{
		io_interface_add_out(7 - i);// set the address to read the targeted pin
		if (HOIST0)
		{
			if (io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (0 * 8)] < IO_INTERFACE_FILTER_MAX)
			{
				io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (0 * 8)]++;
			}
		} 
		else
		{
			if (io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (0 * 8)] > 0)
			{
				io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (0 * 8)]--;
			}
		}
		
		if (HOIST1)
		{
			if (io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (1 * 8)] < IO_INTERFACE_FILTER_MAX)
			{
				io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (1 * 8)]++;
			}
		}
		else
		{
			if (io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (1 * 8)] > 0)
			{
				io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (1 * 8)]--;
			}
		}
#ifndef _SERIAL_DRIVE		
		if (HOIST2)
		{
			if (io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (2 * 8)] < IO_INTERFACE_FILTER_MAX)
			{
				io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (2 * 8)]++;
			}
		}
		else
		{
			if (io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (2 * 8)] > 0)
			{
				io_interface_filters[i + IO_INTERFACE_INPUTOFFSET + (2 * 8)]--;
			}
		}	
#endif
	}
		
}

uint64_t io_interface_readout1 = 0, io_interface_pre_readout1 = 0;
uint64_t io_interface_rise_readout1 = 0, io_interface_fall_readout1 = 0;
#define IO_INTERFACE_MASKER(x) (1ull<<x)
void io_interface_translateCritical(void)
{
	uint32_t i;
	uint64_t f_mask;
	for (i=IO_INTERFACE_INPUTCOUNT; i<(IO_INTERFACE_INPUTCOUNT + 2); i++)
	{
		f_mask = IO_INTERFACE_MASKER(i);
		io_interface_pre_readout1 = io_interface_readout1; // copy the previous register sample
		if (io_interface_filters[i] >= IO_INTERFACE_FILTER_VAL)
		{
			//pin set detected
			//set the respective bit
			io_interface_readout1 |= f_mask;
			//check rising edge flag
			if (!(io_interface_pre_readout1 & f_mask))
			{
				//set the respective rising edge bit
				io_interface_rise_readout1 |= f_mask;
			}
		}
		else
		{
			//pin clear detected
			//check falling edge flag
			if ((io_interface_pre_readout1 & f_mask))
			{
				//set the respective falling edge bit
				io_interface_fall_readout1 |= f_mask;
			}
			//clear the respective bit
			f_mask = ~f_mask;
			io_interface_readout1 &= f_mask;
		}
	}
	
}
void io_interface_translate1(void)
{
	uint32_t i, f_mask;
	for (i=0; i<IO_INTERFACE_INPUTCOUNT; i++)
	{
		f_mask = IO_INTERFACE_MASKER(i);
		io_interface_pre_readout1 = io_interface_readout1; // copy the previous register sample
		if (io_interface_filters[i] >= IO_INTERFACE_FILTER_VAL)
		{
			//pin set detected
			//set the respective bit
			io_interface_readout1 |= f_mask;
			//check rising edge flag
			if (!(io_interface_pre_readout1 & f_mask))
			{
				//set the respective rising edge bit
				io_interface_rise_readout1 |= f_mask;
			}
		} 
		else
		{
			//pin clear detected
			//check falling edge flag
			if ((io_interface_pre_readout1 & f_mask))
			{
				//set the respective falling edge bit
				io_interface_fall_readout1 |= f_mask;
			}
			//clear the respective bit
			f_mask = ~f_mask;
			io_interface_readout1 &= f_mask;
		}
	}
		
}

void io_interface_critical_handler(void)
{
	io_interface_integrate_enableIputs();
	io_interface_translateCritical();
}

void io_interface_handler(void)
{
	io_interface_integrate_iputs();
	io_interface_translate1();
	io_interface_critical_handler();
	scan_buttons_handler();		
}

uint32_t io_interface_get_val(uint32_t val)
{
	uint32_t temp;
	if(val < 22)
	{
		temp = io_interface_readout1;
	}
	else if(val < 30)
	{
		temp = mb_CTB_in;
		val -= 22;
	}
	else temp = 0;
	if (temp & IO_INTERFACE_MASKER(val))
	{
		//io_interface_readout1 &= ~IO_INTERFACE_MASKER(val);
		return 1;
	} 
	else
	{
		return 0;
	}
}

uint32_t io_interface_get_rising_val(uint32_t val)
{
	if (io_interface_rise_readout1 & IO_INTERFACE_MASKER(val))
	{
		io_interface_rise_readout1 &= ~IO_INTERFACE_MASKER(val);
		return 1;
	}
	else
	{
		return 0;
	}
}

uint32_t io_interface_get_falling_val(uint32_t val)
{
	if (io_interface_fall_readout1 & IO_INTERFACE_MASKER(val))
	{
		io_interface_fall_readout1 &= ~IO_INTERFACE_MASKER(val);
		return 1;
	}
	else
	{
		return 0;
	}
}
//#pragma GCC optimize("O1")
uint32_t io_interface_get_ireq_val(uint32_t add,uint32_t data)
{
	
	volatile uint32_t tmp = mb_ireq;
	if(data) 
	{
		mb_ireq_led_port |= IO_INTERFACE_MASKER(add);
	}
	else mb_ireq_led_port &= ~IO_INTERFACE_MASKER(add);
	//mb_ireq = 0;
	if (tmp & IO_INTERFACE_MASKER(add))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint32_t io_interface_get_ereq_val(uint32_t add, uint32_t data)
{
	if(data) mb_ereq_led_port |= IO_INTERFACE_MASKER(add);
	else mb_ereq_led_port &= ~IO_INTERFACE_MASKER(add);
	
	volatile uint32_t tmp = mb_ereq;
	if (tmp & IO_INTERFACE_MASKER(add))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint32_t io_interface_get_eereq_val(uint32_t add, uint32_t data)
{
	if(data) mb_eereq_led_port |= IO_INTERFACE_MASKER(add);
	else mb_eereq_led_port &= ~IO_INTERFACE_MASKER(add);
	
	volatile uint32_t tmp = mb_eereq;
	if (tmp & IO_INTERFACE_MASKER(add))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//#pragma GCC optimize("O2")
void io_interface_set_reqLed_mask(uint32_t add, uint32_t data)
{
	if (data == 0)
	{
		mb_req_led_disable_port &= ~IO_INTERFACE_MASKER(add);
	} 
	else
	{
		mb_req_led_disable_port |= IO_INTERFACE_MASKER(add);
	}
}

void io_interface_set_ereqLed_mask(uint32_t add, uint32_t data)
{
	if (data == 0)
	{
		mb_ereq_led_disable_port &= ~IO_INTERFACE_MASKER(add);
	}
	else
	{
		mb_ereq_led_disable_port |= IO_INTERFACE_MASKER(add);
	}
}

void io_interface_set_eereqLed_mask(uint32_t add, uint32_t data)
{
	if (data == 0)
	{
		mb_eereq_led_disable_port &= ~IO_INTERFACE_MASKER(add);
	}
	else
	{
		mb_eereq_led_disable_port |= IO_INTERFACE_MASKER(add);
	}
}

void io_interface_set_out(uint32_t address, uint32_t output)
{
	switch(address)
	{
		case 0:
		IO_Y1(output);
		break;
		
		case 1:
		IO_Y2(output);
		break;

		case 2:
		IO_Y3(output);
		break;
		
		case 3:
		IO_Y4(output);
		break;

		case 4:
		IO_Y5(output);
		break;
		
		case 5:
		IO_Y6(output);
		break;

		case 6:
		IO_Y7(output);
		break;
		
		case 7:
		IO_Y8(output);
		break;
	}
}

void io_interface_set_out_CTB(uint32_t address, uint32_t output)
{
	uint32_t temp = mb_CTB_out;
	if (output)
	{
		temp |= IO_INTERFACE_MASKER(address);
	} 
	else
	{
		temp &= ~IO_INTERFACE_MASKER(address);
	}
	mb_CTB_out = temp;
}

uint32_t io_interface_get_door_message(uint32_t door_add)
{
	uint32_t tmp = mb_CTB_in;
	if (door_add == 0)
	{
		return ( (tmp >> 8) & 0x007 );
	} 
	else
	{
		return ( (tmp >> 11) & 0x007 );
	}
		
}

uint32_t io_interface_update(uint32_t address, uint32_t set_val, uint32_t default_ret_val)
{
	uint32_t group;
	
	group = address & IO_I_A_GR_MSK;
	group = group >> IO_I_A_GR_POS; //get io group category
	address &= ~IO_I_A_GR_MSK;
	
	if (group == 0)
	{
		// return hoist input type
		return io_interface_get_val(address);
	}
	if (group == 1)
	{
		// return hoist input rising type
		return io_interface_get_rising_val(address);
	}
	if (group == 2)
	{
		// return hoist input falling type
		return io_interface_get_falling_val(address);
	}
	else if (group == 3)
	{
		//return internal service inputs
		io_interface_get_ireq_val(address, set_val);
		return;
	}
	else if (group == 4)
	{
		//return external service inputs
		io_interface_get_ereq_val(address, set_val);
		return;
	}
	else if (group == 5)
	{
		// set outputs
		io_interface_set_out(address, set_val);
		return;
	}
	else if (group == 6)
	{
		// set reqLed mask
		io_interface_set_reqLed_mask(address, set_val);
		return;
	}
	else if (group == 7)
	{
		// set outputs
		io_interface_set_out_CTB(address, set_val);
		return;
	}
	else if (group == 8)
	{
		//return extended external service inputs
		io_interface_get_eereq_val(address, set_val);
		return;
	}	
	else if (group == 9)
	{
		// set reqLed mask
		io_interface_set_ereqLed_mask(address, set_val);
		return;
	}
	else if (group == 10)
	{
		// set reqLed mask
		io_interface_set_eereqLed_mask(address, set_val);
		return;
	}
	else if (group == 11)
	{
		// return ctb door control bits
		if (mb_CTB_in & IO_INTERFACE_MASKER(address + 8))
		{
			//io_interface_readout1 &= ~IO_INTERFACE_MASKER(val);
			return 1;
		}
		else
		{
			return 0;
		}
		return;
	}
	
	else
	{
		return default_ret_val;
	}
}


#define BUTTON_MASK_ENTER	(1ul << 0)
#define BUTTON_MASK_UP		(1ul << 1)
#define BUTTON_MASK_DOWN	(1ul << 2)
#define BUTTON_MASK_ESC		(1ul << 3)
#define BUTTON_MASK_LEFT	(1ul << 4)
#define BUTTON_MASK_RIGHT	(1ul << 5)

#ifndef _SERIAL_DRIVE

#define BUTTON_VAL_ENTER	io_interface_update(22, 0, 0)
#define BUTTON_VAL_UP		io_interface_update(24, 0, 0)
#define BUTTON_VAL_DOWN		io_interface_update(25, 0, 0)
#define BUTTON_VAL_ESC		io_interface_update(26, 0, 0)
#define BUTTON_VAL_LEFT		io_interface_update(27, 0, 0)
#define BUTTON_VAL_RIGHT	io_interface_update(23, 0, 0)

#else

#define BUTTON_VAL_ENTER	io_interface_update(14, 0, 0)
#define BUTTON_VAL_UP		io_interface_update(16, 0, 0)
#define BUTTON_VAL_DOWN		io_interface_update(17, 0, 0)
#define BUTTON_VAL_ESC		io_interface_update(18, 0, 0)
#define BUTTON_VAL_LEFT		io_interface_update(19, 0, 0)
#define BUTTON_VAL_RIGHT	io_interface_update(15, 0, 0)

#endif 


#define BUTTONS_DELAY_TO_AUTO_MULTI_PRESS 650
#define BUTTONS_DELAY_TO_REPRESS 300
#define BUTTONS_COUNT 6

#define BUTTON_ENTER_RE		button_delay_to_repress_arr[0]
#define BUTTON_ESC_RE		button_delay_to_repress_arr[1]
#define BUTTON_UP_RE		button_delay_to_repress_arr[2]
#define BUTTON_DOWN_RE		button_delay_to_repress_arr[3]
#define BUTTON_LEFT_RE		button_delay_to_repress_arr[4]
#define BUTTON_RIGHT_RE		button_delay_to_repress_arr[5]

uint16_t button_delay_to_repress_arr[BUTTONS_COUNT] = {0, 0 ,0 ,0 ,0 ,0};
	
uint32_t button_enter_increment	=	0;
uint32_t button_esc_increment	=	0;
uint32_t button_up_increment	=	0;
uint32_t button_down_increment	=	0;
uint32_t button_left_increment	=	0;
uint32_t button_right_increment =	0;

uByte_t buttons=0xFF,buttons_read_status=0xFF;
void scan_buttons_handler(void)
{
	uint32_t i;
	for (i=0; i<6; i++)
	{
		// a counter to prevent accidental multi-pres from switch debounce
		if (button_delay_to_repress_arr[i] < BUTTONS_DELAY_TO_REPRESS)
		{
			button_delay_to_repress_arr[i]++;
		}
	}
////////////////////////////////////////////////////////////////////////////////
	
	//Enter button
	if((BUTTON_VAL_ENTER == 1))
	{
		if ((buttons & BUTTON_MASK_ENTER) && (BUTTON_ENTER_RE >= BUTTONS_DELAY_TO_REPRESS))
		{
			buttons &= ~BUTTON_MASK_ENTER;
			buttons_read_status &= ~BUTTON_MASK_ENTER; //new value so set read status register
		}
		if(button_enter_increment<BUTTONS_DELAY_TO_AUTO_MULTI_PRESS)
		{
			//button_enter_increment++;
			if(BUTTON_ENTER_RE >= BUTTONS_DELAY_TO_REPRESS) BUTTON_ENTER_RE = 0;
		}
		else
		{
			buttons|=BUTTON_MASK_ENTER; //reset button
		}
	}
	else
	{
		buttons|=BUTTON_MASK_ENTER; //reset button
		button_enter_increment=0;
	}
	/*####################################################################################	*/
	//Esc button
	if((BUTTON_VAL_ESC==1))
	{
		if ((buttons & BUTTON_MASK_ESC) && (BUTTON_ESC_RE >= BUTTONS_DELAY_TO_REPRESS))
		{
			buttons&=~BUTTON_MASK_ESC;
			buttons_read_status&=~BUTTON_MASK_ESC; //new value so set read status register
		}
		if(button_esc_increment<BUTTONS_DELAY_TO_AUTO_MULTI_PRESS)
		{
			button_esc_increment++;
			if(BUTTON_ESC_RE >= BUTTONS_DELAY_TO_REPRESS) BUTTON_ESC_RE = 0;
		}
		else
		{
			buttons|=BUTTON_MASK_ESC; //reset button
		}
	}
	else
	{
		buttons|=BUTTON_MASK_ESC; //reset button
		button_esc_increment=0;
	}
	/*####################################################################################	*/
	//up button
	if((BUTTON_VAL_UP==1))
	{
		if ((buttons & BUTTON_MASK_UP) && (BUTTON_UP_RE >= BUTTONS_DELAY_TO_REPRESS))
		{
			buttons&=~BUTTON_MASK_UP;
			buttons_read_status&=~BUTTON_MASK_UP; //new value so set read status register
		}
		if(button_up_increment<BUTTONS_DELAY_TO_AUTO_MULTI_PRESS)
		{
			button_up_increment++;
			if(BUTTON_UP_RE >= BUTTONS_DELAY_TO_REPRESS) BUTTON_UP_RE = 0;
		}
		else
		{
			buttons|=BUTTON_MASK_UP; //reset button
		}
	}
	else
	{
		buttons|=BUTTON_MASK_UP; //reset button
		button_up_increment=0;
	}
	/*####################################################################################	*/
	//down button
	if((BUTTON_VAL_DOWN==1))
	{
		if ((buttons & BUTTON_MASK_DOWN) && (BUTTON_DOWN_RE >= BUTTONS_DELAY_TO_REPRESS))
		{
			buttons&=~BUTTON_MASK_DOWN;
			buttons_read_status&=~BUTTON_MASK_DOWN; //new value so set read status register
		}
		if(button_down_increment<BUTTONS_DELAY_TO_AUTO_MULTI_PRESS)
		{
			button_down_increment++;
			if(BUTTON_DOWN_RE >= BUTTONS_DELAY_TO_REPRESS) BUTTON_DOWN_RE = 0;
		}
		else
		{
			buttons|=BUTTON_MASK_DOWN; //reset button
		}
	}
	else
	{
		buttons|=BUTTON_MASK_DOWN; //reset button
		button_down_increment=0;
	}
	/*####################################################################################	*/
	//left button
	if((BUTTON_VAL_LEFT==1))
	{
		if ((buttons & BUTTON_MASK_LEFT) && (BUTTON_LEFT_RE >= BUTTONS_DELAY_TO_REPRESS))
		{
			buttons&=~BUTTON_MASK_LEFT;
			buttons_read_status&=~BUTTON_MASK_LEFT; //new value so set read status register
		}
		if(button_left_increment<BUTTONS_DELAY_TO_AUTO_MULTI_PRESS)
		{
			button_left_increment++;
			if(BUTTON_LEFT_RE >= BUTTONS_DELAY_TO_REPRESS) BUTTON_LEFT_RE = 0;
		}
		else
		{
			buttons|=BUTTON_MASK_LEFT; //reset button
		}
	}
	else
	{
		buttons|=BUTTON_MASK_LEFT; //reset button
		button_left_increment=0;
	}
	/*####################################################################################	*/
	//right button
	if((BUTTON_VAL_RIGHT==1))
	{
		if ((buttons & BUTTON_MASK_RIGHT) && (BUTTON_RIGHT_RE >= BUTTONS_DELAY_TO_REPRESS))
		{
			buttons&=~BUTTON_MASK_RIGHT;
			buttons_read_status&=~BUTTON_MASK_RIGHT; //new value so set read status register
		}
		if(button_right_increment<BUTTONS_DELAY_TO_AUTO_MULTI_PRESS)
		{
			button_right_increment++;
			if(BUTTON_RIGHT_RE >= BUTTONS_DELAY_TO_REPRESS) BUTTON_RIGHT_RE = 0;
		}
		else
		{
			buttons|=BUTTON_MASK_RIGHT; //reset button
		}
	}
	else
	{
		buttons|=BUTTON_MASK_RIGHT; //reset button
		button_right_increment=0;
	}
}

uByte_t get_enter_button(void)
{
	if((buttons_read_status & BUTTON_MASK_ENTER)==0)
	{
		buttons_read_status|=BUTTON_MASK_ENTER;
		return 1;
	}
	return 0;
}

uByte_t get_esc_button(void)
{
	if((buttons_read_status & BUTTON_MASK_ESC)==0)
	{
		buttons_read_status|=BUTTON_MASK_ESC;
		return 1;
	}
	return 0;
}

uByte_t get_up_button(void)
{
	if((buttons_read_status & BUTTON_MASK_UP)==0)
	{
		buttons_read_status|=BUTTON_MASK_UP;
		return 1;
	}
	return 0;
}

uByte_t get_down_button(void)
{
	if((buttons_read_status & BUTTON_MASK_DOWN)==0)
	{
		buttons_read_status|=BUTTON_MASK_DOWN;
		return 1;
	}
	return 0;
}

uByte_t get_left_button(void)
{
	if((buttons_read_status & BUTTON_MASK_LEFT)==0)
	{
		buttons_read_status|=BUTTON_MASK_LEFT;
		return 1;
	}
	return 0;
}

uByte_t get_right_button(void)
{
	if((buttons_read_status & BUTTON_MASK_RIGHT)==0)
	{
		buttons_read_status |= BUTTON_MASK_RIGHT;
		return 1;
	}
	return 0;
}

void drop_all_buttons(void)
{
	buttons_read_status = 0xFF;
}