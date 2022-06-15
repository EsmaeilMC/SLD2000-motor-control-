/*
 * menue_output_show.c
 *
 * Created: 11/06/2017 02:58:04 ص
 *  Author: ahmed
 */ 

#include <menue_output_show.h>
#include <menue_data_manager.h>
//util hoist inputs list


menue_design_t menue_util_output_list;
#define OPT_UTIL_OUTPUTS_LIST_MAX 17
option_t opt_util_outputs_list[OPT_UTIL_OUTPUTS_LIST_MAX];


void menue_util_outputList_show(uint32_t indx)
{
	uint32_t i;
	if(indx > 14) return;
	
	menue_util_output_list.save_val = &(menue_lift_data_setup.m_outputs[indx]);
}

menue_design_t *menue_util_outputs_list_init(void)
{
	opt_util_outputs_list[0].text = "Motor Brake";
	opt_util_outputs_list[1].text = "CAM";
	opt_util_outputs_list[2].text = "Light timer";
	opt_util_outputs_list[3].text = "ARD";
	opt_util_outputs_list[4].text = "Arrow up";
	opt_util_outputs_list[5].text = "Arrow Down";
	opt_util_outputs_list[6].text = "MOC";
	opt_util_outputs_list[7].text = "Gong";
	opt_util_outputs_list[8].text = "ARD Toff";
	opt_util_outputs_list[9].text = "Fx";
	opt_util_outputs_list[10].text = "Door1 Close";
	opt_util_outputs_list[11].text = "Door1 Open";
	opt_util_outputs_list[12].text = "Door2 Close";
	opt_util_outputs_list[13].text = "Door2 Open";
	opt_util_outputs_list[14].text = "Door3 Close";
	opt_util_outputs_list[15].text = "Door3 Open";
	opt_util_outputs_list[16].text = "Null";

	fill_menue_list(&menue_util_output_list, opt_util_outputs_list, OPT_UTIL_OUTPUTS_LIST_MAX, 0, 0);

	return &menue_util_output_list;
}