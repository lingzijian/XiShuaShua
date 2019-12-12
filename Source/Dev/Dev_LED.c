#include "Dev_LED.h"
		    
//LED IO初始化
void LED_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);	 //使能端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;			
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 
	 GPIO_Init(GPIOG, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOG, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);	

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; //电解槽、蜂鸣器
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
}
 
