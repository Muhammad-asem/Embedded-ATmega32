/*
 * I2C.c
 *
 * Created: 3/8/2016 5:37:57 PM
 *  Author: Muhammad Asem
 */ 

#if 0

#define F_CPU 1000000ul
#define SCL_CLOCK 10000ul
#include <avr/io.h>
#define START() TWCR |=(1<<TWSTA)|(1<<TWINT)|(1<<TWEN) //ana 5ayef mn el satr da 
#define FLAG_CHECK() while (!(TWCR & (1<<TWINT))) //WAIT the confirmation flag of reciving start cond note i didnt put the semicolon ;
#define FLAG_REFRESH() TWCR = (1<<TWINT) | (1<<TWEN) 
#define STOP() TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO)


#define DS1307 0xD0 //  address of DS1307 
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06
#define CONTROL_REGISTER 0x07
#define RAM_BEGIN 0x08
#define RAM_END 0x3F

void I2C_master_init()
{
	// initialize TWI clock: 10 kHz clock, TWPS = 0 => prescaler = 1 
	TWSR = 0;                         // no prescaler 
	TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  // =42 ...must be > 10 for stable operation 
}

void  I2C_send(unsigned char x)
{
	//start algorithm 
	START(); //for clearing flag and make it ready and also send start command 
	FLAG_CHECK(); //WAIT the confirmation flag of reciving start cond
	
	/*if ((TWSR & 0xF8) != START) //status register to confirm start order recived:)
	ERROR();// if not recived go to error algorithm*/
	
	//send the slave adress and read and write  i will make it general call 0x00
	////////////////////////Send Slave ddress and write bit//////////////////////////////////////////////////
	TWDR = 0x00; //7bit slave general call + 0 at LSB for writting defination ie W and 1 for Read so 0x00
	FLAG_REFRESH(); //clearing the flag from the last operation to be fresh
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////confirmation of address reciveng ///////////////////////////////////////////////
	FLAG_CHECK();
	/*if ((TWSR & 0xF8) != MT_SLA_ACK)
	ERROR();*/
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////sending data ////////////////////////////////////////////
	TWDR = x;
	FLAG_REFRESH(); //refreshig flag & enable the I2c..try to replace these 2 lines in order 
	//////////////////////////////////////////////////////////////////////
	
	///////////////////////////Data receiving confirmation///////////////////////////////////////////////
	
	FLAG_CHECK(); //and to confirm ACK/NACK 
	
	/*if ((TWSR & 0xF8) != MT_DATA_ACK)
	ERROR();*/  //this is check from the status register 
	/////////////////////////////////////////////////////////////////////
	///////////////////////////////////Re-Start Condition to recive///////////////////////////////////////
	START(); //for clearing flag and make it ready and also send start command 
	FLAG_CHECK; //WAIT the confirmation flag of receiving start cond
	
	/*if ((TWSR & 0xF8) != START) //status register to confirm start order recived:)
	ERROR();// if not recived go to error algorithm*/
	///////////////////////STOP condition ///////////////////////////////////////////////////
	STOP();
	
}




void DS1307_GetTime(byte *hours, byte *minutes, byte *seconds)
// returns hours, minutes, and seconds in BCD format
{
	*hours = I2C_ReadRegister(DS1307,HOURS_REGISTER);
	*minutes = I2C_ReadRegister(DS1307,MINUTES_REGISTER);
	*seconds = I2C_ReadRegister(DS1307,SECONDS_REGISTER);
	if (*hours & 0x40)  // 12hr mode:
	*hours &= 0x1F;   // use bottom 5 bits (pm bit = temp & 0x20)
	else *hours &= 0x3F;  // 24hr mode: use bottom 6 bits
}


void DS1307_GetDate(byte *months, byte *days, byte *years)
// returns months, days, and years in BCD format
{
	*months = I2C_ReadRegister(DS1307,MONTHS_REGISTER);
	*days = I2C_ReadRegister(DS1307,DAYS_REGISTER);
	*years = I2C_ReadRegister(DS1307,YEARS_REGISTER);
}


void SetTimeDate()
// simple, hard-coded way to set the date.
{
	I2C_WriteRegister(DS1307,MONTHS_REGISTER,  0x08);
	I2C_WriteRegister(DS1307,DAYS_REGISTER,  0x31);
	I2C_WriteRegister(DS1307,YEARS_REGISTER,  0x13);
	I2C_WriteRegister(DS1307,HOURS_REGISTER,  0x08+0x40);  // add 0x40 for PM
	I2C_WriteRegister(DS1307,MINUTES_REGISTER, 0x51);
	I2C_WriteRegister(DS1307,SECONDS_REGISTER, 0x00);
}


#endif 