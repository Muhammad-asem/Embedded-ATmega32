
#ifndef DIO_H_
#define DIO_H_

#define PA 0x00
#define PB 0x01
#define PC 0x02
#define PD 0x03

void DIO_vidWritePortDirection(unsigned char u8PortName,unsigned char u8PortMask,unsigned char u8Direction);
 void DIO_vidWritePortData(unsigned char u8PortName,unsigned char u8PortMask,unsigned char u8Data);
 void DIO_vidReadPortData(unsigned char u8PortName,unsigned char u8PortMask,unsigned char *pu8DataPtr);

#endif /* DIO_H_ */