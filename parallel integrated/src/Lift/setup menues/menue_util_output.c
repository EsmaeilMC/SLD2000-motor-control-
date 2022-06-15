/*
 * menue_util_output.c
 *
 * Created: 11/06/2017 02:27:38 ص
 *  Author: ahmed
 */ 

#include <menue_util_output.h>
#include <menue_output_show.h>
#include <menue_data_manager.h>

//util hoist list
menue_design_t menue_util_output;
#define OPT_UTIL_OUTPUT_MAX 15
option_t opt_util_output[OPT_UTIL_OUTPUT_MAX];

#define uout_y0	          (opt_util_output[0])
#define uout_y1	          (opt_util_output[1])
#define uout_y2	          (opt_util_output[2])
#define uout_y3	          (opt_util_output[3])
#define uout_y4	          (opt_util_output[4])
#define uout_y5	          (opt_util_output[5])
#define uout_y6	          (opt_util_output[6])
#define uout_y7	          (opt_util_output[7])
#define uout_y8	          (opt_util_output[8])
#define uout_cy0	      (opt_util_output[9])
#define uout_cy1          (opt_util_output[10])
#define uout_cy2          (opt_util_output[11])
#define uout_cy3          (opt_util_output[12])
#define uout_cy4          (opt_util_output[13])
#define uout_cy5          (opt_util_output[14])

menue_design_t *output_options;

void uoutput_y0_init(void)
{
	menue_util_outputList_show(0);
}

void uoutput_y1_init(void)
{
	menue_util_outputList_show(1);
}

void uoutput_y2_init(void)
{
	menue_util_outputList_show(2);
}

void uoutput_y3_init(void)
{
	menue_util_outputList_show(3);
}

void uoutput_y4_init(void)
{
	menue_util_outputList_show(4);
}

void uoutput_y5_init(void)
{
	menue_util_outputList_show(5);
}

void uoutput_y6_init(void)
{
	menue_util_outputList_show(6);
}

void uoutput_y7_init(void)
{
	menue_util_outputList_show(7);
}

void uoutput_y8_init(void)
{
	menue_util_outputList_show(8);
}

void uoutput_cy0_init(void)
{
	menue_util_outputList_show(9);
}

void uoutput_cy1_init(void)
{
	menue_util_outputList_show(10);
}

void uoutput_cy2_init(void)
{
	menue_util_outputList_show(11);
}

void uoutput_cy3_init(void)
{
	menue_util_outputList_show(12);
}

void uoutput_cy4_init(void)
{
	menue_util_outputList_show(13);
}

void uoutput_cy5_init(void)
{
	menue_util_outputList_show(14);
}



menue_design_t *menue_util_output_init(void)
{
	output_options = menue_util_outputs_list_init();
	
	uout_y0.text = "Y0";
	uout_y0.child = output_options;
	uout_y0.enter = &uoutput_y0_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_y1.text = "Y1";
	uout_y1.child = output_options;
	uout_y1.enter = &uoutput_y1_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_y2.text = "Y2";
	uout_y2.child = output_options;
	uout_y2.enter = &uoutput_y2_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_y3.text = "Y3";
	uout_y3.child = output_options;
	uout_y3.enter = &uoutput_y3_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_y4.text = "Y4";
	uout_y4.child = output_options;
	uout_y4.enter = &uoutput_y4_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_y5.text = "Y5";
	uout_y5.child = output_options;
	uout_y5.enter = &uoutput_y5_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_y6.text = "Y6";
	uout_y6.child = output_options;
	uout_y6.enter = &uoutput_y6_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_y7.text = "Y7";
	uout_y7.child = output_options;
	uout_y7.enter = &uoutput_y7_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_y8.text = "Y8";
	uout_y8.child = output_options;
	uout_y8.enter = &uoutput_y8_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_cy0.text = "CTB Y0";
	uout_cy0.child = output_options;
	uout_cy0.enter = &uoutput_cy0_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_cy1.text = "CTB Y1";
	uout_cy1.child = output_options;
	uout_cy1.enter = &uoutput_cy1_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_cy2.text = "CTB Y2";
	uout_cy2.child = output_options;
	uout_cy2.enter = &uoutput_cy2_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_cy3.text = "CTB Y3";
	uout_cy3.child = output_options;
	uout_cy3.enter = &uoutput_cy3_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_cy4.text = "CTB Y4";
	uout_cy4.child = output_options;
	uout_cy4.enter = &uoutput_cy4_init;
	//////////////////////////////////////////////////////////////////////////
	
	uout_cy5.text = "CTB Y5";
	uout_cy5.child = output_options;
	uout_cy5.enter = &uoutput_cy5_init;
	//////////////////////////////////////////////////////////////////////////
		
	fill_menue_list(&menue_util_output, opt_util_output, OPT_UTIL_OUTPUT_MAX, 0, 0);

	return &menue_util_output;
}