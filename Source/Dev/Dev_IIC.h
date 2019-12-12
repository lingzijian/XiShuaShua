#ifndef DEV_IIC_H
#define DEV_IIC_H
#include "stm32f10x.h"
typedef enum{
	IIC1 = 0,
	IIC2 = 1
	}IIC;

#define I2C_DELAY1  5
#define I2C_DELAY2  20

	
void IICxConfig(IIC iic, u32 Speed);
extern I2C_TypeDef* const IICx[2];
extern const uint32_t SCL_RCC_APB2Periph_GPIOx[2];
extern const uint32_t SDA_RCC_APB2Periph_GPIOx[2];
extern GPIO_TypeDef* const SCL_Port[2];
extern GPIO_TypeDef* const SDA_Port[2];
extern const uint16_t SCL_Pin[2];
extern const uint16_t SDA_Pin[2];

void I2CStart(IIC iic) ;
void I2CStop( IIC iic ) ;
unsigned char I2CSlaveAck( IIC iic ) ;
void I2CMasterAck(IIC iic, u8 ack);
void I2CWriteByte( IIC iic,unsigned char byte ) ;
unsigned char I2CReadByte( IIC iic ) ;






#endif

