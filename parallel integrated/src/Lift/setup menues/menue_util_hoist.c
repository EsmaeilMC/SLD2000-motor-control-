/*
 * menue_util_hoist.c
 *
 * Created: 02/06/2017 05:16:02 م
 *  Author: ahmed
 */ 
#include <menue_util_hoist.h>
#include <menue_hoist_ios.h>
#include <menue_data_manager.h>

//util hoist list
menue_design_t menue_util_hoist;
#define OPT_UTIL_HOIST_MAX 25
option_t opt_util_hoist[OPT_UTIL_HOIST_MAX];

#define uhoist_mstp	          (opt_util_hoist[0])
#define uhoist_sel	          (opt_util_hoist[1])
#define uhoist_estp	          (opt_util_hoist[2])
#define uhoist_shk	          (opt_util_hoist[3])
#define uhoist_lock	          (opt_util_hoist[4])
#define uhoist_fire	          (opt_util_hoist[5])
#define uhoist_ins	          (opt_util_hoist[6])
#define uhoist_iup	          (opt_util_hoist[7])
#define uhoist_idn	          (opt_util_hoist[8])
#define uhoist_upl	          (opt_util_hoist[9])
#define uhoist_dnl	          (opt_util_hoist[10])
#define uhoist_fw	          (opt_util_hoist[11])
#define uhoist_ow	          (opt_util_hoist[12])
#define uhoist_hopen	      (opt_util_hoist[13])
#define uhoist_hclose	      (opt_util_hoist[14])
#define uhoist_ersin          (opt_util_hoist[15])
#define uhoist_mConFB         (opt_util_hoist[16])
#define uhoist_bkConFB        (opt_util_hoist[17])
#define uhoist_fxConFB        (opt_util_hoist[18])

#define uhoist_upfl		      (opt_util_hoist[19])
#define uhoist_dnfl		      (opt_util_hoist[20])
#define uhoist_upfdl1         (opt_util_hoist[21])
#define uhoist_upfdl2         (opt_util_hoist[22])
#define uhoist_dnfdl1		  (opt_util_hoist[23])
#define uhoist_dnfdl2         (opt_util_hoist[24])
menue_design_t *hoist_io_options;

void uhoist_mstp_init(void)
{
	menue_util_hoist_show(0);
}

void uhoist_sel_init(void)
{
	menue_util_hoist_show(1);
}

void uhoist_estp_init(void)
{
	menue_util_hoist_show(2);
}

void uhoist_shk_init(void)
{
	menue_util_hoist_show(3);
}

void uhoist_lock_init(void)
{
	menue_util_hoist_show(4);
}

void uhoist_fire_init(void)
{
	menue_util_hoist_show(5);
}
void uhoist_ins_init(void)
{
	menue_util_hoist_show(6);
}

void uhoist_iup_init(void)
{
	menue_util_hoist_show(7);
}

void uhoist_idn_init(void)
{
	menue_util_hoist_show(8);
}

void uhoist_upl_init(void)
{
	menue_util_hoist_show(9);
}
void uhoist_dnl_init(void)
{
	menue_util_hoist_show(10);
}

void uhoist_fw_init(void)
{
	menue_util_hoist_show(11);
}

void uhoist_ow_init(void)
{
	menue_util_hoist_show(12);
}

void uhoist_hopen_init(void)
{
	menue_util_hoist_show(13);
}

void uhoist_hclose_init(void)
{
	menue_util_hoist_show(14);
}

void uhoist_ersin_init(void)
{
	menue_util_hoist_show(15);
}

void uhoist_mConFB_init(void)
{
	menue_util_hoist_show(16);
}

void uhoist_bkConFB_init(void)
{
	menue_util_hoist_show(17);
}

void uhoist_fxConFB_init(void)
{
	menue_util_hoist_show(18);
}

void uhoist_upfl_init(void)
{
	menue_util_hoist_show(19);
}

void uhoist_dnfl_init(void)
{
	menue_util_hoist_show(20);
}

void uhoist_upfdl1_init(void)
{
	menue_util_hoist_show(21);
}

void uhoist_upfdl2_init(void)
{
	menue_util_hoist_show(22);
}

void uhoist_dnfdl1_init(void)
{
	menue_util_hoist_show(23);
}

void uhoist_dnfdl2_init(void)
{
	menue_util_hoist_show(24);
}

