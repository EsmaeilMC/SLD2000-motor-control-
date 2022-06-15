/*
 * graphical_menues.h
 *
 * Created: 23/11/2015 11:06:26 م
 *  Author: ahmed
 */ 


#ifndef GRAPHICAL_MENUES_H_
#define GRAPHICAL_MENUES_H_

#include <u_glcd.h>

#define GRAPHICAL_MENUE_LIST		 0
#define GRAPHICAL_MENUE_PHOTO		 1
#define GRAPHICAL_MENUE_GET_PASSWORD 2

#define OPTION_HIDDEN 1ul
#define OPTION_DISABLED 2ul


typedef enum {uint8_dt, uint16_dt, fixedpt_dt, float_dt} graphical_menue_datatypes_t;

typedef struct struct_g_menue_option option_t;
typedef struct 
{
	graphical_menue_datatypes_t data_types;
	uByte_t real_count;
	uByte_t mantissa_count;
	const char *unit;
	const char *text;
	float max_value;
	float min_value;
	int32_t *val;
	uint8_t  *new_val_flag;	
}option_val_t;

typedef struct struct_menue_design
{
	char m_type;// = GRAPHICAL_MENUE_LIST;
	struct struct_menue_design *parent;
	option_t *lst_itms;
	char list_max;
	char list_start, list_highlight;
	char *save_val; //the save_val to save option number
	const char *image;
	const char *menue_name;
}menue_design_t;
typedef struct struct_menue_design_image
{
	char m_type;// = GRAPHICAL_MENUE_LIST;
	menue_design_t *child;
	struct struct_menue_design_image *m_left;
	struct struct_menue_design_image *m_right;
	uint16_t *menue_password;
	char menue_opened;
	const char *image;
	const char *menue_name;
}menue_design_image_t;

struct struct_g_menue_option
{
	const char *text;
	const char *confirm_msg;
	char (*exe)(char enter, char esc, char up, char down, char left, char right);
	uint8_t  *new_val_flag;
	uint8_t option_characteristics;
	void (*enter)(void);
	void (*exit_menue)(void);
	void (*confirmed_sub)(void);
	menue_design_t *child;
	option_val_t *option_save_val;
};

void fill_menue_list(menue_design_t *mnu ,option_t *list ,char lst_max, \
                        menue_design_t *parent,char *mnuval);
void fill_menue_image( menue_design_image_t *mnu, menue_design_t *child,\
                       char *image, char *menue_name, uint16_t *menue_pas);

void list_show(menue_design_t *mnu);
void inc_list(void);
void dec_list(void);
void list_view(void);
uint_t list_enter(void);
uint_t list_exe(char enter, char esc, char up, char down, char left, char right);
uint_t list_join(void);

uint_t menue_execute(char enter, char esc, char up, char down, char left, char right);
void set_current_list(menue_design_t *menue);
void connect_graphical_menues(menue_design_image_t *m_left, menue_design_image_t *m_right);

#endif /* GRAPHICAL_MENUES_H_ */