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
	u32		baudrate;				//������
	u32		TotalRunningTime;		//�ۼ�����ʱ��		//��
	u32		ElectrolyzerUsageTime;	//�����ۼ�ʹ��ʱ�� //��
	u32		washingNumber;			//ϴ������ �Ѿ�ϴ�����ܴ���
	u16		washingTime;			//ϴ��ʱ�� ��
	u8 		devAddr;   				//modbus�豸��ַ
	u8		null;   				//�ռ�Ԥ�� (�ڴ����)
	float  	SetElectrolysisFlow; 	//���õ��Һ���� mL/Min
	float	SetAcidFlow;			//����������	L/Min
	float	SetAlkaliFlow;			//���ü�ˮ���� L/Min
	float	SetWaterFlow1;			//����ˮ����1 L/Min
	float	SetWaterFlow2;			//����ˮ����2 L/Min
	}stSaveArgument;
	
typedef struct{
	stSaveArgument SaveArg;			//�洢��E2�Ĳ���
	stTime 	time;					//ϵͳRTCʱ��
	stUart 	Uart[5];				//�������
	u32		RunningTime;			//���ο�������ʱ�� //��
	float  	CPUTempereture;   		//CPU�¶�
	float	waterTempereture;		//ˮ��
	float  	AcidPH;					//��PHֵ
	float  	AlkaliPH;				//��PHֵ
	float  	PPM; 					//PPMŨ��
	float 	ORP;			 		//������ԭ��λ
	float	TDS;					//���ܽ����
	float  	ElectrolysisFlow; 		//���Һ���� mL/Min
	float	AcidFlow;				//������ L/Min
	float	AlkaliFlow;				//��ˮ���� L/Min
	float	WaterFlow1;				//ˮ���� L/Min
	float	WaterFlow2;				//ˮ���� L/Min
	float	AirTemperature;			//����
	float	AirPressure;			//��ѹ
	float	PipePressure;			//�ܵ�ѹ�� //��©ѹ��
	int		ElectrolysisCount;		//���Ƶ�� 
	u16		FaultCode;				//������
	u16		Status;					//����״̬
	u16		Operat;					//����
	u8		ElectriGate;			//�綯�Ų��� 0Ϊ�޲��� 1Ϊ�� 2Ϊ��
	char    Version[50];			//�汾
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
	u16 		RegAddress;	//�Ĵ�����ַ	
	char*		VarAddress;	//������ַ
	char*		MemAddress;	//�ڴ��ַ
	u16			len;		//���ݳ���
	DataType	type;		//��������
	BOOL		Save;		//�Ƿ񱣴浽E2
}stMBRegHoldingNode; //���ּĴ����ڵ�	//�ɶ���д


typedef struct{
	u16 		RegAddress;	//�Ĵ�����ַ	
	char*		VarAddress;	//������ַ
	char*		MemAddress;	//�ڴ��ַ
	u16			len;		//���ݳ���
	DataType	type;		//��������
}stMBRegInputNode; //����Ĵ����ڵ�              //ֻ��


extern stSysParameter sys;

#define HOLDING_REG_LEN 	0x0015 	+ 1
#define INPUT_REG_LEN		0x0025	+ 1

extern const stMBRegHoldingNode MBRegHoldingNode[HOLDING_REG_LEN];
extern const stMBRegInputNode MBRegInputNode[INPUT_REG_LEN];
void Dev_SystemInit(void);

extern stTimer* TimerHeadNode;


#endif

