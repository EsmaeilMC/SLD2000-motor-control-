/*
 * SVM.h
 *
 * Created: 29/01/2017 01:25:24 م
 *  Author: ahmed
 */ 


#ifndef SVM_H_
#define SVM_H_
#include "fixedptc.h"
extern const fixedpt SVM_SIN_tab[];

void svm_polar_to_ABC(uint16_t * a, uint16_t * b, uint16_t * c, fixedpt ang, fixedpt m);
void svm_rectangular_to_ABC(uint16_t * a, uint16_t * b, uint16_t * c, fixedpt alpha, fixedpt beta);


#endif /* SVM_H_ */