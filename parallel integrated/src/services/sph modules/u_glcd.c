/*
 * u_glcd.c
 *
 * Created: 14/11/2015 09:37:39 ص
 *  Author: ahmed
 */ 


#include <u_glcd.h>
#include <GH12864.h>

char temp_surface1[1312];
char temp_surface2[1312];

char *temp_surface = temp_surface1;
char *surface_print = temp_surface2;

char temp[21];
char *itos(uint64_t val)
{

	char *temp_ptr=&temp[20];
	temp[20]=0;
	if(val > 0)
	{
		while(val)
		{
			*--temp_ptr = (val%10)|0x30;
			val/=10;
		}
	}
	else *--temp_ptr = 0x30;
	return temp_ptr;
}

char *sitos(int64_t val)
{

	char *temp_ptr=&temp[20], sign = 0;
	temp[20] = 0;
	
	if(val < 0)
	{
		sign = 1;
		val = -val;
	}
	
	if(val > 0)
	{
		while(val)
		{
			*--temp_ptr = (val%10)|0x30;
			val/=10;
		}
	}
	else *--temp_ptr = 0x30;
	
	if (sign)
	{
		*--temp_ptr = '-';
	}
	
	return temp_ptr;
}
void glcd_clear(void)
{
    unsigned int i;
    for(i=0; i<1024; i++) temp_surface[i]=0;
}
void u_glcd_send_line(uchar *dis_data, uchar page)
{
	uint32_t i;
	uchar tmp_data;
	
	SdCmd(0xb0 | page);	// select page 0~7
	SdCmd(0x10);	    // start from column 0
	SdCmd(0x00);	    // (2byte command)
	
	for (i=0; i<128; i++)
	{
		tmp_data = *(dis_data+i);
		SdData(tmp_data);
	}
}

void UpdateScreen(uchar *DisplayData)    // DisplayData should be 164x64/8 = 1312byte
{
	uchar TempData;
	uchar i, j;
	for(i=0;i<8;i++)
	    {
    	SdCmd(0xb0 | i);	// select page 0~7
    	SdCmd(0x10);	    // start from column 0
    	SdCmd(0x00);	    // (2byte command)
		u_glcd_send_line(DisplayData + (i*128), i);
        }
}

void u_glcd_surfaceFlip()
{
	if (temp_surface == temp_surface1) 
	{
		surface_print = temp_surface1;
		temp_surface = temp_surface2;
	}
	else 
	{
		surface_print = temp_surface2;
		temp_surface = temp_surface1;
	}
}

uchar lcd_page_counter = 0;
void u_glcd_screen_updater(void)
{
	
	u_glcd_send_line(surface_print + (lcd_page_counter * 128), lcd_page_counter);
	if(++lcd_page_counter >= 8) 
	{
		lcd_page_counter = 0;
	}
}

void lcd_update(void)
{
	u_glcd_surfaceFlip();
	return;
    UpdateScreen(temp_surface);
}
//---------------------------
void flip_line(char page,char x,char mask)
{
    temp_surface[(page * 128)+x]^=mask;
}
void flip_v_line(char y,char x,char length)
{
    char page,bits,msk,tmp;
    page=y>>3;
    bits=y&0x07;
    

    msk=(0xFF << bits);//get mask
    if(length < (8-bits))
    {
        // short mask from below
        tmp = 0xFF >> (8 - bits - length );
        msk&= tmp;
        length=0;
    }
    else length-=(8-bits);
        
    flip_line(page,x,msk);
    page++;
    
    while(length>=8)
    {
        flip_line(page,x,0xFF);
        length-=8;
        page++;            
    }
    
    // the remainder of the flip
    msk = 0xFF >> (8 - length );
    flip_line(page,x,msk);
}
//----------------------
void goto_col(unsigned char x)
{
   SdCmd(0x10 | (x>>4));	    // start form column x
   SdCmd(x&0x0F);	    // start form column x
   //prevX=x;
}
//----------------------
void goto_row(unsigned char y)
{
   SdCmd(0xb0 | y);	// select page 0~7

}

