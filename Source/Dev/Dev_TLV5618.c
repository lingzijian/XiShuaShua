#include "Dev_TLV5618.h"
#include "delay.h"
void TLV5618Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,  ENABLE); //SPI3ʱ��ʹ�� 	

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB3/4/5����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	SPI_I2S_DeInit(SPI3);
	//����SPIͨ��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;		//���岨����Ԥ��Ƶ��ֵ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����
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


