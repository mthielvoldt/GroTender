/* 
 * File:   main.c
 * Author: rileyt
 *
 * Created on October 4, 2014, 10:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p24FV32KA301.h>

#include "testbed.h"


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


unsigned int millis1 = 0;
unsigned int millis2 = 0;
unsigned int flame_duty  = 0;
unsigned int testcount = 0;

int main(int argc, char** argv) {

	char c;

    Configure();

    while(1) {
		
		while(SWITCH) {
			// actions for switch on.
		}

		if (millis1 >= 200){
			millis1 = 0;
			LED = ~LED;
		}
		
		if (U1STAbits.URXDA) {
			c = U1RXREG;
			switch(c) {
				case 't' :
					Report(T1);
					break;
				case 'l' :
					Send_Command(LOCKcmd);
					break;
			}
		}
	}
    return (EXIT_SUCCESS);
}


/***************	INTERRUPT SERVICE ROUTINES    *********************/



