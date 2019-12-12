#include "Dev_UART.h"
#include "FreeRTOS.h"
#include "task.h"

USART_TypeDef* const UARTx[5] = {USART1, USART2, USART3, UART4, UART5};
const uint32_t RCC_Periph_UARTx[5] = {RCC_APB2Periph_USART1, RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_UART4, RCC_APB1Periph_UART5};
const uint32_t TX_RCC_APB2Periph_GPIOx[5] = {RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOC};
const uint32_t RX_RCC_APB2Periph_GPIOx[5] = {RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD};
GPIO_TypeDef* const TX_Port[5] = {GPIOA, GPIOA, GPIOD, GPIOC, GPIOC};
GPIO_TypeDef* const RX_Port[5] = {GPIOA, GPIOA, GPIOD, GPIOC, GPIOD};
const uint16_t TX_Pin[5] = {GPIO_Pin_9, GPIO_Pin_2, GPIO_Pin_8, GPIO_Pin_10, GPIO_Pin_12};
const uint16_t RX_Pin[5] = {GPIO_Pin_10, GPIO_Pin_3, GPIO_Pin_9, GPIO_Pin_11, GPIO_Pin_2};
const IRQn_Type UARTx_IRQn[5] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn};


static void RS485ENInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = RS485PORTREPIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(RS485PORTREIO, &GPIO_InitStructure);
}


void UARTConfig(COM COMx, u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if(COMx == COM1)
	{
		RCC_APB2PeriphClockCmd(RCC_Periph_UARTx[COMx] | TX_RCC_APB2Periph_GPIOx[COMx] | RX_RCC_APB2Periph_GPIOx[COMx] | RCC_APB2Periph_AFIO, ENABLE);	//ʹ��ʱ��
	}
	else
	{
		RCC_APB1PeriphClockCmd(RCC_Periph_UARTx[COMx], ENABLE);
		RCC_APB2PeriphClockCmd(TX_RCC_APB2Periph_GPIOx[COMx] | RX_RCC_APB2Periph_GPIOx[COMx] | RCC_APB2Periph_AFIO, ENABLE);	
	}

	if(COMx == COM3)
	{
		GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
	}
	
	//UARTx_TX   
	GPIO_InitStructure.GPIO_Pin = TX_Pin[COMx]; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(TX_Port[COMx], &GPIO_InitStructure);
   
	//UARTx_RX	  
	GPIO_InitStructure.GPIO_Pin = RX_Pin[COMx];//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(RX_Port[COMx], &GPIO_InitStructure);  

	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UARTx_IRQn[COMx];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(UARTx[COMx], &USART_InitStructure); //��ʼ������
	USART_ITConfig(UARTx[COMx], USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(UARTx[COMx], ENABLE);                    //ʹ�ܴ���

	if(COMx == RS485PORT)
	{
		RS485ENInit();
		RS485RE = 0;
	}
}


void UART_Send(COM com,u8* data, u16 size)
{
	if(com == RS485PORT)
	{
		RS485RE = 1; //485����ʹ��
	}
	while(USART_GetFlagStatus(UARTx[com], USART_FLAG_TC) == RESET);
	
	while(size--)
	{
		USART_SendData(UARTx[com], (u16)(*data++));
		while(USART_GetFlagStatus(UARTx[com], USART_FLAG_TC) == RESET);
	}
	
	if(com == RS485PORT)
	{
		RS485RE = 0; //485����ʹ��
	}
}


