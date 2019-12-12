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





//任务句柄
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

TimerHandle_t 	FlowTimer_Handle;			//流量采样软件定时器
TimerHandle_t	DataUpdateTimer_Handle; 	//数据更新
TimerHandle_t	ValveControlTimer_Handle; 	//调节阀控制
TimerHandle_t	TimeLogTimer_Handle; 		//时间日志定时器




//开始任务任务函数
void Task_SysIinit(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	Dev_SystemInit();				//设备初始化
	
    //创建LED任务
    xTaskCreate((TaskFunction_t )Task_LED,     	
                (const char*    )"Task_LED",   	
                (u16       		)LED_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED_TASK_PRIO,	
                (TaskHandle_t*  )&LEDTask_Handler);
	#if (WATCHDOGENABLE == 1)
    //创建独立看门狗任务
    xTaskCreate((TaskFunction_t )Task_IWDG,     
                (const char*    )"Task_IWDG",   
                (u16       		)IWDG_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )IWDG_TASK_PRIO,
                (TaskHandle_t*  )&IWDGTask_Handler);
	#endif
	 //创建串口通信任务
    xTaskCreate((TaskFunction_t )Task_UART_Communication,     
                (const char*    )"Task_UART_Communication",   
                (u16       		)UART_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )UART_TASK_PRIO,
                (TaskHandle_t*  )&UARTTask_Handler); 
	  //创建按键检测任务
    xTaskCreate((TaskFunction_t )Task_KeyCheck,     
                (const char*    )"Task_KeyCheck",   
                (u16       		)KEY_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&KEYTask_Handler); 
	  //创建按键检测任务
    xTaskCreate((TaskFunction_t )Task_ModbusHandler,     
                (const char*    )"Task_KeyCheck",   
                (u16       		)MB_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MB_TASK_PRIO,
                (TaskHandle_t*  )&MBTask_Handler); 
	  #if (RTCENABLE == 1)
	   //创建RTC时钟更新任务
    xTaskCreate((TaskFunction_t )Task_RTC,     
                (const char*    )"Task_RTC",   
                (u16       		)RTC_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )RTC_TASK_PRIO,
                (TaskHandle_t*  )&RTCTask_Handler); 
	  #endif
	   //创建事件处理任务
    xTaskCreate((TaskFunction_t )Task_EventHandle,     
                (const char*    )"Task_EventHandle",   
                (u16       		)EVENTHANDLE_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )EVENTHANDLE_TASK_PRIO,
                (TaskHandle_t*  )&EventHandleTask_Handler); 
	  //创建系统执行任务
    xTaskCreate((TaskFunction_t )Task_SysExcute,     
                (const char*    )"Task_SysExcute",   
                (u16       		)SYSEXCUTE_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )SYSEXCUTE_TASK_PRIO,
                (TaskHandle_t*  )&SysExcuteTask_Handler); 
	  
	//流量采样定时器	周期为1s
	FlowTimer_Handle = xTimerCreate((const char*		)"FlowTimer",
						    (TickType_t			)1000,
				            (UBaseType_t		)pdTRUE,
				            (void*				)1,
				            (TimerCallbackFunction_t)GetFlow); //周期定时器，周期1s(1000个时钟节拍)，周期模式		
	//数据更新定时器	周期为500ms
	DataUpdateTimer_Handle = xTimerCreate((const char*		)"DataUpdateTimer",
						    (TickType_t			)500,
				            (UBaseType_t		)pdTRUE,
				            (void*				)1,
				            (TimerCallbackFunction_t)DataUpdate); //周期定时器，周期500ms(500个时钟节拍)，周期模式			
	//调节阀控制定时器	周期为200ms
	ValveControlTimer_Handle = xTimerCreate((const char*		)"ValveControlTimer",
						    (TickType_t			)200,
				            (UBaseType_t		)pdTRUE,
				            (void*				)1,
				            (TimerCallbackFunction_t)ValveController); //周期定时器，周期200ms(200个时钟节拍)，周期模式			
	//时间日志定时器	周期为300ms
	TimeLogTimer_Handle = xTimerCreate((const char*		)"TimeLogTimer",
						    (TickType_t			)300,
				            (UBaseType_t		)pdTRUE,
				            (void*				)1,
				            (TimerCallbackFunction_t)TimeLog); //周期定时器，周期300ms(300个时钟节拍)，周期模式	
	//开启软件定时器
	xTimerStart(FlowTimer_Handle, 0);
	xTimerStart(DataUpdateTimer_Handle, 0);
	xTimerStart(ValveControlTimer_Handle, 0);
	xTimerStart(TimeLogTimer_Handle, 0);
	
    vTaskDelete(SysIinitTask_Handler); //删除系统初始化任务
    taskEXIT_CRITICAL();            //退出临界区
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
//LED0任务函数 
void Task_LED(void *pvParameters)
{	
    while(TRUE)
    {
	   vTaskDelay(100);
	   LED_Linght();
    }
}   

//独立看门狗任务函数
void Task_IWDG(void *pvParameters)
{
    while(TRUE)
    {
        IWDG_Feed();
        vTaskDelay(200);
    }
}

//串口通信任务
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

//按键检测任务
void Task_KeyCheck(void *pvParameters)
{
	while(TRUE)
	{	
		EventChenck();
		vTaskDelay(10);
	}
}

//Modbus轮询任务
void Task_ModbusHandler(void *pvParameters)
{
	while(TRUE)
	{
		eMBPoll();
		vTaskDelay(20);
	}
}

//RTC时钟更新任务
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

//事件处理任务
void Task_EventHandle(void *pvParameters)
{
	u32 Notify;
	while (TRUE)
	{
		//有待处理事件
		if(xTaskNotifyWait((u32	)0x00,//进入函数的时候不清除任务bit
						(u32	)0xFFFFFFFF,		//退出函数的时候清除所有的bit
						(u32*	)&Notify,		//保存任务通知值
						(TickType_t	)5)				//阻塞时间
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

//系统执行任务
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

