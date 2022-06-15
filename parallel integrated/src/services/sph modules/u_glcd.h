/*
 * u_glcd.h
 *
 * Created: 14/11/2015 09:37:12 ص
 *  Author: ahmed
 */ 


#ifndef U_GLCD_SPH_H_
#define U_GLCD_SPH_H_



//-----------------------
#include <fontbasic.h>
#include <def.h>
//#include <string.h>
//-----------------------
#define cols 128;
#define rows 64;
#define DATAPORT PORTD
#define CS1 PORTC.4
#define CS2 PORTC.3
#define RS  PORTC.2
#define RW  PORTC.1
#define EN  PORTC.0
//-------------------------

#include <GH12864.h>

unsigned char d_x,d_y;

typedef struct rect
{
    char x;
    char y;
    char width;
    char length;
};

extern char temp_surface1[1312];
extern char temp_surface2[1312];

extern char *temp_surface;

void glcd_clear(void);
void UpdateScreen(uchar *DisplayData);    // DisplayData should be 164x64/8 = 1312byte
void u_glcd_screen_updater(void);
void lcd_update(void);
//---------------------------
void flip_line(char page,char x,char mask);
void flip_v_line(char y,char x,char length);
//----------------------
void goto_col(unsigned char x);
//----------------------
void goto_row(unsigned char y);

void glcd_write_Tbyte(uchar data);
//----------------------
void glcd_write(unsigned char b);
//------------------------
void out_image(uchar x, uchar y, const char *DisplayData) ;
void glcd_dot(void);
//---------------------------
// x1 must be less than x2
void d_line(uchar x1,uchar x2,uchar y1, uchar y2);

//---------------------------

void flip_rect(struct rect rec);

void draw_rect(struct rect rec);
char out_char(uchar c,uByte_t u);
char putIt(unsigned char c);
unsigned char get_font_width(unsigned char c);
void glcd_gotoxy(char x,char y);
//--------------------------
void glcd_puts(char *c,unsigned char l);

void glcd_putsf(const char *c,unsigned char l);


//---------------------------
// text formating pre-processors
//-------------------------------
unsigned char get_text_width(const char *c,unsigned char count);
unsigned int get_fitted_word(const char *c,unsigned char awidth);
void write_r_text(const char *c, unsigned char rtl);

void special_putsf(const char *c,uchar l,uchar count);

char *itos(uint64_t val);
char *sitos(int64_t val);

#endif /* U_GLCD_H_ */