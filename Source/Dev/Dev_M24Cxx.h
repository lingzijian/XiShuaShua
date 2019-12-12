#ifndef DEV_M24C64_H
#define DEV_M24C64_H
#include "stm32f10x.h"
#include "sys.h"
#include "Dev_IIC.h"

#define  E2I2CMODE	  0    //IIC模式 0:模拟IIC 1:硬件IIC

#define E2PI2C IIC1

#define		EEaddress  0xA0 

#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                  /* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_APB2Periph_GPIOB
#define sEE_I2C_SDA_PIN                  GPIO_Pin_7                  /* PB.07 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_APB2Periph_GPIOB

#define sEE_M24C64_32					//32 64
//#define sEE_M24C08						//02 04 08 16


#define sEE_I2C_DMA                      DMA1   
#define sEE_I2C_DMA_CHANNEL_TX           DMA1_Channel6
#define sEE_I2C_DMA_CHANNEL_RX           DMA1_Channel7 
#define sEE_I2C_DMA_FLAG_TX_TC           DMA1_IT_TC6   
#define sEE_I2C_DMA_FLAG_TX_GL           DMA1_IT_GL6 
#define sEE_I2C_DMA_FLAG_RX_TC           DMA1_IT_TC7 
#define sEE_I2C_DMA_FLAG_RX_GL           DMA1_IT_GL7    
#define sEE_I2C_DMA_CLK                  RCC_AHBPeriph_DMA1
#define sEE_I2C_DR_Address               ((uint32_t)0x40005410)
#define sEE_USE_DMA
   
#define sEE_I2C_DMA_TX_IRQn              DMA1_Channel6_IRQn
#define sEE_I2C_DMA_RX_IRQn              DMA1_Channel7_IRQn
//#define sEE_I2C_DMA_TX_IRQHandler        DMA1_Channel6_IRQHandler
//#define sEE_I2C_DMA_RX_IRQHandler        DMA1_Channel7_IRQHandler   
#define sEE_I2C_DMA_PREPRIO              0
#define sEE_I2C_DMA_SUBPRIO              0   
   
#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1   

#define sEE_HW_ADDRESS         0xA0   /* E0 = E1 = E2 = 0 */ 

#define I2C_SPEED               300000
#define I2C_SLAVE_ADDRESS7      0xA0

#if defined (sEE_M24C08)
 #define sEE_PAGESIZE           16
#elif defined (sEE_M24C64_32)
 #define sEE_PAGESIZE           32
#endif


#if defined (sEE_M24C08)
#define sEE_Block0_ADDRESS     0xA0   /* E2 = 0 */ 
 /*#define sEE_Block1_ADDRESS     0xA2*/ /* E2 = 0 */  
 /*#define sEE_Block2_ADDRESS     0xA4*/ /* E2 = 0 */
 /*#define sEE_Block3_ADDRESS     0xA6*/ /* E2 = 0 */
#endif

/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define sEE_FLAG_TIMEOUT         ((uint32_t)0x2000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(10 * sEE_FLAG_TIMEOUT))
/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     256
   
/* Defintions for the state of the DMA transfer */   
#define sEE_STATE_READY           0
#define sEE_STATE_BUSY            1
#define sEE_STATE_ERROR           2
   
#define sEE_OK                    0
#define sEE_FAIL                  1 

/*
void sEE_Init(void);
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
*/

#define	E2Address_Check			0x0004
#define	E2Address_Arg			0x0020

void M24CxxInit(void);
void E2ReadBuff(u8* buff, u16 ReadAddr, u16 len);
void E2WriteBuff(u8* buff, u16 WriteAddr, u16 len);

#endif

