/*
 * SPI.h
 *
 * Created: 07/02/2016 06:03:21 م
 *  Author: hossam
 */ 


#ifndef SPI_H_
#define SPI_H_
#include "DIO.h"

#define SS_PORT PB
#define SS_MASK 1<<4

#define SLAVE_SELCET_INIT() DIO_vidWritePortDirection(SS_PORT,SS_MASK,0xff);\
DIO_vidWritePortData(SS_PORT,SS_MASK,0x00)

#define SLAVE_SELECT_ACTIVATE() DIO_vidWritePortData(SS_PORT,SS_MASK,0xff)
#define SLAVE_SELECT_DEACTIVATE() DIO_vidWritePortData(SS_PORT,SS_MASK,0x00)


typedef struct 
{
	unsigned char PreScaler;
	unsigned char Mode;
	unsigned char DataOrder;
	}
	SpiConfigType;

void SPI_MasterInit(SpiConfigType * ConfigPtr);
unsigned char SPI_Trsfr(unsigned char InputData);



#endif /* SPI_H_ */