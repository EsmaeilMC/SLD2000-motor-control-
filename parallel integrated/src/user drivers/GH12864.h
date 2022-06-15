/*
 * GH12864.h
 *
 * Created: 13/11/2015 04:31:35 م
 *  Author: ahmed
 */ 


#ifndef GH12864_H_
#define GH12864_H_

//#include <stdio.h>
//#include <SEDAC components/util/SEDACIO.h>
// From module: Delay routines
#include <def.h>
#include <delay.h>

//============****************************==========
#define    SCLK(x)        pio_set_data(PIOB,LCD_SCLK,x)
#define    TAB_DO(x)      pio_set_data(PIOB,LCD_TAB_DO,x)
#define    _RST(x)        pio_set_data(PIOD,PIO_PD22,x)
#define    A0(x)		  pio_set_data(PIOA,PIO_PA26,x)
#define    _CS1(x)        pio_set_data(PIOA,PIO_PA25,x)
#define    _CS2(x)        pio_set_data(PIOA,PIO_PA25,x)
#define    SID(x)		  pio_set_data(PIOB,LCD_SID,x)

#define    Rd(x)          pio_set_data(PIOA,PIO_PA11,x)
#define    Wr(x)          pio_set_data(PIOD,PIO_PD21,x)
#define    BKL(x)         pio_set_data(PIOD,PIO_PD23,x)

#define lc_out(x) pio_out_data(PIOC, (0xFFu), x, 1)

//#define uchar unsigned char // 0~255




void SdCmd(uByte_t Command);
void SdData(uByte_t DData);
//-----------------------------------
// Write a Screen
//-----------------------------------


//-----------------------------------
// Write a Screen of Pattern
//-----------------------------------
void CLRRAM(void);

//-----------------------------------
// Init LCD module
//-----------------------------------
void set_contrast(uByte_t contrast);
void init_glcd(void);

void display_off(void);




#endif /* GH12864_H_ */