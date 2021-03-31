
#include "DIO.h"
#include "GPT.h"
#include "DISP.h"


#define SEVEN_SEGMENT_DELAY 10

#define UNIT_ENABLE 1<<2
#define TENTH_ENABLE 1<<3

static unsigned long int TimeoutCounter ;
static void TimeoutInc(void)
{
	TimeoutCounter ++;
}

 void DISP_Init(void)
{
	TimeoutCounter = 0;
	DIO_vidWritePortDirection(PC,0xfc,0xff);
	DIO_vidWritePortData(PC,0xfc,0x00);
	GPT_Init(TimeoutInc);
	GPT_TimerStart();
}
 void DISP_WriteData(unsigned char u8Data)
{
	
	static int flag =0 ; 
	unsigned char data;
	if (TimeoutCounter>10)
	{
		if (flag==0)
		{
			data = 0x00;
			data |= ((u8Data %10) << 4) | UNIT_ENABLE;
			DIO_vidWritePortData(PC,0xFc,data);
			flag=1;
		}
		else
		{
			data = 0x00;
			data |= ((u8Data/10) << 4) | TENTH_ENABLE;
			DIO_vidWritePortData(PC,0xFc,data);
			
			flag=0;
		}
		TimeoutCounter=0;
	}
}