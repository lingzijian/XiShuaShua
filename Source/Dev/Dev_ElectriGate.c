#include "Dev_ElectriGate.h"


void ElectriGateInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF, ENABLE);	

	 //控制方向引脚	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		

	 GPIO_Init(GPIOB, &GPIO_InitStructure);	

	 //门限引脚
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		     //上拉输入
	 
	 GPIO_Init(GPIOF, &GPIO_InitStructure);	
}


