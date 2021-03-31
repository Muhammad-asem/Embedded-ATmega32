/*
 * i2c.h
 *
 * Created: 3/9/2016 3:54:53 PM
 *  Author: Muhammad Asem
 */ 


#ifndef I2C_H_
#define I2C_H_

typedef unsigned char byte;
void I2C_master_init(void);
byte I2C_Detect(byte addr);
byte I2C_FindDevice(byte start);
void I2C_Start (byte slaveAddr);
byte I2C_Write (byte data);
byte I2C_ReadACK (void);
byte I2C_ReadNACK (void);
void I2C_WriteByte(byte busAddr, byte data);
void I2C_WriteRegister(byte busAddr, byte deviceRegister, byte data);
byte I2C_ReadRegister(byte busAddr, byte deviceRegister);
void DS1307_GetTime(byte *hours, byte *minutes, byte *seconds);
void DS1307_GetDate(byte *months, byte *days, byte *years);
void SetTimeDate(byte month,byte days ,byte years,byte hours,byte min,byte sec);
void TwoDigits(byte data);
void WriteDate(void);
void WriteTime(void);


#endif /* I2C_H_ */