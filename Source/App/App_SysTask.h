#ifndef APP_SYSTASK_H
#define APP_SYSTASK_H
#include "FreeRTOS.h"
#include "task.h"
#include "Dev_LED.h"

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
extern TaskHandle_t SysIinitTask_Handler;
//任务函数
void Task_SysIinit(void *pvParameters);

#define LED_TASK_PRIO		2
#define LED_STK_SIZE 		64  
extern TaskHandle_t LEDTask_Handler;
void Task_LED(void *pvParameters);

#define IWDG_TASK_PRIO		3
#define IWDG_STK_SIZE 		64  
extern TaskHandle_t IWDGTask_Handler;
void Task_IWDG(void *pvParameters);

#define UART_TASK_PRIO		4
#define UART_STK_SIZE 		128  
extern TaskHandle_t UARTTask_Handler;
void Task_UART_Communication(void *pvParameters);

#define KEY_TASK_PRIO		4
#define KEY_STK_SIZE 		64  
extern TaskHandle_t KEYTask_Handler;
void Task_KeyCheck(void *pvParameters);

#define MB_TASK_PRIO		4
#define MB_STK_SIZE 		64  
extern TaskHandle_t MBTask_Handler;
void Task_ModbusHandler(void *pvParameters);

#define RTC_TASK_PRIO		4
#define RTC_STK_SIZE 		64  
extern TaskHandle_t RTCTask_Handler;
void Task_RTC(void *pvParameters);

#define EVENTHANDLE_TASK_PRIO		4
#define EVENTHANDLE_STK_SIZE 		64  
extern TaskHandle_t EventHandleTask_Handler;
void Task_EventHandle(void *pvParameters);

#define SYSEXCUTE_TASK_PRIO		4
#define SYSEXCUTE_STK_SIZE 		64  
extern TaskHandle_t SysExcuteTask_Handler;
void Task_SysExcute(void *pvParameters);
void ArgumentTest(void);



#endif
