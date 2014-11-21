/*===========================================================================

	Hardware Initialization functions for Fire Pixel Testbed PIC24F

        R01 Oct 2014 / MGT
            Initial revision.

============================================================================*/

#include <p24FV32KA301.h>
#include "testbed.h"

void Initialize(void) {

}

void IO_Init(void) {

    LED_TRIS = 0;			// PORTB<15> as output for LED
	RESET_TRIS = 0;			// RA4 as output for resetting target.
	FLAME_TRIS = 0;

	SWITCH_ANS	= 0;		// digital input on switch
	SPARK_ANS	= 0;		// digital input on spark
	PV_ANS		= 0;		// digital input on pv.

	FLAME = 0;				// no flame sim
	RESET = 0;				// target running
}

void UART1_Init(void) {

	U1BRG = 12;			// 38400 @ Fcy = 2MHz and BRGH=0.

	ANSBbits.ANSB2 = 0;             // PORTB<2> (U1RX) digital input.
	TRISBbits.TRISB2 = 1;           // PORTB<2> (U1RX) as input,

	U1MODEbits.UARTEN = 1;		// UART1 is Enabled
	U1MODEbits.USIDL = 0;		// Continue operation at Idlestate
	U1MODEbits.IREN = 0;		// IrDA En/Decoder is disabled
	U1MODEbits.RTSMD = 0; 		// flow control mode
	U1MODEbits.UEN = 0b00;		// U2CTS, U2RTS are NOT used.
	U1MODEbits.WAKE = 0;		// Wake-up on start bit is DISabled.
	U1MODEbits.LPBACK = 0;		// Loop-back is disabled
	U1MODEbits.ABAUD = 0;		// auto baud is disabled
	U1MODEbits.RXINV = 0;		// No RX inversion
	U1MODEbits.BRGH = 1;		// high baud rate
	U1MODEbits.PDSEL = 0b00; 	// 8bit no parity
	U1MODEbits.STSEL = 0;		// one stop bit


	U1STAbits.UTXISEL1 = 0b00;
	U1STAbits.UTXINV = 0; 		// cant compile TXINV.. why???
	U1STAbits.UTXBRK = 0;		// sync break tx is disabled
	U1STAbits.UTXEN = 1;		// transmit  is enabled
	U1STAbits.URXISEL = 0b00;	// interrupt flag bit is set when RXBUF is filled whith 1 character
	U1STAbits.ADDEN = 0;		// address detect mode is disabled

	IFS0bits.U1RXIF = 0;		// clear interrupt flag of rx
	IEC0bits.U1RXIE = 1;		// enable rx recieved data interrupt

	IFS0bits.U1TXIF = 0;		// clear interrupt flag of rx
	IEC0bits.U1TXIE = 0;		// disable rx recieved data interrupt
}

void UART2_Init(void) {

	unsigned char c;
	U2BRG = 12;					// 38400 baud

    ANSBbits.ANSB1 = 0;         // PORTB<1> (U2RX) digital input.
    TRISBbits.TRISB1 = 1;       // PORTB<1> (U2RX) as input,

	U2MODEbits.UARTEN = 1;		// UART2 is Enabled
	U2MODEbits.USIDL = 0;		// Continue operation at Idlestate
	U2MODEbits.IREN = 0;		// IrDA En/Decoder is disabled
	U2MODEbits.RTSMD = 0; 		// flow control mode
	U2MODEbits.UEN = 0b00;		// U2CTS, U2RTS are not used.
	U2MODEbits.WAKE = 1;		// Wake-up on start bit is enabled
	U2MODEbits.LPBACK = 0;		// Loop-back is disabled
	U2MODEbits.ABAUD = 0;		// auto baud is disabled
	U2MODEbits.RXINV = 0;		// No RX inversion
	U2MODEbits.BRGH = 1;		// high baud rate
	U2MODEbits.PDSEL = 0b00; 	// 8bit no parity
	U2MODEbits.STSEL = 0;		// one stop bit


	U2STAbits.UTXISEL1 = 0b00;
	U2STAbits.UTXINV = 0; 		// cant compile TXINV.. why???
	U2STAbits.UTXBRK = 0;		// sync break tx is disabled
	U2STAbits.UTXEN = 1;		// transmit  is enabled
	U2STAbits.URXISEL = 0b00;	// interrupt flag bit is set when RXBUF is filled with 1 character
	U2STAbits.ADDEN = 0;		// address detect mode is disabled

	//IFS1bits.U2RXIF = 0;		// clear interrupt flag of rx
	IEC1bits.U2RXIE = 1;		// disnable rx recieved data interrupt

	IFS1bits.U2TXIF = 0;		// clear interrupt flag of rx
	IEC1bits.U2TXIE = 0;		// disable rx recieved data interrupt

	while(U2STAbits.URXDA) 
		c = U2RXREG;
}

