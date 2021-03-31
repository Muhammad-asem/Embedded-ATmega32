/*
 * EEPROM.h
 *
 * Created: 2/28/2016 10:44:01 PM
 *  Author: Muhammad Asem
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
//void eeprom_write()

void EEPROM_write( unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read( unsigned int uiAddress);

#endif /* EEPROM_H_ */