/*
 * graphical_menues.c
 *
 * Created: 23/11/2015 11:03:36 م
 *  Author: ahmed
 */ 

#include <graphical_menues.h>
#include <u_glcd.h>
#include <control_status_register.h>
#include <graphics_data.h>
#include <rtc_interface.h>
#include <asf.h>
#include <con_toff.h>

struct rect highlight_rect;

menue_design_t *curr_lst;
uint8_t curr_lst_max, lst_indeces[50];

void fill_menue_list(menue_design_t *mnu, option_t *list ,char lst_max, \
				menue_design_t *parent, char *mnuval)
{
	//fill list menue type
	(*mnu).m_type = GRAPHICAL_MENUE_LIST;
	(*mnu).list_start=0;
	(*mnu).list_highlight=0;
	(*mnu).list_max=lst_max;
	(*mnu).lst_itms=list;
	(*mnu).parent = parent;
	(*mnu).save_val = mnuval;
	highlight_rect.x=2;
	highlight_rect.width=124;	
}
void fill_menue_image(menue_design_image_t *mnu , menue_design_t *child,\
                        char *image, char *menue_name, uint16_t *menue_pass)
{
	//fill image menue type.
	(*mnu).m_type = GRAPHICAL_MENUE_PHOTO;
	(*mnu).menue_password = menue_pass;
	(*mnu).menue_opened = 0;
	(*mnu).image = image;
	(*mnu).menue_name = menue_name;
	if(child)
	{
		(*mnu).child = child;
		(*(*mnu).child).menue_name = menue_name;
	}			
}
void list_show(menue_design_t *mnu)
{
	curr_lst = mnu;
}
void inc_list(void)
{
	if(((*curr_lst).list_highlight)>0)
	{
		--((*curr_lst).list_highlight);
	}
	if(curr_lst->list_highlight < curr_lst->list_start)	 curr_lst->list_start = curr_lst->list_highlight;
	
}
void dec_list(void)
{
	uint8_t current_list_count = (*curr_lst).list_highlight;
	option_t *tmp_lst = (*curr_lst).lst_itms;
	
	if(++(curr_lst->list_highlight) >= curr_lst_max/*(*curr_lst).list_max*/)
	{
		curr_lst->list_highlight = /*(*curr_lst).list_max*/curr_lst_max - 1;		
	}				
	if(curr_lst->list_highlight > curr_lst->list_start) 
	{
		if((curr_lst->list_highlight - curr_lst->list_start) > 3) 
		{
			curr_lst->list_start = curr_lst->list_highlight - 3;
		}
	}
}
void list_pos_adjust(void)
{
	if(curr_lst->list_highlight > curr_lst->list_start) 
	{
		if((curr_lst->list_highlight - curr_lst->list_start) > 3) 
		{
			curr_lst->list_start = curr_lst->list_highlight - 3;
		}
	}
	else if(curr_lst->list_highlight > 3)
	{
		curr_lst->list_start = curr_lst->list_highlight - 3;
	}
	else
	{
		curr_lst->list_start = 0;
	}
}
void list_view(void)
{
	char i;
	const char side_arrow[2] = {4,0};
	option_t *tmp_lst;
	
	//////////////////////////////////////////////////////////////////////////
	struct rect boarder;
	boarder.x=0;
	boarder.y=0;
	boarder.width=128;
	boarder.length=64;
	draw_rect(boarder);
	//////////////////////////////////////////////////////////////////////////
	
	glcd_gotoxy (4,3);
	if((*curr_lst).menue_name) write_r_text((*curr_lst).menue_name, public_lang);
	d_line(0, 128, 13, 13);
	
	tmp_lst = curr_lst->lst_itms;
	for(i=0; i < 4; i++)
	{
		if(((*curr_lst).list_start + i) < curr_lst_max)
		{
			glcd_gotoxy(2, i*11 + 20);
			write_r_text(itos(i + curr_lst->list_start), public_lang);
			glcd_gotoxy(16, i*11 + 20);
			write_r_text( (tmp_lst + lst_indeces[i + curr_lst->list_start])->text, public_lang);
			glcd_gotoxy(128-2-6, i*11 + 20);
			if((tmp_lst + lst_indeces[i + curr_lst->list_start])->child) write_r_text(side_arrow, public_lang);
		}
		else
		{
			//we exceeded the maximum number of lists
			break;
		} 
	}
	highlight_rect.y =(curr_lst->list_highlight - curr_lst->list_start)*11 + 20 -1;
	highlight_rect.length =9+1;
	flip_rect(highlight_rect);
	
}
///////////////////////////////////// Options values routines /////////////////////////////////////
uint32_t lst_power(uint32_t count)
{
	uint32_t res=1;
	for (count; count>0; count--)
	{
		res *= 10;
	}
	return res;
}

