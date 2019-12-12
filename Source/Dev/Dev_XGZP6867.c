#include "Dev_XGZP6867.h"
#include "delay.h"
#include "Dev_IIC.h"

static u8 XGZP_Write(u8 a, u8 b);
static u8 XGZP_Read(u8 a);

void XGZP6867Init(void)
{
	u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB6,PB7 输出高


	 i=XGZP_Read(0xa5);
	 i=i&0xfd;
	 delay_us(5);
	 XGZP_Write(0xa5,i);
}

u8 XGZP_Write(u8 a, u8 b)
{
	I2CStart(IIC2);
	I2CWriteByte(IIC2, 0xDA);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	I2CWriteByte(IIC2, a);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	I2CWriteByte(IIC2, b);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	I2CStop(IIC2);
	return SET;
}

u8 XGZP_Read(u8 a)
{
	u8 data;
	I2CStart(IIC2);
	I2CWriteByte(IIC2, 0xDA);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	I2CWriteByte(IIC2, a);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	I2CStart(IIC2);
	I2CWriteByte(IIC2, 0xDB);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	data = I2CReadByte(IIC2);
	I2CMasterAck(IIC2, 0);
	I2CStop(IIC2); 
	return data;
}

u8 XGZP_ReadBuff(u8 a, u8* buff,u8 len)
{
	u8 i;
	I2CStart(IIC2);
	I2CWriteByte(IIC2, 0xDA);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	I2CWriteByte(IIC2, a);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	I2CStart(IIC2);
	I2CWriteByte(IIC2, 0xDB);
	if( RESET == I2CSlaveAck(IIC2) ) 
	{ 
		return RESET; 
	} 
	for(i = 0; i < len; i++)
	{
		buff[i] = I2CReadByte(IIC2);
		I2CMasterAck(IIC2, 1);
	}
	I2CStop(IIC2); 

	return SET;
}




void XGZP_ReadData(float* temperature, float *AirPressure)
{
	int ad;
	u8 data[5];
	__disable_irq();
	XGZP_Write(0x30,0x0a);
  	delay_us(2500);
	XGZP_ReadBuff(0x06, data, 5);
	__enable_irq();
	
	if(data[0] < 0x80)
	{
		ad = (int)((data[0] << 16) |  (data[1] << 8) | data[2]);
	}
	else
	{
		ad = (int)((data[0] << 16) |  (data[1] << 8) | data[2] | 0xFF000000);
	}

	*AirPressure = (float)ad / 0x800000 * 0x80000;
	
	if(data[3] < 0x80)
	{
		ad = (int)((data[3] << 8) | data[4]);
	}
	else
	{
		ad = (int)((data[3] << 8) | data[4] | 0xFFFF0000);
	}
		
	*temperature = (float)ad / 0x100;
}

