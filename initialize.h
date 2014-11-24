/* 
 * File:   initialize.h
 * Author: rileyt
 *
 * Created on October 9, 2014, 3:30 PM
 */

#ifndef INITIALIZE_H
#define	INITIALIZE_H

void Initialize(void);

void IO_Init(void);
// Sets pins as either input or output and as either digital or analog.

void UART1_Init(void);
// Sets up UART1 hardware peripheral (host PC to tester)

void UART2_Init(void);
// Sets up UART2 hardware peripheral ( target PIC to tester )

void TMR1_Init(void);
// Sets up TMR1 hardware timer (used for interrupt-based milisecond timing)

void TMR2_Init(void);
// Sets up TMR2 (used for interrupt-based PWM flame simulation)

void ADC_Init(void);
// Sets up Analog to Digital converter for taking input.

void Input_Capture3_Init(void);
// Sets up Input capture using Timer 3 for measuring pulse widths on pin 15 (RB12)

#endif	/* INITIALIZE_H */
