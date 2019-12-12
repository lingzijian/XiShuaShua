#include "Dev_IIC.h"

I2C_TypeDef* const IICx[2] = {I2C1, I2C2};
const uint32_t RCC_Periph_IICx[2] = {RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2};
const uint32_t SCL_RCC_APB2Periph_GPIOx[2] = {RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOB};
const uint32_t SDA_RCC_APB2Periph_GPIOx[2] = {RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOB};
GPIO_TypeDef* const SCL_Port[2] = {GPIOB, GPIOB};
GPIO_TypeDef* const SDA_Port[2] = {GPIOB, GPIOB};
const uint16_t SCL_Pin[2] = {GPIO_Pin_6, GPIO_Pin_10};
const uint16_t SDA_Pin[2] = {GPIO_Pin_7, GPIO_Pin_11};

void IICxConfig(IIC iic, u32 Speed)
{
	 /* Initialize the I2C1 according to the I2C_InitStructure members */ 
	I2C_InitTypeDef I2C_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_Periph_IICx[iic], ENABLE);
	RCC_APB2PeriphClockCmd(SCL_RCC_APB2Periph_GPIOx[iic] | SDA_RCC_APB2Periph_GPIOx[iic], ENABLE);

	GPIO_InitStructure.GPIO_Pin = SCL_Pin[iic]; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	//���ÿ�©���
	GPIO_Init(SCL_Port[iic], &GPIO_InitStructure);
	GPIO_SetBits(SCL_Port[iic], SCL_Pin[iic]);
	
	GPIO_InitStructure.GPIO_Pin = SDA_Pin[iic];
	GPIO_Init(SDA_Port[iic], &GPIO_InitStructure);
	GPIO_SetBits(SDA_Port[iic], SDA_Pin[iic]);

	I2C_DeInit(IICx[iic]);
	  /* I2C ���� */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	I2C_InitStructure.I2C_OwnAddress1 = 0x60; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = Speed; 


	I2C_Init(IICx[iic], &I2C_InitStructure);	   
	
	I2C_Cmd(IICx[iic],ENABLE); 
	/*����Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(IICx[iic], ENABLE);
}

static void IIC_Delay1(u16 u)
{
        u16       i, j;

    for (i=0; i<u; i++) 
    {
        for (j=0; j<I2C_DELAY1; j++);
    }
}

static void IIC_Delay2(u16 u)
{
        u16       i, j;

    for (i=0; i<u; i++) 
    {
        for (j=0; j<I2C_DELAY2; j++);
    }
}

void (*IIC_Delay[])(u16) = {IIC_Delay1, IIC_Delay2};

/************************************************************* 
*�������ƣ�I2CStart 
*�������ܣ�I2C��ʼ�ź� 
*��������� 
*��������� 
*��           ע��ʱ���߸�ʱ���������ɸߵ��͵����䣬��ʾI2C��ʼ�ź� 
**************************************************************/ 
void I2CStart(IIC iic) 
{ 
	  GPIO_SetBits( SDA_Port[iic], SDA_Pin[iic] ); 
	  IIC_Delay[iic](1); 
	  GPIO_SetBits( SCL_Port[iic], SCL_Pin[iic] ); 
	  IIC_Delay[iic](1); 
	  GPIO_ResetBits( SDA_Port[iic], SDA_Pin[iic] ); 
	  IIC_Delay[iic](1); 
	  GPIO_ResetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
	  IIC_Delay[iic](1); 
} 

/************************************************************* 
*�������ƣ�I2CStop 
*�������ܣ�I2Cֹͣ�ź� 
*��������� 
*��������� 
*��           ע��ʱ���߸�ʱ���������ɵ͵��ߵ����䣬��ʾI2Cֹͣ�ź� 
**************************************************************/ 
void I2CStop( IIC iic ) 
{ 
	  GPIO_ResetBits( SDA_Port[iic], SDA_Pin[iic] ); 
	  IIC_Delay[iic](1); 
	  GPIO_SetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
	  IIC_Delay[iic](1); 
	  GPIO_SetBits( SDA_Port[iic], SDA_Pin[iic] ); 
	  IIC_Delay[iic](1); 

	  GPIO_ResetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
} 

