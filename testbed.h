/* 
 * File:   FirePixel_Testbed.h
 * Author: rileyt
 *
 * Created on October 4, 2014, 10:42 PM
 */

#ifndef TESTBED_H
#define	TESTBED_H

// outputs
#define LED			LATBbits.LATB15
#define LED_TRIS	TRISBbits.TRISB15
#define LED_LATCH	LATB
#define LED_PIN		_LATB_LATB15_MASK

#define HEATCOOL		LATAbits.LATA4
#define HEATCOOL_TRIS	TRISAbits.TRISA4
#define HEATCOOL_LATCH	LATA
#define HEATCOOL_PIN	_LATA_LATA4_MASK

#define HEATPUMP		LATBbits.LATB9
#define HEATPUMP_TRIS	TRISBbits.TRISB9
#define HEATPUMP_LATCH	LATB
#define HEATPUMP_PIN	_LATB_LATB9_MASK


// digital general purpose inputs (doesn't include UARTs)
#define SWITCH		PORTBbits.RB14
#define SWITCH_ANS	ANSBbits.ANSB14
#define SWITCH_PORT	PORTB
#define SWITCH_PIN	_PORTB_RB14_MASK

// analog inputs
#define TEMP1_ANS	ANSAbits.ANSA2
#define TEMP1_CHAN	13		// AN13 = RA2



/****************  USEFUL CONSTANTS  *******************/



/***************   GLOBAL VARIABLES   *******************/
extern unsigned int millis1, millis2;	// for millisecond timiing using interrupts
unsigned int temp1, temp2;				// these are temperatures
unsigned int testcount;					// for counting the tests done.


/**************   FUNCTION PROTOTYPES   *****************/
void Run_Tests(void);


#endif	/* TESTBED_H */

