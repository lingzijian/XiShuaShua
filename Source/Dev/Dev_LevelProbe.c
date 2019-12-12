#include "Dev_LevelProbe.h"

void LevelProbeInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);	 
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		     //上拉输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	
	 
	 GPIO_InitStructure.GPIO_Pin = /*GPIO_Pin_11 | */GPIO_Pin_12 | GPIO_Pin_13;
	 GPIO_Init(GPIOF, &GPIO_InitStructure);	

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	 GPIO_Init(GPIOE, &GPIO_InitStructure);	
}
/*
void LevelProbeTest(void)
{
	u8 t = 0;
	
	if(!ELECTROLYSIS_LP)
	{
		t = 1;
	}
	if(!ALCOHOL_LP)
	{
		t = 1;
	}
	if(!ACIDEMPTY_LP)
	{
		t = 1;
	}
	if(!ACIDFULL_LP)
	{
		t = 1;
	}
	if(!ALKALIEMPTY_LP)
	{
		t = 1;
	}
	if(!ALKALIFULL_LP)
	{
		t = 1;
	}
	if(!WATHEREMPTY_LP)
	{
		t = 1;
	}
}
*/
