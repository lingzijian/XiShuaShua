#include "App_SystemExecute.h"
#include "Dev_UART.h"
#include "Dev_Key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Dev_Valve.h"
#include <arm_math.h>
#include "Dev_System.h"
#include "Dev_LevelProbe.h"
#include "Dev_Pump.h"
#include "Dev_ElectriGate.h"
#include "Dev_ADC.h"
#include "App_SysTask.h"
#include "Configure.h"
#include "Dev_XGZP6867.h"
#include <string.h>
#include "Dev_M24Cxx.h"
#include "App_Communicate.h"
#include "delay.h"
#include "Dev_TLV5618.h"



extern u32 flowCount1, flowCount2, flowCount3, flowCount4, flowCount5;

static float /*Volume_AcidFlow = 0, Volume_Alkali = 0,*/ Volume_Water1 = 0, Volume_Water2 = 0; // 体积

stTimer* timer1 = NULL, *timer2 = NULL;
 
u8	runStatus = 0;
extern u8 init;

static stTimer* bt = NULL;  // 控制蜂鸣器

//系统参数初始化
void ArgumentInit(void)
{
	char check[6] = "check";
	char buff[6];
	E2ReadBuff((u8*)buff, E2Address_Check, 6);
	if(strcmp(buff, check) == 0)
	{
		E2ReadBuff((u8*)&sys.SaveArg, E2Address_Arg, sizeof(sys.SaveArg));
	}
	else
	{
		sys.SaveArg.baudrate 				= 9600;
		sys.SaveArg.TotalRunningTime		= 1;
		sys.SaveArg.ElectrolyzerUsageTime 	= 2;
		sys.SaveArg.washingNumber			= 3;
		sys.SaveArg.washingTime				= 4;
		sys.SaveArg.devAddr  				= 6;
		sys.SaveArg.SetAcidFlow				= 10;
		sys.SaveArg.SetAlkaliFlow 			= 10;
		sys.SaveArg.SetElectrolysisFlow 	= 10;
		sys.SaveArg.SetWaterFlow1			= 10;
		sys.SaveArg.SetWaterFlow2			= 10;
				
		E2WriteBuff((u8*)check, E2Address_Check, 6);	
		delay_xms(50);
		E2WriteBuff((u8*)&sys.SaveArg, E2Address_Arg, sizeof(sys.SaveArg));	
		delay_ms(100);
		E2ReadBuff((u8*)&sys.SaveArg, E2Address_Arg, sizeof(sys.SaveArg));
	}
	
	runStatus = STATUS_NULL;
	/*
	sys.FaultCode = FAULT_AirLeakage | FAULT_CloseLid;
	sys.Status    = STATUS_PumpAir   | STATUS_PumpWater;
	*/
	bt = CreatTimer();
	sys.ElectrolysisCount = 0;
}

void ArgumentTest(void)
{
	sys.SaveArg.baudrate 				= 10;
	sys.SaveArg.TotalRunningTime		= 10;
	sys.SaveArg.ElectrolyzerUsageTime 	= 10;
	sys.SaveArg.washingNumber			= 10;
	sys.SaveArg.washingTime				= 10;
	sys.SaveArg.devAddr  				= 10;
	sys.SaveArg.SetAcidFlow				= 10;
	sys.SaveArg.SetAlkaliFlow 			= 10;
	sys.SaveArg.SetElectrolysisFlow 	= 10;
	sys.SaveArg.SetWaterFlow1			= 10;
	sys.SaveArg.SetWaterFlow2			= 10;

	sys.CPUTempereture                  = 10;   		//CPU温度
	sys.waterTempereture				= 10;		    //水温
	sys.AcidPH							= 10;		    //酸PH值
	sys.AlkaliPH						= 10;		    //碱PH值
	sys.PPM								= 10; 			//PPM浓度
	sys.ORP								= 10;			//氧化还原电位
	sys.TDS								= 10;			//总溶解固体
	sys.ElectrolysisFlow				= 10; 			//电解液流量 mL/Min
	sys.AcidFlow						= 10;			//酸流量 L/Min
	sys.AlkaliFlow						= 10;			//碱水流量 L/Min
	sys.WaterFlow1						= 10;			//水流量 L/Min
	sys.WaterFlow2						= 10;			//水流量 L/Min
	sys.AirTemperature					= 10;			//气温
	sys.AirPressure						= 10;			//气压
	sys.PipePressure					= 10;			//管道压力 //侧漏压力
	//bt = CreatTimer();
}


