/*
 * sph_faults.c
 *
 * Created: 25/05/2017 06:47:52 ص
 *  Author: ahmed
 */ 

#include <sph_faults.h>
#include <faults.h>

sph_fault_t sph_faults[SPH_FAULT_COUNT];

uint32_t sph_faults_display_time_counter = 0;
uint32_t sph_faults_stop_i = 0, sph_faults_pos = 0, sph_faults_stop_i_to = 0, sph_faults_pos_to = 0; 
void sph_faults_handler(void)
{
	uint32_t i, fault_counter;
	for (i=0; i<SPH_FAULT_COUNT; i++)
	{
		if ((sph_faults[i].fault_timeout > 0) && (sph_faults[i].fault_timeout != 0xFFFFFFFF))
		{
			//it is a fault timeout
			fault_counter = sph_faults[i].fault_counter;
			fault_counter &= ~0x80000000; //clear the fault set bit
			if (fault_counter > 0)
			{
				//there is time to pass for the fault
				fault_counter--; //decrement the fault counter
				fault_counter |= 0x80000000; //reset the fault set bit
				sph_faults[i].fault_counter = fault_counter;
			}
			else if (sph_faults[i].fault_counter == 0x80000000)
			{
				//clear the fault
				sph_faults[i].fault_counter = 0;
				if (sph_faults[i].fault_type == STOP_IMMEDIATE)
				{
					//no timeout fault
					if(sph_faults_stop_i>0) sph_faults_stop_i--;
				}
				else if (sph_faults[i].fault_type == POS_FAULT)
				{
					if(sph_faults_pos>0) sph_faults_pos--;
				}
				else if (sph_faults[i].fault_type == T_STOP_IMMEDIATE)
				{
					if(sph_faults_stop_i_to>0) sph_faults_stop_i_to--;
				}
				else if (sph_faults[i].fault_type == T_POS_FAULT)
				{
					if(sph_faults_pos_to>0) sph_faults_pos_to--;
				}
			}
		}	
	}
	if(sph_faults_display_time_counter) sph_faults_display_time_counter--;
}

uint32_t sph_faults_add_fault(fault_type_t fault_type, uint32_t fault_timeout, uint8_t fault_prefix, uint8_t fault_code, const char *e_string)
{
	uint32_t i;
	for (i=0; i<SPH_FAULT_COUNT; i++)
	{
		if (sph_faults[i].fault_timeout == 0)
		{
			//there is an empty place for a fault
			sph_faults[i].fault_type = fault_type;
			sph_faults[i].fault_timeout = fault_timeout;
			sph_faults[i].e_string = e_string;
			sph_faults[i].fault_counter = 0;
			sph_faults[i].fault_prefix = fault_prefix;
			sph_faults[i].fault_code = fault_code;
			if (fault_type == STOP_IMMEDIATE)
			{
				//no timeout fault
				sph_faults[i].fault_timeout = 0xFFFFFFFF;	
			}
			else if (fault_type == POS_FAULT)
			{
				sph_faults[i].fault_timeout = 0xFFFFFFFF;
			}
			else if (fault_type == T_STOP_IMMEDIATE)
			{
				sph_faults[i].fault_timeout = fault_timeout;
			}
			else if (fault_type == T_POS_FAULT)
			{
				sph_faults[i].fault_timeout = fault_timeout;
			}
			return i;
		}
	}
	return 0xFFFFFFFF;		
}

void sph_faults_init(void)
{
	uint32_t i;
	for (i=0; i<SPH_FAULT_COUNT; i++)
	{
		sph_faults[i].fault_timeout = 0;	
	}
}

void sph_faults_setfault(uint32_t fault_index)
{
	if (fault_index < SPH_FAULT_COUNT)
	{
		if(!(sph_faults[fault_index].fault_counter & 0x80000000))
		{
			//fault not set, so set it.
			if (sph_faults[fault_index].fault_type == STOP_IMMEDIATE)
			{
				//no timeout fault
				if(sph_faults[fault_index].fault_counter == 0) sph_faults_stop_i++;	
			}
			else if (sph_faults[fault_index].fault_type == POS_FAULT)
			{
				if(sph_faults[fault_index].fault_counter == 0) sph_faults_pos++;
			}
			else if (sph_faults[fault_index].fault_type == T_STOP_IMMEDIATE)
			{
				if(sph_faults[fault_index].fault_counter == 0) sph_faults_stop_i_to++;
			}
			else if (sph_faults[fault_index].fault_type == T_POS_FAULT)
			{
				if(sph_faults[fault_index].fault_counter == 0) sph_faults_pos_to++;
			}
			fault_history_setFault(fault_index);
		}
		sph_faults[fault_index].fault_counter = sph_faults[fault_index].fault_timeout | 0x80000000;
	}
}

uint32_t sph_faults_is_present(uint32_t fault_index)
{
	if (fault_index < SPH_FAULT_COUNT)
	{
		
		if (sph_faults[fault_index].fault_counter & 0x80000000) return 1;
	}
	return 0;
}
uint32_t sph_faults_sie_check(void)
{
	//if there is stop imediate error return 1, else 0
	if((sph_faults_stop_i) || (sph_faults_stop_i_to)) 
	{
		return 1;
	}
	else return 0;
}

uint32_t sph_faults_pose_check(void)
{
	//if there is stop imediate error return 1, else 0
	if((sph_faults_pos) || (sph_faults_pos_to))
	{
		return 1;
	}
	else return 0;
}

uint8_t sph_faults_display_index=0;
uint32_t sph_faults_get_fault_to_display(const char **str, char *fault_prefix, char *fault_code)
{
	uint32_t i;
	if (!(sph_faults[sph_faults_display_index].fault_counter & 0x80000000) || (sph_faults_display_time_counter == 0))
	{
		//there is no fault here, or display timed-out
		for(i=0; i<SPH_FAULT_COUNT; i++)
		{
			if(++sph_faults_display_index >= SPH_FAULT_COUNT) sph_faults_display_index=0;
			if (sph_faults[sph_faults_display_index].fault_counter & 0x80000000)
			{
				//a fault found to display
				sph_faults_display_time_counter = SPH_FAULT_DISPLAY_TIMEOUT;
				*str = sph_faults[sph_faults_display_index].e_string;
				*fault_code = sph_faults[sph_faults_display_index].fault_code;
				*fault_prefix = sph_faults[sph_faults_display_index].fault_prefix;
				return 1;
			}
		}
	}
	else
	{
		// the fault is still there to display
		*str = sph_faults[sph_faults_display_index].e_string;
		*fault_code = sph_faults[sph_faults_display_index].fault_code;
		*fault_prefix = sph_faults[sph_faults_display_index].fault_prefix;		
		return 1;
	}
	return 0;	
}

const char *sph_faults_get_fault_string(uint8_t fault_id)
{
	if (fault_id < SPH_FAULT_COUNT)
	{
		return sph_faults[fault_id].e_string;
	} 
	else
	{
		return "";
	}
}

uint8_t dont_record_fault()
{
	// function return 0 if there's something inhibit service call
	if(sph_faults_sie_check())
	return 1;
	if(sph_faults_pose_check())
	return 1;
	
	return 0;
}