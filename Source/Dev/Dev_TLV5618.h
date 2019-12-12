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
		channel1 = 0,	//供水1调节阀
		channel2,		//供水2调节阀
		channel3,   	//碱水调节阀
		channel4    	//酸水调节阀
	}DaChannel;

void TLV5618Init(void);
void TLV5618Out(DaChannel chanel, DaMode mode, float v);

#endif