u32 KeyCheck(void)
{
	if((KEY_OPEN == 0) || (KEY_RUN == 0) || (KEY_PAUSE == 0) || (KEY_STOP == 0)) 
	{
		vTaskDelay(10);
		if(KEY_OPEN == 0)
			return OPEN;
		else if(KEY_RUN == 0)
			return RUN;
		else if(KEY_PAUSE == 0)
			return PAUSE;
		else if(KEY_STOP == 0)
			return STOP;
	}
	return 0;
}

 

u32 LevelProbeCheck(void)
{
	static u32 event = 0;
	u32 t = event;
	
	if((ELECTROLYSIS_LP == 1) && ((event & ELECTROLYSISEMPTY) == 0)) 
		event |= ELECTROLYSISEMPTY;
	else if((ELECTROLYSIS_LP == 0) && ((event & ELECTROLYSISEMPTY) != 0))
		event &= ~ELECTROLYSISEMPTY;
	if((ALCOHOL_LP == 1) && ((event & ALCOHOLENPTY) == 0)) 
		event |= ALCOHOLENPTY;
	else if((ALCOHOL_LP == 0) && ((event & ALCOHOLENPTY) != 0))
		event &= ~ALCOHOLENPTY;
	if((ACIDEMPTY_LP == 1) && ((event & ACIDEMPTY) == 0)) 
		event |= ACIDEMPTY;
	else if((ACIDEMPTY_LP == 0) && ((event & ACIDEMPTY) != 0))
		event &= ~ACIDEMPTY;
	if((ACIDFULL_LP == 1) && ((event & ACIDFULL) == 0)) 
		event |=	ACIDFULL;
	else if((ACIDFULL_LP == 0) && ((event & ACIDFULL) != 0))
		event &= ~ACIDFULL;
	if((ALKALIEMPTY_LP == 1) && ((event & ALKALIEMPTY) == 0)) 
		event |= ALKALIEMPTY;
	else if((ALKALIEMPTY_LP == 0) && ((event & ALKALIEMPTY) != 0))
		event &= ~ALKALIEMPTY;
	if((ALKALIFULL_LP == 1) && ((event & ALKALIFULL) == 0)) 
		event |= ALKALIFULL;
	else if((ALKALIFULL_LP == 0) && ((event & ALKALIFULL) != 0))
		event &= ~ALKALIFULL;
	if((WATHEREMPTY_LP == 1) && ((event & WATHEREMPTY) == 0)) 
		event |= WATHEREMPTY;
	else if((WATHEREMPTY_LP == 0) && ((event & WATHEREMPTY) != 0))
		event &= ~WATHEREMPTY;


	if(ACIDFULL_LP == 1)
	{
		PeristalticPumpSetSpeed(100, OFF);
		ELECTVALVE 	  = OFF;
		ELECTROLYSIS   = OFF;
		sys.Status &= ~STATUS_Electrolysis;
		sys.Status &= ~STATUS_PumpElectrolysis;
		sys.FaultCode |= FAULT_AcidFull;
	}
	else
	{
		sys.FaultCode &= ~FAULT_AcidFull;
	}
	
	if(ALKALIFULL_LP == 1)
	{
		PeristalticPumpSetSpeed(100, OFF);   
		ELECTROLYSIS    = OFF;
		ELECTVALVE 	  = OFF;
		sys.Status &= ~STATUS_Electrolysis;
		sys.Status &= ~STATUS_PumpElectrolysis;
		sys.FaultCode |= FAULT_AlkaliFull;
	}
	else
	{
		sys.FaultCode &= ~FAULT_AlkaliFull;
	}

	
	if(t != event) //状态更新
	{
		t = event | (1 << 30); //如果上一个状态撤销了，也进行更新
	}
	else
	{
		t = 0;
	}
	
	return t;
}


//红外检测
u32 InfraredChek(void)
{
	u32 event = 0;
	if(INFRAREDSENSOR1 == 0)
		event |= DISTANCEINDUCTION1;
	if(INFRAREDSENSOR2 == 0)
		event |= DISTANCEINDUCTION2;

	return event;
}

u32 GateLimitSwitch(void)
{
	static u32 event = 0;
	u32 t = event;
	if((LS1 == 0) && (event & GATELS1 == 0))
		event |= GATELS1;
	else if((LS1 == 1) && (event & GATELS1 != 0))
		event &= ~GATELS1;
	if((LS2 == 0) && (event & GATELS2 == 0))
		event |=  GATELS2;
	else if((LS2 == 1) && (event & GATELS2 != 0))
		event &= ~GATELS2;

	if(t != event)
		t = event;
	else
		t = 0;
	
	return t;
}

