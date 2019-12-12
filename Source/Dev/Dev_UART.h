#ifndef DEV_UART_H
#define DEV_UART_H
#include "stm32f10x.h"
#include "sys.h"

#define RXADC3BUFFSIZE  50

#define MODBUSPORT  COM1  //USB


#define RS485PORT	COM4
#define RS485RE		   PAout(15)
#define RS485PORTREIO  GPIOA
#define RS485PORTREPIN GPIO_Pin_15

typedef enum{
		COM1 = 0,
		COM2,
		COM3,
		COM4,
		COM5
	}COM;

typedef struct{
	u16 rxCount;
	u8 rxBuff[RXADC3BUFFSIZE];
}stUart;

extern USART_TypeDef* const UARTx[5];
void UARTConfig(COM COMx, u32 BaudRate);
void UART_Send(COM com,u8* data, u16 size);


#endif

