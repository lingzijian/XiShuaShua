#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


#define LED_PAUSE	PGout(5)
#define LED_STOP	PGout(6)
#define LED_OPEN	PGout(7)
#define LED_RUN 	PGout(8)

#define BEEP		PBout(13)	//������
#define ELECTROLYSIS PBout(12)  //���

void LED_Init(void);//��ʼ��

#endif



