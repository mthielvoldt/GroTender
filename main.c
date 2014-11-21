/* 
 * File:   main.c
 * Author: rileyt
 *
 * Created on October 4, 2014, 10:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p24FV32KA301.h>

#include "testmethods.h"
#include "testbed.h"
#include "tests.h"

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
			Relay_Byte();
		}

		if (millis1 >= 200){
			millis1 = 0;
			LED = ~LED;
		}
		
		if (U1STAbits.URXDA) {
			c = U1RXREG;
			switch(c) {
				case 't' :
					Run_Tests();
					break;
				case 'l' :
					Send_Command(LOCKcmd);
					break;
				case 'u' :
					Send_Command(UNLOCKcmd);
					break;
				case 'f' :
					if (flame_duty) Set_Flame(0);
					else Set_Flame(10);
					break;
			}
		}
	}
    return (EXIT_SUCCESS);
}


void Run_Tests()
// Runs all the tests
{
	Set_Flame(0);

	testcount = 0;
	Test_Wakeup();			// 1 test
	//printf("Wakeup Tests: %d\n",testcount);

	Test_Error_Message();	// 3 tests
	//printf("Err Msg. Tests: %d\n",testcount);

	Test_Report_Settings();		// 3 tests
	//printf("Report Tests: %d\n",testcount);

	Test_Ignition();		// 8 tests
	//printf("Ignition Tests: %d\n",testcount);

	Test_Flame_Detect();	// 2 tests
	//printf("Flame Detect Tests: %d\n",testcount);

	Test_Lit_Operation();
	//printf("Flame Control Tests: %d\n",testcount);

	Test_Calibration();
	//printf("Calibration Tests: %d\n",testcount);

	Test_Overshoot();
	//printf("Overshoot Tests: %d\n",testcount);

	Test_Normalization();
	//printf("Normalization Tests: %d\n",testcount);

	Test_Enter_Lockout();
	//printf("Lockout Tests: %d\n",testcount);
	
	Set_Flame(0);

	printf("Did %d tests.\n", testcount);
}



/***************	INTERRUPT SERVICE ROUTINES    *********************/