void EventChenck(void)
{
	u32 event = 0;
	event	 |= KeyCheck();
	event	 |= LevelProbeCheck();
	event	 |= InfraredChek();
	event	 |= GateLimitSwitch();
	/*
	if(event != 0)
	{
		KeyHandle(event);
		ExternEventHandle(event);
	}
	*/
	if(event != 0)
	{
		xTaskNotify((TaskHandle_t	)EventHandleTask_Handler,		//接收任务通知的任务句柄
					(u32			)event,						//任务通知值
					(eNotifyAction	)eSetValueWithoutOverwrite);	//不覆盖的方式发送任务通知	
	}
}
static u8 PauseStatus;

static u16 PauseSW = 0;

void KeyHandle(u32 event)
{
	switch(event & 0x0F)
	{
		case RUN:
			if(runStatus == 16)
			{
				if(init == 1)
					ResumeTimer(timer1);
				else
					ResumeTimer(timer2);
				
				if(PauseSW & (1 << 0))
					WATERCYCLEPUMP = ON;
				if(PauseSW & (1 << 1))
					ACIDPUMP = ON;
				if(PauseSW & (1 << 2))
					ALKALIPUMP = ON;
				if(PauseSW & (1 << 3))
					DRAINAGEPUMP = ON;
				if(PauseSW & (1 << 4))
					AIRPUMP1 = ON;
				if(PauseSW & (1 << 5))
					AIRPUMP2 = ON;
				if(PauseSW & (1 << 6))
					ALCOHOLPUMP = ON;
				if(PauseSW & (1 << 7))
					WATERVALVE1 = ON;
				if(PauseSW & (1 << 8))
					WATERVALVE2 = ON;
				if(PauseSW & (1 << 9))
					ELECTVALVE = ON;
				if(PauseSW & (1 << 10))
					WATERCYCLEVALVE = ON;
				if(PauseSW & (1 << 11))
					DRAINAGEVALVE = ON;
				
				PauseSW = 0;
				runStatus = PauseStatus;
			}
			else if(runStatus == 0)
			{
				runStatus = 1;
			}
			break;
		case OPEN:
			if(!((runStatus > 0) && (runStatus < 16)))
			{
				if(LS1 != 0)
				{ELECTRIGATE_OPEN;}
				else
				{ELECTRIGATE_CLOSE;}
			}
			break;
		case STOP:
				runStatus = 20;
			break;
		case PAUSE:
			if((runStatus > 0) && (runStatus < 16))
			{	
				if(init == 1)
					PauseTimer(timer1);
				else
					PauseTimer(timer2);
				
				PauseStatus = runStatus;
				runStatus = 16;
				
				if(WATERCYCLEPUMP)
					PauseSW |= 1 << 0;
				if(ACIDPUMP)
					PauseSW |= 1 << 1;
				if(ALKALIPUMP)
					PauseSW |= 1 << 2;
				if(DRAINAGEPUMP)
					PauseSW |= 1 << 3;
				if(AIRPUMP1)
					PauseSW |= 1 << 4;
				if(AIRPUMP2)
					PauseSW |= 1 << 5;
				if(ALCOHOLPUMP)
					PauseSW |= 1 << 6;
				if(WATERVALVE1)
					PauseSW |= 1 << 7;
				if(WATERVALVE2)
					PauseSW |= 1 << 8;
				if(ELECTVALVE)
					PauseSW |= 1 << 9;
				if(WATERCYCLEVALVE)
					PauseSW |= 1 << 10;
				if(DRAINAGEVALVE)
					PauseSW |= 1 << 11;

				WATERCYCLEPUMP			= OFF;
				ACIDPUMP			    = OFF;
				ALKALIPUMP			  	= OFF;
				DRAINAGEPUMP		  	= OFF;
				AIRPUMP1			    = OFF;
				AIRPUMP2			    = OFF;
				ALCOHOLPUMP			  	= OFF;

				WATERVALVE1			= OFF;
				WATERVALVE2			= OFF;
				ELECTVALVE			= OFF;
				WATERCYCLEVALVE		= OFF;
				DRAINAGEVALVE		= OFF;
			}
			break;
		default:
		return;
	}
	//BEEP = ON;
	SetTimer_HMS(bt, 1);
}


