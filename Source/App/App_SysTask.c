#include "App_SysTask.h"
#include "Dev_System.h"
#include "Dev_IWDG.h"
#include "App_Communicate.h"
#include  "Dev_UART.h"
#include "App_SystemExecute.h"
#include "mb.h"
#include "timers.h"
#include "Dev_M24Cxx.h"
#include "Configure.h"
#include "Dev_ElectriGate.h"
#include "Dev_Timer.h"
#include "Dev_TLV5618.h"
#include "Dev_LevelProbe.h"
#include "Dev_Valve.h"





//������
TaskHandle_t SysIinitTask_Handler;
TaskHandle_t LEDTask_Handler;
TaskHandle_t IWDGTask_Handler;
TaskHandle_t UARTTask_Handler;
TaskHandle_t KEYTask_Handler;
TaskHandle_t MBTask_Handler;
TaskHandle_t IWDGTask_Handler;
TaskHandle_t RTCTask_Handler;
TaskHandle_t EventHandleTask_Handler;
TaskHandle_t SysExcuteTask_Handler;

TimerHandle_t 	FlowTimer_Handle;			//�������������ʱ��
TimerHandle_t	DataUpdateTimer_Handle; 	//���ݸ���
TimerHandle_t	ValveControlTimer_Handle; 	//���ڷ�����
TimerHandle_t	TimeLogTimer_Handle; 		//ʱ����־��ʱ��




//��ʼ����������
void Task_SysIinit(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	Dev_SystemInit();				//�豸��ʼ��
	
    //����LED����
    xTaskCreate((TaskFunction_t )Task_LED,     	
                (const char*    )"Task_LED",   	
                (u16       		)LED_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED_TASK_PRIO,	
                (TaskHandle_t*  )&LEDTask_Handler);
	#if (WATCHDOGENABLE == 1)
    //�����������Ź�����
    xTaskCreate((TaskFunction_t )Task_IWDG,     
                (const char*    )"Task_IWDG",   
                (u16       		)IWDG_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )IWDG_TASK_PRIO,
                (TaskHandle_t*  )&IWDGTask_Handler);
	#endif
	 //��������ͨ������
    xTaskCreate((TaskFunction_t )Task_UART_Communication,     
                (const char*    )"Task_UART_Communication",   
                (u16       		)UART_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )UART_TASK_PRIO,
                (TaskHandle_t*  )&UARTTask_Handler); 
	  //���������������
    xTaskCreate((TaskFunction_t )Task_KeyCheck,     
                (const char*    )"Task_KeyCheck",   
                (u16       		)KEY_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&KEYTask_Handler); 
	  //���������������
    xTaskCreate((TaskFunction_t )Task_ModbusHandler,     
                (const char*    )"Task_KeyCheck",   
                (u16       		)MB_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MB_TASK_PRIO,
                (TaskHandle_t*  )&MBTask_Handler); 
	  #if (RTCENABLE == 1)
	   //����RTCʱ�Ӹ�������
    xTaskCreate((TaskFunction_t )Task_RTC,     
                (const char*    )"Task_RTC",   
                (u16       		)RTC_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )RTC_TASK_PRIO,
                (TaskHandle_t*  )&RTCTask_Handler); 
	  #endif
	   //�����¼���������
    xTaskCreate((TaskFunction_t )Task_EventHandle,     
                (const char*    )"Task_EventHandle",   
                (u16       		)EVENTHANDLE_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )EVENTHANDLE_TASK_PRIO,
                (TaskHandle_t*  )&EventHandleTask_Handler); 
	  //����ϵͳִ������
    xTaskCreate((TaskFunction_t )Task_SysExcute,     
                (const char*    )"Task_SysExcute",   
                (u16       		)SYSEXCUTE_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )SYSEXCUTE_TASK_PRIO,
                (TaskHandle_t*  )&SysExcuteTask_Handler); 
	  
	//����������ʱ��	����Ϊ1s
	FlowTimer_Handle = xTimerCreate((const char*		)"FlowTimer",
						    (TickType_t			)1000,
				            (UBaseType_t		)pdTRUE,
				            (void*				)1,
				            (TimerCallbackFunction_t)GetFlow); //���ڶ�ʱ��������1s(1000��ʱ�ӽ���)������ģʽ		
	//���ݸ��¶�ʱ��	����Ϊ500ms
	DataUpdateTimer_Handle = xTimerCreate((const char*		)"DataUpdateTimer",
						    (TickType_t			)500,
				            (UBaseType_t		)pdTRUE,
				            (void*				)1,
				            (TimerCallbackFunction_t)DataUpdate); //���ڶ�ʱ��������500ms(500��ʱ�ӽ���)������ģʽ			
	//���ڷ����ƶ�ʱ��	����Ϊ200ms
	ValveControlTimer_Handle = xTimerCreate((const char*		)"ValveControlTimer",
						    (TickType_t			)200,
				            (UBaseType_t		)pdTRUE,
				            (void*				)1,
				            (TimerCallbackFunction_t)ValveController); //���ڶ�ʱ��������200ms(200��ʱ�ӽ���)������ģʽ			
	//ʱ����־��ʱ��	����Ϊ300ms
	TimeLogTimer_Handle = xTimerCreate((const char*		)"TimeLogTimer",
						    (TickType_t			)300,
				            (UBaseType_t		)pdTRUE,
				            (void*				)1,
				            (TimerCallbackFunction_t)TimeLog); //���ڶ�ʱ��������300ms(300��ʱ�ӽ���)������ģʽ	
	//���������ʱ��
	xTimerStart(FlowTimer_Handle, 0);
	xTimerStart(DataUpdateTimer_Handle, 0);
	xTimerStart(ValveControlTimer_Handle, 0);
	xTimerStart(TimeLogTimer_Handle, 0);
	
    vTaskDelete(SysIinitTask_Handler); //ɾ��ϵͳ��ʼ������
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

