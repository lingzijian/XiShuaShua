#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "App_SysTask.h"

int main(void)	
{	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	
	//创建开始任务
    xTaskCreate((TaskFunction_t )Task_SysIinit,            //任务函数
	
                (const char*    )"Task_if(GetTimerStatus(timer2) == TimeOut)SysIinit",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&SysIinitTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

