/*
* LCD.c
*
* Created: 1/29/2016 4:48:19 PM
*  Author: Muhammad Asem
*/
//local symbols
#include"DIO.h"
#define F_CPU 8000000u //check this 
#include <util/delay.h>
#define LCD_CTRL_PORT PB
#define LCD_DATA_PORT PA

#define LCD_RS_PIN_NUM 1u //enumeration is starting from 0 take care
#define LCD_RW_PIN_NUM 2u
#define LCD_EN_PIN_NUM 3u
#define LCD_DATA_PIN_NUM 4u
#define LCD_RS_MASK (1<<LCD_RS_PIN_NUM)
#define LCD_RW_MASK (1<<LCD_RW_PIN_NUM)
#define LCD_EN_MASK (1<<LCD_EN_PIN_NUM)
#define LCD_DATA_MASK (0xf0) // i.e 0b11110000 in port A  :)


//local macros
#define LCD_RS_SET() DIO_vidWritePortData(LCD_CTRL_PORT,LCD_RS_MASK,0xff)
#define LCD_RS_CLR() DIO_vidWritePortData(LCD_CTRL_PORT,LCD_RS_MASK,0x00)
#define LCD_RS_INIT()  DIO_vidWritePortDirection(LCD_CTRL_PORT,LCD_RS_MASK,0xff);\
                       DIO_vidWritePortData(LCD_CTRL_PORT,LCD_RS_MASK,0x00)


#define LCD_RW_SET() DIO_vidWritePortData(LCD_CTRL_PORT,LCD_RW_MASK,0xff)
#define LCD_RW_CLR() DIO_vidWritePortData(LCD_CTRL_PORT,LCD_RW_MASK,0x00)
#define LCD_RW_INIT()  DIO_vidWritePortDirection(LCD_CTRL_PORT,LCD_RW_MASK,0xff);\
                       DIO_vidWritePortData(LCD_CTRL_PORT,LCD_RW_MASK,0x00)



#define LCD_EN_SET() DIO_vidWritePortData(LCD_CTRL_PORT,LCD_EN_MASK,0xff)
#define LCD_EN_CLR() DIO_vidWritePortData(LCD_CTRL_PORT,LCD_EN_MASK,0x00)
#define LCD_EN_INIT()  DIO_vidWritePortDirection(LCD_CTRL_PORT,LCD_EN_MASK,0xff);\
                       DIO_vidWritePortData(LCD_CTRL_PORT,LCD_EN_MASK,0x00)


#define LCD_DATA_SET_VAL(VAL) DIO_vidWritePortData(LCD_DATA_PORT,LCD_DATA_MASK,(VAL<<LCD_DATA_PIN_NUM))

#define LCD_DATA_INIT()  DIO_vidWritePortDirection(LCD_DATA_PORT,LCD_DATA_MASK,0xff);\
                         DIO_vidWritePortData(LCD_CTRL_PORT,LCD_DATA_MASK,0x00)

#define HIGH(data) (data&0xf0)>>4
#define LOW(data)  (data&0x0f)
/*******************/

//private functions declaration
static void SendCommand(unsigned char command);
static void sendData(unsigned char data);


//private functions  
static void SendCommand(unsigned char command)
{
LCD_RS_CLR();
LCD_RW_CLR();
LCD_DATA_SET_VAL(HIGH(command));
LCD_EN_SET();
_delay_us(2);
LCD_EN_CLR();
LCD_DATA_SET_VAL(LOW(command));
LCD_EN_SET();
_delay_us(2);
LCD_EN_CLR();
_delay_us(2000);
}

static void sendData(unsigned char data)
{   LCD_RS_SET();
	LCD_RW_CLR();
	LCD_DATA_SET_VAL(HIGH(data));
	LCD_EN_SET();
	_delay_us(2);
	LCD_EN_CLR();
	LCD_DATA_SET_VAL(LOW(data));
	LCD_EN_SET();
	_delay_us(2);
	LCD_EN_CLR();
	_delay_us(2000);
}
						 
//global function 
void lcd_init() // can we 
{
	/* iwill send commands to the lcd so we need to activate the commands mode then send the
	init command and clear command of the lcd firmware   */
		LCD_RS_INIT();
		LCD_EN_INIT();
		LCD_RW_INIT();
		LCD_DATA_INIT();
	SendCommand(0x33);
	SendCommand(0x32);
	SendCommand(0x28);
	SendCommand(0x0c);	
	SendCommand(0x01);
	//sendData('z');
}

void lcd_writeChar(unsigned char z  )
{
	
sendData(z);
		
}
	
	
void lcd_writeString( char *p)
{

	int x=0;
	/*int c=0;
		while(1)
	{
		
		sendData((*(p+c)));
		
		c++;
		if (*(p+c)=='\0') break;
	}*/
	for (x=0; p[x]!=('\0'); x++)
	{
		sendData(p[x]);
	}
	
}	
	
void lcd_clear()
{
	SendCommand(0x01);
}

void LCD_GoToXy(char PosX,char PosY)
{
	char i;
	/*Return Home*/
	SendCommand(0x02);
	if( PosY!= 0)
	{
		/*Move to the second line*/
		SendCommand(0xC0);
	}
	/*Move to the column position*/
	for(i=0 ; i < PosX; i++)
	{
		SendCommand(0x14);
	}
}