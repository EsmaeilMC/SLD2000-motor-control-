//-------------------------------------------------
// Shenzhen TOPWAY Technology Co.,Ltd.
// FONT Size:         多国字库
// Interface:         serial mode
// by:                GuoQ
// Date:              2012-01-04
// note:    显示模块为LM6063N
//-------------------------------------------------

#ifndef GH12864_C_
#define GH12864_C_

#include <GH12864.h>
#include <GIO.h>




uchar   ContrastLevel; 




//-----------------------------------
// LCM  IO Routine
//-----------------------------------
//#define lcd_s_mode

#ifdef lcd_s_mode

void SdCmd(uByte_t Command)       //send command
{
    SCLK= 0;
    A0  = 0;
    _CS1 = 0;
    //delay_ms(1);
    SID = Command & 0x80;  SCLK= 1;  SCLK= 0; 
    SID = Command & 0x40;  SCLK= 1;  SCLK= 0; 
    SID = Command & 0x20;  SCLK= 1;  SCLK= 0;  
    SID = Command & 0x10;  SCLK= 1;  SCLK= 0;  
    SID = Command & 0x08;  SCLK= 1;  SCLK= 0;  
    SID = Command & 0x04;  SCLK= 1;  SCLK= 0;  
    SID = Command & 0x02;  SCLK= 1;  SCLK= 0;  
    SID = Command & 0x01;  SCLK= 1;  SCLK= 0; 
    _CS1 = 1;
}



void SdData(uByte_t DData)        //send data
{
    SCLK= 0;
    A0  = 1;
    _CS1 = 0;
    //delay_ms(1);
    SID = DData & 0x80;  SCLK= 1;  SCLK= 0; 
    SID = DData & 0x40;  SCLK= 1;  SCLK= 0; 
    SID = DData & 0x20;  SCLK= 1;  SCLK= 0;  
    SID = DData & 0x10;  SCLK= 1;  SCLK= 0;  
    SID = DData & 0x08;  SCLK= 1;  SCLK= 0;  
    SID = DData & 0x04;  SCLK= 1;  SCLK= 0;  
    SID = DData & 0x02;  SCLK= 1;  SCLK= 0;  
    SID = DData & 0x01;  SCLK= 1;  SCLK= 0;
    _CS1 = 1;
}
#else
void SdCmd(uByte_t Command)       //send command
{
    Wr(1);
    Rd(1);
    A0(0);
    _CS1(0);
    //delay_ms(1);
    lc_out(Command);
	//lc_out(0xaa);
    Wr(0);
    Wr(1); 
    _CS1(1);
}
void SdData(uByte_t DData)        //send data
{
    Wr(1);
    Rd(1);
    A0(1);
    _CS1(0);
    lc_out(DData);
    Wr(0);
    Wr(1);
    _CS1(1);
}
#endif
//-----------------------------------
// Write a Screen
//-----------------------------------


//-----------------------------------
// Write a Screen of Pattern
//-----------------------------------
void CLRRAM(void)
{
	uByte_t i, j;
	for(i=0;i<8;i++)
	    {
    	SdCmd(0xb0 | i);	// select page 0~7
    	SdCmd(0x10);	    // start form column 0
    	SdCmd(0x00);	    // (2byte command)
     	for(j=0;j<128;j++)
     		{
     	
     		SdData(0x00);
     	
 		    }
        }
}

//-----------------------------------
// Init LCD module
//-----------------------------------
void set_contrast(uByte_t contrast)
{
    ContrastLevel=contrast; 
	SdCmd(0x81);            // E-Vol setting
    SdCmd(ContrastLevel);   // (2byte command)
}
void init_glcd(void)
{

    BKL(1);
    _RST(1);                 // hardware reset LCD module
    delay_ms(10);
    _RST(0);
    delay_ms(10);
    _RST(1);
    delay_ms(10);
        
    ContrastLevel=0x17;     // default Contrast Level
    
	SdCmd(0xaf);            // display on
	SdCmd(0x40);            // display start line=0
    SdCmd(0xa0);            // ADC=0
    SdCmd(0xa6);            // normal display
    SdCmd(0xa4);            // Display all point = off
    
    SdCmd(0xa2);            // LCD bias = 1/9
    SdCmd(0xc8);            // Common output mode select= reverse


    SdCmd(0x2f);            // Power control = all on

   	SdCmd(0x25);			// RA/RB setting

   	SdCmd(0xf8);SdCmd(0x00);// Booster Ratio Set= 2x,3x,4x (2byte command)

	SdCmd(0x81);            // E-Vol setting
    SdCmd(ContrastLevel);   // (2byte command)
}
void display_off(void)
{
    SdCmd(0xae);            // display off
    _RST(0);
    BKL(0);
}

#endif /* GH12864_C_ */
