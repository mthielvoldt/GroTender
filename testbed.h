/* 
 * File:   FirePixel_Testbed.h
 * Author: rileyt
 *
 * Created on October 4, 2014, 10:42 PM
 */

#ifndef TESTBED_H
#define	TESTBED_H

// outputs
#define LED			LATBbits.LATB14
#define LED_TRIS	TRISBbits.TRISB14
#define LED_LATCH	LATB
#define LED_PIN		_LATB_LATB14_MASK

#define	RESET		LATAbits.LATA2
#define RESET_TRIS	TRISAbits.TRISA2

#define FLAME		LATBbits.LATB13
#define FLAME_TRIS	TRISBbits.TRISB13


// digital general purpose inputs (doesn't include UARTs)
#define SWITCH		PORTBbits.RB15
#define SWITCH_ANS	ANSBbits.ANSB15
#define SWITCH_PORT	PORTB
#define SWITCH_PIN	_PORTB_RB15_MASK

#define SPARK		PORTBbits.RB4
#define SPARK_PORT	PORTB
#define SPARK_MASK	_PORTB_RB4_MASK
#define SPARK_ANS	ANSBbits.ANSB4

#define	COP			PORTAbits.RA4
#define	COP_PORT	PORTA
#define	COP_MASK	_PORTA_RA4_MASK
#define	COP_ANS		//NULL, NOT AN ANALOG PIN

#define PV			PORTBbits.RB12
#define PV_PORT		PORTB
#define PV_MASK		_PORTB_RB12_MASK
#define PV_ANS		ANSBbits.ANSB12


// analog inputs
#define CHG_ANS	ANSAbits.ANSA3

#define PV_CHAN		-1		// this now uses input capture, so we make it a non-valid channel
#define CHG_CHAN	14		// AN14



/****************  USEFUL CONSTANTS  *******************/


#define SETTLE_TIME	12		// time in ms between a command and when we expect
							// the value to be readable on LP'd output pins.

#define PV_ZERO_THRESH	60	// the threshold for saying whether PV is at 0 or not


/***************   GLOBAL VARIABLES   *******************/
extern unsigned int millis1, millis2;		// for millisecond timiing using interrupts
extern unsigned int flame_duty;				// for interrupt-based pwm output so simulate flame
extern unsigned int testcount;				// for counting the tests done. 


/**************   FUNCTION PROTOTYPES   *****************/
void Run_Tests(void);


#endif	/* TESTBED_H */

