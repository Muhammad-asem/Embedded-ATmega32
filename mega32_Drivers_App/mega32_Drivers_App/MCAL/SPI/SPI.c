/*
 * SPI.c
 *
 * Created: 07/02/2016 06:09:02 م
 *  Author: hossam
 */ 
#include <avr/io.h>
#include "SPI.h"
#include "DIO.h"

#define MOSI_PORT PB
#define MOSI_MASK 1<<5
#define SCK_PORT PB
#define SCK_MASK 1<<7

#define INIT_MOSI_DIRECTION() DIO_vidWritePortDirection(MOSI_PORT,MOSI_MASK,0xff)
#define INIT_SCK_DIRECTION() DIO_vidWritePortDirection(SCK_PORT,SCK_MASK,0xff)

void SPI_MasterInit(SpiConfigType * ConfigPtr)
{
	
	INIT_MOSI_DIRECTION(); //for master config
	INIT_SCK_DIRECTION(); //also for master config
	SPCR = 0x00;// why not the bit mask method
	SPCR|= (ConfigPtr->DataOrder << DORD) | (ConfigPtr->Mode << CPHA) | (1<<MSTR) | (ConfigPtr->PreScaler);
	SPCR|= 1 <<SPE;
	
}
unsigned char SPI_Trsfr(unsigned char InputData)
{
	SPDR = InputData;
	while((SPSR & (1<<SPIF)) == 0x00)
	;
	return SPDR;
}