void TMR1_Init(void) {

	T1CONbits.TCKPS = 0b00;		// set prescale to 1:1 ( 1 cycles per tick )
	T1CONbits.TCS = 0;			// internal clock (Fosc/2) = 2MHz

	PR1 = 2000;		// 16-bit period register.  T1IF set when TMR1 = PR1
					// 2000 tics * .5e-6 s = .001s

	_T1IF = 0;		// overflow flag cleared
	_T1IE = 1;		// interrupt ensabled
	_TON = 1;		// turn on timer.  (same as T1CONbits.TON)

}

void TMR2_Init(void) {

	T2CONbits.T32 = 0;			// 16-bit mode (not 32-bit).
	T2CONbits.TCKPS = 0b01;		// set prescale to 1:8 (8 cycles per tick)

	PR2 = 100;					// set period to 100 for simple percent scaling

	_T2IF = 0;
	_T2IE = 1;
	//T2CONbits.TON = 1;		// turned on by Set_Flame(duty)
}

void ADC_Init(void) {

	// port pins as analog inputs (all are by default)
	//PV_ANS = 1;
	CHG_ANS = 1;

	// Configure sample clock source and conversion trigger mode.
	AD1CON1bits.ADSIDL = 1;		// No operation in Idle mode
	AD1CON1bits.SSRC = 0b111;	//internal counter ends sampling and starts converting.
	AD1CON1bits.MODE12 = 0;		// 10-bit format (not 12).
	AD1CON1bits.FORM = 0b00;	// Unsigned decimal format (max 1023 in 10-bit mode)

	AD1CON2 = 0;	// Configure A/D voltage reference and buffer fill modes.
					// Vr+ and Vr- from AVdd and AVss(PVCFG<1:0>=00, NVCFG=0),
					// Inputs are not scanned,
					// Interrupt after every sample

	AD1CON3bits.ADCS = 0;	// 1*Tcy = Tad (.5us)
	AD1CON3bits.SAMC = 20;	// Auto-sample time = 20Tad = 10us (max here is 31 Tad)
							// 5kohm * 32pF * ln(2^13) = 1.44 us
							// conversion (after sampling) takes (N-bits + 2) * Tad

	AD1CSSL = 0;			// No inputs are scanned.

	IFS0bits.AD1IF = 0;		// Clear A/D conversion interrupt.
							// Configure A/D interrupt priority bits (AD1IP<2:0>) here, if
							// required. Default priority level is 4.
	//IEC0bits.AD1IE = 1;	// Enable A/D conversion interrupt
}

void Input_Capture3_Init(void)
// also uses TMR3
{
	ANSBbits.ANSB12	= 0;		// use digital input on RB12 pin.

	//IC3CON1bits.IC3TSEL = 0b000;	// (Default) use system TMR3; 111 = Fcy (Fosc/2)
	//IC3CON1bits.ICI = 0b00;		// (Default) interrupt on every capture event
	//IC3CON1bits.ICSIDL = 0;		// (Default) continues to operate in cpu idle mode
	//IC3CON1bits.ICOV				// Read-only: 1 when overflow occurred	
	//IC3CON1bits.ICBNE				// Read-only: 1 when a byte to read is in the buffer


	//IC3CON2bits.ICTRIG = 0;			// (Default) syncronized mode;  1= 'triggered' mode
	//IC3CON2bits.SYNCSEL = 0b01101;	// (Default) synchronized to Timer 3. reset TMR3 to reset internal counter.
	//IC3CON2bits.TRIGSTAT = 0;		// Indicates if timer source is running. Can be manually set if
									// SYNCSEL = 00000 to start timing manually.

	//IC3CON1bits.ICM = 0b001;		// capture every edge. 000 turns module off.


	//T3CONbits.TCS = 0;		// (Default) internal clock (Fcy)
	//T3CONbits.TCKPS = 0b00;	// (Default) set prescale to 1:1 (8 cycles per tick)

	//PR3 = 0xFF;				// (Default) set period to 65536 (max)

	//_T3IE = 0;				// (Default) disable interrupts
	//_IC3IE = 0;
	//T3CONbits.TON	= 0;		// (Default) turned on by Sample_Duty()
}