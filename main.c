/* 
 * File:   main.c
 * Author: rileyt
 *
 * Created on October 4, 2014, 10:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p24FV32KA301.h>
#include "grotender.h"


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


int main(int argc, char** argv) {

    Initialize();

    while(1) {
		ServiceSerial();
		ServiceCounters();
		ReadSensors(reading);
		ReportStatus(reading);
		TendSystems();
	}
    return (EXIT_SUCCESS);
}
void ReadSensors(unsigned int reading[]){
	unsigned long v1;
	v1 = SampleAD(TEMP_CHAN);
	v1 *= 10000;
	v1 -= 1024000;
	v1 >>= 11;		// /2048
	reading[TEMP] = (unsigned int) v1;
	//Temp = 10*(AD-102.4)/2.048;

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

	if (enableBits.humidifier)
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

void ReportStatus() {
	if (screenTmr < REFRESH)
		return;

	screenTmr = 0;
	fprintf()

}

void ServiceCounters(void){
	if(~_T1IF)
		return;
	_T1IF = 0;
	screenTmr++;

}

void ServiceSerial(void) {
	unsigned char c;
	if (U1STAbits.URXDA) {
		c = U1RXREG;
		switch(c) {
			case 't' :
				printf("Temperature: %d\n",readings(TEMP));
				break;
			case 'h' :
				printf("Humidity: %d\n",readings(HUMIDITY));
				break;
			case 'T' :
				SetSetpoint(TEMP);
				break;
			case 'H' :
				SetSetpoint(HUMIDITY);
				break;
				
		}
	}
}

void SetSetpoint(unsigned char setpoint_i){
	unsigned char c = ListenForNum();
	if(c) 
		setpoint(setpoint_i) = c;
}

unsigned char ListenForNum(void) {
	unsigned char str[10];
	if(fgets(str, 10, stdin))
	{
		return (unsigned char) atoi(str);
	}
	else
		return 0;
	
}

unsigned int SampleAD(unsigned int channel)
{

	unsigned int ADCValue;

	AD1CHS = channel;			// Configure positive and negative input channels
	AD1CON1bits.ADON = 1;		// Turn on A/D
	AD1CON1bits.SAMP = 1;		// Start sampling the input (automatically cleared)

	while (!AD1CON1bits.DONE);	// stay here until conversion is done

	ADCValue = ADC1BUF0;		// then get ADC value
	AD1CON1bits.ADON = 0;		// turn AD off again
	return ADCValue;

}

/***************	INTERRUPT SERVICE ROUTINES    *********************/