void ExternEventHandle(u32 event)
{
	if(event & ELECTROLYSISEMPTY)
	{
		PeristalticPumpSetSpeed(100, OFF);
		ELECTVALVE 	   = OFF;
		ELECTROLYSIS   = OFF;
		sys.Status &= ~STATUS_Electrolysis;
		sys.Status &= ~STATUS_PumpElectrolysis;
		sys.FaultCode |= FAULT_ElectrolysisEnpty;
	}
	else
	{
		sys.FaultCode &= ~FAULT_ElectrolysisEnpty;
	}
	if(event & ACIDEMPTY)
	{
		sys.FaultCode |= FAULT_AcidEmpty;
	}
	else
	{
		sys.FaultCode &= ~FAULT_AcidEmpty;
	}
	if(event & ACIDFULL)
	{
		PeristalticPumpSetSpeed(100, OFF);
		ELECTVALVE 	  = OFF;
		ELECTROLYSIS   = OFF;
		sys.Status &= ~STATUS_Electrolysis;
		sys.Status &= ~STATUS_PumpElectrolysis;
		sys.FaultCode |= FAULT_AcidFull;
	}
	else
	{
		sys.FaultCode &= ~FAULT_AcidFull;
	}
	if(event & ALKALIEMPTY)
	{
		sys.FaultCode |= FAULT_AlkaliEmpty;
	}
	else
	{
		sys.FaultCode &= ~FAULT_AlkaliEmpty;
	}
	if(event & ALKALIFULL)
	{
		PeristalticPumpSetSpeed(100, OFF);   
		ELECTROLYSIS    = OFF;
		ELECTVALVE 	  = OFF;
		sys.Status &= ~STATUS_Electrolysis;
		sys.Status &= ~STATUS_PumpElectrolysis;
		sys.FaultCode |= FAULT_AlkaliFull;
	}
	else
	{
		sys.FaultCode &= ~FAULT_AlkaliFull;
	}
	if(event & WATHEREMPTY)
	{
		sys.FaultCode |= FAULT_WatherEmpty;
	}
	else
	{
		sys.FaultCode &= ~FAULT_WatherEmpty;
	}
	if(event & ALCOHOLENPTY)
	{
		sys.FaultCode |= FAULT_AlcoholEmpty;
	}
	else
	{
		sys.FaultCode &= ~FAULT_AlcoholEmpty;
	}
	
	if((event & GATELS1) || (event & GATELS2))
	{
		ELECTRIGATE_STOP;  //电动门关闭
		if(event & GATELS1)
		{ELECTRIGATE_OPEN;}
		else
		{ELECTRIGATE_CLOSE;}
	}
	
	if(event & DISTANCEINDUCTION1)
	{
		if(!((runStatus > 0) && (runStatus < 16)))
		{
			if(LS1 != 0)
			{ELECTRIGATE_OPEN;}
			else
			{ELECTRIGATE_CLOSE;}
		}
	}
	if(event & DISTANCEINDUCTION2)
	{
		if(runStatus == 16)
		{
			if(init == 1)
				ResumeTimer(timer1);
			else
				ResumeTimer(timer2);
				
				if(PauseSW & (1 << 0))
					WATERCYCLEPUMP = ON;
				if(PauseSW & (1 << 1))
					ACIDPUMP = ON;
				if(PauseSW & (1 << 2))
					ALKALIPUMP = ON;
				if(PauseSW & (1 << 3))
					DRAINAGEPUMP = ON;
				if(PauseSW & (1 << 4))
					AIRPUMP1 = ON;
				if(PauseSW & (1 << 5))
					AIRPUMP2 = ON;
				if(PauseSW & (1 << 6))
					ALCOHOLPUMP = ON;
				if(PauseSW & (1 << 7))
					WATERVALVE1 = ON;
				if(PauseSW & (1 << 8))
					WATERVALVE2 = ON;
				if(PauseSW & (1 << 9))
					ELECTVALVE = ON;
				if(PauseSW & (1 << 10))
					WATERCYCLEVALVE = ON;
				if(PauseSW & (1 << 11))
					DRAINAGEVALVE = ON;
				
				PauseSW = 0;
				runStatus = PauseStatus;
		}else if((runStatus > 0) && (runStatus < 16))
		{
			if(init == 1)
				PauseTimer(timer1);
			else
				PauseTimer(timer2);
			
			PauseStatus = runStatus;
			runStatus = 16;
			
			if(WATERCYCLEPUMP)
				PauseSW |= 1 << 0;
			if(ACIDPUMP)
				PauseSW |= 1 << 1;
			if(ALKALIPUMP)
				PauseSW |= 1 << 2;
			if(DRAINAGEPUMP)
				PauseSW |= 1 << 3;
			if(AIRPUMP1)
				PauseSW |= 1 << 4;
			if(AIRPUMP2)
				PauseSW |= 1 << 5;
			if(ALCOHOLPUMP)
				PauseSW |= 1 << 6;
			if(WATERVALVE1)
				PauseSW |= 1 << 7;
			if(WATERVALVE2)
				PauseSW |= 1 << 8;
			if(ELECTVALVE)
				PauseSW |= 1 << 9;
			if(WATERCYCLEVALVE)
				PauseSW |= 1 << 10;
			if(DRAINAGEVALVE)
				PauseSW |= 1 << 11;

			WATERCYCLEPUMP			= OFF;
			ACIDPUMP			    = OFF;
			ALKALIPUMP			  	= OFF;
			DRAINAGEPUMP		  	= OFF;
			AIRPUMP1			    = OFF;
			AIRPUMP2			    = OFF;
			ALCOHOLPUMP			  	= OFF;

			WATERVALVE1			= OFF;
			WATERVALVE2			= OFF;
			ELECTVALVE			= OFF;
			WATERCYCLEVALVE		= OFF;
			DRAINAGEVALVE		= OFF;
		}
		else if(runStatus == 0)
		{
			runStatus = 1;
		}
	}
}

