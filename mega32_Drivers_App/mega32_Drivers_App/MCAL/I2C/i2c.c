/*
 * i2c.c
 *
 * Created: 3/9/2016 3:54:38 PM
 *  Author: Muhammad Asem
 */ 
#include "i2c.h"
#include "LCD.h"
#define F_CPU 1000000ul
#define SCL_CLOCK 10000ul //takecare of this 
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





//#define F_SCL 100000L // I2C clock speed 100 KHz
#define READ 1
#define TW_START 0xA4 // send start condition (TWINT,TWSTA,TWEN)
#define TW_STOP 0x94 // send stop condition (TWINT,TWSTO,TWEN)
#define TW_ACK 0xC4 // return ACK to slave
#define TW_NACK 0x84 // don't return ACK to slave
#define TW_SEND 0x84 // send data (TWINT,TWEN)
#define TW_READY (TWCR & 0x80) // ready when TWINT returns to logic 1.
#define TW_STATUS (TWSR & 0xF8) // returns value of status register
#define I2C_Stop()  TWCR = TW_STOP



void I2C_master_init()
{
	// initialize TWI clock: 10 kHz clock, TWPS = 0 => prescaler = 1
	TWSR = 0;                         // no prescaler
	TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  // =42 ...must be > 10 for stable operation
}


/*byte I2C_Detect(byte addr) // ithink this has no meaning ----------------------
// look for device at specified address; return 1=found, 0=not found
{
	TWCR = TW_START;  // send start condition
	while (!TW_READY);  // wait
	TWDR = addr;  // load device's bus address
	TWCR = TW_SEND;  // and send it
	while (!TW_READY);  // wait
	return (TW_STATUS==0x18);  // return 1 if found; 0 otherwise
}
byte I2C_FindDevice(byte start)
// returns with address of first device found; 0=not found
{
	for (byte addr=start;addr<0xFF;addr++)  // search all 256 addresses
	{
		if (I2C_Detect(addr) )  // I2C detected?
		return addr;  // leave as soon as one is found
	}
	return 0;  // none detected, so return 0.
}*/
void I2C_Start (byte slaveAddr)// start and send slave address
{
	
	START();
	FLAG_CHECK();
	TWDR = slaveAddr;  // load device's bus address
	FLAG_REFRESH();
	FLAG_CHECK();
	/*TWCR = TW_START;  // send start condition
	while (!TW_READY);  // wait to check 
	TWDR = slaveAddr;  // load device's bus address
	TWCR = TW_SEND;  // and send it
	while (!TW_READY);  // wait
	//I2C_Detect(slaveAddr);*/
}
byte I2C_Write (byte data)  // sends a data byte to slave
{
	TWDR = data;  // load data to be sent witing the interrupt clear
	FLAG_REFRESH(); //to allow twi to work
	//TWCR = TW_SEND;  //and send it this will refresh flag and enable the TWI
	//while (!TW_READY);  // wait for check of flag  
	FLAG_CHECK();
	return (TW_STATUS!=0x28); //return zero for not satisfy and 1 for satisfiction 
}
byte I2C_ReadACK ()  // reads a data byte from slave
{
	TWCR = TW_ACK;  // ack = will read more data
	//while (!TW_READY);  // wait
	FLAG_CHECK();
	return TWDR;
	//return (TW_STATUS!=0x28);
}
byte I2C_ReadNACK ()  // reads a data byte from slave
{
	TWCR = TW_NACK;  // nack = not reading more data
	while (!TW_READY);  // wait
	return TWDR;
	//return (TW_STATUS!=0x28);
}
void I2C_WriteByte(byte busAddr, byte data)
{
	I2C_Start(busAddr);  // send bus address and select addres
	I2C_Write(data);  // then send the data byte
	I2C_Stop();
}
void I2C_WriteRegister(byte busAddr, byte deviceRegister, byte data)//------------->
{
	I2C_Start(busAddr);  // send bus addres THIS IS LIKE THE I2C READ_REGISTER I2C_Start(bussAdd+0 for write) but it has no mean to add+0 
	I2C_Write(deviceRegister); //first byte = device register address
	I2C_Write(data);  // second byte = data for device register
	I2C_Stop();
}
byte I2C_ReadRegister(byte busAddr, byte deviceRegister) ////--------->
{
	byte data = 0;
	I2C_Start(busAddr);  // send device address
	I2C_Write(deviceRegister);  // set register pointer
	I2C_Start(busAddr+READ);  // restart as a read operation
	data = I2C_ReadNACK();  // read the register data
	I2C_Stop();  // stop
	return data;
}
// ---------------------------------------------------------------------------
// DS1307 RTC ROUTINES

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
void SetTimeDate(byte month,byte days ,byte years,byte hours,byte min,byte sec)
// simple, hard-coded way to set the date.
{
	I2C_WriteRegister(DS1307,MONTHS_REGISTER,  month);//0x08
	I2C_WriteRegister(DS1307,DAYS_REGISTER,  days);
	I2C_WriteRegister(DS1307,YEARS_REGISTER,  years);
	I2C_WriteRegister(DS1307,HOURS_REGISTER,  hours+0x40);  // add 0x40 for PM
	I2C_WriteRegister(DS1307,MINUTES_REGISTER, min);
	I2C_WriteRegister(DS1307,SECONDS_REGISTER, sec);
}

//// output format handling 
void TwoDigits(byte data)
// helper function for WriteDate() and WriteTime()
// input is two digits in BCD format
// output is to LCD display at current cursor position
{
	byte temp = data>>4;  // get upper 4 bits
	lcd_writeChar(temp+'0');  // display upper digit
	data &= 0x0F;  // get lower 4 bits
	lcd_writeChar(data+'0');  // display lower digit
}
void WriteDate()
{
	byte months, days, years;
	DS1307_GetDate(&months,&days,&years);
	TwoDigits(months);
	lcd_writeChar('/');
	TwoDigits(days);
	lcd_writeChar('/');
	TwoDigits(years);
}
void WriteTime()
{
	byte hours, minutes, seconds;
	DS1307_GetTime(&hours,&minutes,&seconds);
	TwoDigits(hours);
	lcd_writeChar(':');
	TwoDigits(minutes);
	lcd_writeChar(':');
	TwoDigits(seconds);
}