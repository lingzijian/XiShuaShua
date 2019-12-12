#ifndef DEV_VALVE_H
#define DEV_VALVE_H
#include "stm32f10x.h"

#define WATERVALVE1		PDout(5)	//供水电磁阀1
#define WATERVALVE2		PGout(10)	//供水电磁阀2             
#define ELECTVALVE		PGout(12)  //PDout(7)	//电解槽供水(酸碱)电磁阀  
#define DRAINAGEVALVE	PDout(6)	//排水电磁阀
#define WATERCYCLEVALVE	PGout(9)	//循环电磁阀

void RadiotubeInit(void);
void PID_ValveInit(void);

#endif