void BuzzerControl(void)
{
	if(sys.FaultCode & (FAULT_AirLeakage | FAULT_AlcoholEmpty))  //错误报警
	{
		if(GetTimerStatus(bt) != Timing)
		{
			SetTimer_HMS(bt, 5);
		}
		else if(GetTimerStatus(bt) == TimeOut)
		{
			BEEP = ~BEEP;
		}
	}
	else if((BEEP) && (GetTimerStatus(bt) == TimeOut))
	{
		BEEP = OFF;
	}
}

// 0.0467    0.0472 / 0.0479   -0.0063  / 0.0476
void GetFlow(void)
{
	sys.ElectrolysisFlow = (0.0476f * flowCount5) * 1000.0f; //ml
	sys.AcidFlow   = 0.0476f * flowCount4; 
	sys.AlkaliFlow = 0.0476f * flowCount3; 
	sys.WaterFlow1 = 0.0476f * flowCount1; 
	sys.WaterFlow2 = 0.0476f * flowCount2; 
	
	flowCount1 = flowCount2 = flowCount3 = flowCount4 = flowCount5 = 0;
	
	if(sys.Status & STATUS_PumpWater)
	{
		Volume_Water1 += sys.WaterFlow1 / 60;
		Volume_Water2 += sys.WaterFlow2 / 60;
	}
	/*
	if(sys.Status & STATUS_PumpAlkali)
	{
		Volume_Alkali = sys.AlkaliFlow / 60;
	}
	if(sys.Status & STATUS_PumpAcid)
	{
		Volume_AcidFlow = sys.AcidFlow / 60;
	}
	*/
}



void DataUpdate(void)
{
	#define ORP_Offset 0
	
	ADC_SoftwareStartConvCmd(ADC3, DISABLE);
	sys.PPM				= 0;
	sys.ORP 			= 2000 - (AD3_Average(ChannelORP) * VREF * 1000 / 4096) + ORP_Offset;
	
    sys.AcidPH 			= -0.0065 * (AD3_Average(ChannelPH1)*VREF*1000/4096/0.417) + 23.9246;
	sys.AlkaliPH 		= -0.0065 * (AD3_Average(ChannelPH2)*VREF*1000/4096/0.417) + 23.9246;
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	sys.CPUTempereture 	= (1.43 - AD1_Average(ChannelCPU_temperature)*VREF/4096) / 0.0043 + 25;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	XGZP_ReadData(&sys.AirTemperature, &sys.AirPressure);
	
	ReadPipePressure();	
	ReadTDS();
}


