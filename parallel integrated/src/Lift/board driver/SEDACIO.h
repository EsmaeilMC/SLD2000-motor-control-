/*
 * SEDACIO.h
 *
 * Created: 13/11/2015 02:21:36 م
 *  Author: ahmed
 */ 


#ifndef SEDACIO_H_
#define SEDACIO_H_

#include <pio.h>
#include <def.h>
#include <GIO.h>
#include <io_interface.h>
#include <drive_interface_data.h>


typedef enum
{
	BRKS,
	CAM,
	LTMR,
	ARD,
	AUP,
	ADN,
	MOC,
	GNG,
	ARDTOFF,
	FX,
	D1_CLOSE,
	D1_OPEN,
	D2_CLOSE,
	D2_OPEN,
	D3_CLOSE,
	D3_OPEN
}input_sigType;

void sedacio_setOutputSignal(input_sigType O_signal, uint32_t val);
void sedacio_output_handler(void);
int32_t sedacio_is_output_assigned(input_sigType o_signal);
/* LCD definition
definition for Graphical LCD GH12864

*/
#define LCD_PORT	PIOC

#define LCD_WR		PIO_PB18
#define LCD_RD		PIO_PB19
#define LCD_BL		PIO_PC19
#define LCD_SID		PIO_PC18
#define LCD_CS1		PIO_PA3
#define LCD_CS2		PIO_PC12
#define LCD_A0		PIO_PB17
#define LCD_RST		PIO_PA2
#define LCD_TAB_DO	PIO_PC11
#define LCD_SCLK	PIO_PC17

/*
	Board LED definitions
*/
#define LED_heart_beat_set			pio_set(PIOA, PIO_PA4)
#define LED_heart_beat_clear		pio_clear(PIOA, PIO_PA4)
#define LED_heart_beat_toggle		pio_toggle_pin(PIO_PA4_IDX)

#define LED_ph_correct_set			pio_set(PIOA, PIO_PA6)
#define LED_ph_correct_clear		pio_clear(PIOA, PIO_PA6)
#define LED_ph_correct_toggle		pio_toggle_pin(PIO_PA6_IDX)


//#define march_cam(x)    io_interface_update(menue_lift_data_applied.m_outputs[1] | (7 << IO_I_A_GR_POS), x, 2)
#define march_cam(x)		sedacio_setOutputSignal(CAM, x); \
							io_interface_update(1 | (7 << IO_I_A_GR_POS), x, 2)
							
#define march_light(x)		sedacio_setOutputSignal(LTMR, x); \
							io_interface_update(0 | (7 << IO_I_A_GR_POS), x, 2)

#define m_ers(x)			sedacio_setOutputSignal(ARD, x)
#define ar_up(x)			sedacio_setOutputSignal(AUP, x)
#define ar_dwn(x)			sedacio_setOutputSignal(ADN, x)
#define m_mainOutput(x)     sedacio_setOutputSignal(MOC, x)
#define m_fx(x)				sedacio_setOutputSignal(FX, x)
#define m_gong(x)			sedacio_setOutputSignal(GNG, x)

#define m_d1open(x)			sedacio_setOutputSignal(D1_OPEN, x)
#define m_d1close(x)			sedacio_setOutputSignal(D1_CLOSE, x)
#define m_d2open(x)			sedacio_setOutputSignal(D2_OPEN, x)
#define m_d2close(x)			sedacio_setOutputSignal(D2_CLOSE, x)
#define m_d3open(x)			sedacio_setOutputSignal(D3_OPEN, x)
#define m_d3close(x)			sedacio_setOutputSignal(D3_CLOSE, x)

