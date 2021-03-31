
#ifndef ADC_H_
#define ADC_H_

#define POT0_ID 0x00
#define POT1_ID 0x01
#define POT_DIF 0x10

void          ADC_Init(void);
unsigned char ADC_Read(unsigned char ChannelId);



#endif /* ADC_H_ */