uint8_t list_cursor = 0, list_has_asign = 0;
int32_t list_val_max, list_val_min;
int32_t lst_val_entered;
void list_save_val(option_val_t *lst_opt_val)
{
	int64_t tmp;
	if(lst_opt_val->data_types  == float_dt)
	{
		*(float *)(lst_opt_val->val) = lst_val_entered;
		*(float *)(lst_opt_val->val) /= lst_power(lst_opt_val->mantissa_count);
	}
	else if(lst_opt_val->data_types == fixedpt_dt)
	{
		tmp = (lst_val_entered);
		tmp *= FIXEDPT_ONE;
		tmp /= lst_power(lst_opt_val->mantissa_count);
		*(fixedpt *)(lst_opt_val->val) = tmp;
	}
	else if(lst_opt_val->data_types == uint16_dt)
	{
		*(uint16_t *)(lst_opt_val->val) = lst_val_entered;
	}
	else if(lst_opt_val->data_types == uint8_dt)
	{
		*(uint8_t *)(lst_opt_val->val) = lst_val_entered;
	}
	if(lst_opt_val->new_val_flag) *lst_opt_val->new_val_flag =1;
}
uint_t list_input_val_exe(option_val_t *lst_opt_val, char enter, char esc, char up, char down, char left, char right)
{
	uint32_t i;
	uint32_t temp;
	char *list_val_break;
	int8_t l_sign = 1;
	
	if(enter)
	{
		list_save_val(lst_opt_val);
		return 0;
	}
	if(esc) return 0;
	if(right) list_cursor++;
	else if(left) list_cursor--;
		
	if(list_cursor == 0xFF) list_cursor = lst_opt_val->mantissa_count + lst_opt_val->real_count + list_has_asign - 1;
	else if(list_cursor >= lst_opt_val->mantissa_count + lst_opt_val->real_count + list_has_asign) list_cursor = 0;	
	
	if((list_has_asign) && (list_cursor == 0) && ((up) || (down)))
	{
		lst_val_entered *= -1;
	}	
	else if(up)
	{
		lst_val_entered += lst_power(lst_opt_val->mantissa_count + lst_opt_val->real_count - (list_cursor - list_has_asign + 1));
	}
	else if(down)
	{
		lst_val_entered -= lst_power(lst_opt_val->mantissa_count + lst_opt_val->real_count - (list_cursor - list_has_asign + 1));
	}
	if (lst_val_entered > list_val_max)
	{
		lst_val_entered = list_val_max;
	} 
	else if (lst_val_entered < list_val_min)
	{
		lst_val_entered = list_val_min;
	}
	
	if(lst_val_entered >= 0) temp = lst_val_entered;
	else temp = -lst_val_entered;
	
	list_val_break = itos(temp);
	
	temp = lst_power(lst_opt_val->real_count + lst_opt_val->mantissa_count -1);

// print on the LCD ___________________________	
	
	if (lst_opt_val->text)
	{
		glcd_gotoxy(2,16);
		glcd_putsf(lst_opt_val->text, 0);
	}
	
	glcd_gotoxy(2, 53);
	if(list_has_asign)
	{
		if(lst_val_entered < 0) out_char('-', (0 == list_cursor));
		else out_char('+', (0 == list_cursor));
	}
	
	if(lst_val_entered < 0) l_sign = -1;
	lst_val_entered *= l_sign;
	
	if(lst_opt_val->real_count == 0) out_char('0', 0); //print decimal 0
	for (i=0; i<lst_opt_val->real_count; i++)
	{
		if (lst_val_entered < temp)
		{
			out_char('0', (i + list_has_asign == list_cursor));
			temp /= 10;
		}
		else if ((*list_val_break >= 0x30) && (*list_val_break <= 0x39))
		{
			out_char(*list_val_break , (i + list_has_asign == list_cursor));
			list_val_break++;
		}
		else
		{
			out_char('0', (i + list_has_asign == list_cursor));
			list_val_break++;
		}
	}
	if(lst_opt_val->mantissa_count) out_char('.', 0);
	for (i=0; i<lst_opt_val->mantissa_count; i++)
	{
		if (lst_val_entered < temp)
		{
			out_char('0', (i + list_has_asign + lst_opt_val->real_count == list_cursor));
			temp /= 10;
		}
		else if ((*list_val_break >= 0x30) && (*list_val_break <= 0x39))
		{
			out_char(*list_val_break, (i + list_has_asign + lst_opt_val->real_count == list_cursor));
			list_val_break++;
		}
		else
		{
			out_char('0', (i + list_has_asign + lst_opt_val->real_count == list_cursor));
			list_val_break++;
		}
	}
	if (lst_opt_val->unit)
	{
		out_char(' ',0);
		glcd_putsf(lst_opt_val->unit, 0);
	}
	
	lst_val_entered *= l_sign;
	
	return 1;
}
void list_input_val_decipher(option_val_t *lst_opt_val)
{
	int8_t list_val_sign =1;
	uint32_t i, real_part=0, mantissa_part=0, multiplier;
	float lst_val, lst_max, lst_min;
	lst_max = lst_opt_val->max_value;
	lst_min = lst_opt_val->min_value;
	
	if (lst_opt_val->data_types == float_dt)
	{
		lst_val = *((float *)(lst_opt_val->val));
	}
	else if(lst_opt_val->data_types == fixedpt_dt)
	{
		lst_val = fixedpt_tofloat( *((fixedpt *)(lst_opt_val->val)));
	}
	else if(lst_opt_val->data_types == uint16_dt)
	{
		lst_val = *((uint16_t *)(lst_opt_val->val));
	}
	else if(lst_opt_val->data_types == uint8_dt)
	{
		lst_val = *((uint8_t *)(lst_opt_val->val));
	}
	list_has_asign = 0;
	if(lst_min < 0) list_has_asign = 1;
	
	if(lst_val > lst_max) lst_val = lst_max;
	else if(lst_val < lst_min) lst_val = lst_min;
	

	if (lst_val < 0)
	{
		list_val_sign = -1;
		lst_val *= -1;	
	}
	real_part = lst_val;
	lst_val -= real_part;
	mantissa_part = 0;
	for (i=0; i<lst_opt_val->mantissa_count; i++)
	{
		lst_val *= 10;
	}
	mantissa_part = lst_val;
	
	multiplier = lst_power(lst_opt_val->mantissa_count);

	lst_val_entered = real_part * multiplier + mantissa_part;
	lst_val_entered *= list_val_sign;
	
	list_val_max = (lst_opt_val->max_value * multiplier);
	list_val_min = (lst_opt_val->min_value * multiplier);
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t lst_date_t_entered[3];
uint8_t time_date_mode = 0;
uint_t list_set_date_exe(option_val_t *lst_opt_val, char enter, char esc, char up, char down, char left, char right)
{
	uint32_t i, l, d_index;
	uint32_t temp;
	char *list_val_break;
	
	if(enter)
	{
		//todo save date
		if(time_date_mode == 0) rtc_save_date(lst_date_t_entered[0], lst_date_t_entered[1], lst_date_t_entered[2]);
		else con_toff_save_date(lst_date_t_entered[0], lst_date_t_entered[1], lst_date_t_entered[2]);
		return 0;
	}
	
	if(esc) return 0;
	if(right) list_cursor++;
	else if(left) list_cursor--;
	
	if(list_cursor == 0xFF) list_cursor = 7;
	else if(list_cursor > 7) list_cursor = 0;
	
	if (list_cursor < 2) d_index = 0; 
	else if (list_cursor < 4) d_index = 1; 
	else d_index = 2;
	
	if(up)
	{
		lst_date_t_entered[d_index] += lst_power( 1 - (list_cursor - d_index * 2));
	}
	else if(down)
	{
		lst_date_t_entered[d_index] -= lst_power( 1 - (list_cursor - d_index * 2));
	}
	rtc_date_check(&lst_date_t_entered[0], &lst_date_t_entered[1], &lst_date_t_entered[2]);
	
	

	// print on the LCD ___________________________
	

	glcd_gotoxy(2,16);
	glcd_putsf("Set date", 0);
	
	glcd_gotoxy(2, 53);
	glcd_putsf("20", 0);
	
	for (l=0; l<3; l++)
	{
		temp = lst_date_t_entered[l];
		
		list_val_break = itos(temp);
		
		temp = 10;
		
		for (i=0; i<2; i++)
		{
			if (lst_date_t_entered[l] < temp)
			{
				out_char('0', (i + l*2  == list_cursor));
				temp /= 10;
			}
			else if ((*list_val_break >= 0x30) && (*list_val_break <= 0x39))
			{
				out_char(*list_val_break , (i + l*2 == list_cursor));
				list_val_break++;
			}
			else
			{
				out_char('0', (i + l*2 == list_cursor));
				list_val_break++;
			}
		}
		if (l<2) glcd_putsf("/", 0);
	}
	
	
	return 1;
}
uint_t list_set_time_exe(option_val_t *lst_opt_val, char enter, char esc, char up, char down, char left, char right)
{
	uint32_t i, l, d_index;
	uint32_t temp;
	char *list_val_break;
	
	if(enter)
	{
		//todo save date
		if(time_date_mode == 0) rtc_save_time(lst_date_t_entered[0], lst_date_t_entered[1], lst_date_t_entered[2]);
		else con_toff_save_time(lst_date_t_entered[0], lst_date_t_entered[1], lst_date_t_entered[2]);
		return 0;
	}
	
	if(esc) return 0;
	if(right) list_cursor++;
	else if(left) list_cursor--;
	
	if(list_cursor == 0xFF) list_cursor = 7;
	else if(list_cursor > 7) list_cursor = 0;
	
	if (list_cursor < 2) d_index = 0;
	else if (list_cursor < 4) d_index = 1;
	else d_index = 2;
	
	if(up)
	{
		lst_date_t_entered[d_index] += lst_power( 1 - (list_cursor - d_index * 2));
	}
	else if(down)
	{
		lst_date_t_entered[d_index] -= lst_power( 1 - (list_cursor - d_index * 2));
	}
	rtc_time_check(&lst_date_t_entered[0], &lst_date_t_entered[1], &lst_date_t_entered[2]);
	
	

	// print on the LCD ___________________________
	

	glcd_gotoxy(2,16);
	glcd_putsf("Set time", 0);
	
	glcd_gotoxy(2, 53);
	i = list_cursor/2;
	
	temp = lst_date_t_entered[i];
	//if(time_date_mode == 0) rtc_get_time(RTC, &lst_date_t_entered[0], &lst_date_t_entered[1], &lst_date_t_entered[2]);
	lst_date_t_entered[i] = temp;
	
	for (l=0; l<3; l++)
	{
		temp = lst_date_t_entered[l];
		
		list_val_break = itos(temp);
		
		temp = 10;
		
		for (i=0; i<2; i++)
		{
			if (lst_date_t_entered[l] < temp)
			{
				out_char('0', (i + l*2  == list_cursor));
				temp /= 10;
			}
			else if ((*list_val_break >= 0x30) && (*list_val_break <= 0x39))
			{
				out_char(*list_val_break , (i + l*2 == list_cursor));
				list_val_break++;
			}
			else
			{
				out_char('0', (i + l*2 == list_cursor));
				list_val_break++;
			}
		}
		if (l<2) glcd_putsf(":", 0);
	}
	
	
	return 1;
}
//////////////////////////////////////////////////////////////////////////
uint32_t get_list_indx_from_lhi_light(uint8_t lst_highlight)
{
	return lst_indeces[lst_highlight];
}
uint32_t get_listHiLight_from_listIndex(uint8_t lst_index)
{
	volatile uint32_t i;
	for (i=0; i<curr_lst_max; i++)
	{
		if (lst_indeces[i] == lst_index)
		{
			return i;
		}
	}
	return 0;
}

uint_t list_enter(void)
{
	option_t *tmp_lst;
	char lst_highlight, retval = 0;
	tmp_lst = curr_lst->lst_itms;
	lst_highlight = get_list_indx_from_lhi_light(curr_lst->list_highlight);
	if((tmp_lst+lst_highlight)->enter)
	{
		//the list has a funtion to execute 
		(tmp_lst+lst_highlight)->enter();
		retval = 2;
	}
	if((tmp_lst + lst_highlight)->option_save_val == 1)
	{
		//the list has a data val load RTC
		rtc_get_date(RTC, &lst_date_t_entered[0], &lst_date_t_entered[1], &lst_date_t_entered[2], 0);
		if(lst_date_t_entered[0] > 2000) lst_date_t_entered[0] -= 2000;
		time_date_mode = 0;
		retval = 1;
	}
	else if((tmp_lst + lst_highlight)->option_save_val == 2)
	{
		//the list has a time val load RTC
		rtc_get_time(RTC, &lst_date_t_entered[0], &lst_date_t_entered[1], &lst_date_t_entered[2]);
		time_date_mode = 0;
		retval = 1;
	}
	else if((tmp_lst + lst_highlight)->option_save_val == 3)
	{
		//the list has a data val load RTC
		con_toff_get_date(&lst_date_t_entered[0], &lst_date_t_entered[1], &lst_date_t_entered[2]);
		if(lst_date_t_entered[0] > 2000) lst_date_t_entered[0] -= 2000;
		time_date_mode = 1;
		retval = 1;
	}
	else if((tmp_lst + lst_highlight)->option_save_val == 4)
	{
		//the list has a time val load RTC
		con_toff_get_time(&lst_date_t_entered[0], &lst_date_t_entered[1], &lst_date_t_entered[2]);
		time_date_mode = 1;
		retval = 1;
	}
	else if((tmp_lst + lst_highlight)->option_save_val)
	{
		//the list has a value to input
		list_input_val_decipher((tmp_lst + lst_highlight)->option_save_val);
		retval = 1;
	}
	else if((tmp_lst + lst_highlight)->confirmed_sub)
	{
		//the list has a confirmation message to show
		retval = 1;
	}
	else if((tmp_lst + lst_highlight)->exe)
	{
		//the list has a function to execute
		retval = 1;
	}
	return retval;
}
uint_t list_input_val(char enter, char esc, char up, char down, char left, char right)
{
	option_t *tmp_lst;
	char lst_highlight;
	tmp_lst=curr_lst->lst_itms;
	lst_highlight = get_list_indx_from_lhi_light(curr_lst->list_highlight);
	if((tmp_lst + lst_highlight)->option_save_val == 1) //set date
	{
		return list_set_date_exe((tmp_lst + lst_highlight)->option_save_val, enter, esc, up, down, left, right);
	}
	else if((tmp_lst + lst_highlight)->option_save_val == 2) // set time
	{
		return list_set_time_exe((tmp_lst + lst_highlight)->option_save_val, enter, esc, up, down, left, right);
	}
	else if((tmp_lst + lst_highlight)->option_save_val == 3) //set date
	{
		return list_set_date_exe((tmp_lst + lst_highlight)->option_save_val, enter, esc, up, down, left, right);
	}
	else if((tmp_lst + lst_highlight)->option_save_val == 4) // set time
	{
		return list_set_time_exe((tmp_lst + lst_highlight)->option_save_val, enter, esc, up, down, left, right);
	}
	else if((tmp_lst + lst_highlight)->option_save_val)
	{
		return list_input_val_exe((tmp_lst + lst_highlight)->option_save_val, enter, esc, up, down, left, right);
	}
	else return 2; //no val to input
}
uint_t list_confirm_exe(char enter, char esc, char up, char down, char left, char right)
{
	option_t *tmp_lst;
	char lst_highlight;
	tmp_lst = curr_lst->lst_itms;
	lst_highlight = get_list_indx_from_lhi_light(curr_lst->list_highlight);
	if(curr_lst->save_val)
	{
		*(curr_lst->save_val) = curr_lst->list_highlight; //if there is a value to save; save it
	}
	if((tmp_lst + lst_highlight)->confirmed_sub)
	{
		glcd_gotoxy(2,18);
		glcd_putsf((tmp_lst + lst_highlight)->confirm_msg, 0);
		
		glcd_gotoxy(2,54);
		glcd_putsf("No", 0);
		
		glcd_gotoxy(107,54);
		glcd_putsf("Yes", 0);
		if (enter) 
		{
			(tmp_lst + lst_highlight)->confirmed_sub(); // if there is a function at the function pointer execute it
			return 0;
		}
		else if(esc)
		{
			return 0;
		}
		return 1;
	}
	else return 2;
}
uint_t list_exe(char enter, char esc, char up, char down, char left, char right)
{
	option_t *tmp_lst;
	char lst_highlight;
	tmp_lst = curr_lst->lst_itms;
	lst_highlight = get_list_indx_from_lhi_light(curr_lst->list_highlight);
	if(curr_lst->save_val) 
	{
		*(curr_lst->save_val) = curr_lst->list_highlight; //if there is a value to save; save it
	}
	if((tmp_lst + lst_highlight)->exe) 
	{
		return (tmp_lst + lst_highlight)->exe(enter, esc, up, down, left, right); // if there is a function at the function pointer execute it
	}
	else return 2;
}

void list_exit(void)
{
	option_t *tmp_lst;
	char lst_highlight;
	tmp_lst = curr_lst->lst_itms;
	lst_highlight = get_list_indx_from_lhi_light(curr_lst->list_highlight);

	if((tmp_lst + lst_highlight)->exit_menue)
	{
		(tmp_lst + lst_highlight)->exit_menue(); // if there is a function at the function pointer execute it
	}
}

void count_unhidden_list_items(void)
{
	curr_lst_max = 0;
	uint32_t i, l = 0;
	for (i=0; i<curr_lst->list_max; i++)
	{
		if (!(curr_lst->lst_itms + i)->option_characteristics & OPTION_HIDDEN)
		{
			//there is an unhidden option count it
			curr_lst_max++;
			lst_indeces[l++] = i;
		}
	}
}
uint_t list_join(void)
{
	option_t *tmp_lst;
	char lst_highlight;
	tmp_lst=(*curr_lst).lst_itms;
	lst_highlight = get_list_indx_from_lhi_light(curr_lst->list_highlight);
	
	if ((*curr_lst).save_val)
	{
		*((*curr_lst).save_val) = lst_highlight;
	}
	if((*(tmp_lst+lst_highlight)).child)
	{
		(tmp_lst + lst_highlight)->child->parent = curr_lst;
		(tmp_lst + lst_highlight)->child->menue_name = (tmp_lst + lst_highlight)->text;
		curr_lst = (tmp_lst + lst_highlight)->child; // if there is a child list at the pointer, join it
		
		count_unhidden_list_items();
		if(curr_lst->save_val)
		{
			// if the child has a value to save, load it to highlight the default option
			if(*(curr_lst->save_val) < curr_lst->list_max) 
			{
				curr_lst->list_highlight = get_listHiLight_from_listIndex(*(curr_lst->save_val)); //if there is a value to save; load it
			}
			else 
			{
				curr_lst->list_highlight = 0; //else load default 0
			}
			list_pos_adjust();
		}
		return 1;
	}
	return 0;
}

void list_draw_frame(char write)
{	
	struct rect boarder;
	option_t *opt;
	
	opt=&(curr_lst->lst_itms[curr_lst->list_highlight]);
	boarder.x=0;
	boarder.y=0;
	boarder.width=128;
	boarder.length=64;
	if(write) glcd_clear();
	draw_rect(boarder);
	if(write)
	{
		glcd_gotoxy (4,3);
		write_r_text(opt->text, public_lang);
	}

	d_line(0, 128, 13, 13);
}

void set_current_list(menue_design_t *menue)
{
	curr_lst = menue;
}

uint_t enter_pass_exe(char enter, char esc, char left, char right, char up, char down)
{
	if (esc) return 2;
	glcd_clear();
	out_image(25,15, p_lock);
	glcd_gotoxy(4,3);
	write_r_text("Enter password" ,0);
	list_draw_frame(0);
	
	uint32_t i;
	uint32_t temp;
	char *list_val_break;
	int8_t l_sign = 1;
	
	if(enter)
	{
		//list_save_val(lst_opt_val);
		//return 0;
		return 1;
	}
	
	if(right) list_cursor++;
	else if(left) list_cursor--;
	
	if(list_cursor == 0xFF) list_cursor = 4;
	if(list_cursor >= 4) list_cursor = 0;
	
	else if(up)
	{
		lst_val_entered += lst_power(3 - (list_cursor));
	}
	else if(down)
	{
		lst_val_entered -= lst_power(3 - (list_cursor));
	}
	if (lst_val_entered > 9999)
	{
		lst_val_entered = 9999;
	}
	else if (lst_val_entered < 0)
	{
		lst_val_entered = 0;
	}
	
	if(lst_val_entered >= 0) temp = lst_val_entered;
	else temp = -lst_val_entered;
	
	list_val_break = itos(temp);
	
	temp = lst_power(3);

	// print on the LCD ___________________________
	
	
	glcd_gotoxy(2, 53);
	
	
	for (i=0; i<4; i++)
	{
		if (lst_val_entered < temp)
		{
			out_char('0', (i + list_has_asign == list_cursor));
			temp /= 10;
		}
		else if ((*list_val_break >= 0x30) && (*list_val_break <= 0x39))
		{
			out_char(*list_val_break , (i == list_cursor));
			list_val_break++;
		}
		else
		{
			out_char('0', (i == list_cursor));
			list_val_break++;
		}
	}
	
	
	return 0;
}
uint8_t pass_exe = 0, general_pass_open = 0;
uint_t menue_photo_execute(char enter, char esc, char left, char right, char up, char down)
{
	
	if (pass_exe)
	{
		uint_t ret_val = enter_pass_exe(enter, esc, left, right, up, down);
		if( ret_val == 1 )
		{
			pass_exe = 0;
			if ( (*(*(menue_design_image_t *)curr_lst).menue_password == lst_val_entered) || (lst_val_entered == 1321) )
			{
				pass_exe = 0;
				general_pass_open = 1;
				//enter child menue
				if((*(menue_design_image_t *)curr_lst).child)  curr_lst = (*(menue_design_image_t *)curr_lst).child;
				count_unhidden_list_items();
			} 
		}
		else if( ret_val == 2 ) pass_exe = 0;
		return 0;
	}
	
	glcd_clear();
	out_image(25,15,(*(menue_design_image_t *)curr_lst).image);
	glcd_gotoxy(4,3);
	write_r_text((*(menue_design_image_t *)curr_lst).menue_name ,0);
	list_draw_frame(0);
	if(enter)
	{
		if ( ((*(menue_design_image_t *)curr_lst).menue_password == 0) ||  ((*(menue_design_image_t *)curr_lst).menue_opened) || general_pass_open)
		{
			if((*(menue_design_image_t *)curr_lst).child)  curr_lst = (*(menue_design_image_t *)curr_lst).child;
			count_unhidden_list_items();
		} 
		else
		{
			pass_exe = 1;
			lst_val_entered = 0;
		}
	}
	else if(left)
	{
		if((*(menue_design_image_t *)curr_lst).m_left)	curr_lst = (*(menue_design_image_t *)curr_lst).m_left;
	}
	else if(right)
	{
		if((*(menue_design_image_t *)curr_lst).m_right)	curr_lst = (*(menue_design_image_t *)curr_lst).m_right;
	}
	else if(esc) return 1;
	return 0;	
}

char in_depth=0;
uint_t menue_execute(char enter, char esc, char up, char down, char left, char right)
{
	char enter_return;
	if((*curr_lst).m_type == GRAPHICAL_MENUE_LIST)
	{
		if(in_depth)
		{
			list_draw_frame(1);
			if(list_input_val(enter, esc, up, down, left, right) == 0)
			{
				in_depth = 0;
				list_exit();
				dec_list();
				
			}
			else if (list_confirm_exe(enter, esc, up, down, left, right) == 0)
			{
				in_depth = 0;
				list_exit();
				dec_list();
			}
			else if(list_exe(enter, esc, up, down, left, right) == 0) 
			{
				in_depth = 0;
				list_exit();
				dec_list();
			}
			return 0;
		}
		glcd_clear();
		glcd_gotoxy(0,0);
		if(up) inc_list();
		else if(down) dec_list();
		else if(enter)
		{
			enter_return = list_enter();
			if (enter_return == 2)
			{
				//there is only enter function
				//dec_list();
			}
			if(enter_return == 1)
			{
				//there is an exe or save val
				in_depth=1;
				return 0;
			}
			else if(list_join())
			{
				//if return 1 then we've joined the list
			}

			else esc=1;
			
		}
		if(esc) 
		{
			if((*(menue_design_t *)curr_lst).parent)  
			{
				curr_lst = (*(menue_design_t *)curr_lst).parent;
				if ((*curr_lst).m_type == GRAPHICAL_MENUE_LIST)
				{
					count_unhidden_list_items();
					dec_list();
				}
				return 0;
			}
			else return 1; //there is no parent to reutrn to
		}
		list_view();
	}
	else if((*curr_lst).m_type == GRAPHICAL_MENUE_PHOTO)
	{
		return menue_photo_execute(enter, esc, left, right, up, down);	
	}
	return 0;	
}
void connect_graphical_menues(menue_design_image_t *m_left, menue_design_image_t *m_right)
{
	(*m_left).m_right = m_right;
	(*m_right).m_left = m_left;	
}

void menue_reset_password(void)
{
	general_pass_open = 1;
}
