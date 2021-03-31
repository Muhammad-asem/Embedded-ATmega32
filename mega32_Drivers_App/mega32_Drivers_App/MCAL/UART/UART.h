/*
 * UART.h
 *
 * Created: 2/12/2016 6:07:17 PM
 *  Author: Muhammad Asem
 */ 


#ifndef UART_H_
#define UART_H_


void USART_Init( unsigned int baud );
void USART_Transmit( unsigned char data);
unsigned char USART_Receive( void );

#endif /* UART_H_ */