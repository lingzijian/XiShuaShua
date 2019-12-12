#ifndef APP_SYSTEMEXECUTE_H
#define APP_SYSTEMEXECUTE_H
#include "stm32f10x.h"

/* 事件值 */
#define RUN		(u32)(1 << 0)  	//按键 运行
#define OPEN	(u32)(1 << 1)	//按键 打开
#define	STOP	(u32)(1 << 2)	//按键 停止
#define	PAUSE	(u32)(1 << 3)	//按键 暂停

#define DISTANCEINDUCTION1	(u32)(1 << 4)	//红外距离感应1
#define DISTANCEINDUCTION2	(u32)(1 << 5)	//红外距离感应2


#define		ELECTROLYSISEMPTY	(u32)(1 << 6)	//电解液空
#define		ALCOHOLENPTY		(u32)(1 << 7)	//酒精空
#define		ACIDEMPTY			(u32)(1 << 8)	//酸空
#define		ACIDFULL			(u32)(1 << 9)	//酸满
#define		ALKALIEMPTY			(u32)(1 << 10)	//碱空
#define		ALKALIFULL			(u32)(1 << 11)	//碱满
#define		WATHEREMPTY			(u32)(1 << 12)	//水空

#define		GATELS1		(u32)(1 << 13)	//门限1
#define		GATELS2		(u32)(1 << 14)	//门限2

#define		STATUS_NULL		0

/* 故障代码 */
#define		FAULT_OpenLid				(u16)(1 << 0)//开盖异常
#define 	FAULT_CloseLid				(u16)(1 << 1)//关盖异常
#define		FAULT_AirLeakage			(u16)(1 << 2)//漏气
#define		FAULT_ElectrolysisEnpty		(u16)(1	<< 3)//电解液空
#define		FAULT_AlcoholEmpty			(u16)(1	<< 4)//酒精满
#define		FAULT_AcidEmpty				(u16)(1	<< 5)//酸空
#define		FAULT_AcidFull				(u16)(1	<< 6)//酸满
#define		FAULT_AlkaliEmpty			(u16)(1	<< 7)//碱空
#define		FAULT_AlkaliFull			(u16)(1	<< 8)//碱满
#define		FAULT_WatherEmpty			(u16)(1	<< 9)//水空


/* 设备状态 */
#define		STATUS_Init						(u16)(1 << 0)//自检
#define 	STATUS_PumpElectrolysis			(u16)(1 << 1)//抽电解液
#define		STATUS_PumpAcid					(u16)(1 << 2)//抽酸
#define		STATUS_PumpAlkali				(u16)(1	<< 3)//抽碱
#define		STATUS_PumpWater				(u16)(1	<< 4)//抽水
#define		STATUS_Electrolysis				(u16)(1	<< 5)//电解
#define		STATUS_CheckAirLeakage			(u16)(1	<< 6)//测漏
#define		STATUS_OpenLid					(u16)(1	<< 7)//开盖
#define		STATUS_CloseLid					(u16)(1	<< 8)//关盖
#define		STATUS_Sprayvalve				(u16)(1	<< 9)//喷淋
#define		STATUS_PumpAir					(u16)(1 << 10)//抽气
#define		STATUS_PumpAlcohol				(u16)(1 << 11)//抽酒精
#define		STATUS_Drainage					(u16)(1 << 12)//排空


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

