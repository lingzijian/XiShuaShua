#include "Dev_ElectriGate.h"


void ElectriGateInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF, ENABLE);	

	 //���Ʒ�������	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		

	 GPIO_Init(GPIOB, &GPIO_InitStructure);	

	 //��������
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		     //��������
	 
	 GPIO_Init(GPIOF, &GPIO_InitStructure);	
}