menue_design_t *menue_util_hoist_init(void)
{
	hoist_io_options = menue_util_hoist_i_init();
	
	uhoist_mstp.text = "Door level signal";
	uhoist_mstp.child = hoist_io_options;
	uhoist_mstp.enter = &uhoist_mstp_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_sel.text = "Door counter signal";
	uhoist_sel.child = hoist_io_options;
	uhoist_sel.enter = &uhoist_sel_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_estp.text = "Emergency stop";
	uhoist_estp.child = hoist_io_options;
	uhoist_estp.enter = &uhoist_estp_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_shk.text = "Fork contact";
	uhoist_shk.child = hoist_io_options;
	uhoist_shk.enter = &uhoist_shk_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_lock.text = "Lock contact";
	uhoist_lock.child = hoist_io_options;
	uhoist_lock.enter = &uhoist_lock_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_fire.text = "Fire mode";
	uhoist_fire.child = hoist_io_options;
	uhoist_fire.enter = &uhoist_fire_init;
	//////////////////////////////////////////////////////////////////////////

	uhoist_ins.text = "Inspection";
	uhoist_ins.child = hoist_io_options;
	uhoist_ins.enter = &uhoist_ins_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_iup.text = "Inspection up";
	uhoist_iup.child = hoist_io_options;
	uhoist_iup.enter = &uhoist_iup_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_idn.text = "Inspection down";
	uhoist_idn.child = hoist_io_options;
	uhoist_idn.enter = &uhoist_idn_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_upl.text = "Up f_decel lmt0";
	uhoist_upl.child = hoist_io_options;
	uhoist_upl.enter = &uhoist_upl_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_dnl.text = "Down f_decel lmt0";
	uhoist_dnl.child = hoist_io_options;
	uhoist_dnl.enter = &uhoist_dnl_init;
	//////////////////////////////////////////////////////////////////////////

	uhoist_fw.text = "Full weight";
	uhoist_fw.child = hoist_io_options;
	uhoist_fw.enter = &uhoist_fw_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_ow.text = "Over weight";
	uhoist_ow.child = hoist_io_options;
	uhoist_ow.enter = &uhoist_ow_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_hopen.text = "Open door";
	uhoist_hopen.child = hoist_io_options;
	uhoist_hopen.enter = &uhoist_hopen_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_hclose.text = "Close door";
	uhoist_hclose.child = hoist_io_options;
	uhoist_hclose.enter = &uhoist_hclose_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_ersin.text = "ARD mode";
	uhoist_ersin.child = hoist_io_options;
	uhoist_ersin.enter = &uhoist_ersin_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_mConFB.text = "Main cont FBack";
	uhoist_mConFB.child = hoist_io_options;
	uhoist_mConFB.enter = &uhoist_mConFB_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_bkConFB.text = "Brake cont FBack";
	uhoist_bkConFB.child = hoist_io_options;
	uhoist_bkConFB.enter = &uhoist_bkConFB_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_fxConFB.text = "Fx cont FBack";
	uhoist_fxConFB.child = hoist_io_options;
	uhoist_fxConFB.enter = &uhoist_fxConFB_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_upfl.text = "Up final limit";
	uhoist_upfl.child = hoist_io_options;
	uhoist_upfl.enter = &uhoist_upfl_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_dnfl.text = " Down final limit";
	uhoist_dnfl.child = hoist_io_options;
	uhoist_dnfl.enter = &uhoist_dnfl_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_upfdl1.text = " Up f_decel lmt1";
	uhoist_upfdl1.child = hoist_io_options;
	uhoist_upfdl1.enter = &uhoist_upfdl1_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_upfdl2.text = " Up f_decel lmt2";
	uhoist_upfdl2.child = hoist_io_options;
	uhoist_upfdl2.enter = &uhoist_upfdl2_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_dnfdl1.text = " Down f_decel lmt1";
	uhoist_dnfdl1.child = hoist_io_options;
	uhoist_dnfdl1.enter = &uhoist_dnfdl1_init;
	//////////////////////////////////////////////////////////////////////////
	
	uhoist_dnfdl2.text = " Down f_decel lmt2";
	uhoist_dnfdl2.child = hoist_io_options;
	uhoist_dnfdl2.enter = &uhoist_dnfdl2_init;
	//////////////////////////////////////////////////////////////////////////		
	fill_menue_list(&menue_util_hoist, opt_util_hoist, OPT_UTIL_HOIST_MAX, 0, 0);

	return &menue_util_hoist;
}