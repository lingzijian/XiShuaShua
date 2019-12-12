#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "App_SysTask.h"

int main(void)	
{	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	
	//������ʼ����
    xTaskCreate((TaskFunction_t )Task_SysIinit,            //������
	
                (const char*    )"Task_if(GetTimerStatus(timer2) == TimeOut)SysIinit",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&SysIinitTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

