/*
 * overload.h
 *
 * Created: 7/25/2017 4:25:33 AM
 *  Author: Ahmed
 */ 


#ifndef OVERLOAD_H_
#define OVERLOAD_H_

void overload_check(float ia, float ib, float ic);
void overload_checkIm(fixedpt imSqr);
void overload_init(void);
void overload_update(void);
float get_olval();
float get_olia();
float get_olib();
float get_olic();
float get_olim();
#endif /* OVERLOAD_H_ */