/*
 * mb_frame.h
 *
 * Created: 09/05/2017 08:13:12 م
 *  Author: ahmed
 */ 


#ifndef MB_FRAME_H_
#define MB_FRAME_H_

void mb_add_crc(uint8_t *buffer, uint8_t *size);
uint32_t mb_check_crc( uint8_t *buffer, uint8_t *size );

#endif /* MB_FRAME_H_ */