/*
typedef struct{
	float v;
	u8 da1;
	u8 da2;
	u8 da3;
	u8 da4;
}DaTest;

DaTest da;
*/
//LED0������ 
void Task_LED(void *pvParameters)
{	
    while(TRUE)
    {
	   vTaskDelay(100);
	   LED_Linght();
    }
}   

//�������Ź�������
void Task_IWDG(void *pvParameters)
{
    while(TRUE)
    {
        IWDG_Feed();
        vTaskDelay(200);
    }
}

//����ͨ������
void Task_UART_Communication(void *pvParameters)
{
	while(TRUE)
	{
		UART_Check(COM2);
		UART_Check(COM3);
		UART_Check(COM4);
		vTaskDelay(10);
	}
}

//�����������
void Task_KeyCheck(void *pvParameters)
{
	while(TRUE)
	{	
		EventChenck();
		vTaskDelay(10);
	}
}

//Modbus��ѯ����
void Task_ModbusHandler(void *pvParameters)
{
	while(TRUE)
	{
		eMBPoll();
		vTaskDelay(20);
	}
}

//RTCʱ�Ӹ�������
void Task_RTC(void *pvParameters)
{
	if(RTC_Init() != 0)
		vTaskDelete(RTCTask_Handler);
	while(TRUE)
	{
		RTC_Get(&sys.time);
		vTaskDelay(200);
	}
}

u8 init = 0;

//�¼���������
void Task_EventHandle(void *pvParameters)
{
	u32 Notify;
	while (TRUE)
	{
		//�д������¼�
		if(xTaskNotifyWait((u32	)0x00,//���뺯����ʱ���������bit
						(u32	)0xFFFFFFFF,		//�˳�������ʱ��������е�bit
						(u32*	)&Notify,		//��������ֵ֪ͨ
						(TickType_t	)5)				//����ʱ��
						== pdTRUE)	
		{
				KeyHandle(Notify);
				ExternEventHandle(Notify);
		}
						
		if(init == 1)
		{
			ElectrolysisControl();
		}
		vTaskDelay(10);
	}
}

//ϵͳִ������
void Task_SysExcute(void *pvParameters)
{
	runStatus = 1;
	while(TRUE)
	{
		if(init == 0){
			WashingInit();
		}
		else{
			SysExcute(); 
		}
		vTaskDelay(20);
	}
}

