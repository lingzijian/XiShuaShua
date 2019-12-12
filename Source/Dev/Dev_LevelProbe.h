#ifndef DEV_LEVELPROBE_H
#define DEV_LEVELPROBE_H

#include "stm32f10x.h"
#include "sys.h"

#define  ELECTROLYSIS_LP	PFin(12)   	//电解液空   	 //空高
#define  ALCOHOL_LP			PEin(2)//PFin(11)	//酒精限位       //排完为高
#define  ACIDEMPTY_LP		PAin(6)		//酸空         //空高
#define  ACIDFULL_LP		PAin(5)		//酸满		//满高
#define	 ALKALIEMPTY_LP		PCin(4)		//碱空		 //空高
#define  ALKALIFULL_LP		PAin(7)		//碱满		//满高
#define  WATHEREMPTY_LP		PFin(13)	//水空		 //空高


void LevelProbeInit(void);
void LevelProbeTest(void);

#endif

