/*
 * menue_hoist_ios.c
 *
 * Created: 02/06/2017 06:11:41 م
 *  Author: ahmed
 */ 

#include <menue_hoist_ios.h>
#include <menue_data_manager.h>
#include <drive_interface_data.h>
//util hoist inputs list


menue_design_t menue_util_hoist_i;
#define OPT_UTIL_HOIST_I_MAX 31
//#define OPT_UTIL_HOIST_I_PMAX 22
option_t opt_util_hoist_i[OPT_UTIL_HOIST_I_MAX];


void menue_util_hoist_show(uint32_t indx)
{
	uint32_t i;
	if(indx > 24) return;
	for (i=0; i<OPT_UTIL_HOIST_I_MAX; i++)
	{
		//unhide all options
		if (i < 16)
		{
			opt_util_hoist_i[i].option_characteristics = 0;
		}
		#ifndef _SERIAL_DRIVE
		else if (i < 22)
		{
			opt_util_hoist_i[i].option_characteristics = 0;
		}
		
		else if (i < 30)
		{
			opt_util_hoist_i[i].option_characteristics = OPTION_HIDDEN;
		}
		#else
		
		else if (i < 22)
		{
			opt_util_hoist_i[i].option_characteristics = OPTION_HIDDEN;
		}
		else if (i < 30)
		{
			opt_util_hoist_i[i].option_characteristics = 0;
		}
		#endif
		
		opt_util_hoist_i[30].option_characteristics = 0;
	}
	
	for (i=0; i<25; i++)
	{
		if ((menue_lift_data_setup.m_hoist[i] < 30) && (i != indx))
		{
			//hide selected inputs
			opt_util_hoist_i[menue_lift_data_setup.m_hoist[i]].option_characteristics = OPTION_HIDDEN;
		} 		
	}
	menue_util_hoist_i.save_val = &(menue_lift_data_setup.m_hoist[indx]);
}

menue_design_t *menue_util_hoist_i_init(void)
{
	opt_util_hoist_i[0].text = "X0";
	opt_util_hoist_i[1].text = "X1";
	opt_util_hoist_i[2].text = "X2";
	opt_util_hoist_i[3].text = "X3";
	opt_util_hoist_i[4].text = "X4";
	opt_util_hoist_i[5].text = "X5";
	opt_util_hoist_i[6].text = "X6";
	opt_util_hoist_i[7].text = "X7";
	opt_util_hoist_i[8].text = "X8";
	opt_util_hoist_i[9].text = "X9";
	opt_util_hoist_i[10].text = "X10";
	opt_util_hoist_i[11].text = "X11";
	opt_util_hoist_i[12].text = "X12";
	opt_util_hoist_i[13].text = "X13";
	opt_util_hoist_i[14].text = "X14";
	opt_util_hoist_i[15].text = "X15";
	opt_util_hoist_i[16].text = "X16";
	opt_util_hoist_i[17].text = "X17";
	opt_util_hoist_i[18].text = "X18";
	opt_util_hoist_i[19].text = "X19";
	opt_util_hoist_i[20].text = "X20";
	opt_util_hoist_i[21].text = "X21";
	opt_util_hoist_i[22].text = "CTB X0";
	opt_util_hoist_i[23].text = "CTB X1";
	opt_util_hoist_i[24].text = "CTB X2";
	opt_util_hoist_i[25].text = "CTB X3";
	opt_util_hoist_i[26].text = "CTB X4";
	opt_util_hoist_i[27].text = "CTB X5";
	opt_util_hoist_i[28].text = "CTB X6";
	opt_util_hoist_i[29].text = "CTB X7";
	opt_util_hoist_i[30].text = "None";

	fill_menue_list(&menue_util_hoist_i, opt_util_hoist_i, OPT_UTIL_HOIST_I_MAX, 0, 0);

	return &menue_util_hoist_i;
}