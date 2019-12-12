#include "Dev_DAC7551.h"

void DAC7551Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOE, ENABLE );
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE ); 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3,  ENABLE );//SPI3ʱ��ʹ�� 	

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	//vTaskDelay(200);
	CLR1 = 1; CLR2 = 1; CLR3 = 1; CLR4 = 1;
	delay_us(10);
	CLR1 = 0; CLR2 = 0; CLR3 = 0; CLR4 = 0;

	CS1 = 0; CS2 = 0; CS3 = 0; CS4 = 0;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB3/4/5����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	SPI_I2S_DeInit(SPI3);
	//����SPIͨ��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//���岨����Ԥ��Ƶ��ֵ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 	//SPI_SSOutputCmd(SPI3, ENABLE);
	SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����
}

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI3->CR1&=0XFFC7;
	SPI3->CR1|=SPI_BaudRatePrescaler;	//����SPI3�ٶ� 
	SPI_Cmd(SPI3, ENABLE); 
} 


u16 SPI3_ReadWrite(u16 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI3, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����					    
}

void DAC7551Out(u8 CS, float vol)
{
	u16 da = (u16)(vol / 5.0f * 4096);
	
	if(CS == 1) CS1 = 1;
	else if(CS == 2) CS2 = 1;
	else if(CS == 3) CS3 = 1;
	else if(CS == 4) CS4 = 1;
	else	return;
	delay;
	
	if(vol > 5)
		return;
	SPI3_ReadWrite(da);
	
	delay;
	if(CS == 1) CS1 = 0;
	else if(CS == 2) CS2 = 0;
	else if(CS == 3) CS3 = 0;
	else if(CS == 4) CS4 = 0;
}