void WashingInit(void)
{
	u8 i = 0;
	switch(runStatus)
	{
		case 1:
			while((timer2 == NULL) && (i++ < 10)) // 定时器创建失败就不执行往下流程
			{
				timer2 = CreatTimer();
				vTaskDelay(100);
			}
			if(timer2 != NULL)
				runStatus = 2;
			else
				runStatus = 20;	
			break;
		case 2:
			if(WATHEREMPTY_LP == 0) //如果有水，则先排空 
			{
				DRAINAGEVALVE  = ON;
				DRAINAGEPUMP   = ON;
				AIRPUMP1	   = ON;
				sys.Status |= STATUS_Drainage;
				sys.Status |= STATUS_PumpAir;
				
				SetTimer(timer2, 10);
				runStatus = 3;
			}
			else
			{
				runStatus = 4;
			}
			break;
		case 3://抽水
			/*
			if(GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_13) == SET)
			{
				
			}
			*/
			if(GetTimerStatus(timer2) == TimeOut)  //排水超时
			{
				DRAINAGEVALVE  = OFF;
				DRAINAGEPUMP   = OFF;
				AIRPUMP1	   = OFF;
				sys.Status &= ~STATUS_Drainage;
				sys.Status &= ~STATUS_PumpAir;
				runStatus = 4;
			}
			break;
		case 4:
			Volume_Water1 = 0;
			Volume_Water2 = 0;
			if(sys.SaveArg.SetWaterFlow1 > 0)
				WATERVALVE1 = ON;
			if(sys.SaveArg.SetWaterFlow1 > 0)
				WATERVALVE2 = ON;
			
			SetTimer(timer2, 10);
			
			runStatus = 5;
			sys.Status |= STATUS_PumpWater;			
		case 5:
			//if(Volume_Water1 + Volume_Water2 > 1)
			if(GetTimerStatus(timer2) == TimeOut)
			{
				WATERVALVE1 = OFF;
				WATERVALVE2 = OFF;
				
				DRAINAGEVALVE  = ON;
				DRAINAGEPUMP   = ON;
				AIRPUMP1	   = ON;

				sys.Status &= ~STATUS_PumpWater;
				sys.Status |= STATUS_Drainage;
				sys.Status |= STATUS_PumpAir;
				SetTimer(timer2, 10);
				runStatus = 6;
			}
			/*
			else if(GetTimerStatus(timer2) == TimeOut)
			{
				runStatus = 20;
			}
			*/
			break;
		case 6:
			if(GetTimerStatus(timer2) == TimeOut)
			{
				DRAINAGEVALVE  = OFF;
				DRAINAGEPUMP   = OFF;
				AIRPUMP1	   = OFF;
				sys.Status &= ~STATUS_Drainage;
				sys.Status &= ~STATUS_PumpAir;
				runStatus = 0;
				DeleteTimer(timer2);
				init = 1;
			}
			/*
			else if(GetTimerStatus(timer2) == TimeOut)
			{
				runStatus = 20;
			}
			*/
			break;
		case 20:
			WATERCYCLEPUMP		= OFF;
			ACIDPUMP			= OFF;
			ALKALIPUMP			= OFF;
			DRAINAGEPUMP		= OFF;
			AIRPUMP1			= OFF;
			AIRPUMP2			= OFF;
			ALCOHOLPUMP			= OFF;

			WATERVALVE1			= OFF;
			WATERVALVE2			= OFF;
			ELECTVALVE			= OFF;
			WATERCYCLEVALVE		= OFF;
			DRAINAGEVALVE		= OFF;
			sys.Status			= 0;
			DeleteTimer(timer2);
			init = 1;
			break;
		default:
			break;
	}
}