void glcd_write_Tbyte(uchar data)
{
    uchar page,bits;
    uchar * data1,*data2;
    page=(d_y >> 3);
    bits=d_y & 0x07;
    
    data1=&temp_surface[page * 128 + d_x]; //first partial byte
    data2=&temp_surface[(page + 1) * 128 + d_x]; // last partial byte
    
    *data1 &=(0xff >> (8-bits));
    *data2 &=(0xff << bits);
    *data1 |=(data << bits);
    *data2 |=(data >> (8-bits));
}
//----------------------
void glcd_write(unsigned char b)
{
   //SdData(b);
   glcd_write_Tbyte(b);
   if(++d_x > 127)
   {
        d_x=0;
   }
}
//------------------------
void out_image(uchar x, uchar y, const char *DisplayData)    // DisplayData should be 164x64/8 = 1312byte
{
	uchar TempData;
	uchar i, j;
    uint width,height;
    

    width=*DisplayData + ((*(DisplayData+1))<<8);
    DisplayData+=2;
    height=*DisplayData + ((*(DisplayData+1))<<8);
    if(height&0x0007) height+=8;
    height=height>>3;
    
    DisplayData+=2;

	for(i=0;i<height;i++)
	    {
    	d_y=(8 * i)+y;	// select page 0~7
    	d_x=x;
     	for(j=0;(j< width );j++)
     		{
     		TempData=*DisplayData;
     		glcd_write(TempData);
            DisplayData++;
 		    }
        }
}


void glcd_dot(void)
{
    char page,bits;
    uchar * data1;
    page=(d_y >> 3);
    bits=d_y & 0x07;
    
    data1=&temp_surface[page * 128 + d_x]; //first partial byte
    *data1 |=1 << bits;    
}
//---------------------------
// x1 must be less than x2
void d_line(uchar x1,uchar x2,uchar y1, uchar y2)
{
    float m,res;
    float x;
    
    if(x1==x2)
    {
        d_x=x1;
        d_y=y1;
        if(y2>=y1)
        {
            while(d_y<=y2)
            {
                glcd_dot();
                d_y++;
            }
        }
        else
        {
            while(d_y>=y2)
            {
                glcd_dot();
                d_y--;
            } 
        }
        return;       
    }
    if(x2<x1)
    {
        x=x1;
        x1=x2;
        x2=x1;
    }    
    res=(y2-y1);
    x=(x2-x1);
    
    m=res / x;
    x=0;

        d_x=x1;
        while(d_x<=x2)
        {
        
            d_y = (x * m) + y1;
            glcd_dot();
            if(++d_x > 127)
            {
                d_x=0;
                return;
            }
            x+=1;
        }

}

//---------------------------

void flip_rect(struct rect rec)
{
    char i;
    for(i=rec.x;i<rec.x+rec.width;i++) flip_v_line(rec.y,i,rec.length);
}

