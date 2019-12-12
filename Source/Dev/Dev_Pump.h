#ifndef DEV_PUMP_H
#define DEV_PUMP_H
#include "stm32f10x.h"
#include "sys.h"
#include "Dev_System.h"


#define WATERCYCLEPUMP		PDout(4)	//ˮѭ����
#define	ACIDPUMP			PDout(14)	//��ˮ�� 
#define	ALKALIPUMP			PDout(15)	//��ˮ��  
#define	DRAINAGEPUMP		PDout(13)	//��ˮ�� 
#define AIRPUMP1			PGout(2)	//������1
#define AIRPUMP2			PGout(3)	//������2
#define ALCOHOLPUMP			PGout(4)	//�ƾ���

#define DRV8825EN	PDout(1)	//�䶯��ʹ��

void PumpInit(void);

void DRV8825Init(void);

void PeristalticPumpSetSpeed(u32 speed,Switch SW);


#endif

