#include "Dev_TLV5618.h"
#include "delay.h"
void TLV5618Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,  ENABLE); //SPI3时钟使能 	

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB3/4/5复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	SPI_I2S_DeInit(SPI3);
	//配置SPI通信
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;		//定义波特率预分频的值
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI3, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(SPI3, ENABLE); //使能SPI外设
}

static u16 DAC5618_WriteReg(uint16_t _RegValue)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, _RegValue);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3);
}

void TLV5618Out(DaChannel chanel, DaMode mode, float v)
{	
	uint16_t data;
	if(v > 4)
	{
		v = 4;
	}
	
	data = v / DAC_REF * 4095;
	if(v < 0)
		return;
	
	if(data > 4095) data = 4095;
	
	switch(chanel)
	{
		case channel1: 
			CS1 = 0;
			delay;
			DAC5618_WriteReg(0x8000 | (mode << 14) | data); 
			delay;
			CS1 = 1;
		    break;
		case channel2:
			CS1 = 0;
			delay;
			DAC5618_WriteReg(0x0000 | (mode << 14) | data);
			delay;
			CS1 = 1;
		    break;
		case channel3:
			CS2 = 0;
			delay;
			DAC5618_WriteReg(0x8000 | (mode << 14) | data); 
			delay;
			CS2 = 1;
		    break;
		case channel4:
			CS2 = 0;
			delay;
			DAC5618_WriteReg(0x0000 | (mode << 14) | data); 
			delay;
			CS2 = 1;
		    break;
		default: break; 
	}
	delay_ms(50);
}


