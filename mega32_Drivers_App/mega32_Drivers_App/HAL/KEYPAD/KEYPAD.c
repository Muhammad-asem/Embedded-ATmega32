/*
 * KEYPAD.c
 *
 * Created: 1/30/2016 6:39:01 PM
 *  Author: Muhammad Asem
 */ 

#include "DIO.h"

/*Local Symbols*/
#define KPD_COL_PORT PC
#define KPD_ROW_PORT PD
#define KPD_COL_MASK 0x70
#define KPD_ROW_MASK 0x3C
#define KPD_COL_PIN_NUM 4u
#define KPD_ROW_PIN_NUM 2u
/**************************************************/
#define KPD_COL_INIT() DIO_vidWritePortDirection(KPD_COL_PORT,KPD_COL_MASK,0x00)
#define KPD_ROW_INIT() DIO_vidWritePortDirection(KPD_ROW_PORT,KPD_ROW_MASK,0xff); \
DIO_vidWritePortData(KPD_ROW_PORT,KPD_ROW_MASK,0x00)

#define KPD_COL_READ(VALPTR) DIO_vidReadPortData(KPD_COL_PORT,KPD_COL_MASK,(VALPTR));\
*(VALPTR) = (*(VALPTR)) >> KPD_COL_PIN_NUM


#define KPD_ROW_WRITE(DATA) DIO_vidWritePortData(KPD_ROW_PORT,KPD_ROW_MASK,((DATA) << KPD_ROW_PIN_NUM))



void KPD_Init(void)
{
KPD_COL_INIT();
KPD_ROW_INIT();
}
void KPD_ReadVal(char* ValuePtr)
{	
unsigned char x;
KPD_ROW_WRITE(0x01);
KPD_COL_READ(&x);// if
 
if (x==0b000)
{
	*ValuePtr='n';
}

switch (x)
{
	//////////////////////////////////////////////////////////////////////////
case(0b100 ): //here is the issue

	
	*ValuePtr='3';


	break;
	//////////////////////////////////////////////////////////////////////////
	case(0b010):
	
		*ValuePtr='2';
	break;


	
	//////////////////////////////////////////////////////////////////////////
	
	case(0b001):
	
	
		
		*ValuePtr='1';
	
	
	break;
}
//////////////////////////////////////////////////////////////////////////
KPD_ROW_WRITE(0x02);
KPD_COL_READ(&x);

switch (x)
{
	case(0b100):
	*ValuePtr='6';	
	break;
	//////////////////////////////////////////////////////////////////////////
	case(0b010):
	*ValuePtr='5';	
	break;
	//////////////////////////////////////////////////////////////////////////
	case(0b001):
	*ValuePtr='4';	
	break;
}
//////////////////////////////////////////////////////////////////////////
KPD_ROW_WRITE(0x04);
KPD_COL_READ(&x);

switch (x)
{
	case(0b100 ):
	*ValuePtr='9';
	break;
	//////////////////////////////////////////////////////////////////////////
	case(0b010):
		*ValuePtr='8';	
	break;
	//////////////////////////////////////////////////////////////////////////
	case(0b001):
	*ValuePtr='7';	
	break;
}

KPD_ROW_WRITE(0x08);

KPD_COL_READ(&x);

 switch (x)
{
	case(0b100 ):
		*ValuePtr='#';
	break;
	//////////////////////////////////////////////////////////////////////////
	case(0b010):
		*ValuePtr='0';
	
	break;
	//////////////////////////////////////////////////////////////////////////
	case(0b001):
		*ValuePtr='*';
	break;
}

}


