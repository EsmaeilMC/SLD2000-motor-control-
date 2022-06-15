/*
 * axistransform.h
 *
 * Created: 26/03/2017 10:05:15 ص
 *  Author: ahmed
 */ 


#ifndef AXISTRANSFORM_H_
#define AXISTRANSFORM_H_

#include <fixedptc.h>

void axt_clark_exe(void);
void axt_set_theta(float theta);
void axt_park_exe(void);
void axt_get_park_transform(float ina, float inb, float *outd, float *outq);
void axt_ipark_exe(float vd, float vq, float *valpha, float *vbeta);

float axt_get_id(void);
float axt_get_iq(void);

float axt_get_ialpha(void);
float axt_get_ibeta(void);

void axt_integrate_current(float curr);
float axt_get_current_avg(void);
void axt_reset_current_avg(void);

#endif /* AXISTRANSFORM_H_ */