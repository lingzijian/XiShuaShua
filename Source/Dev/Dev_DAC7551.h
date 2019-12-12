#ifndef DEV_DAC7551_H
#define DEV_DAC7551_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#define CS1   	PEout(0)
#define CLR1	PEout(1)
#define CS2		PBout(8)
#define	CLR2	PBout(9)
#define CS3		PGout(14)
#define	CLR3	PGout(15)
#define CS4		PGout(12)
#define	CLR4	PGout(13)

#define delay	{__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}

#define DAC7557CS1 {CS1 = 1; CS2 = 1; CS3 = 1; CS4 = 1; delay; CS1 = 0;}
#define DAC7557CS2 {CS1 = 1; CS2 = 1; CS3 = 1; CS4 = 1; delay; CS2 = 0;}
#define DAC7557CS3 {CS1 = 1; CS2 = 1; CS3 = 1; CS4 = 1; delay; CS3 = 0;}
#define DAC7557CS4 {CS1 = 1; CS2 = 1; CS3 = 1; CS4 = 1; delay; CS4 = 0;}

void DAC7551Init(void);
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);
void DAC7551Out(u8 CS, float vol);

#endif