#define ctb_activate_door1(x)				io_interface_update((8) | (7 << IO_I_A_GR_POS), x, 2)
#define ctb_activate_door2(x)				io_interface_update((9) | (7 << IO_I_A_GR_POS), x, 2)
#define ctb_close_door1(x)					io_interface_update((10) | (7 << IO_I_A_GR_POS), x, 2)
#define ctb_close_door2(x)					io_interface_update((11) | (7 << IO_I_A_GR_POS), x, 2)
#define ctb_light_curtain1_ignore(x)		io_interface_update((12) | (7 << IO_I_A_GR_POS), x, 2)
#define ctb_light_curtain2_ignore(x)		io_interface_update((13) | (7 << IO_I_A_GR_POS), x, 2)
#define ctb_force_door_close(x)				io_interface_update((14) | (7 << IO_I_A_GR_POS), x, 2)
#define ctb_ignore_door_limits(x)			io_interface_update((15) | (7 << IO_I_A_GR_POS), x, 2)

#define ctb_light_curtain_fb				(io_interface_update((6) | (11 << IO_I_A_GR_POS), 0, 2))

#define CTB_DOOR_STATE_OPEN					(1)
#define CTB_DOOR_STATE_CLOSE				(0)
#define CTB_DOOR_STATE_OPENING				(3)
#define CTB_DOOR_STATE_CLOSING				(2)
#define CTB_DOOR_STATE_ABNORMAL_FEEDBACK	(4)

#define ctb_door1_state						(io_interface_get_door_message(0))
#define ctb_door1_state_open				(ctb_door1_state == CTB_DOOR_STATE_OPEN)
#define ctb_door1_state_close				(ctb_door1_state == CTB_DOOR_STATE_CLOSE)
#define ctb_door1_state_opening				(ctb_door1_state == CTB_DOOR_STATE_OPENING)
#define ctb_door1_state_closing				(ctb_door1_state == CTB_DOOR_STATE_CLOSING)
#define ctb_door1_state_abnormal_feedback	(ctb_door1_state == CTB_DOOR_STATE_ABNORMAL_FEEDBACK)

#define ctb_door2_state						(io_interface_get_door_message(1))
#define ctb_door2_state_open				(ctb_door2_state == CTB_DOOR_STATE_OPEN)
#define ctb_door2_state_close				(ctb_door2_state == CTB_DOOR_STATE_CLOSE)
#define ctb_door2_state_opening				(ctb_door2_state == CTB_DOOR_STATE_OPENING)
#define ctb_door2_state_closing				(ctb_door2_state == CTB_DOOR_STATE_CLOSING)
#define ctb_door2_state_abnormal_feedback	(ctb_door2_state == CTB_DOOR_STATE_ABNORMAL_FEEDBACK)

#define h_open_set(x)      (io_interface_update((f_max + 2) | (3 << IO_I_A_GR_POS), x, 0))
#define h_close_set(x)     (io_interface_update((f_max + 1) | (3 << IO_I_A_GR_POS), x, 0))

#define ereqGroup ((collect==0)?4:3)
#define ereqAdd(x) ((collect==0)?x:(x+8))

#define IO_YARD(x)			sedacio_setOutputSignal(ARDTOFF, x)

#ifndef _SERIAL_DRIVE

#define get_ireq(add, y)	io_interface_update(add | (3 << IO_I_A_GR_POS), y, 2)
#define get_ereq(add, y)	io_interface_update(ereqAdd(add) | (ereqGroup << IO_I_A_GR_POS), y, 2)
#define get_eereq(add, y)	io_interface_update(add + 16 | (3 << IO_I_A_GR_POS), y, 2)

#define set_ireq_ledMask(add, y)	io_interface_update(add | (6 << IO_I_A_GR_POS), y, 2)
#define set_ereq_ledMask(add, y)	io_interface_update(add + 8 | (6 << IO_I_A_GR_POS), y, 2)
#define set_eereq_ledMask(add, y)	io_interface_update(ereqAdd(add) | (6 << IO_I_A_GR_POS), y, 2)

#else

