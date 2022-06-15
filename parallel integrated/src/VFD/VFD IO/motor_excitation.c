/*
 * motor_excitation.c
 *
 * Created: 07/04/2020 05:38:54 م
 *  Author: ahmed
 */ 

#include <motor_excitation.h>
#include <SEDACIO.h>
#include <d_faults.h>
#include <vfd.h>

#define FX_TIMER (550) 
#define MOC_TIMER (350)
#define BK_TIMER (500)
#define MOC_TA_TIMER (250)
#define BK_TA_TIMER (250)

void motor_excitation_stage1_enter(void);

void set_fx_err(void)
{
	if (sedacio_is_output_assigned(FX) == 1)
	{
		vfd_trip();
		fx_err_set;
	}
}

void set_moc_err(void)
{
	if (sedacio_is_output_assigned(MOC) == 1)
	{
		vfd_trip();
		moc_err_set;
	}
}

void set_bk_err(void)
{
	if (sedacio_is_output_assigned(BRKS) == 1)
	{
		vfd_trip();
		bk_err_set;
	}
}

uint8_t motor_excitation_stage = 0, connect_motor = 0, disconnect_motor = 0, motor_connected = 0, bk_state = 0;
uint16_t fx_counter = 0, moc_counter = 0, moc_ta_counter = 0, bk_counter = 0, moc_trials = 0, bk_trials = 0;
void motor_excitation_stage0_enter(void)
{
	motor_excitation_stage = 0;
	motor_connected = 0;
	disconnect_motor = 0;
	fx_counter = 0;
}
void motor_excitation_stage0_exit(void)
{
	if (sedacio_is_output_assigned(FX) == 0)
	{
		motor_excitation_stage1_enter();
		return;
	}
	
	if(fx_counter < FX_TIMER)
	{
		fx_counter++;
	}
	else
	{
		if (fxConFB == 1)
		{
			motor_excitation_stage1_enter();
		} 
		else
		{
			set_fx_err();
		}
	}
}
void motor_excitation_stage0_exe(void)
{
	m_mainOutput(0);
	if (connect_motor)
	{
		m_fx(1);
		motor_excitation_stage0_exit();
	} 
	else
	{
		m_fx(0);
	}	
}


void motor_excitation_stage1_enter(void)
{
	motor_excitation_stage = 1;
	motor_connected = 0;
	connect_motor = 0;
	moc_counter = 0;
	moc_ta_counter = 0;
	moc_trials = 0;
	m_fx(1);
	m_mainOutput(1);
}
void motor_excitation_stage1_exit(void)
{
	m_mainOutput(0);
	motor_connected = 0;
	
	if (sedacio_is_output_assigned(FX) == 0)
	{
		motor_excitation_stage0_enter();
	}
	
	if(moc_counter < MOC_TIMER)
	{
		moc_counter++;
	}
	else
	{
		motor_excitation_stage0_enter();
	}
}
void motor_excitation_stage1_exe(void)
{
	if (disconnect_motor)
	{
		motor_excitation_stage1_exit();
		return;
	}
	if (sedacio_is_output_assigned(MOC) == 0)
	{
		motor_connected = 1;
	}
	if (mConFB == 1)
	{
		motor_connected = 1;
	}
	
	if (fxConFB == 0)
	{
		set_fx_err();
	}
	
	if(moc_ta_counter < MOC_TA_TIMER)
	{
		moc_ta_counter++;
	}
	else
	{
		if (mConFB == 1)
		{
			motor_connected = 1;
			moc_trials = 0;
		}
		else
		{
			if (moc_trials < 254)
			{
				moc_trials++;
			} 
			else
			{
				set_moc_err();
			}
		}
	}
}

void motor_excitation_handler(void)
{
	if (motor_excitation_stage == 0)
	{
		motor_excitation_stage0_exe();
	} 
	else if (motor_excitation_stage == 1)
	{
		motor_excitation_stage1_exe();
	}
	
	
	if (bk_state)
	{
		if(bk_counter < BK_TIMER)
		{
			bk_counter++;
		}
		else
		{
			if (bkConFB == 1)
			{
				bk_trials = 0;
			}
			else
			{
				if (bk_trials < 254)
				{
					bk_trials++;
				}
				else
				{
					set_bk_err();
				}
			}
		}
	}
}
void motor_excitation_connect_motor(void)
{
	if (connect_motor == 0)
	{
		fx_counter = 0;
		disconnect_motor = 0;
		connect_motor = 1;
	}
}

void motor_excitation_disconnect_motor(void)
{
	if (disconnect_motor == 0)
	{
		connect_motor = 0;
		moc_counter = 0;
		disconnect_motor = 1;
	}
}

uint8_t motor_excitation_is_motor_connected(void)
{
	return motor_connected;
}


void BRKS_set(uint32_t bsign)
{
	if (bsign == 0)
	{
		vfd_brake(0);
		bk_counter = 0;
		bk_trials = 0;
		bk_state = 0;
	} 
	else
	{
		vfd_brake(1);
		bk_state = 1;
	}
}