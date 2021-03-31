
#include <avr/io.h>
void  ADC_Init(void)
{
	/*Initialize channel to 0*/
	ADMUX = 0x00;
	/*Select AVCC as a reference Voltage*/
	ADMUX |= 1<<REFS0;
	/*Select 8 bit precession*/
	ADMUX |= 1<<ADLAR;
	
	ADCSRA = 0x00;
	/*Enable ADC*/
	ADCSRA |= 1<<ADEN;
	/*Init Prescaler*/
	ADCSRA |= 0x01;
	
}
unsigned char ADC_Read(unsigned char ChannelId)
{
	/*Select channel*/
	ADMUX |= (ChannelId & 0x1f);
	/*Start Conversion*/
	ADCSRA |= 1<<ADSC;
	/*Wait for Conversion complete flag*/
	while((ADCSRA & (1 << ADIF)) == 0x00) //while flag is not 1 dont do any thing just wait until conversion completes 
	;
	/*Clear ADIF*/ 
	ADCSRA |= 1<<ADIF;
	
	/*Clear channel*/
	ADMUX &= 0xE0;
	/*return Data*/
	return ADCH;//return from 0-255
}