#define get_ireq(add, y)	io_interface_update(add | (3 << IO_I_A_GR_POS), y, 2)
#define get_ereq(add, y)	io_interface_update(add | (4 << IO_I_A_GR_POS), y, 2)
#define get_eereq(add, y)	io_interface_update(add | (8 << IO_I_A_GR_POS), y, 2)

#define set_ireq_ledMask(add, y)	io_interface_update(add | (6 << IO_I_A_GR_POS), y, 2)
#define set_ereq_ledMask(add, y)	io_interface_update(add | (9 << IO_I_A_GR_POS), y, 2)
#define set_eereq_ledMask(add, y)	io_interface_update(add | (10 << IO_I_A_GR_POS), y, 2)

#endif



#define  h(x)	(menue_lift_data_applied.m_hoist[x])
#define  hoist(x)	((h(x) > 13)?(h(x) + 14):h(x))

#define  i_level(x)		(((hoist(x) >= 30)?250:0) << IO_I_A_GR_POS)
#define  i_rising(x)	(((hoist(x) >= 30)?250:1) << IO_I_A_GR_POS)
#define  i_falling(x)   (((hoist(x) >= 30)?250:2) << IO_I_A_GR_POS)

#define E0 1//io_interface_update(28, 0, 0)
#define E1 1//io_interface_update(29, 0, 0)

// hoist terminals
#define mstop			1//(io_interface_update(hoist(0) | i_level(0), 0, 0))
#define sel				1//(io_interface_update(hoist(1) | i_rising(1), 0, 0))
#define Estop			1//(io_interface_update(hoist(2) | i_level(2), 0, 0))
#define shk				1//(io_interface_update(hoist(3) | i_level(3), 0, 0))
#define lock			1//(io_interface_update(hoist(4) | i_level(4), 0, 0))
#define fire			0//(io_interface_update(hoist(5) | i_level(5), 0, 0))
#define mnt				1//(io_interface_update(hoist(6) | i_level(6), 0, 0)==0)
#define mup				(io_interface_update(hoist(7) | i_level(7), 0, 0))
#define mdwn			(io_interface_update(hoist(8) | i_level(8), 0, 0))
#define upfdl0			1//(io_interface_update(hoist(9) | i_level(9), 0, 0))
#define dnfdl0			1//(io_interface_update(hoist(10) | i_level(10), 0, 0))
#define fw				0//(io_interface_update(hoist(11) | i_level(11), 0, 0))
#define ow				0//(io_interface_update(hoist(12) | i_level(12), 0, 0))
#define h_open_aux      (io_interface_update(hoist(13) | i_level(13), 0, 0))
#define h_close_aux     (io_interface_update(hoist(14) | i_level(14), 0, 0))

#define h_open			(io_interface_update((f_max + 2) | (3 << IO_I_A_GR_POS), 0, 0) | h_open_aux)
#define h_close			(io_interface_update((f_max + 1) | (3 << IO_I_A_GR_POS), 0, 0) | h_close_aux)

#define ersin			0//(io_interface_update(hoist(15) | i_level(15), 0, 0))
#define mConFB			1//((io_interface_update(hoist(16) | i_level(16), 0, 0))==0)
#define bkConFB			1//((io_interface_update(hoist(17) | i_level(17), 0, 0))==0)
#define fxConFB			1//(io_interface_update(hoist(18) | i_level(18), 0, 0))

#define upfl			1//(io_interface_update(hoist(19) | i_level(19), 0, 2))
#define dnfl			1//(io_interface_update(hoist(20) | i_level(20), 0, 2))

#define upfdl1			(io_interface_update(hoist(21) | i_level(21), 0, 0))
#define upfdl2			(io_interface_update(hoist(22) | i_level(22), 0, 0))
#define dnfdl1			(io_interface_update(hoist(23) | i_level(23), 0, 0))
#define dnfdl2			(io_interface_update(hoist(24) | i_level(24), 0, 0))

#endif /* SEDACIO_H_ */