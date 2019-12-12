#include "Dev_System.h"
#include "Dev_ADC.h"
#include "Dev_IWDG.h"
#include "Dev_Key.h"
#include "Dev_LED.h"
#include "Dev_Relay.h"
#include "Dev_RTC.h" 
#include "Dev_Timer.h"
#include "Dev_UART.h"
#include "Dev_Valve.h"
#include "delay.h"
#include "Dev_XGZP6867.h"
#include "Dev_FlowSensor.h"
#include "mb.h"
#include "Dev_ElectriGate.h"
#include "Dev_M24Cxx.h"
#include "Dev_IIC.h"
#include "Configure.h"
#include "App_SystemExecute.h"
#include "Dev_Pump.h"
#include "Dev_Pump.h"
#include <string.h>
#include "Dev_TLV5618.h"
#include "Dev_LevelProbe.h"



void GetVersion(char *version)
{
	u8 i = 0;
	const char *pMonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	const char *pMonth2[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
	
	while(i < 12)
	{
		if(strstr(__DATE__, pMonth[i]) != NULL)
			break;
		i++;
	}
	
	strcat(version, PCB);
	strcat(version, &__DATE__[7]);
	strcat(version, pMonth2[i]);
	strncat(version, &__DATE__[4], 2);
	strncat(version, &__TIME__[0], 2);
	strncat(version, &__TIME__[3], 2);
	strcat(version, AUTHOR);
}

stSysParameter sys; //系统参数
u32	timer;

void Dev_SystemInit(void)
{
	LED_Init();	
	ElectriGateInit();
	ADC1_Init();
	ADC3_Init();
	TLV5618Init();
	FlowSensorInit();
	RadiotubeInit();
	PumpInit();
	XGZP6867Init();
	DRV8825Init();
	M24CxxInit();
	LevelProbeInit();
	KeyInit();
	InfraredSensorInit();
	UARTConfig(COM2, 9600);
	UARTConfig(COM3, 115200);
	UARTConfig(COM4, 9600);
	
	IWDG_Init(IWDG_Prescaler_256, 20000);
	#if (USINGFREEMODBUS  == 1)
	eMBInit(MB_RTU, 0x01, 0x01, 57600, MB_PAR_NONE);//初始化 RTU模式 从机地址为1  9600 无校验
	eMBEnable();    //启动FreeModbus
	#endif
	GetVersion(sys.Version);
	SysTimerInit();

	//ArgumentTest();
	ArgumentInit();
	
}


const stMBRegHoldingNode MBRegHoldingNode[HOLDING_REG_LEN] =
{
	{
		0x0000,
		(char*)&sys.SaveArg.baudrate,
		(char*)&sys.SaveArg.baudrate,
		1,
		Long,
		FALSE
	},
	{
		0x0001,
		(char*)&sys.SaveArg.baudrate,
		(char*)&sys.SaveArg.baudrate + 2,
		1,
		Long,
		TRUE
	},
	{
		0x0002,
		(char*)&sys.SaveArg.devAddr,
		(char*)&sys.SaveArg.devAddr,
		1,
		Char,
		TRUE
	},
	{
		0x0003,
		(char*)&sys.SaveArg.washingTime,
		(char*)&sys.SaveArg.washingTime,
		1,
		Short,
		TRUE
	},
	{
		0x0004,
		(char*)&sys.SaveArg.SetElectrolysisFlow,
		(char*)&sys.SaveArg.SetElectrolysisFlow,
		1,
		Float,
		FALSE
	},
	{
		0x0005,
		(char*)&sys.SaveArg.SetElectrolysisFlow,
		(char*)&sys.SaveArg.SetElectrolysisFlow + 2,
		1,
		Float,
		TRUE
	},
	{
		0x0006,
		(char*)&sys.SaveArg.SetAcidFlow,
		(char*)&sys.SaveArg.SetAcidFlow,
		1,
		Float,
		FALSE
	},
	{
		0x0007,
		(char*)&sys.SaveArg.SetAcidFlow,
		(char*)&sys.SaveArg.SetAcidFlow + 2,
		1,
		Float,
		TRUE
	},
	{
		0x0008,
		(char*)&sys.SaveArg.SetAlkaliFlow,
		(char*)&sys.SaveArg.SetAlkaliFlow,
		1,
		Float,
		FALSE
	},
	{
		0x0009,
		(char*)&sys.SaveArg.SetAlkaliFlow,
		(char*)&sys.SaveArg.SetAlkaliFlow + 2,
		1,
		Float,
		TRUE
	},
	{
		0x000A,
		(char*)&sys.SaveArg.SetWaterFlow1,
		(char*)&sys.SaveArg.SetWaterFlow1,
		1,
		Float,
		FALSE
	},
	{
		0x000B,
		(char*)&sys.SaveArg.SetWaterFlow1,
		(char*)&sys.SaveArg.SetWaterFlow1 + 2,
		1,
		Float,
		TRUE
	},
	{
		0x000C,
		(char*)&sys.SaveArg.SetWaterFlow2,
		(char*)&sys.SaveArg.SetWaterFlow2,
		1,
		Float,
		FALSE
	},
	{
		0x000D,
		(char*)&sys.SaveArg.SetWaterFlow2,
		(char*)&sys.SaveArg.SetWaterFlow2 + 2,
		1,
		Float,
		TRUE
	},
	{
		0x000E,
		(char*)&sys.time.year,
		(char*)&sys.time.year,
		1,
		Short,
		FALSE
	},
	{
		0x000F,
		(char*)&sys.time.month,
		(char*)&sys.time.month,
		1,
		Char,
		FALSE
	},
	{
		0x0010,
		(char*)&sys.time.date,
		(char*)&sys.time.date,
		1,
		Char,
		FALSE
	},
	{
		0x0011,
		(char*)&sys.time.hour,
		(char*)&sys.time.hour,
		1,
		Char,
		FALSE
	},
	{
		0x0012,
		(char*)&sys.time.min,
		(char*)&sys.time.min,
		1,
		Char,
		FALSE
	},
	{
		0x0013,
		(char*)&sys.time.sec,
		(char*)&sys.time.sec,
		1,
		Char,
		FALSE
	},
	{
		0x0014,
		(char*)&sys.time.week,
		(char*)&sys.time.week,
		1,
		Char,
		FALSE
	},
	{
		0x0015,
		(char*)&sys.Operat,
		(char*)&sys.Operat,
		1,
		Short,
		FALSE
	}
};


const stMBRegInputNode MBRegInputNode[INPUT_REG_LEN] =
{
	{
		0x0000,
		(char*)&sys.CPUTempereture,
		(char*)&sys.CPUTempereture,
		1,
		Float
	},
	{
		0x0001,
		(char*)&sys.CPUTempereture,
		(char*)&sys.CPUTempereture + 2,
		1,
		Float
	},
	{
		0x0002,
		(char*)&sys.waterTempereture,
		(char*)&sys.waterTempereture,
		1,
		Float
	},
	{
		0x0003,
		(char*)&sys.waterTempereture,
		(char*)&sys.waterTempereture + 2,
		1,
		Float
	},
	{
		0x0004,
		(char*)&sys.AcidPH,
		(char*)&sys.AcidPH,
		1,
		Float
	},
	{
		0x005,
		(char*)&sys.AcidPH,
		(char*)&sys.AcidPH + 2,
		1,
		Float
	},
	{
		0x0006,
		(char*)&sys.AlkaliPH,
		(char*)&sys.AlkaliPH,
		1,
		Float
	},
	{
		0x0007,
		(char*)&sys.AlkaliPH,
		(char*)&sys.AlkaliPH + 2,
		1,
		Float
	},
	{
		0x0008,
		(char*)&sys.PPM,
		(char*)&sys.PPM,
		1,
		Float
	},
	{
		0x0009,
		(char*)&sys.PPM,
		(char*)&sys.PPM + 2,
		1,
		Float
	},
	{
		0x000A,
		(char*)&sys.ORP,
		(char*)&sys.ORP,
		1,
		Float
	},
	{
		0x000B,
		(char*)&sys.ORP,
		(char*)&sys.ORP + 2,
		1,
		Float
	},
	{
		0x000C,
		(char*)&sys.ElectrolysisFlow,
		(char*)&sys.ElectrolysisFlow,
		1,
		Float
	},
	{
		0x000D,
		(char*)&sys.ElectrolysisFlow,
		(char*)&sys.ElectrolysisFlow + 2,
		1,
		Float
	},
	{
		0x000E,
		(char*)&sys.AcidFlow,
		(char*)&sys.AcidFlow,
		1,
		Float
	},
	{
		0x000F,
		(char*)&sys.AcidFlow,
		(char*)&sys.AcidFlow+ 2,
		1,
		Float
	},
	{
		0x0010,
		(char*)&sys.AlkaliFlow,
		(char*)&sys.AlkaliFlow,
		1,
		Float
	},
	{
		0x0011,
		(char*)&sys.AlkaliFlow,
		(char*)&sys.AlkaliFlow + 2,
		1,
		Float
	},
	{
		0x0012,
		(char*)&sys.WaterFlow1,
		(char*)&sys.WaterFlow1,
		1,
		Float
	},
	{
		0x0013,
		(char*)&sys.WaterFlow1,
		(char*)&sys.WaterFlow1 + 2,
		1,
		Float
	},
	{
		0x0014,
		(char*)&sys.WaterFlow2,
		(char*)&sys.WaterFlow2,
		1,
		Float
	},
	{
		0x0015,
		(char*)&sys.WaterFlow2,
		(char*)&sys.WaterFlow2 + 2,
		1,
		Float
	},
	{
		0x0016,
		(char*)&sys.SaveArg.TotalRunningTime,
		(char*)&sys.SaveArg.TotalRunningTime,
		1,
		Long
	},
	{
		0x0017,
		(char*)&sys.SaveArg.TotalRunningTime,
		(char*)&sys.SaveArg.TotalRunningTime + 2,
		1,
		Long
	},
	{
		0x0018,
		(char*)&sys.SaveArg.ElectrolyzerUsageTime,
		(char*)&sys.SaveArg.ElectrolyzerUsageTime,
		1,
		Long
	},
	{
		0x0019,
		(char*)&sys.SaveArg.ElectrolyzerUsageTime,
		(char*)&sys.SaveArg.ElectrolyzerUsageTime + 2,
		1,
		Long
	},
	{
		0x001A,
		(char*)&sys.SaveArg.washingNumber,
		(char*)&sys.SaveArg.washingNumber,
		1,
		Long
	},
	{
		0x001B,
		(char*)&sys.SaveArg.washingNumber,
		(char*)&sys.SaveArg.washingNumber + 2,
		1,
		Long
	},
	{
		0x001C,
		(char*)&sys.RunningTime,
		(char*)&sys.RunningTime,
		1,
		Long
	},
	{
		0x001D,
		(char*)&sys.RunningTime,
		(char*)&sys.RunningTime + 2,
		1,
		Long
	},
	{
		0x001E,
		(char*)&sys.AirTemperature,
		(char*)&sys.AirTemperature,
		1,
		Float
	},
	{
		0x001F,
		(char*)&sys.AirTemperature,
		(char*)&sys.AirTemperature + 2,
		1,
		Float
	},
	{
		0x0020,
		(char*)&sys.AirPressure,
		(char*)&sys.AirPressure,
		1,
		Float
	},
	{
		0x0021,
		(char*)&sys.AirPressure,
		(char*)&sys.AirPressure + 2,
		1,
		Float
	},
	{
		0x0022,
		(char*)&sys.PipePressure,
		(char*)&sys.PipePressure,
		1,
		Float
	},
	{
		0x0023,
		(char*)&sys.PipePressure,
		(char*)&sys.PipePressure + 2,
		1,
		Float
	},
	{
		0x0024,
		(char*)&sys.FaultCode,
		(char*)&sys.FaultCode,
		1,
		Short
	},
	{
		0x0025,
		(char*)&sys.Status,
		(char*)&sys.Status,
		1,
		Short
	}
};
		
