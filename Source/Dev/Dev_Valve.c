#include "Dev_Valve.h"
#include "arm_math.h"

extern arm_pid_instance_f32 pid[4];

void RadiotubeInit(void) //��ŷ���ʼ��
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ�ܶ˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOG, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOG,GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12);
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOD, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	 //GPIO_ResetBits(GPIOD, GPIO_Pin_5 | GPIO_Pin_6);
}

void PID_ValveInit(void)		//PID���ڷ���ʼ��
{
	u8 i;
	
	for(i = 0; i < 5; i++)
	{
		pid[i].Kp = 1.4;
		pid[i].Ki = 2;
		pid[i].Kd = 0;
		arm_pid_init_f32(&pid[i], 1);
	}
}


