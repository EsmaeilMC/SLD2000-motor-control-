/*
 * scCheck.h
 *
 * Created: 10/09/2017 04:18:59 م
 *  Author: ahmed
 */ 


#ifndef SCCHECK_H_
#define SCCHECK_H_

#include <vfd.h>





// #ifdef VFD5K5
// //#define EQ_SRES 1.1
// #define SC_CURRENT ((2.0f))
// #elif defined(VFD7K5)
// //#define EQ_SRES 1.6 
// #define SC_CURRENT ((2.0f))
// #else defined(VFD11K)
// //#define EQ_SRES 0.5
// #define SC_CURRENT ((2.0f))  
// #endif

void scCheck_init(void);
uint32_t scCheck_exe(void);
void scCheck_current_handler(float ia, float ib, float ic);

//#define SC_CURRENT (fixedpt_rconst(40))//(fixedpt_rconst(SC_TEST_VOLTAGE / EQ_SRES))

#endif /* SCCHECK_H_ */