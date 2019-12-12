#include "Dev_ADC.h"
#include "Dev_System.h"

u16 ADC1_Buff[ADC1BUFFSIZE][ADC1CHANNELNUM];
u16 ADC3_Buff[ADC3BUFFSIZE][ADC3CHANNELNUM];

void ADC1_DMA_Configuration(void) 
{ 
	/* ADC1  DMA1 Channel Config */      
	DMA_InitTypeDef DMA_InitStructure;    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMAʱ��     
	DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ��ADC1����DMA1ͨ��1    
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&(ADC1->DR);  //DMA����ADC����ַ    
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_Buff;      //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = ADC1CHANNELNUM * ADC1BUFFSIZE;  //��ֵΪ����һ��DMA����Ĵ���    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ����    
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������λ���16λ����DMA����ߴ�    
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��16λ    
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ��һ�ֽ������Զ���ʼ���ִ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�    
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��x��ֹ�ڴ浽�ڴ�
	DMA_Init((DMA1_Channel1), &DMA_InitStructure);  //����DMA_InitStruct�в���DMAͨ��

	DMA_Cmd((DMA1_Channel1), ENABLE);
}

void  ADC1_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	
	ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);
	
	ADC1_DMA_Configuration();
	ADC_DMACmd(ADC1, ENABLE); //ʹ��DMA����
	ADC_Cmd(ADC1, ENABLE);	//ʹָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����	

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC3_DMA_Configuration(void) 
{ 
	/* ADC3  DMA1 Channel Config */      
	DMA_InitTypeDef DMA_InitStructure;    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); //ʹ��DMAʱ��     
	DMA_DeInit(DMA2_Channel5);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ��ADC3����DMA2ͨ��5    
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&(ADC3->DR);  //DMA����ADC����ַ    
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC3_Buff;      //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = ADC3CHANNELNUM*ADC3BUFFSIZE;  //��ֵΪ����һ��DMA����Ĵ���    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ����    
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������λ���16λ����DMA����ߴ�    
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��16λ    
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ��һ�ֽ������Զ���ʼ���ִ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�    
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��x��ֹ�ڴ浽�ڴ�
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);  //����DMA_InitStruct�в���DMAͨ��

	DMA_Cmd(DMA2_Channel5, ENABLE);
}

void  ADC3_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF | RCC_APB2Periph_ADC3, ENABLE );	  //ʹ��ADC3ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//��©���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOF, GPIO_Pin_3 | GPIO_Pin_4);
	
	ADC_DeInit(ADC3);  //��λADC3,������ ADC3 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = ADC3CHANNELNUM;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC3, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
		
	ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5); 
	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5);    
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);    
	
	ADC3_DMA_Configuration();
	ADC_DMACmd(ADC3, ENABLE); //ʹ��DMA����
	ADC_Cmd(ADC3, ENABLE);	//ʹָ����ADC3
	
	ADC_ResetCalibration(ADC3);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC3));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC3);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC3));	 //�ȴ�У׼����	

	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}	

float AD1_Average(u8 channel)
{
	float ave = 0;
	u16	i, j, k = 0;
	if(channel >= ADC1CHANNELNUM)
		return 0;
	
	for(i = 0; i < ADC1BUFFSIZE - 1; i++)
	{
		for(j = i + 1; j < ADC1BUFFSIZE; j++)
		{
			if(ADC1_Buff[i][channel] > ADC1_Buff[j][channel])
			{
				ADC1_Buff[i][channel] += ADC1_Buff[j][channel];
				ADC1_Buff[j][channel] = ADC1_Buff[i][channel] - ADC1_Buff[j][channel];
				ADC1_Buff[i][channel] = ADC1_Buff[i][channel] - ADC1_Buff[j][channel];
			}
		}
	}
	
	if(ADC1BUFFSIZE > 10)
	{
		i = ADC1BUFFSIZE / 5;
		j = ADC1BUFFSIZE / 5.0 * 4;
	}
	else
	{
		i = 0;
		j = ADC1BUFFSIZE;
	}
	
	for(; i < j; i++, k++)
	{
		ave = (float)(ADC1_Buff[i][channel] + ave * k) / (k + 1);
	}

	return ave;
}

float AD3_Average(u8 channel)
{
	float ave = 0;
	u16	i, j, k = 0;
	if(channel >= ADC3CHANNELNUM)
		return 0;
	
	for(i = 0; i < ADC3BUFFSIZE - 1; i++)
	{
		for(j = i + 1; j < ADC3BUFFSIZE; j++)
		{
			if(ADC3_Buff[i][channel] > ADC3_Buff[j][channel])
			{
				ADC3_Buff[i][channel] += ADC3_Buff[j][channel];
				ADC3_Buff[j][channel] = ADC3_Buff[i][channel] - ADC3_Buff[j][channel];
				ADC3_Buff[i][channel] = ADC3_Buff[i][channel] - ADC3_Buff[j][channel];
			}
		}
	}
	
	if(ADC3BUFFSIZE > 10)
	{
		i = ADC3BUFFSIZE / 5;
		j = ADC3BUFFSIZE / 5.0 * 4;
	}
	else
	{
		i = 0;
		j = ADC3BUFFSIZE;
	}
	
	for(; i < j; i++, k++)
	{
		ave = (float)(ADC3_Buff[i][channel] + ave * k) / (k + 1);
	}

	return ave;
}


