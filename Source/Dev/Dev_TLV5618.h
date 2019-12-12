#ifndef DEV_TLV5618_H
#define DEV_TLV5618_H
#include "stm32f10x.h"
#include "sys.h"

#define DAC_REF 5.0f
#define CS2		PBout(8)
#define	CS1  	PBout(9)


#define delay	{__NOP();__NOP();}

typedef enum{
		slow = 0,
		fast
	}DaMode;
		
typedef enum{
		channel1 = 0,	//��ˮ1���ڷ�
		channel2,		//��ˮ2���ڷ�
		channel3,   	//��ˮ���ڷ�
		channel4    	//��ˮ���ڷ�
	}DaChannel;

void TLV5618Init(void);
void TLV5618Out(DaChannel chanel, DaMode mode, float v);

#endif

