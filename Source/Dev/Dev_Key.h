#ifndef DEV_KEY_H
#define DEV_KEY_H
#include "stm32f10x.h"
#include "sys.h"

#define KEY_OPEN 	PCin(7)  	//����
#define KEY_RUN		PCin(8)		//����
#define KEY_PAUSE	PCin(9)		//��ͣ
#define KEY_STOP 	PAin(8)		//��ͣ

#define INFRAREDSENSOR1   PEin(6)		//���Ǹ�Ӧ
#define INFRAREDSENSOR2   PEin(5)		//������Ӧ

void KeyInit(void);
void InfraredSensorInit(void);

#endif


