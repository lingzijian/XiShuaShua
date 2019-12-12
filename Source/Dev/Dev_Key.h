#ifndef DEV_KEY_H
#define DEV_KEY_H
#include "stm32f10x.h"
#include "sys.h"

#define KEY_OPEN 	PCin(7)  	//开盖
#define KEY_RUN		PCin(8)		//运行
#define KEY_PAUSE	PCin(9)		//暂停
#define KEY_STOP 	PAin(8)		//急停

#define INFRAREDSENSOR1   PEin(6)		//开盖感应
#define INFRAREDSENSOR2   PEin(5)		//启动感应

void KeyInit(void);
void InfraredSensorInit(void);

#endif


