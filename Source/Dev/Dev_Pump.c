#include "Dev_Pump.h"
#include "Dev_Timer.h"

void PumpInit(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE);	 //使能端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOG, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOG,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_4;
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_4);
}


void DRV8825Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	

	 TIM8_PWM_Init(17, 50);

	 //DRV8825EN	= ON;
}

//设置蠕动泵速率
void PeristalticPumpSetSpeed(u32 speed,Switch SW)
{
	u16 reload = 50000 / speed;
	
	if(SW == ON)
	{
		DRV8825EN = ON;
		TIM_SetAutoreload(TIM8, reload);
		TIM_SetCompare1(TIM8, reload / 2);
		TIM_Cmd(TIM8, ENABLE);
	}
	else
	{
		DRV8825EN = OFF;
		TIM_Cmd(TIM8, DISABLE);
	}
}

