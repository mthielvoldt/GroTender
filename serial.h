/* 
 * File:   serial.h
 * Author: rileyt
 *
 * Created on November 23, 2014, 1:41 PM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#define byte_from_target() (rx2_buffer_head != rx2_buffer_tail)
#define BUFFER_SIZE	32

void U2Print(const char [], const unsigned char );


#endif	/* SERIAL_H */

