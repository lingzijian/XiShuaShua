#include "Dev_LED.h"
		    
//LED IO��ʼ��
void LED_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ�ܶ˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;			
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 
	 GPIO_Init(GPIOG, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOG, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);	

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; //���ۡ�������
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
}
 