void SysExcute(void)
{
	u8 i = 0;
	static u8 flag = 0; //第二次碱洗置1
	switch(runStatus)
	{
		case 1://
			if(sys.Status & STATUS_Electrolysis)
			{
				runStatus = 20;
			}
			else
			{
				while((timer1 == NULL) && (i++ < 10)) // 定时器创建失败就不执行往下流程
				{
					timer1 = CreatTimer();
					vTaskDelay(100);
				}
				if(timer1 != NULL)
					runStatus = 2;
				else
					runStatus = 20;
				/*
				if(LS2 != 0)
				{
					ELECTRIGATE_CLOSE; //关盖
				}
				*/
				sys.FaultCode &= ~FAULT_AirLeakage;
			}
			break;
		case 2:  //测漏
			//if(LS2 == 0) //门已关闭
			if(1)
			{
				AIRPUMP2 = ON;
				SetTimer(timer1, 5);
				sys.Status |= STATUS_CheckAirLeakage;
				runStatus = 3;
			}
			break;
		case 3: //抽8L水
			if(/*(sys.AirPressure > 22000)*/(GetTimerStatus(timer1) == TimeOut) || (flag == 1))
			{
				AIRPUMP2 = OFF;
				if(sys.SaveArg.SetWaterFlow1 > 0)
					WATERVALVE1 = ON;
				if(sys.SaveArg.SetWaterFlow1 > 0)
					WATERVALVE2 = ON;
				Volume_Water1 = 0;
				Volume_Water2 = 0;
				
				SetTimer(timer1, 10);
				
				sys.Status |= STATUS_PumpWater;
				sys.Status &= ~STATUS_CheckAirLeakage;
				runStatus = 4;
			}
			/*
			else if(GetTimerStatus(timer1) == TimeOut)
			{
				sys.FaultCode |= FAULT_AirLeakage;
				runStatus = 20;
			}
			*/
			break;
		case 4://抽2L碱水
			//if(Volume_Water1 + Volume_Water2 >= 1)
			if(GetTimerStatus(timer1) == TimeOut)
			{
				WATERVALVE1   = OFF;
				WATERVALVE2   = OFF;
				ALKALIPUMP	  = ON;
				SetTimer(timer1, 10);
				sys.Status &= ~STATUS_PumpWater;
				sys.Status |= STATUS_PumpAlkali;
				runStatus = 5;
			}
			/*
			else if(GetTimerStatus(timer1) == TimeOut)
			{
				runStatus = 20;
			}
			*/
			break;
		case 5://碱洗
			if(GetTimerStatus(timer1) == TimeOut)
			{
				ALKALIPUMP	= OFF;
				WATERCYCLEVALVE  = ON;
				WATERCYCLEPUMP = ON;
				DRAINAGEPUMP	= ON;
				SetTimer(timer1, 10);
				sys.Status &= ~STATUS_PumpAlkali;
				sys.Status |= STATUS_Sprayvalve;
				runStatus = 6;
			}
			case 6://排水
				if(GetTimerStatus(timer1) == TimeOut)
				{
					WATERCYCLEPUMP = OFF;
					WATERCYCLEVALVE  = OFF;
					DRAINAGEVALVE  = ON;
					DRAINAGEPUMP   = ON;
					AIRPUMP1	   = ON;
					SetTimer(timer1, 10);
					sys.Status &= ~STATUS_Sprayvalve;
					sys.Status |= STATUS_Drainage;
					sys.Status |= STATUS_PumpAir;
					runStatus = 7;
				}
				break;
			case 7:
				if(GetTimerStatus(timer1) == TimeOut)
				{
					DRAINAGEVALVE  = OFF;
					DRAINAGEPUMP   = OFF;
					AIRPUMP1	   = OFF;
					sys.Status &= ~STATUS_Drainage;
					sys.Status &= ~STATUS_PumpAir;
					if(flag == 0)
						runStatus = 8;
					else
						runStatus = 13;
				}
				/*
				else if(GetTimerStatus(timer1) == TimeOut)
				{
					runStatus = 20;
				}
				*/
				break;
			case 8://抽水6L
				if(sys.SaveArg.SetWaterFlow1 > 0)
					WATERVALVE1 = ON;
				if(sys.SaveArg.SetWaterFlow1 > 0)
					WATERVALVE2 = ON;
				Volume_Water1 = 0;
				Volume_Water2 = 0;
				SetTimer(timer1, 10);
				sys.Status |= STATUS_PumpWater;
				runStatus = 9;
				break;
			case 9://抽4L酸
				//if(Volume_Water1 + Volume_Water2 >= 6)
				if(GetTimerStatus(timer1) == TimeOut)
				{
					WATERVALVE1 = OFF;
					WATERVALVE2 = OFF;
					ACIDPUMP	= ON;
					SetTimer(timer1, 10);
					sys.Status &= ~STATUS_PumpWater;
					sys.Status |= STATUS_PumpAcid;
					runStatus = 10;
				}
				/*
				else if(GetTimerStatus(timer1) == TimeOut)
				{
					runStatus = 20;
				}
				*/
				break;
			case 10://酸洗
				if(GetTimerStatus(timer1) == TimeOut)
				{
					ACIDPUMP	= OFF;
					WATERCYCLEVALVE  = ON;
					WATERCYCLEPUMP  = ON;
					DRAINAGEPUMP	= ON;
					SetTimer(timer1, 10);
					sys.Status &= ~STATUS_PumpAcid;
					sys.Status |= STATUS_Sprayvalve;
					runStatus = 11;
					sys.ElectrolysisCount++;  //进行电解
				}
				break;
			case 11://排水
				if(GetTimerStatus(timer1) == TimeOut)
				{
					WATERCYCLEPUMP = OFF;
					WATERCYCLEVALVE  = OFF;
					DRAINAGEVALVE  = ON;
					DRAINAGEPUMP   = ON;
					AIRPUMP1	   = ON;
					SetTimer(timer1, 10);
					sys.Status &= ~STATUS_Sprayvalve;
					sys.Status |= STATUS_Drainage;
					sys.Status |= STATUS_PumpAir;
					runStatus = 12;
				}
				break;
			case 12:
				if(GetTimerStatus(timer1) == TimeOut)
				{
					DRAINAGEVALVE  = OFF;
					DRAINAGEPUMP   = OFF;
					AIRPUMP1	   = OFF;
					flag      = 1;
					runStatus = 3;
				}
				break;
			case 13://第二次碱洗抽酒精2S
			/*
				if(GetTimerStatus(timer1) == TimeOut)
				{
					if(flag == 0)
					{
						flag = 1;
						DRAINAGEVALVE  = OFF;
						DRAINAGEPUMP   = OFF;
						AIRPUMP1	   = OFF;
						sys.Status &= ~STATUS_Drainage;
						sys.Status &= ~STATUS_PumpAir;
						runStatus = 3;
					}
					else
					{
						ALCOHOLPUMP    = ON;
						SetTimer(timer1, 2);
						sys.Status |= STATUS_PumpAlcohol;
						runStatus = 14;
					}
				}
				else if(GetTimerStatus(timer1) == TimeOut)
				{
					runStatus = 20;
				}
				*/
				ALCOHOLPUMP    = ON;
				SetTimer(timer1, 3);
				sys.Status |= STATUS_PumpAlcohol;
				runStatus = 14;
				break;
			case 14://抽气2S
				if(GetTimerStatus(timer1) == TimeOut)
				{
					ALCOHOLPUMP    = OFF;
					AIRPUMP1	   = ON;
					SetTimer(timer1, 3);
					sys.Status &= ~STATUS_PumpAlcohol;
					sys.Status |= STATUS_PumpAir;
					runStatus = 15;
				}
				break;
			case 15://洗消结束
				if(GetTimerStatus(timer1) == TimeOut)
				{
					AIRPUMP1	   = OFF;
					sys.Status &= ~STATUS_PumpAir;
					flag		   = 0;
					runStatus 	   = 0;
					//leteTimer(timer1);
					sys.SaveArg.washingNumber++;
					SaveArguments((u8*)&sys.SaveArg.washingNumber, 4);
				}
				break;
			case 20://错误/停止
				//关闭所有外部电路
				WATERCYCLEPUMP			= OFF;
				ACIDPUMP			    = OFF;
				ALKALIPUMP			  	= OFF;
				DRAINAGEPUMP		  	= OFF;
				AIRPUMP1			    = OFF;
				AIRPUMP2			    = OFF;
				ALCOHOLPUMP			  	= OFF;

				WATERVALVE1			= OFF;
				WATERVALVE2			= OFF;
				ELECTVALVE			= OFF;
				WATERCYCLEVALVE		= OFF;
				DRAINAGEVALVE		= OFF;
				flag				= 0;
				sys.Status			= 0;
				//leteTimer(timer1);
				runStatus 			= 0;   
				break;
		default:
			break;
	}
}



