#include "Dev_Timer.h"
#include "Dev_System.h"
#include "FreeRTOS.h"

/*
void TIM1_PWM_Init(u16 psc, u16 arr)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//使能定时器1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
		 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

   //初始化TIM1
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM1 Channel4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性         
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//使能互补端输出         
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态         
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态 
	TIM_OCInitStructure.TIM_Pulse		= arr / 2; 
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4

	TIM_CtrlPWMOutputs(TIM1,ENABLE);

	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR4上的预装载寄存器

	 TIM_ARRPreloadConfig(TIM1, ENABLE); 
 
	TIM_Cmd(TIM1, ENABLE);	//使能TIM1
}
*/

void TIM8_PWM_Init(u16 psc, u16 arr)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//使能定时器8时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
		 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM8_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM8
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM8 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性         
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//使能互补端输出         
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态         
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态 
	TIM_OCInitStructure.TIM_Pulse		= arr / 2; 
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC1

	TIM_CtrlPWMOutputs(TIM8,ENABLE);

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR1上的预装载寄存器
 
	TIM_Cmd(TIM8, ENABLE);	//使能TIM8
}


void TIM3_Init(uint16_t ms)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//使能定时器3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//定时器3初始化
	TIM_TimeBaseStructure.TIM_Period = ms;
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//预装载使能
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//定时器3中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//清除溢出中断标志位
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	//定时器3溢出中断关闭
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//定时器3禁能
	TIM_Cmd(TIM3,  ENABLE);
}

stTimer* TimerHeadNode = NULL;

void SysTimerInit(void)
{
	TIM3_Init(100);
	TimerHeadNode = (stTimer*)pvPortMalloc(sizeof(stTimer));
	TimerHeadNode->count = 0;
	TimerHeadNode->status = None;
	TimerHeadNode->Prev   = NULL;
	TimerHeadNode->next   = NULL;
}

stTimer* CreatTimer(void)
{
	 stTimer* timer = NULL;
	if(TimerHeadNode == NULL)
		return NULL;

	 timer = (stTimer*)pvPortMalloc(sizeof(stTimer));
	 timer->count = 0;
	 timer->status = None;
	 timer->Prev   = TimerHeadNode; 
	 if(TimerHeadNode->next != NULL)
	 {
	 	TimerHeadNode->next->Prev = timer;
		timer->next = TimerHeadNode->next;
	 }
	 else
	 {
	 	timer->next = NULL;
	 }
	 TimerHeadNode->next = timer;
	 return timer;
}

void SetTimer(stTimer* timer, u32 s)
{
	if(timer == NULL)
		return;
	
	timer->count = s * 10;  //定时器是100ms计时的
	timer->status = Timing;
}

void SetTimer_HMS(stTimer* timer, u32 hms)
{
	if(timer == NULL)
		return;
	
	timer->count = hms;  //定时器是100ms计时的
	timer->status = Timing;
}

TimerStatus GetTimerStatus(stTimer* timer)
{
	return (TimerStatus)timer->status;
}

void DeleteTimer(stTimer* timer)
{
	if((timer == NULL) || (timer == TimerHeadNode))  //头结点不允许删除
		return;
	
	if(timer->next != NULL)
	{
		timer->Prev->next = timer->next;
		timer->next->Prev = timer->Prev;
	}
	else
	{
		timer->Prev->next = NULL;
	}
	
	timer->count = 0;
	timer->status = None;
	vPortFree(timer);
}

void PauseTimer(stTimer* timer)
{
	if(timer->status == Timing)
		timer->status = None;
}

void ResumeTimer(stTimer* timer)
{
	if(timer->status == None)
		timer->status = Timing;
}