void draw_rect(struct rect rec)
{
    d_line(rec.x , rec.x+rec.width-1 , rec.y , rec.y);
    d_line(rec.x,rec.x, rec.y , rec.y+rec.length-1);
    d_line(rec.x , rec.x+rec.width-1 , rec.y+rec.length-1 , rec.y+rec.length-1);
    d_line(rec.x+rec.width-1 , rec.x+rec.width-1 ,rec.y , rec.y+rec.length-1);
}
char out_char(uchar c,uByte_t u)
{
  uByte_t i;
  char length,uline=0;
  if(u) uline=0x80;

  length=font[9*c];
  for(i=1;i<=length;i++)
  {
     glcd_write(font[(9*c)+i] | uline);
  }
  return length;
}
char putIt(unsigned char c)
{
  byte i;
  char length;
  length=font[9*c];
  for(i=1;i<=length;i++)
  {
     glcd_write(font[(9*c)+i]);
  }
  return length;
}
uchar get_font_width(unsigned char c)
{
  return font[9*c];
}
void glcd_gotoxy(char x,char y)
{
    d_x=x; d_y=y;
    //goto_col(x);
    //goto_row(y);
}
//--------------------------
void glcd_puts(char *c,unsigned char l)
{
    char special = 0;
    char initial=0;
    char _c_width;
    uchar temp_char=0;
    char connection=0; // flags if the charcter connected in the front and rear 
    if(l) // arabic detected
    {
        while(*c)
        {
            //get the لا 
            if((*c == 225) && (*(c+1) == 199))
                special = 249;
            else if((*c == 225) && (*(c+1) == 195))
                special = 231;
            else if((*c == 225) && (*(c+1) == 194))
                special = 232;
            else if((*c == 225) && (*(c+1) == 197))
                special = 233;
                
            if(special)
            {
                /*
                glcd_putchar(special,x,y,1);
                i+=2;
                x -= 8;
                special = 0;
                continue;
                */
            }    
            if(initial)// if this is not the initial charcter
            {
                if((map[*(c-1)-193][5]) && (map[(*c)-193][4]) && (*c >= 193) && (*(c-1) >= 193))
                {
                    connection = 1;
                }
            }
            if((map[*(c+1)-193][4]) && (map[(*c)-193][5]) && (*c >= 193) && (*(c+1) >= 193))
            {
                    connection |= 2;
            }
            if(*c >= 193)
            {
                switch(connection)
                {
                    case 0:
                    temp_char=map[*c-193][0];
                    break;
                    case 1:
                    temp_char=map[*c-193][2];
                    break;
                    case 2:
                    temp_char=map[*c-193][1];
                    break;
                    case 3:
                    temp_char=map[*c-193][3];
                    break;                    
                }
            }
            else temp_char = *c;
            _c_width=get_font_width(temp_char);//get the width of char
            if(d_x>=_c_width) d_x-=_c_width;
            else
            {
                d_x=128-_c_width;
                d_y++;
            }
            glcd_gotoxy(d_x,d_y);
            putIt(temp_char);

            d_x-=_c_width;
            //return;
            c++;
            connection=0;
            initial=1;
        }           
    }
    else // normal english
    {
        while(*c)
        {
            temp_char = *c;
            _c_width=get_font_width(temp_char);//get the width of char
            if((128-d_x)<_c_width) 
            {
                d_x=0;
                d_y+=8;
            }
            putIt(temp_char);
            //return;
            (c++);
        }
    }  
}

void glcd_putsf(const char *c,unsigned char l)
{
    char special = 0;
    char initial=0;
    char _c_width;
    unsigned char temp_char=0;
    char connection=0; // flags if the charcter connected in the front and rear 
    if(l) // arabic detected
    {
        while(*c)
        {
            //get the لا 
            if((*c == 225) && (*(c+1) == 199))
                special = 249;
            else if((*c == 225) && (*(c+1) == 195))
                special = 231;
            else if((*c == 225) && (*(c+1) == 194))
                special = 232;
            else if((*c == 225) && (*(c+1) == 197))
                special = 233;
                
            if(special)
            {
                /*
                glcd_putchar(special,x,y,1);
                i+=2;
                x -= 8;
                special = 0;
                continue;
                */
            }    
            if(initial)// if this is not the initial charcter
            {
                if((map[*(c-1)-193][5]) && (map[(*c)-193][4]) && (*c >= 193) && (*(c-1) >= 193))
                {
                    connection = 1;
                }
            }
            if((map[*(c+1)-193][4]) && (map[(*c)-193][5]) && (*c >= 193) && (*(c+1) >= 193))
            {
                    connection |= 2;
            }
            if(*c >= 193)
            {
                switch(connection)
                {
                    case 0:
                    temp_char=map[*c-193][0];
                    break;
                    case 1:
                    temp_char=map[*c-193][2];
                    break;
                    case 2:
                    temp_char=map[*c-193][1];
                    break;
                    case 3:
                    temp_char=map[*c-193][3];
                    break;                    
                }
            }
            else temp_char = *c;
            _c_width=get_font_width(temp_char);//get the width of char
            if(d_x>=_c_width) d_x-=_c_width;
            else
            {
                d_x=128-_c_width;
                d_y++;
            }
            //glcd_gotoxy(d_x,d_y);
            putIt(temp_char);
            d_x-=_c_width;
            //return;
            (c++);
            connection=0;
            initial=1;
        }           
    }
    else // normal english
    {
        while(*c)
        {
            temp_char = *c;
            _c_width=get_font_width(temp_char);//get the width of char
            if((128-d_x)<_c_width) 
            {
                d_x=0;
                d_y+=8;
            }
            putIt(temp_char);
            //return;
            (c++);
        }
    }   
}


