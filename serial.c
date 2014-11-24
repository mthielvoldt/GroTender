
#include "serial.h"
#include <xc.h>

unsigned char rx2_buffer[BUFFER_SIZE];
unsigned char rx2_buffer_head, rx2_buffer_tail, rx1_buffer_head, rx1_buffer_tail;

unsigned char U2_Read(void)
{
	unsigned char val;
	val = rx2_buffer[ rx2_buffer_tail ];

    if (++rx2_buffer_tail >= BUFFER_SIZE)
        rx2_buffer_tail = 0;
	return val;
}

void U2_Purge(void)
{
	rx2_buffer_tail = rx2_buffer_head;
}

void U2Print(const char str[], const unsigned char len){
	static unsigned char i;
	for( i=0 ; i<len ; i++ )
	{
		while ( !U2STAbits.TRMT );  // pause here while sending the byte
		U2TXREG = str[i];
	}
}

// Receive interrupt buffering
/*void __attribute__((interrupt, shadow, no_auto_psv)) _U1RXInterrupt()
{
	++rx1_buffer_head;
    if (rx1_buffer_head >= BUFFER_SIZE) // Wrap around the fifo.
        rx1_buffer_head = 0;

    if (rx1_buffer_head == rx1_buffer_tail) { // Fifo is full.  FIXME: Do we need to worry about ever dropping this on the floor?
		--rx1_buffer_head;
        return;
	}
    rx1_buffer[ rx1_buffer_head ] = U2RXREG;
	IFS0bits.U1RXIF = 0;		// manually cleared U2RX Interrupt flag
}*/

// Transmit handling.
/*void Service_Tx2(void) {
    if (tx_buffer_tail != tx_buffer_head) // Do we have stuff to send?
    {
        if (PIR1bits.TXIF) // Are we ready to transmit another byte.
        {
            if (++tx_buffer_tail >= sizeof ( tx_buffer))
                tx_buffer_tail = 0;

            TXREG = tx_buffer[ tx_buffer_tail ];
        }
    }
}*/