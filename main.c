/* 
 * File:   main.c
 * Author: rileyt
 *
 * Created on October 4, 2014, 10:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "grotender.h"
#include "serial.h"
#include "initialize.h"


// <editor-fold defaultstate="collapsed" desc="CONFIG bits">

//_FBS        (); // no boot-program flash segment, no protection
//_FGS        (); // no general segment code protection or write protection
_FOSCSEL	( SOSCSRC_DIG ); // no switchover mode, High-power high accuracy mode, 8MHz with Divide by B (FRCDIV)
_FOSC		( OSCIOFNC_OFF ); // Fail-safe Clock monitor off, Secondary osc high-power, Primary osc. >8MHz,
_FWDT		( FWDTEN_OFF ); // Watchdog timer is disabled in hardware, SWDTEN bit disabled;
//_FPOR       (); // MCLR enabled, default I2C pin locations, Brown-out reset enabled in hardware.
_FICD		( ICS_PGx2 ); // Background debugger disabled, PGEC2/PGED2 used to program.
//_FDS        (); // Deep sleep watchdog timer enabled
// </editor-fold>

//#define U1RX_INTERRUP
unsigned int msTic, displayTmr = 0;
unsigned int millis, millis2, minutes, ledTmr = 0;			// these are temperatures
unsigned int reading[] = {0,0,0};
unsigned int setpoint[] = {0,0,0};
unsigned int progTime[] = {1, 500, 1440, 0};
unsigned int progT[] = {266, 131, 233, 0};
unsigned int progH[] = {975, 975, 975, 0};
unsigned char progL[] = {1, 0, 1, 0};
unsigned char progIndex = 0;
unsigned int  day = 0;
struct enableBits_s enableBits;

unsigned int __attribute__ ((space(eedata))) eeData = 0x1234;

int main(int argc, char** argv) {

    Initialize();

    while(1) {
		ServiceSerial();
		ReadSensors(reading);
		ServiceTimers();
		ReportStatus(reading);
		TendSystems();
		ServiceSeq();
		
		if (ledTmr > 200) {
			ledTmr = 0;
			LED = ~LED;
		}
	}
    return (EXIT_SUCCESS);
}

void ReadSensors(unsigned int reading[]){
	static unsigned int l;
	static unsigned int val[AD_BUFFER_SIZE];
	static unsigned char j, i=0;

	++i;
    if (i >= AD_BUFFER_SIZE) // Wrap around the fifo.
        i = 0;

	l = SampleAD(TEMP_CHAN);
	l -= 252;
	l <<= 1;		// *2  */
	//l *= 1650;
	//l >>= 10;	// /1024
	//l -=400;
	val[i] = (unsigned int) l;

	reading[TEMP] = 0;
	for ( j=0 ; j < AD_BUFFER_SIZE ; j++) {
		reading[TEMP] += val[j];
	}
	reading[TEMP] /= AD_BUFFER_SIZE; // /8
	//Temp * 10  = 10*(AD-250)*2; (positive ref = 2.048V) 0C = .5V 1C = 10mV

	//readings[HUMIDITY] = SampleAD(HUMIDITY_CHAN);
}

void TendSystems(void) {

	if (enableBits.heatpump){
		if (HEATCOOL) {			// heater mode
			if (HEATPUMP) {		// heating on
				if (reading[TEMP] > (setpoint[TEMP]+HYST))
					HEATPUMP = 0;
			}
			else {				// heating off
				if (reading[TEMP] < (setpoint[TEMP]-HYST))	// natually cooled
					HEATPUMP = 1;
				else if (reading[TEMP] > (setpoint[TEMP]+HYST2)) { // naturally heated.
					HEATCOOL = 0;		// switch to cooling
					HEATPUMP = 1;
				}
			}
		}
		else {					// cooler mode
			if (HEATPUMP) {		//cooling on
				if (reading[TEMP] < (setpoint[TEMP]-HYST))
					HEATPUMP = 0;
			}
			else {				// cooling off
				if (reading[TEMP] > (setpoint[TEMP]+HYST))	// natually warming
					HEATPUMP = 1;
				else if (reading[TEMP] < (setpoint[TEMP]-HYST2)) { // naturally cooled.
					HEATCOOL = 1;		// switch to heating
					HEATPUMP = 1;
				}
			}
		}
	}

	if (enableBits.air)
	{
		if (DRYAIR) {		// drying
			if (reading[HUMIDITY] < (setpoint[HUMIDITY]-HYST))
			{
				DRYAIR = 0;
				WETAIR = 1;
			}
		}
		else {				// moistening
			if (reading[HUMIDITY] > (setpoint[HUMIDITY]+HYST))
			{
				DRYAIR = 1;
				WETAIR = 0;
			}
		}
	}
}

void ServiceSeq(void) {
	if (minutes < progTime[progIndex]) return;

	progIndex++;
	if (progTime[progIndex] == 0){
		progIndex = 0;			// loop the index
		minutes = 0;			// start counting time from bottom (new day)
		day++;
	}

	setpoint[TEMP] = progT[progIndex];
	setpoint[HUMIDITY] = progH[progIndex];
	LIGHT = (progL[progIndex] > 0);

}

