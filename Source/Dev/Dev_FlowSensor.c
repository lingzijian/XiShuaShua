#include "Dev_FlowSensor.h"

void FlowSensorInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG, ENABLE); //使能引脚时钟
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能复用功能时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  	EXTI_Init(&EXTI_InitStructure);	  

 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource7); 
  	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  	EXTI_Init(&EXTI_InitStructure);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource8); 
  	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  	EXTI_Init(&EXTI_InitStructure);	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource9); 
  	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  	EXTI_Init(&EXTI_InitStructure);		


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;				
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  	NVIC_Init(&NVIC_InitStructure);  	 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
  	NVIC_Init(&NVIC_InitStructure);  
/*
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
  	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
  	NVIC_Init(&NVIC_InitStructure); 
  	*/
}