//---------------------------
// text formating pre-processors
//-------------------------------
unsigned char get_text_width(const char *c,unsigned char count)
{
    char special = 0;
    char initial=0;
    char _c_width,ret_width=0;
    unsigned char temp_char=0;
    char connection=0; // flags if the charcter connected in the front and rear 

        while(count-->0)
        {
            //get the لا 
            if((*c == 225) && (*(c+1) == 199))
                special = 249;
            else if((*c == 225) && (*(c+1) == 195))
                special = 231;
            else if((*c == 225) && (*(c+1) == 194))
                special = 232;
            else if((*c == 225) && (*(c+1) == 197))
                special = 233;
                
            if(special)
            {
                /*
                glcd_putchar(special,x,y,1);
                i+=2;
                x -= 8;
                special = 0;
                continue;
                */
            }    
            if(initial)// if this is not the initial charcter
            {
                if((map[*(c-1)-193][5]) && (map[(*c)-193][4]) && (*c >= 193) && (*(c-1) >= 193))
                {
                    connection = 1;
                }
            }
            if((map[*(c+1)-193][4]) && (map[(*c)-193][5]) && (*c >= 193) && (*(c+1) >= 193))
            {
                    connection |= 2;
            }
            if(*c >= 193)
            {
                switch(connection)
                {
                    case 0:
                    temp_char=map[*c-193][0];
                    break;
                    case 1:
                    temp_char=map[*c-193][2];
                    break;
                    case 2:
                    temp_char=map[*c-193][1];
                    break;
                    case 3:
                    temp_char=map[*c-193][3];
                    break;                    
                }
            }
            _c_width=get_font_width(temp_char);//get the width of char
            ret_width+=_c_width;


            c++;
            connection=0;
            initial=1;
        }
        return ret_width;
}
unsigned int get_fitted_word(const char *c,unsigned char awidth)
{
    //function to get how many words to fit in a single line
    unsigned char nchar=0,nwidth=0,temp_calc_w=0,temp_calc_char=0;
    char lang=0,t_lang;//language flag 2 = arabic 1 = english 0 unknown
    const char *c1;
    //unsigned int ret_val=0;
    //en 65 2 122
    //Ar 187 & 192 2 251
    c1=c;
    while( awidth > (nwidth + temp_calc_w) )
    {
        //if the available width > needed width continue
        if(*c==0)
        {
            if(lang!=2)
            {
                //english or other l 2 r languages
                //word break
                nwidth+= temp_calc_w; //add the last word
            
                nchar+= temp_calc_char; //add the last word no of char
            }
            else
            {
                //Arabic
                //word break
                nwidth = nwidth + get_text_width(c1, temp_calc_char);
                
                nchar+= temp_calc_char; //add the last word no of char            
            }
            break; //if end of string return
        }
        //detect language
        if((*c>47)&&(*c<123))
        {
            //english detected
            t_lang=1;
            //putIt('E');
        }
        else if(((*c>191)&&(*c<252))||(*c==187))
        {
            //arabic detected
            t_lang=2;
            //putIt('A');
        }
        else t_lang=0; //unknown language
        
        //if language change break;
        if(lang==0) lang=t_lang;//if unknown lang set language
        else
        {
            if(t_lang!=lang)
            {
                if(t_lang!=0)
                {
                    //language change detected
                    //word break
                    nwidth+= temp_calc_w; //add the last word
                    nchar+= temp_calc_char; //add the last word no of char
                    break;
                }
            }
        }
        if(*c==' ')
        {
            if(lang!=2)
            {
                //english or other l 2 r languages
                //word break
                nwidth+= temp_calc_w; //add the last word
                temp_calc_w=0;
            
                nchar+= temp_calc_char; //add the last word no of char
                temp_calc_char=0;
            }
            else
            {
                //Arabic
                //word break
                nwidth = nwidth + get_text_width(c1, temp_calc_char);
                temp_calc_w=0;
                c1=c-1;
                
                nchar+= temp_calc_char; //add the last word no of char
                temp_calc_char=0;            
            }
        }
        temp_calc_char++;// increment char by 1 [ 1 char added ]
        temp_calc_w = temp_calc_w +  get_font_width(*c);
        c++; //prepare pointer for next check
    }
    

    //putIt(nchar + 0x30);
    if(lang==2) 
    {
        //arabic needed acurate calculations for connected chars
        //nwidth=get_text_width(c1, nchar);
        nchar|=0x80;
        //putIt('A');
    }
    
    return (nchar<<8)+nwidth;
}
void special_putsf(const char *c,uchar l,uchar count)
{
    char special = 0;
    char initial=0;
    char _c_width;
    unsigned char temp_char = 0;
    char connection=0; // flags if the charcter connected in the front and rear 
    if(l) // arabic detected
    {
        while(count-->0)
        {
            //get the لا 
            if((*c == 225) && (*(c+1) == 199))
                special = 249;
            else if((*c == 225) && (*(c+1) == 195))
                special = 231;
            else if((*c == 225) && (*(c+1) == 194))
                special = 232;
            else if((*c == 225) && (*(c+1) == 197))
                special = 233;
                
            if(special)
            {
                /*
                glcd_putchar(special,x,y,1);
                i+=2;
                x -= 8;
                special = 0;
                continue;
                */
            }    
            if(initial)// if this is not the initial charcter
            {
                if((map[*(c-1)-193][5]) && (map[(*c)-193][4]) && (*c >= 193) && (*(c-1) >= 193))
                {
                    connection = 1;
                }
            }
            if((map[*(c+1)-193][4]) && (map[(*c)-193][5]) && (*c >= 193) && (*(c+1) >= 193))
            {
                    connection |= 2;
            }
            if(*c >= 193)
            {
                switch(connection)
                {
                    case 0:
                    temp_char=map[*c-193][0];
                    break;
                    case 1:
                    temp_char=map[*c-193][2];
                    break;
                    case 2:
                    temp_char=map[*c-193][1];
                    break;
                    case 3:
                    temp_char=map[*c-193][3];
                    break;                    
                }
            }
            else temp_char = *c;
            _c_width=get_font_width(temp_char);//get the width of char
            if(d_x>=_c_width) d_x-=_c_width;
            else
            {
                d_x=128-_c_width;
                d_y++;
            }
            //glcd_gotoxy(d_x,d_y);
            putIt(temp_char);
            d_x-=_c_width;
            //return;
            (c++);
            connection=0;
            initial=1;
        }           
    }
    else // normal english
    {
        while(count-->0)
        {
            temp_char = *c;
            _c_width=get_font_width(temp_char);//get the width of char
            if((128-d_x)<_c_width) 
            {
                d_x=0;
                d_y+=8;
            }
            putIt(temp_char);
            //return;
            (c++);
        }
    }   
}
void write_r_text(const char *c, unsigned char rtl)
{
    unsigned char nchar,width_pix,lang,awidth=128;
    unsigned int res;
    if(rtl) d_x=127-d_x; //flip the surface
    while(*c)
    {
        res=get_fitted_word(c,awidth);
        //return;
        //get usefull data of res
        width_pix=res;
        nchar = (res>>8);
        if(nchar&0x80) lang=1;
        else lang=0;
        nchar&=0x7F;
        
        if(width_pix==0)
        {
            //no space to fit text
            awidth=128;
            d_y+=9;
            d_x=0;
            if(rtl) d_x=127-d_x; //flip the surface

        }
        else
        {
            if(rtl)
            {
                //right to left text
                awidth-=width_pix; //dec available width
                if(lang==0) d_x-=width_pix;
                special_putsf(c,lang,nchar);
                if(lang==0) d_x-=width_pix;
                c+=nchar;
            }
            else
            {
                awidth-=width_pix; //dec available width
                if(lang) d_x+=width_pix;
                special_putsf(c,lang,nchar);
                if(lang) d_x+=width_pix;
                c+=nchar;
            }
        }
    }
}
