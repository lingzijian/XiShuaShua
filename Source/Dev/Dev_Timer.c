#include "Dev_Timer.h"
#include "Dev_System.h"
#include "FreeRTOS.h"

/*
void TIM1_PWM_Init(u16 psc, u16 arr)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
		 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

   //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM1 Channel4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������         
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//ʹ�ܻ��������         
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬         
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬ 
	TIM_OCInitStructure.TIM_Pulse		= arr / 2; 
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC4

	TIM_CtrlPWMOutputs(TIM1,ENABLE);

	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR4�ϵ�Ԥװ�ؼĴ���

	 TIM_ARRPreloadConfig(TIM1, ENABLE); 
 
	TIM_Cmd(TIM1, ENABLE);	//ʹ��TIM1
}
*/

void TIM8_PWM_Init(u16 psc, u16 arr)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//ʹ�ܶ�ʱ��8ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
		 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM8_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM8
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM8 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������         
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//ʹ�ܻ��������         
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬         
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬ 
	TIM_OCInitStructure.TIM_Pulse		= arr / 2; 
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM8 OC1

	TIM_CtrlPWMOutputs(TIM8,ENABLE);

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM8, ENABLE);	//ʹ��TIM8
}


void TIM3_Init(uint16_t ms)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//ʹ�ܶ�ʱ��3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//��ʱ��3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = ms;
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//��ʱ��3�ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//�������жϱ�־λ
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	//��ʱ��3����жϹر�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//��ʱ��3����
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
	
	timer->count = s * 10;  //��ʱ����100ms��ʱ��
	timer->status = Timing;
}

void SetTimer_HMS(stTimer* timer, u32 hms)
{
	if(timer == NULL)
		return;
	
	timer->count = hms;  //��ʱ����100ms��ʱ��
	timer->status = Timing;
}

TimerStatus GetTimerStatus(stTimer* timer)
{
	return (TimerStatus)timer->status;
}

void DeleteTimer(stTimer* timer)
{
	if((timer == NULL) || (timer == TimerHeadNode))  //ͷ��㲻����ɾ��
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


