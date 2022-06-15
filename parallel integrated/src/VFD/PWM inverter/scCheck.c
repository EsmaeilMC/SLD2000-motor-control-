/*
 * scCheck.c
 *
 * Created: 10/09/2017 04:18:47 م
 *  Author: ahmed
 */ 


#include <scCheck.h>
#include <Inverter.h>
#include <vfdstates.h>
#include <d_faults.h>


#define SC_TEST_TIME_MS 3
#define SC_TEST_VOLTAGE (30)
#define SC_CURRENT ((15.0f))

void scCheck_testVector(uint32_t vectorId)
{
	if (vectorId == 0)
	{
		inverter_out_single_axis(SC_TEST_VOLTAGE);
	} 
	else if (vectorId == 1)
	{
		inverter_out_single_axis_b(SC_TEST_VOLTAGE);
	}
	else if (vectorId == 2)
	{
		inverter_out_single_axis_c(SC_TEST_VOLTAGE);
	}
	else
	{
		inverter_out_0();
		//SC_LEVEL_HIGH;
	}
		
}
uint16_t scCheck_timer = 0, scCheck_phase = 0;
void scCheck_init(void)
{
	scCheck_timer = 0;
	scCheck_phase = 0;
	inverter_out_0();
	SC_LEVEL_LOW;
}

//#pragma GCC optimize("O1")
uint32_t scCheck_exe(void)
{
	scCheck_testVector(scCheck_phase);

	if(++scCheck_timer >= SC_TEST_TIME_MS) 
	{
		scCheck_phase++;
		scCheck_timer = 0;
	}
	if(scCheck_phase > 5)
	{
		//sc check complete		
		return 1;
		
	}
	return 0;
}

void scCheck_current_handler(float ia, float ib, float ic)
{
	if(!vfd_states_is_sctest_mode()) return;
	
	if ((fixedpt_abs(ia) >= SC_CURRENT) || (fixedpt_abs(ib) >= SC_CURRENT) || (fixedpt_abs(ic) >= SC_CURRENT))
	{
		inverter_set_override();
		vfd_states_trip();
		sc_err_set;
	}
}
//#pragma GCC optimize("O2")