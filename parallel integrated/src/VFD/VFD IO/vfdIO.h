/*
 * vfdIO.h
 *
 * Created: 27/01/2017 10:52:47 ص
 *  Author: ahmed
 */ 


#ifndef VFDIO_H_
#define VFDIO_H_

#include <asf.h>
#include <SEDACIO.h>
#include <io_interface.h>

#define LED_ST1_set			pio_set(PIOC, PIO_PC30)
#define LED_ST1_clear		pio_clear(PIOC, PIO_PC30)
#define LED_ST1_toggle		pio_toggle_pin(PIO_PC30_IDX)

void ST1_LED(void);

#define LED_ST2_set			pio_set(PIOC, PIO_PC31)
#define LED_ST2_clear		pio_clear(PIOC, PIO_PC31)
#define LED_ST2_toggle		pio_toggle_pin(PIO_PC31_IDX)

void ST2_LED(void);

#define Fan_On				pio_set(PIOD, PIO_PD31)
#define Fan_Off				pio_clear(PIOD, PIO_PD31)

#define vfd_brake(x)		sedacio_setOutputSignal(BRKS, x)

#define SCRELAY_On			pio_set(PIOC, PIO_PC25)
#define SCRELAY_Off			pio_clear(PIOC, PIO_PC25)

#define SC_LEVEL_LOW	/*SC_LEVEL_HIGH*/ pio_set(PIOE, PIO_PE4)
#define SC_LEVEL_HIGH	pio_clear(PIOE, PIO_PE4)
#endif /* VFDIO_H_ */