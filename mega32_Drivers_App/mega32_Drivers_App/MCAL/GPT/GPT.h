

#ifndef GPT_H_
#define GPT_H_


typedef void (*IsrCallBackFnType)(void);

extern void GPT_Init(IsrCallBackFnType IsrCbkFun);
extern void GPT_TimerStart(void);
extern void GPT_TimerStop(void);


#endif /* GPT_H_ */