void ReportStatus(const unsigned int reading[]) {
	static char str[32];
	static const char reset[]= {0xFE, 0x51};

	if (displayTmr < REFRESH)
		return;

	displayTmr = 0;
	U2Print(reset,2);
	DelayMs(5);
	sprintf(str, "Temp: %d", reading[0]);
	U2Print(str,9);

}

void ServiceSerial(void) {
	static unsigned char c;
	unsigned int i;

	if (U1STAbits.URXDA) {
		c = U1RXREG;
		switch(c) {
			case 't' :
				printf("Temp. Setpoint: %d\n",setpoint[TEMP]);
				break;
			case 'h' :
				printf("Humidity: %d\n",reading[HUMIDITY]);
				break;
			case 'T' :
				SetSetpoint(TEMP);
				break;
			case 'H' :
				SetSetpoint(HUMIDITY);
				break;
			case 'i' :
				printf("progIndex: %d\n", progIndex);
				break;
			case 'm' :
				printf("Minutes: %d\n", minutes);
				break;
			case 'M' :
				i = ListenForNum();
				if (i) {
					minutes = i;
					progIndex = 0;
					setpoint[TEMP] = progT[0];
					setpoint[HUMIDITY] = progH[0];
					LIGHT = (progL[0] > 0);
					printf("minutes set to: %d\n", i);
				}
				else
					printf("invalid number\n");
				break;
			case 'e' :
				printf("progTime[progIndex]: %d\n", progTime[progIndex]);
				break;
			case 'l' :
				if (LIGHT)
					printf("Light turned off\n");
				else
					printf("Light turned on\n");
				LIGHT = ~LIGHT;
				break;
			case 'd' :
				printf("Day: %d\n", day);
				break;
			case 'D' :
				i = ListenForNum();
				if (i) {
					day = i;
					printf("day set to: %d\n", i);
				}
				else
					printf("invalid number\n");
				break;
		}
		while(U1STAbits.URXDA)
			c = U1RXREG;
	}
}

void ServiceTimers(void) {
	if(!msTic)
		return;
	msTic = 0;
	ledTmr++;
	displayTmr++;
	millis2++;
	if (millis2 >= 60000)
	{
		minutes++;
		millis2 = 0;
	}
}

void SetSetpoint(unsigned char setpoint_i){
	unsigned int i = ListenForNum();
	if(i) {
		setpoint[setpoint_i] = i;
		printf("New Setpoint: %d\n", i);
	}
	else
		printf("Invalid Septoint\n");

	switch (setpoint_i) {
		case TEMP:
			enableBits.heatpump = (i != 0);
			break;
		case HUMIDITY:
			enableBits.air = (i != 0);
			break;
	}
}

unsigned int ListenForNum(void) {
	char str[10];
	char *next_p;
	unsigned int i;
	if(fgets(str, 6, stdin))
	{
		i = (unsigned int) strtol(str, &next_p, 10);
		if(i>0)
			return i;
	}
	return 0;
	
}

unsigned int SampleAD(unsigned char channel){

	unsigned int ADCValue;

	AD1CHS = channel;			// Configure positive and negative input channels
	AD1CON1bits.ADON = 1;		// Turn on A/D
	AD1CON1bits.SAMP = 1;		// Start sampling the input (automatically cleared)

	while (!AD1CON1bits.DONE);	// stay here until conversion is done

	ADCValue = ADC1BUF0;		// then get ADC value
	AD1CON1bits.ADON = 0;		// turn AD off again
	return ADCValue;

}

void DelayMs(unsigned int delay){
	TMR1 = 0;
	millis = 0;
	while ( millis < delay );
}

void NVWrite(unsigned int newData, unsigned char i) {
	
	unsigned int offset;
	// Set up NVMCON to erase one word of data EEPROM
	NVMCON = 0x4004;
	// Set up a pointer to the EEPROM location to be erased
	TBLPAG = __builtin_tblpage(&eeData);	// Initialize EE Data page pointer
	offset = __builtin_tbloffset(&eeData);	// Initizlize lower word of address
	i <<= 1;
	offset += i;
	__builtin_tblwtl(offset, newData);		// Write EEPROM data to write latch
	asm volatile ("disi #5");				// Disable Interrupts For 5 Instructions
	__builtin_write_NVM();					// Issue Unlock Sequence & Start Write Cycle
	while(NVMCONbits.WR); // Optional: Poll WR bit to wait for
	// write sequence to complete
}

unsigned int NVRead(unsigned char i) {
	unsigned int offset;
	// Set up a pointer to the EEPROM location to be erased
	TBLPAG = __builtin_tblpage(&eeData); // Initialize EE Data page pointer
	offset = __builtin_tbloffset(&eeData); // Initizlize lower word of address
	i <<= 1;
	offset += i;
	return __builtin_tblrdl(offset); // Write EEPROM data to write latch
}

void __attribute__((interrupt, shadow, no_auto_psv)) _T1Interrupt()
// This  runs every 1 millisecond.
{
	IFS0bits.T1IF = 0;			// TMR1 cleared by hardware.

	//millis1++;
	//millis2++;
	asm ("inc %0\n inc %1"
			:"+T"(msTic), "+T"(millis)
			: // no inputs
			:"cc"); //*/
}

/***************	INTERRUPT SERVICE ROUTINES    *********************/



