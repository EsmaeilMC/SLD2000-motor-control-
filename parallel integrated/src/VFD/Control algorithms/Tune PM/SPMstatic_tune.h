/*
 * SPMstatic_tune.h
 *
 * Created: 09/07/2018 05:16:35 م
 *  Author: ahmed
 */ 


#ifndef SPMSTATIC_TUNE_H_
#define SPMSTATIC_TUNE_H_
#include <asf.h>

void SPMstatic_tune_current_handler(void);
void SPMtune_init(void);
uint32_t SPMstatic_getchosenvec(void);

#endif /* SPMSTATIC_TUNE_H_ */