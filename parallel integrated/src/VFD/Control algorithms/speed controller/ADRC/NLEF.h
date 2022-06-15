/*
 * NLEF.h
 *
 * Created: 02/02/2019 01:31:12 م
 *  Author: ahmed
 */ 


#ifndef NLEF_H_
#define NLEF_H_

float nlef_exe(float err, float ff, float kp);
float nlef_saturation_trim(float sat_limit, float val);



#endif /* NLEF_H_ */