#include "App_Communicate.h"
#include "Dev_UART.h"
#include "Dev_System.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include	"mbcrc.h"

void ReadPipePressure(void)
{
	u8 cmd[] = {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC5, 0xCB};
	UART_Send(COM4, cmd, sizeof(cmd));
}

void ReadTDS(void)
{
	u8 cmd[] = {0xff, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x00, 0x00, 0x04, 0x00, 0x00, 0xee};
	UART_Send(COM2, cmd, sizeof(cmd));
}
	
static void UART1_CommunicateHandler(void)
{
	UART_Send(COM1, sys.Uart[COM1].rxBuff, sys.Uart[COM1].rxCount);
}

 static void UART2_CommunicateHandler(void)
{
	if((sys.Uart[COM2].rxBuff[0] == 0xFF) && (sys.Uart[COM2].rxBuff[sys.Uart[COM2].rxCount - 1] == 0xEE))
	{
		float i = 1.0f;
		u8 t = sys.Uart[COM2].rxBuff[9];
		sys.TDS = sys.Uart[COM2].rxBuff[2] + (sys.Uart[COM2].rxBuff[3] << 8);
		while(t)
		{
			t /= 10;
			i *= 0.1f;
		}
		sys.waterTempereture = sys.Uart[COM2].rxBuff[8] + (float)sys.Uart[COM2].rxBuff[9] * i;
	}
}


static void UART3_CommunicateHandler(void)
{
	UART_Send(COM3, sys.Uart[COM3].rxBuff, sys.Uart[COM3].rxCount);
}

static void UART4_CommunicateHandler(void)
{
	/*
	u16 crc = usMBCRC16(sys.Uart[COM4].rxBuff, sys.Uart[COM4].rxCount);
	if((sys.Uart[COM4].rxBuff[0] == 0x01) 
		&& ((crc >> 8) == sys.Uart[COM4].rxBuff[sys.Uart[COM4].rxCount - 2])
		&& ((crc & 0x00FF) == sys.Uart[COM4].rxBuff[sys.Uart[COM4].rxCount - 1]))
		{
			sys.PipePressure = ((sys.Uart[COM4].rxBuff[3] << 8) | sys.Uart[COM4].rxBuff[4]) * 0.1f;
		}
		*/
		UART_Send(COM4, sys.Uart[COM4].rxBuff, sys.Uart[COM4].rxCount);
}


static void UART5_CommunicateHandler(void)
{
	
}

void UART_Check(COM com)
{
	u16 check;
	if(sys.Uart[com].rxCount != 0)
	{
		check = sys.Uart[com].rxCount;
		vTaskDelay(10);
		if(check == sys .Uart[com].rxCount)
		{
			switch(com)
			{
				case COM1:
					UART1_CommunicateHandler();
					break;
				case COM2:
					UART2_CommunicateHandler();
					break;
				case COM3:
					UART3_CommunicateHandler();
					break;
				case COM4:
					UART4_CommunicateHandler();
					break;
				case COM5:
					UART5_CommunicateHandler();
					break;
				default:
					break;
			}
			memset((char*)&sys.Uart[com], 0x00, sizeof(stUart));
		}
	}
}


