/*
 * menue_faults.c
 *
 * Created: 07/06/2017 07:24:00 م
 *  Author: ahmed
 */ 
 
#include <menue_driver.h>
#include <faults.h>
#include <rtc_interface.h>
#include <fixedptc.h>

menue_design_t menue_faults;
#define OPT_FAULTS_MAX 2
option_t opt_faults[OPT_FAULTS_MAX];

#define faults_view				(opt_faults[0])
#define faults_clear			(opt_faults[1])

fault_history_t *current_fualt_id;
uint8_t fault_ptr, fault_index;
void display_faults_enter(void)
{
	current_fualt_id = get_first_fault(&fault_ptr, &fault_index);
}

char display_faults(char enter, char esc, char up, char down, char left, char right)
{
	uint32_t hour, PM;
	fixedpt tmp;
	
	if (esc || enter)
	{
		return 0;
	}
	else if(up)
	{
		current_fualt_id = get_next_fault(&fault_ptr, &fault_index);	
	}
	else if (down)
	{
		current_fualt_id = get_previous_fault(&fault_ptr, &fault_index);
	}
	
	
	if (current_fualt_id)
	{
		glcd_gotoxy(60, 3);
		if (fault_index > 0)
		{
			write_r_text(itos(fault_index), 0);
		} 
		else
		{
			write_r_text(" (Last)", 0);	
		}
		glcd_gotoxy(1, 18);
		write_r_text(sph_faults_get_fault_string(fault_ptr), 0);
		glcd_gotoxy(1, 26);
		write_r_text("N ", 0);
		glcd_puts(itos(current_fualt_id->occurance_count), 0);
		
		write_r_text(" / F", 0);
		if(current_fualt_id->floor & 0x80) write_r_text(" =", 0);
		glcd_puts(itos(current_fualt_id->floor & 0x7F), 0);
		
		write_r_text(" / E", 0);
		glcd_puts(itos(current_fualt_id->states), 0);
		
		glcd_gotoxy(1, 26 + 9);
		tmp = (current_fualt_id->id * FIXEDPT_ONE);
		write_r_text("Id ", 0);
		glcd_puts(fixedpt_cstr(tmp, 2),0);
		tmp = (current_fualt_id->iq * FIXEDPT_ONE);
		write_r_text(" Iq ", 0);
		glcd_puts(fixedpt_cstr(tmp, 2),0);
		
		glcd_gotoxy(1, 36 + 9);
		glcd_puts(itos(current_fualt_id->f_year + 2012), 0);
		glcd_putsf("/", 0);
		glcd_puts(itos(current_fualt_id->f_month), 0);
		glcd_putsf("/", 0);
		glcd_puts(itos(current_fualt_id->f_day), 0);
		glcd_putsf("  ", 0);
		glcd_putsf(rtc_get_week(current_fualt_id->f_year + 2012, current_fualt_id->f_month, current_fualt_id->f_day), 0);
		
		hour = current_fualt_id->f_hour;
		if (hour > 12)
		{
			PM = 1;
			hour -= 12;
		}
		if (hour == 0) hour = 12;
		glcd_gotoxy(1, 45 + 9);
		glcd_puts(itos(hour), 0);
		glcd_putsf(":", 0);
		glcd_puts(itos(current_fualt_id->f_minute), 0);
		
		if (PM) glcd_putsf(" PM ", 0);
		else glcd_putsf(" AM ", 0);
		
		
	} 
	else
	{
		glcd_gotoxy(1, 26);
		write_r_text("No fault to view", 0);
	}
	return 1;
}

void menue_faults_init(menue_design_image_t *imgae_menue)
{
	fill_menue_image(imgae_menue, &menue_faults, faults, "Faults (F)", 0);
	
	faults_view.text = "View faults";
	faults_view.exe = &display_faults;
	faults_view.enter = &display_faults_enter;
	
	faults_clear.text = "Clear history";
	faults_clear.confirmed_sub = &fault_history_eraseAndSaveAll;
	faults_clear.confirm_msg = "Are you sure?";
	
	fill_menue_list(&menue_faults, opt_faults, OPT_FAULTS_MAX, imgae_menue, 0);
}