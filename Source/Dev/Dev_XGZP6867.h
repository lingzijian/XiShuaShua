#ifndef DEV_XGZP6867_H
#define DEV_XGZP6867_H
#include "stm32f10x.h"
#include "Dev_IIC.h"

void XGZP6867Init(void);

void XGZPdata(void)	;
void XGZP_ReadData(float* temperature, float *AirPressure);


#endif

