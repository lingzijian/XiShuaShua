#ifndef DEV_ELECTRIGATE_H
#define DEV_ELECTRIGATE_H

#include "stm32f10x.h"
#include "sys.h"

#define	A4950IN1			PBout(0)
#define	A4950IN2			PBout(1)

/*
#define	A4950IN3			PAout(0)
#define	A4950IN4			PAout(1)
*/

#define	ELECTRIGATE_OPEN	{A4950IN1 = 1; A4950IN2 = 0;}
#define	ELECTRIGATE_CLOSE	{A4950IN1 = 0; A4950IN2 = 1;}
#define ELECTRIGATE_STOP	{A4950IN1 = 0; A4950IN2 = 0;}

#define LS1		PFin(14)  //限位开关1
#define LS2		PFin(15)  //限位开关2

void ElectriGateInit(void);
#endif

