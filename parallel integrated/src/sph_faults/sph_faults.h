/*
 * sph_faults.h
 *
 * Created: 25/05/2017 06:48:09 ص
 *  Author: ahmed
 */ 


#ifndef SPH_FAULTS_H_
#define SPH_FAULTS_H_

#include <faults.h>

	
typedef struct  
{
	fault_type_t fault_type;
	uint32_t fault_counter;
	uint32_t fault_timeout;
	uint8_t fault_prefix;
	uint8_t fault_code;
	const char *e_string;
}sph_fault_t;



#endif /* SPH_FAULTS_H_ */