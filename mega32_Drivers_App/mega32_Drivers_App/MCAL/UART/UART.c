/*
 * UART.c
 *
 * Created: 2/12/2016 6:06:44 PM
 *  Author: Muhammad Asem
 */ 
//#define
#include <avr/io.h>
#define F_CPU 1000000u
#define CALC_UBRR(BAUD) (F_CPU/16/BAUD) - 1
void USART_Init( unsigned int baudRate )
{
	/* Set baud rate */
	
	unsigned int baud = CALC_UBRR(baudRate);
	UBRRH = ( unsigned char)(baud>>8);
	UBRRL = ( unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
}
