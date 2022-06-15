/*
 * selfLearn.h
 *
 * Created: 8/5/2018 5:44:07 AM
 *  Author: Ahmed
 */ 


#ifndef SELFLEARN_H_
#define SELFLEARN_H_


uint32_t enter_Hlearn(int32_t up_dir);
uint32_t selfLearn_exe(void);
uint32_t selfLearn_isLearning(void);
void selfLearn_terminateLearning(void);
void self_learn_handler(void);

#endif /* SELFLEARN_H_ */