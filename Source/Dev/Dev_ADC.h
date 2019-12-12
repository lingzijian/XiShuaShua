#ifndef DEV_ADC_H
#define DEV_ADC_H
#include "stm32f10x.h"

#define ADC1CHANNELNUM  1
#define ADC1BUFFSIZE   30

#define ADC2CHANNELNUM 0
#define ADC2BUFFSIZE   0

#define ADC3CHANNELNUM 3
#define ADC3BUFFSIZE   30

typedef enum{
		AD1 = 0,
		AD2,
		AD3
	}AD;

#define ChannelORP	0
#define ChannelPH1	1
#define ChannelPH2	2
//#define ChannelNTC	3
//#define	ChannelTDS	4

#define ChannelCPU_temperature  0

#define VREF	3.3f

extern u16 ADC1_Buff[ADC1BUFFSIZE][ADC1CHANNELNUM];
extern u16 ADC3_Buff[ADC3BUFFSIZE][ADC3CHANNELNUM];

void  ADC1_Init(void);
void  ADC3_Init(void);

float AD1_Average(u8 channel);
float AD3_Average(u8 channel);



#endif