void LED_Linght(void)
{
	if((runStatus > 0) && (runStatus < 16))
		LED_RUN = 1;
	else
		LED_RUN = 0;
	if(runStatus == 16)
		LED_PAUSE = 1;
	else
		LED_PAUSE = 0;
	if(runStatus == 0)
		LED_STOP = 1;
	else
		LED_STOP = 0;
	if(LS2 != 0)
		LED_OPEN = 1;
	else
		LED_OPEN = 0;
}

void ElectrolysisControl(void)
{
	static int flag = -1;
	static u16 i = 0;
	if(flag != sys.ElectrolysisCount)
	{
		flag = sys.ElectrolysisCount;
		PeristalticPumpSetSpeed(100, ON);
		sys.Status |= STATUS_PumpElectrolysis;
		i = 0;
	}
	if(sys.Status & STATUS_PumpElectrolysis)
	{
		if(i < 200)
		{
				i++;
		}
		else if(i == 200)
		{
				ELECTVALVE = ON;
				ELECTROLYSIS   = ON;
				sys.Status |= STATUS_Electrolysis;
		}
	}
}


arm_pid_instance_f32 pid[5];
void ValveController(void)
{
	/*
	float out;
	if(sys.Status & STATUS_PumpWater)
	{
		out = arm_pid_f32(&pid[0], sys.WaterFlow1 - sys.SaveArg.SetWaterFlow1);
		TLV5618Out(channel1, fast, out);
		TLV5618Out(channel2, fast, out);
	}
	if(sys.Status & STATUS_PumpElectrolysis)
	{
		out = arm_pid_f32(&pid[2], sys.AlkaliFlow - sys.SaveArg.SetAlkaliFlow);
		TLV5618Out(channel3, fast, out);
		TLV5618Out(channel4, fast, out);
	}
 	if(sys.Status & STATUS_PumpElectrolysis)
 	{
 		out = arm_pid_f32(&pid[4], sys.ElectrolysisFlow - sys.SaveArg.SetElectrolysisFlow);
 		PeristalticPumpSetSpeed((u32)(out * 10), ON);
 	}
 	*/
}


void TimeLog(void)
{
	/*
	 static u8 flag = 0;
	 static u32 Start;
	 static	u32 min = 0;
	 if((sys.Status & STATUS_Electrolysis) && (flag == 0))
	 {
	 	flag = 1;
		Start = RTC_GetCounter();
	 }
	 else if(!(sys.Status & STATUS_Electrolysis) && (flag == 1))
	 {
	 	flag = 0;
		sys.SaveArg.ElectrolyzerUsageTime += (RTC_GetCounter() - Start) / 60;
		SaveArguments(&sys.SaveArg.ElectrolyzerUsageTime, 4);
	 }
	 if(min != sys.time.min)
	 {
	 	sys.SaveArg.TotalRunningTime++;
		SaveArguments(&sys.SaveArg.TotalRunningTime, 4);
	 }
	 */
}


void SaveArguments(void* Arguments, u16 len)
{
	s16 addr = E2Address_Arg + (int)((u32)Arguments - (u32)&sys.SaveArg);
	if(addr < E2Address_Arg)
		return;
	E2WriteBuff((u8*)Arguments, addr, len);
}

