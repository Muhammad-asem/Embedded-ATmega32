 
#include <avr/io.h>
#include "GPT.h"
#include <avr/interrupt.h>
#define TIMER_START_MASK 0x02
#define TIMER_STOP_MASK 0xf8

static IsrCallBackFnType CallBackPtr;

void GPT_Init(IsrCallBackFnType IsrCbkFun)
{
	TCCR0 = 0x00;
	TCCR0 |= 1<<WGM01;
	OCR0 = 124;
	TIMSK |= 1 <<OCIE0;	
	CallBackPtr = IsrCbkFun;// 3ashan ana msh ha3raf abasy 7aga lel isr ..f ba7otaha fe global variable lel file dah 
}
 void GPT_TimerStart(void)
 {
	 TCCR0 |= TIMER_START_MASK;
 }
 void GPT_TimerStop(void)
 {
	 TCCR0 &= TIMER_STOP_MASK;
	 TCNT0 = 0x00;
 }
 
 ISR(TIMER0_COMP_vect)
 {
	
	 CallBackPtr();
 }