/************************************************************* 
*�������ƣ�I2CSlaveAck 
*�������ܣ�I2C�ӻ��豸Ӧ���ѯ 
*��������� 
*��������� 
*��           ע�� ����SET˵����Ӧ��
**************************************************************/ 
unsigned char I2CSlaveAck( IIC iic ) 
{ 
	  GPIO_InitTypeDef GPIO_InitStruct; 
	  unsigned int TimeOut; 
	  unsigned char RetValue; 

	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  /*����һ��Ҫ������������������ܶ�������*/ 
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	  GPIO_InitStruct.GPIO_Pin = SDA_Pin[iic]; 
	  GPIO_Init( SDA_Port[iic], &GPIO_InitStruct ); 

	  GPIO_SetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
	  TimeOut = 300; 
	  while( TimeOut--) 
	  { 
	    	if( SET == GPIO_ReadInputDataBit( SDA_Port[iic], SDA_Pin[iic] ) ) 
	    	{ 
	      		RetValue = RESET; 
	    	} 
	    	else 
	    	{ 
	      		RetValue = SET; 
	      		break; 
	    	} 
	  } 
	  GPIO_ResetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
	   
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
	  GPIO_Init(  SDA_Port[iic], &GPIO_InitStruct ); 
	  
	  return RetValue; 
} 

void I2CMasterAck(IIC iic, u8 ack)
{
	if(ack == 1)
		GPIO_ResetBits(  SDA_Port[iic], SDA_Pin[iic] );
	else
		GPIO_SetBits( SDA_Port[iic], SDA_Pin[iic] ); 
	IIC_Delay[iic](1); 
	 GPIO_SetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
	IIC_Delay[iic](1);
	GPIO_ResetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
}

/************************************************************* 
*�������ƣ�I2CWriteByte 
*�������ܣ�I2Cдһ�ֽ����� 
*��������� 
*��������� 
*��           ע�� 
**************************************************************/ 
void I2CWriteByte( IIC iic,unsigned char byte ) 
{ 
	  unsigned char i = 0; 

	  while(i++ < 8) 
	  { 
	    	if( 0X80 & byte ) 
	      		GPIO_SetBits( SDA_Port[iic], SDA_Pin[iic] ); 
	    	else 
	      		GPIO_ResetBits( SDA_Port[iic], SDA_Pin[iic] ); 
	    	byte <<= 1; 
	    	IIC_Delay[iic](1); 

	    	GPIO_SetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
	    	IIC_Delay[iic](2); 
	    	GPIO_ResetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
	    	IIC_Delay[iic](1); 
	  } 
} 

/************************************************************* 
*�������ƣ�I2CReadByte 
*�������ܣ�I2C��һ�ֽ����� 
*��������� 
*��������� 
*��           ע�� 
**************************************************************/ 
unsigned char I2CReadByte( IIC iic ) 
{ 
	unsigned char i; 
	unsigned char ReadValue = 0; 
	GPIO_InitTypeDef GPIO_InitStruct; 
	unsigned char bit; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStruct.GPIO_Pin = SDA_Pin[iic]; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init( SDA_Port[iic], &GPIO_InitStruct ); 
	
	for(i = 0; i < 8; i++)
	{ 
		GPIO_SetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
		IIC_Delay[iic](1); 
		if( SET == GPIO_ReadInputDataBit( SDA_Port[iic], SDA_Pin[iic] ) ) 
		  	bit = 0X01; 
		else 
		  	bit = 0x00; 
		   
		ReadValue = (ReadValue<<1)|bit; 
		GPIO_ResetBits(  SCL_Port[iic], SCL_Pin[iic] ); 
		IIC_Delay[iic](1); 
	} 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init( SDA_Port[iic], &GPIO_InitStruct ); 
	return ReadValue; 
} 


