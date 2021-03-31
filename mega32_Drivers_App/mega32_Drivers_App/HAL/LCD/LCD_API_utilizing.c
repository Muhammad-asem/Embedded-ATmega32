/*
 * LCD_API_utilizing.c
 *
 * Created: 3/2/2016 10:55:01 PM
 *  Author: Muhammad Asem
 */ 

/************************************************************************/
/* function definations                                                 */
/************************************************************************/
#define F_CPU 8000000ul
#include "LCD.h"
#include "KEYPAD.h"
#include <util/delay.h>
unsigned char dd;
void MoveStrinrgRight(char *StrPt, int length,unsigned char* fl)
{
	int i;
	length --;
	for(i=0;i<(LCD_LENGTH - length) ;i++)
	{
		KPD_ReadVal(&dd);
		if (dd!='n')
		{
			*fl=1;	
		break;
		
		} 
		
		if(i>0)
		{
			LCD_GoToXy(i-1,0);
			lcd_writeChar(' ');
		}
		LCD_GoToXy(i,0);
		lcd_writeString(StrPt);
		_delay_ms(50);
	}
	lcd_clear();
}


void MoveStrinrgLeft(char *StrPt, int length,unsigned char* fl)
{
	
	int i;
	length --;
	for(i = (LCD_LENGTH - length) ;i > 0 ; i --)
	{
		KPD_ReadVal(&dd);
		if (dd!='n')
		{
			*fl=1;
			break;
			
		}
		LCD_GoToXy(i,0);
		lcd_writeString(StrPt);
		_delay_ms(50);
		LCD_GoToXy(i+length-1,0);
		lcd_writeChar(' ');
	}
	lcd_clear();
}

