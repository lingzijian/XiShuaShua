#ifndef DEV_PUMP_H
#define DEV_PUMP_H
#include "stm32f10x.h"
#include "sys.h"
#include "Dev_System.h"


#define WATERCYCLEPUMP		PDout(4)	//水循环泵
#define	ACIDPUMP			PDout(14)	//酸水泵 
#define	ALKALIPUMP			PDout(15)	//碱水泵  
#define	DRAINAGEPUMP		PDout(13)	//排水泵 
#define AIRPUMP1			PGout(2)	//空气泵1
#define AIRPUMP2			PGout(3)	//空气泵2
#define ALCOHOLPUMP			PGout(4)	//酒精泵

#define DRV8825EN	PDout(1)	//蠕动泵使能

void PumpInit(void);

void DRV8825Init(void);

void PeristalticPumpSetSpeed(u32 speed,Switch SW);


#endif

