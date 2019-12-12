#ifndef DEV_VALVE_H
#define DEV_VALVE_H
#include "stm32f10x.h"

#define WATERVALVE1		PDout(5)	//��ˮ��ŷ�1
#define WATERVALVE2		PGout(10)	//��ˮ��ŷ�2             
#define ELECTVALVE		PGout(12)  //PDout(7)	//���۹�ˮ(���)��ŷ�  
#define DRAINAGEVALVE	PDout(6)	//��ˮ��ŷ�
#define WATERCYCLEVALVE	PGout(9)	//ѭ����ŷ�

void RadiotubeInit(void);
void PID_ValveInit(void);

#endif

