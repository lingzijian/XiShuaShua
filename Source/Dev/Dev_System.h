#ifndef DEV_SYSTEM_H
#define DEV_SYSTEM_H
#include "Dev_RTC.h" 		    
#include "Dev_UART.h"
#include "port.h"
#include "Dev_Timer.h"

#define NULL 0

#define PCB 			"TRMH-HR_V3.0_"
#define AUTHOR 			"_LZJ"


typedef enum{
	OFF = 0,
	ON 	= 1
	}Switch;
typedef struct{
	u32		baudrate;				//波特率
	u32		TotalRunningTime;		//累计运行时长		//分
	u32		ElectrolyzerUsageTime;	//电解槽累计使用时长 //分
	u32		washingNumber;			//洗消次数 已经洗消的总次数
	u16		washingTime;			//洗消时间 分
	u8 		devAddr;   				//modbus设备地址
	u8		null;   				//空间预留 (内存对齐)
	float  	SetElectrolysisFlow; 	//设置电解液流量 mL/Min
	float	SetAcidFlow;			//设置酸流量	L/Min
	float	SetAlkaliFlow;			//设置碱水流量 L/Min
	float	SetWaterFlow1;			//设置水流量1 L/Min
	float	SetWaterFlow2;			//设置水流量2 L/Min
	}stSaveArgument;
	
typedef struct{
	stSaveArgument SaveArg;			//存储到E2的参数
	stTime 	time;					//系统RTC时间
	stUart 	Uart[5];				//串口相关
	u32		RunningTime;			//本次开机运行时长 //分
	float  	CPUTempereture;   		//CPU温度
	float	waterTempereture;		//水温
	float  	AcidPH;					//酸PH值
	float  	AlkaliPH;				//碱PH值
	float  	PPM; 					//PPM浓度
	float 	ORP;			 		//氧化还原电位
	float	TDS;					//总溶解固体
	float  	ElectrolysisFlow; 		//电解液流量 mL/Min
	float	AcidFlow;				//酸流量 L/Min
	float	AlkaliFlow;				//碱水流量 L/Min
	float	WaterFlow1;				//水流量 L/Min
	float	WaterFlow2;				//水流量 L/Min
	float	AirTemperature;			//气温
	float	AirPressure;			//气压
	float	PipePressure;			//管道压力 //测漏压力
	int		ElectrolysisCount;		//控制电解 
	u16		FaultCode;				//故障码
	u16		Status;					//工作状态
	u16		Operat;					//操作
	u8		ElectriGate;			//电动门操作 0为无操作 1为开 2为关
	char    Version[50];			//版本
	u8      simu;
}stSysParameter;



typedef enum{
	Char = 0,
	Short,
	Long,
	Float,
	Double
	}DataType;

typedef struct{
	u16 		RegAddress;	//寄存器地址	
	char*		VarAddress;	//变量地址
	char*		MemAddress;	//内存地址
	u16			len;		//数据长度
	DataType	type;		//数据类型
	BOOL		Save;		//是否保存到E2
}stMBRegHoldingNode; //保持寄存器节点	//可读可写


typedef struct{
	u16 		RegAddress;	//寄存器地址	
	char*		VarAddress;	//变量地址
	char*		MemAddress;	//内存地址
	u16			len;		//数据长度
	DataType	type;		//数据类型
}stMBRegInputNode; //输入寄存器节点              //只读


extern stSysParameter sys;

#define HOLDING_REG_LEN 	0x0015 	+ 1
#define INPUT_REG_LEN		0x0025	+ 1

extern const stMBRegHoldingNode MBRegHoldingNode[HOLDING_REG_LEN];
extern const stMBRegInputNode MBRegInputNode[INPUT_REG_LEN];
void Dev_SystemInit(void);

extern stTimer* TimerHeadNode;


#endif

