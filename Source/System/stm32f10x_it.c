/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "Dev_UART.h"
#include "Dev_System.h"
#include "Configure.h"

u32 flowCount1 = 0, flowCount2 = 0, flowCount3 = 0, flowCount4 = 0, flowCount5 = 0;

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
//void SVC_Handler(void)
//{
//}
 
void DebugMon_Handler(void)
{
}
 
//void PendSV_Handler(void)
//{
//}
// 
//void SysTick_Handler(void)
//{
//}

#if (USINGFREEMODBUS  == 0)
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		//缓存接收到的数据
		if(sys.Uart[COM1].rxCount < RXADC3BUFFSIZE - 1)
		{
			sys.Uart[COM1].rxBuff[sys.Uart[COM1].rxCount++] = USART_ReceiveData(USART1);
		}
		else
		{
			USART_ReceiveData(USART1);
		}
     } 
} 
#endif

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		//缓存接收到的数据
		if(sys.Uart[COM2].rxCount < RXADC3BUFFSIZE - 1)
		{
			sys.Uart[COM2].rxBuff[sys.Uart[COM2].rxCount++] = USART_ReceiveData(USART2);
		}
		else
		{
			USART_ReceiveData(USART2);
		}
     } 
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		//缓存接收到的数据
		if(sys.Uart[COM3].rxCount < RXADC3BUFFSIZE - 1)
		{
			sys.Uart[COM3].rxBuff[sys.Uart[COM3].rxCount++] = USART_ReceiveData(USART3);
		}
		else
		{
			USART_ReceiveData(USART3);
		}
     } 
}

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断
	{
		//缓存接收到的数据
		if(sys.Uart[COM4].rxCount < RXADC3BUFFSIZE - 1)
		{
			sys.Uart[COM4].rxBuff[sys.Uart[COM4].rxCount++] = USART_ReceiveData(UART4);
		}
		else
		{
			USART_ReceiveData(UART4);
		}
     } 
}

void UART5_IRQHandler(void)
{
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断
	{
		//缓存接收到的数据
		if(sys.Uart[COM5].rxCount < RXADC3BUFFSIZE - 1)
		{
			sys.Uart[COM5].rxBuff[sys.Uart[COM5].rxCount++] = USART_ReceiveData(UART5);
		}
		else
		{
			USART_ReceiveData(UART5);
		}
     } 
}

void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	flowCount1 ++;
}

void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1);
	flowCount2 ++;
}

/*
void EXTI2_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line2);
	flowCount3 ++;
}

void EXTI3_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line3);
	flowCount4 ++;
}

void EXTI4_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line4);
	flowCount5 ++;
}
*/

void EXTI9_5_IRQHandler(void)
{
	if((EXTI->IMR & EXTI_Line7) && (EXTI->PR & EXTI_Line7))
	{
		flowCount3 ++;
		EXTI->PR = EXTI_Line7;
	}
	else if((EXTI->IMR & EXTI_Line8) && (EXTI->PR & EXTI_Line8))
	{
		flowCount4 ++;
		EXTI->PR = EXTI_Line8;
	}
	else if((EXTI->IMR & EXTI_Line9) && (EXTI->PR & EXTI_Line9))
	{
		flowCount5 ++;
		EXTI->PR = EXTI_Line9;
	}
}

void TIM3_IRQHandler(void)
{
	stTimer* p = TimerHeadNode;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
		//清除定时器3溢出中断标志位

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		while(p->next != NULL)
		{
			p = p->next;
			if(p->status == Timing)
			{
				if(p->count > 0)
					p->count--;
				else
					p->status = TimeOut;
			}
		}
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
