#include "Dev_Key.h"
#include "stm32f10x.h"

void KeyInit(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);	
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		     //上拉输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	

	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	  	
}

//红外距离感应
void InfraredSensorInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		     //上拉输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 
	 GPIO_Init(GPIOE, &GPIO_InitStructure);	
}

