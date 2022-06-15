/*
 * service_priority_organizer.h
 *
 * Created: 22/11/2015 12:22:49 ص
 *  Author: ahmed
 */ 


#ifndef SERVICE_PRIORITY_ORGANIZER_H_
#define SERVICE_PRIORITY_ORGANIZER_H_

#define  collect menue_lift_data_applied.m_collection

int get_scheduled_floor(void);

#define r_internal 0
#define r_external 1
#define r_dir_both 2
#define r_dir_both_down 3
#define r_eexternal 4

int get_nearest_floor(char type);
int get_highest_floor(char type);
int get_lowest_floor(char type);

//char check_service();
int down_coll(void);
int up_coll(void);
int all_coll(void);

#endif /* SERVICE_PRIORITY_ORGANIZER_H_ */