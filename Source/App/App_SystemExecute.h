#ifndef APP_SYSTEMEXECUTE_H
#define APP_SYSTEMEXECUTE_H
#include "stm32f10x.h"

/* �¼�ֵ */
#define RUN		(u32)(1 << 0)  	//���� ����
#define OPEN	(u32)(1 << 1)	//���� ��
#define	STOP	(u32)(1 << 2)	//���� ֹͣ
#define	PAUSE	(u32)(1 << 3)	//���� ��ͣ

#define DISTANCEINDUCTION1	(u32)(1 << 4)	//��������Ӧ1
#define DISTANCEINDUCTION2	(u32)(1 << 5)	//��������Ӧ2


#define		ELECTROLYSISEMPTY	(u32)(1 << 6)	//���Һ��
#define		ALCOHOLENPTY		(u32)(1 << 7)	//�ƾ���
#define		ACIDEMPTY			(u32)(1 << 8)	//���
#define		ACIDFULL			(u32)(1 << 9)	//����
#define		ALKALIEMPTY			(u32)(1 << 10)	//���
#define		ALKALIFULL			(u32)(1 << 11)	//����
#define		WATHEREMPTY			(u32)(1 << 12)	//ˮ��

#define		GATELS1		(u32)(1 << 13)	//����1
#define		GATELS2		(u32)(1 << 14)	//����2

#define		STATUS_NULL		0

/* ���ϴ��� */
#define		FAULT_OpenLid				(u16)(1 << 0)//�����쳣
#define 	FAULT_CloseLid				(u16)(1 << 1)//�ظ��쳣
#define		FAULT_AirLeakage			(u16)(1 << 2)//©��
#define		FAULT_ElectrolysisEnpty		(u16)(1	<< 3)//���Һ��
#define		FAULT_AlcoholEmpty			(u16)(1	<< 4)//�ƾ���
#define		FAULT_AcidEmpty				(u16)(1	<< 5)//���
#define		FAULT_AcidFull				(u16)(1	<< 6)//����
#define		FAULT_AlkaliEmpty			(u16)(1	<< 7)//���
#define		FAULT_AlkaliFull			(u16)(1	<< 8)//����
#define		FAULT_WatherEmpty			(u16)(1	<< 9)//ˮ��


/* �豸״̬ */
#define		STATUS_Init						(u16)(1 << 0)//�Լ�
#define 	STATUS_PumpElectrolysis			(u16)(1 << 1)//����Һ
#define		STATUS_PumpAcid					(u16)(1 << 2)//����
#define		STATUS_PumpAlkali				(u16)(1	<< 3)//���
#define		STATUS_PumpWater				(u16)(1	<< 4)//��ˮ
#define		STATUS_Electrolysis				(u16)(1	<< 5)//���
#define		STATUS_CheckAirLeakage			(u16)(1	<< 6)//��©
#define		STATUS_OpenLid					(u16)(1	<< 7)//����
#define		STATUS_CloseLid					(u16)(1	<< 8)//�ظ�
#define		STATUS_Sprayvalve				(u16)(1	<< 9)//����
#define		STATUS_PumpAir					(u16)(1 << 10)//����
#define		STATUS_PumpAlcohol				(u16)(1 << 11)//��ƾ�
#define		STATUS_Drainage					(u16)(1 << 12)//�ſ�


void ArgumentInit(void);
void GetFlow(void);
void DataUpdate(void);
void EventChenck(void);
void PumpController(void);
void KeyHandle(u32 key);
void ExternEventHandle(u32 event);
void LED_Linght(void);
void WashingInit(void);
void SysExcute(void);
void ElectrolysisControl(void);
void ValveController(void);
void SaveArguments(void* Arguments, u16 len);
void TimeLog(void);
void BuzzerControl(void);
void ArgumentTest(void);






extern u8	runStatus;


#endif

