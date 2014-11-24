/* 
 * File:   FirePixel_Testbed.h
 * Author: rileyt
 *
 * Created on October 4, 2014, 10:42 PM
 */

#ifndef GROTENDER_H
#define	GROTENDER_H

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

#define DRYAIR		LATBbits.LATB9
#define DRYAIR_TRIS	TRISBbits.TRISB9
#define DRYAIR_LATCH	LATB
#define DRYAIR_PIN	_LATB_LATB9_MASK

#define WETAIR		LATBbits.LATB9
#define WETAIR_TRIS	TRISBbits.TRISB9
#define WETAIR_LATCH	LATB
#define WETAIR_PIN	_LATB_LATB9_MASK


// digital general purpose inputs (doesn't include UARTs)
#define SWITCH		PORTBbits.RB14
#define SWITCH_ANS	ANSBbits.ANSB14
#define SWITCH_PORT	PORTB
#define SWITCH_PIN	_PORTB_RB14_MASK

// analog inputs
#define TEMP_ANS	ANSAbits.ANSA2
#define TEMP_CHAN	13		// AN13 = RA2


// readings map (corresponds to indices of "readings" array.)
#define TEMP 0
#define HUMIDITY 1


/****************  USEFUL CONSTANTS  *******************/

#define HYST 5		// half a degree
#define HYST2 10	// hysteresis * 2;
#define REFRESH 100	// 10 hz

/***************   GLOBAL VARIABLES   *******************/
extern unsigned int temp1;				// these are temperatures
extern unsigned int reading[3];
extern unsigned int setpoint[3];
extern unsigned char screenTmr;
struct enableBits_s {
  unsigned char heatpump:1;
  unsigned char light:1;
  unsigned char air:1;
};
extern struct enableBits_s enableBits;

/**************   FUNCTION PROTOTYPES   *****************/
void ReadSensors(unsigned int readings[]);
void TendSystems(void);
void ReportStatus(const unsigned int []);

void ServiceSerial(void);
void ServiceCounters(void);
void ProcessCommand(unsigned char command, unsigned char value);

void SetSetpoint(unsigned char setpoint_i);
unsigned char ListenForNum(void);

unsigned int SampleAD(unsigned char);
void SendToDisplay(unsigned char str[]);

#endif	/* GROTENDER_H */

