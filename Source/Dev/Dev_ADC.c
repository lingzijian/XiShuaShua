#include "Dev_ADC.h"
#include "Dev_System.h"

u16 ADC1_Buff[ADC1BUFFSIZE][ADC1CHANNELNUM];
u16 ADC3_Buff[ADC3BUFFSIZE][ADC3CHANNELNUM];

void ADC1_DMA_Configuration(void) 
{ 
	/* ADC1  DMA1 Channel Config */      
	DMA_InitTypeDef DMA_InitStructure;    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA时钟     
	DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值：ADC1连接DMA1通道1    
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&(ADC1->DR);  //DMA外设ADC基地址    
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_Buff;      //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = ADC1CHANNELNUM * ADC1BUFFSIZE;  //此值为完整一轮DMA传输的次数    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不变    
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据位宽度16位，即DMA传输尺寸    
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度16位    
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式，一轮结束后自动开始下轮传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级    
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x禁止内存到内存
	DMA_Init((DMA1_Channel1), &DMA_InitStructure);  //根据DMA_InitStruct中参数DMA通道

	DMA_Cmd((DMA1_Channel1), ENABLE);
}

void  ADC1_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	
	ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);
	
	ADC1_DMA_Configuration();
	ADC_DMACmd(ADC1, ENABLE); //使能DMA传输
	ADC_Cmd(ADC1, ENABLE);	//使指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束	

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC3_DMA_Configuration(void) 
{ 
	/* ADC3  DMA1 Channel Config */      
	DMA_InitTypeDef DMA_InitStructure;    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); //使能DMA时钟     
	DMA_DeInit(DMA2_Channel5);   //将DMA的通道1寄存器重设为缺省值：ADC3连接DMA2通道5    
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&(ADC3->DR);  //DMA外设ADC基地址    
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC3_Buff;      //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = ADC3CHANNELNUM*ADC3BUFFSIZE;  //此值为完整一轮DMA传输的次数    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不变    
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据位宽度16位，即DMA传输尺寸    
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度16位    
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式，一轮结束后自动开始下轮传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级    
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x禁止内存到内存
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中参数DMA通道

	DMA_Cmd(DMA2_Channel5, ENABLE);
}

void  ADC3_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF | RCC_APB2Periph_ADC3, ENABLE );	  //使能ADC3通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOF, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOF, GPIO_Pin_3 | GPIO_Pin_4);
	
	ADC_DeInit(ADC3);  //复位ADC3,将外设 ADC3 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = ADC3CHANNELNUM;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC3, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
		
	ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5); 
	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5);    
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);    
	
	ADC3_DMA_Configuration();
	ADC_DMACmd(ADC3, ENABLE); //使能DMA传输
	ADC_Cmd(ADC3, ENABLE);	//使指定的ADC3
	
	ADC_ResetCalibration(ADC3);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC3));	//等待复位校准结束
	
	ADC_StartCalibration(ADC3);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC3));	 //等待校准结束	

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


