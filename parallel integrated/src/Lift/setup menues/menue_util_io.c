/*
 * menue_util_io.c
 *
 * Created: 02/06/2017 02:29:59 م
 *  Author: ahmed
 */ 

#include <menue_util_io.h>
#include <menue_util_hoist.h>
#include <menue_util_output.h>

//util io list
menue_design_t menue_util_io;
#define OPT_UTIL_IO_MAX 3
option_t opt_util_ios[OPT_UTIL_IO_MAX];

#define uio_input_signal	          (opt_util_ios[0])
#define uio_outputsignal	          (opt_util_ios[1])
#define uio_request_pins	          (opt_util_ios[2])

menue_design_t *menue_util_io_init(void)
{
	uio_input_signal.text = "Hoist input signals";
	uio_input_signal.child = menue_util_hoist_init();
//////////////////////////////////////////////////////////////////////////
	
	uio_outputsignal.text = "Output signals";
	uio_outputsignal.child = menue_util_output_init();
//////////////////////////////////////////////////////////////////////////
	
	uio_request_pins.text = "Request pins";
	uio_request_pins.child = menue_util_hoist_init();
//////////////////////////////////////////////////////////////////////////

	fill_menue_list(&menue_util_io, opt_util_ios, OPT_UTIL_IO_MAX, 0, 0);

	return &menue_util_io;	
}