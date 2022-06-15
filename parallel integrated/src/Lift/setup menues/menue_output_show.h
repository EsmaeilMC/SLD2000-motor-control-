/*
 * menue_output_show.h
 *
 * Created: 11/06/2017 02:57:54 ص
 *  Author: ahmed
 */ 


#ifndef MENUE_OUTPUT_SHOW_H_
#define MENUE_OUTPUT_SHOW_H_

#include <def.h>
#include <graphical_menues.h>
#include <graphics_data.h>
#include <u_glcd.h>

menue_design_t *menue_util_outputs_list_init(void);
void menue_util_outputList_show(uint32_t indx);

#endif /* MENUE_OUTPUT_SHOW_H_ */