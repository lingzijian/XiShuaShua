#ifndef DEV_LEVELPROBE_H
#define DEV_LEVELPROBE_H

#include "stm32f10x.h"
#include "sys.h"

#define  ELECTROLYSIS_LP	PFin(12)   	//���Һ��   	 //�ո�
#define  ALCOHOL_LP			PEin(2)//PFin(11)	//�ƾ���λ       //����Ϊ��
#define  ACIDEMPTY_LP		PAin(6)		//���         //�ո�
#define  ACIDFULL_LP		PAin(5)		//����		//����
#define	 ALKALIEMPTY_LP		PCin(4)		//���		 //�ո�
#define  ALKALIFULL_LP		PAin(7)		//����		//����
#define  WATHEREMPTY_LP		PFin(13)	//ˮ��		 //�ո�


void LevelProbeInit(void);
void LevelProbeTest(void);

#endif

