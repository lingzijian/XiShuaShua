#include "Dev_M24Cxx.h"
#include "delay.h"

__IO uint16_t  sEEAddress = 0;   
__IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;   
__IO uint16_t* sEEDataReadPointer;   
__IO uint8_t*  sEEDataWritePointer;  
__IO uint8_t   sEEDataNum;

DMA_InitTypeDef	sEEDMA_InitStructure; 




/**
  * @brief  Start critical section: these callbacks should be typically used
  *         to disable interrupts when entering a critical section of I2C communication
  *         You may use default callbacks provided into this driver by uncommenting the 
  *         define USE_DEFAULT_CRITICAL_CALLBACK.
  *         Or you can comment that line and implement these callbacks into your 
  *         application.
  * @param  None.
  * @retval None.
  */
void sEE_EnterCriticalSection_UserCallback(void)
{
  __disable_irq();  
}

/**
  * @brief  Start and End of critical section: these callbacks should be typically used
  *         to re-enable interrupts when exiting a critical section of I2C communication
  *         You may use default callbacks provided into this driver by uncommenting the 
  *         define USE_DEFAULT_CRITICAL_CALLBACK.
  *         Or you can comment that line and implement these callbacks into your 
  *         application.
  * @param  None.
  * @retval None.
  */
void sEE_ExitCriticalSection_UserCallback(void)
{
  __enable_irq();
}


/** @defgroup STM32_EVAL_I2C_EE_Private_Functions
  * @{
  */ 
void sEE_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;  
  
    
  /*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_APB2PeriphClockCmd(sEE_I2C_SCL_GPIO_CLK | sEE_I2C_SDA_GPIO_CLK, ENABLE);

  /*!< sEE_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, ENABLE);
    
  /*!< GPIO configuration */  
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 

  /* Configure and enable I2C DMA TX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);  
  
  /*!< I2C DMA TX and RX channels configuration */
  /* Enable the DMA clock */
  RCC_AHBPeriphClockCmd(sEE_I2C_DMA_CLK, ENABLE);

  /* I2C TX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX);
  sEEDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)sEE_I2C_DR_Address;
  sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;   /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_BufferSize = 0xFFFF;            /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  sEEDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  sEEDMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  sEEDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  sEEDMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(sEE_I2C_DMA_CHANNEL_TX, &sEEDMA_InitStructure);  
  
  /* I2C RX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX);
  DMA_Init(sEE_I2C_DMA_CHANNEL_RX, &sEEDMA_InitStructure);  
  
  /* Enable the DMA Channels Interrupts */
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_RX, DMA_IT_TC, ENABLE);    
}


/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;    
   
  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(sEE_I2C, DISABLE);
 
  /* sEE_I2C DeInit */
  I2C_DeInit(sEE_I2C);

  /*!< sEE_I2C Periph clock disable */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, DISABLE);
    
  /*!< GPIO configuration */  
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure and enable I2C DMA TX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);   
  
  /* Disable and Deinitialize the DMA channels */
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, DISABLE);
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX, DISABLE);
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX);
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX);
}

/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_DeInit(void)
{
  sEE_LowLevel_DeInit(); 
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_Init(void)
{ 
  I2C_InitTypeDef  I2C_InitStructure;
  sEE_DeInit();
  sEE_LowLevel_Init();
  
  /*!< I2C configuration */
  /* sEE_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
  /* sEE_I2C Peripheral Enable */
  I2C_Cmd(sEE_I2C, ENABLE);
  /* Apply sEE_I2C configuration after enabling it */
  I2C_Init(sEE_I2C, &I2C_InitStructure);

  /* Enable the sEE_I2C peripheral DMA requests */
  I2C_DMACmd(sEE_I2C, ENABLE);
  
#if defined (sEE_M24C64_32)
  /*!< Select the EEPROM address according to the state of E0, E1, E2 pins */
  sEEAddress = sEE_HW_ADDRESS;  
#elif defined (sEE_M24C08)
  /*!< depending on the sEE Address selected in the i2c_ee.h file */
 #ifdef sEE_Block0_ADDRESS
  /*!< Select the sEE Block0 to write on */
  sEEAddress = sEE_Block0_ADDRESS;
 #endif
  
 #ifdef sEE_Block1_ADDRESS
  /*!< Select the sEE Block1 to write on */
  sEEAddress = sEE_Block1_ADDRESS;
 #endif

 #ifdef sEE_Block2_ADDRESS
  /*!< Select the sEE Block2 to write on */
  sEEAddress = sEE_Block2_ADDRESS;
 #endif
  
 #ifdef sEE_Block3_ADDRESS
  /*!< Select the sEE Block3 to write on */
  sEEAddress = sEE_Block3_ADDRESS;
 #endif 
#endif /*!< sEE_M24C64_32 */    
}


/**
  * @brief  Wait for EEPROM Standby state.
  * 
  * @note  This function allows to wait and check that EEPROM has finished the 
  *        last Write operation. It is mostly used after Write operation: after 
  *        receiving the buffer to be written, the EEPROM may need additional 
  *        time to actually perform the write operation. During this time, it 
  *        doesn't answer to I2C packets addressed to it. Once the write operation 
  *        is complete the EEPROM responds to its address.
  *        
  * @note  It is not necessary to call this function after sEE_WriteBuffer() 
  *        function (sEE_WriteBuffer() already calls this function after each
  *        write page operation).    
  * 
  * @param  None
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t sEE_WaitEepromStandbyState(void)      
{
  __IO uint16_t tmpSR1 = 0;
  __IO uint32_t sEETrials = 0;

  /*!< While the bus is busy */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_FAIL;//sEE_TIMEOUT_UserCallback();
  }

  /* Keep looping till the slave acknowledge his address or maximum number 
     of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER define
     in stm32_eval_i2c_ee.h file) */
  while (1)
  {
    /*!< Send START condition */
    I2C_GenerateSTART(sEE_I2C, ENABLE);

    /*!< Test on EV5 and clear it */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((sEETimeout--) == 0) return sEE_FAIL;//sEE_TIMEOUT_UserCallback();
    }    

    /*!< Send EEPROM address for write */
    I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
    
    /* Wait for ADDR flag to be set (Slave acknowledged his address) */
    sEETimeout = sEE_LONG_TIMEOUT;
    do
    {     
      /* Get the current value of the SR1 register */
      tmpSR1 = sEE_I2C->SR1;
      
      /* Update the timeout value and exit if it reach 0 */
      if((sEETimeout--) == 0) return sEE_FAIL;//sEE_TIMEOUT_UserCallback();
    }
    /* Keep looping till the Address is acknowledged or the AF flag is 
       set (address not acknowledged at time) */
    while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
     
    /* Check if the ADDR flag has been set */
    if (tmpSR1 & I2C_SR1_ADDR)
    {
      /* Clear ADDR Flag by reading SR1 then SR2 registers (SR1 have already 
         been read) */
      (void)sEE_I2C->SR2;
      
      /*!< STOP condition */    
      I2C_GenerateSTOP(sEE_I2C, ENABLE);
        
      /* Exit the function */
      return sEE_OK;
    }
    else
    {
      /*!< Clear AF flag */
      I2C_ClearFlag(sEE_I2C, I2C_FLAG_AF);                  
    }
    
    /* Check if the maximum allowed numbe of trials has bee reached */
    if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return sEE_FAIL;//sEE_TIMEOUT_UserCallback();
    }
  }
}

/**
  * @brief  Initializes DMA channel used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction)
{ 
  /* Initialize the DMA with the new parameters */
  if (Direction == sEE_DIRECTION_TX)
  {
    /* Configure the DMA Tx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;  
    DMA_Init(sEE_I2C_DMA_CHANNEL_TX, &sEEDMA_InitStructure);  
  }
  else
  { 
    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;      
    DMA_Init(sEE_I2C_DMA_CHANNEL_RX, &sEEDMA_InitStructure);    
  }
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to start reading from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to 
  *         be read from the EEPROM.
  * 
  *        @note The variable pointed by NumByteToRead is reset to 0 when all the 
  *              data are read from the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note When number of data to be read is higher than 1, this function just 
  *       configures the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be read is 1, then the DMA is not used. The byte
  *       is read in polling mode.
  * 
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
void sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{
    if(NumByteToRead == 0)
		return;	
	while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY));
			
	I2C_AcknowledgeConfig(sEE_I2C, ENABLE);

    I2C_GenerateSTART(sEE_I2C, ENABLE);
	sEETimeout = sEE_LONG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
    	if(sEETimeout-- == 0) return;
    }
	
#ifdef sEE_M24C64_32	
    I2C_Send7bitAddress(sEE_I2C,  sEEAddress, I2C_Direction_Transmitter);
	sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
    	if(sEETimeout-- == 0) return;
    }

	I2C_SendData(sEE_I2C, (ReadAddr&0xFF00)>>8);
	sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
    	if(sEETimeout-- == 0) return;
    }
	I2C_SendData(sEE_I2C, (ReadAddr&0x00FF));
	sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
    	if(sEETimeout-- == 0) return;
    }
		
	I2C_GenerateSTART(sEE_I2C, ENABLE);
	sEETimeout = sEE_LONG_TIMEOUT;
	while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
    	if(sEETimeout-- == 0) return;
    }
	
	I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Receiver);
	sEETimeout = sEE_LONG_TIMEOUT;
	while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
    	if(sEETimeout-- == 0) return;
    }
	
#else
	I2C_Send7bitAddress(sEE_I2C,  sEEAddress + ((ReadAddr / 0x100) << 1), I2C_Direction_Transmitter);
	sEETimeout = sEE_LONG_TIMEOUT;
	while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
    	if(sEETimeout-- == 0) return;
    }

	I2C_SendData(sEE_I2C, (ReadAddr % 0x100));
	sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
    	if(sEETimeout-- == 0) return;
    }
	
	I2C_GenerateSTART(sEE_I2C, ENABLE);
	sEETimeout = sEE_LONG_TIMEOUT;
	while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
    	if(sEETimeout-- == 0) return;
    }
	
	I2C_Send7bitAddress(sEE_I2C, sEEAddress + ((ReadAddr / 0x100) << 1), I2C_Direction_Receiver);
	sEETimeout = sEE_LONG_TIMEOUT;
	while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
    	if(sEETimeout-- == 0) return;
    }
#endif
	
    while (NumByteToRead)
    {
		if(NumByteToRead == 1)
		{
     		I2C_AcknowledgeConfig(sEE_I2C, DISABLE);	
    		I2C_GenerateSTOP(sEE_I2C, ENABLE);			
		}
	    sEETimeout = sEE_LONG_TIMEOUT;
		while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED));
		{
    		if(sEETimeout-- == 0) return;
   		}
	    *pBuffer = I2C_ReceiveData(sEE_I2C);
	    pBuffer++;
	    /* Decrement the read bytes counter */
	    NumByteToRead--;
    }
	I2C_AcknowledgeConfig(sEE_I2C, ENABLE);
}





/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  *
  * @note   The number of bytes (combined to write start address) must not 
  *         cross the EEPROM page boundary. This function can only write into
  *         the boundaries of an EEPROM page.
  *         This function doesn't check on boundaries condition (in this driver 
  *         the function sEE_WriteBuffer() which calls sEE_WritePage() is 
  *         responsible of checking on Page boundaries).
  * 
  * @param  pBuffer : pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : pointer to the variable holding number of bytes to 
  *         be written into the EEPROM. 
  * 
  *        @note The variable pointed by NumByteToWrite is reset to 0 when all the 
  *              data are written to the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note This function just configure the communication and enable the DMA 
  *       channel to transfer data. Meanwhile, the user application may perform 
  *       other tasks in parallel.
  * 
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{ 
  /* Set the pointer to the Number of data to be written. This pointer will be used 
      by the DMA Transfer Completer interrupt Handler in order to reset the 
      variable to 0. User should check on this variable in order to know if the 
      DMA transfer has been complete or not. */
  sEEDataWritePointer = NumByteToWrite;  
  
  /*!< While the bus is busy */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) 
		return sEE_FAIL;// sEE_TIMEOUT_UserCallback();
  }
  
  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) 
		return sEE_FAIL;// sEE_TIMEOUT_UserCallback();
  }
  
  /*!< Send EEPROM address for write */
  sEETimeout = sEE_FLAG_TIMEOUT;
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) 
		return sEE_FAIL;// sEE_TIMEOUT_UserCallback();
  }

#ifdef sEE_M24C08
  
  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(sEE_I2C, WriteAddr);
  
#elif defined(sEE_M24C64_32)
  
  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;  
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((sEETimeout--) == 0) 
		return sEE_FAIL;// sEE_TIMEOUT_UserCallback();
  }  
  
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /*!< sEE_M24C08 */  
  
  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT; 
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((sEETimeout--) == 0) return sEE_FAIL;// sEE_TIMEOUT_UserCallback();
  }  
  
  /* Configure the DMA Tx Channel with the buffer address and the buffer size */
  sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint8_t)(*NumByteToWrite), sEE_DIRECTION_TX);
    
  /* Enable the DMA Tx Channel */
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, ENABLE);
  
  /* If all operations OK, return sEE_OK (0) */
  return sEE_OK;
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % sEE_PAGESIZE; //页内偏移地址
  count = sEE_PAGESIZE - Addr;     
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
 
  /*!< If WriteAddr is sEE_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage == 0) 
    {
      /* Store the number of data to be written */
      sEEDataNum = NumOfSingle;
      /* Start writing data */
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
      /* Wait transfer through DMA to be complete */
      sEETimeout = sEE_LONG_TIMEOUT;
      while (sEEDataNum > 0)
      {
        if((sEETimeout--) == 0) 
			return;//{sEE_TIMEOUT_UserCallback(); return;};
      }
      sEE_WaitEepromStandbyState();
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;        
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) 
		  	return;//{sEE_TIMEOUT_UserCallback(); return;};
        }      
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) 
		  	return;//{sEE_TIMEOUT_UserCallback(); return;};
        }    
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
  else 
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /*!< If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;        
        /*!< Write the data conained in same page */
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) 
		  	return;//{sEE_TIMEOUT_UserCallback(); return;};
        }          
        sEE_WaitEepromStandbyState();      
        
        /* Store the number of data to be written */
        sEEDataNum = (NumByteToWrite - count);          
        /*!< Write the remaining data in the following page */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) return;//{sEE_TIMEOUT_UserCallback(); return;};
        }     
        sEE_WaitEepromStandbyState();        
      }      
      else      
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) return;//{sEE_TIMEOUT_UserCallback(); return;};
        }          
        sEE_WaitEepromStandbyState();        
      }     
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
      
      if(count != 0)
      {  
        /* Store the number of data to be written */
        sEEDataNum = count;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) 
		  	return;//{sEE_TIMEOUT_UserCallback(); return;};
        }     
        sEE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) 
		  	return;//{sEE_TIMEOUT_UserCallback(); return;};
        }        
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;           
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) 
		  	return;//{sEE_TIMEOUT_UserCallback(); return;};
        }         
        sEE_WaitEepromStandbyState();
      }
    }
  }  
}


/**
  * @brief  This function handles the DMA Tx Channel interrupt Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel6_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_FLAG_TX_TC) != RESET)
  {  
    /* Disable the DMA Tx Channel and Clear all its Flags */  
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_FLAG_TX_GL);

    /*!< Wait till all data have been physically transferred on the bus */
    sEETimeout = sEE_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF))
    {
      if((sEETimeout--) == 0) break;
    }
    
    /*!< Send STOP condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);
    
    /* Reset the variable holding the number of data to be written */
    *sEEDataWritePointer = 0;  
  }
}


/**
  * @brief  This function handles the DMA Rx Channel interrupt Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel7_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_FLAG_RX_TC) != RESET)
  {      
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);    
    
    /* Disable the DMA Rx Channel and Clear all its Flags */  
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_FLAG_RX_GL);
    
    /* Reset the variable holding the number of data to be read */
    *sEEDataReadPointer = 0;
  }
}

void M24CxxInit(void)
{
	#if (E2I2CMODE == 1)
	sEE_Init();
	#else
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
	#endif
}

void E2ReadBuff(u8* buff, u16 ReadAddr, u16 len)
{
	u8 err;
	#if (E2I2CMODE == 1)
	sEE_ReadBuffer(buff, ReadAddr, len);
	#else
	u16 i = 0;
	__disable_irq();
	do
	{
		err = 0;
		I2CStart(E2PI2C);
		#ifdef  sEE_M24C08
		I2CWriteByte(E2PI2C, EEaddress + ((ReadAddr / 0x100) << 1));
		if( RESET == I2CSlaveAck(E2PI2C) ) 
			{err = 1; continue;}
		I2CWriteByte(E2PI2C, ReadAddr % 0x100);
		#elif defined (sEE_M24C64_32)
		I2CWriteByte(E2PI2C, EEaddress);
		if( RESET == I2CSlaveAck(E2PI2C) ) 
			{err = 1; continue;} 
		I2CWriteByte(E2PI2C, ReadAddr >> 8);
		if( RESET == I2CSlaveAck(E2PI2C) ) 
			{err = 1; continue;}
		I2CWriteByte(E2PI2C, ReadAddr & 0x00FF);
		#else
		#error "请选择正确的配置"
		#endif
	}while((err == 1) && (i++ < 20));
	if(i >= 20)
		goto stop; 
	if( RESET == I2CSlaveAck(E2PI2C) ) 
		goto stop; 
	I2CStart(E2PI2C);
	#ifdef  sEE_M24C08
	I2CWriteByte(E2PI2C, EEaddress + 1 + ((ReadAddr / 0x100) << 1));
	#elif defined (sEE_M24C64_32)
	I2CWriteByte(E2PI2C, EEaddress + 1);
	#endif
	if( RESET == I2CSlaveAck(E2PI2C) ) 
		goto stop; 
	for(i = 0; i < len; i++)
	{
		buff[i] = I2CReadByte(E2PI2C);
		if(i < len - 1)
			I2CMasterAck(E2PI2C, 1);
		else
			I2CMasterAck(E2PI2C, 0);
	}
	stop:I2CStop(E2PI2C); 
	__enable_irq();
	#endif
}

u8 ByteWrite(u16 WriteAddr, u8 Byte)
{
	u8 err = 0;
	I2CStart(E2PI2C);
	#ifdef sEE_M24C08
	I2CWriteByte(E2PI2C, EEaddress + ((WriteAddr / 0x100) << 1));
	if( RESET == I2CSlaveAck(E2PI2C) ) {err = 1; goto stop;} 
	I2CWriteByte(E2PI2C, WriteAddr % 0x100);
	#elif defined (sEE_M24C64_32)
	I2CWriteByte(E2PI2C, EEaddress);
	if( RESET == I2CSlaveAck(E2PI2C) ) {err = 1; goto stop;} 
	I2CWriteByte(E2PI2C, WriteAddr >> 8);
	if( RESET == I2CSlaveAck(E2PI2C) ) {err = 1; goto stop;} 
	I2CWriteByte(E2PI2C, WriteAddr & 0x00FF);
	#else
	#error "请选择正确的配置"
	#endif
	if( RESET == I2CSlaveAck(E2PI2C) ) {err = 1; goto stop;} 

	I2CWriteByte(E2PI2C, Byte);
	I2CSlaveAck(E2PI2C);
		
	stop:I2CStop(E2PI2C); 
	
	if(err)
		return RESET;
	else
		return SET;
}


u8 PageWrite(u8* buff, u16 WriteAddr, u16 len)
{
	u16 i = 0;
	u8 err = 0;
	__disable_irq();
	do{
		err = 0;
		I2CStart(E2PI2C);
		#ifdef sEE_M24C08
		I2CWriteByte(E2PI2C, EEaddress + ((WriteAddr / 0x100) << 1));
		if( RESET == I2CSlaveAck(E2PI2C) ) {
			err = 1; continue;} 
		I2CWriteByte(E2PI2C, WriteAddr % 0x100);
		#elif defined (sEE_M24C64_32)
		I2CWriteByte(E2PI2C, EEaddress);
		if( RESET == I2CSlaveAck(E2PI2C) ) {
			err = 1; continue;} 
		I2CWriteByte(E2PI2C, WriteAddr >> 8);
		if( RESET == I2CSlaveAck(E2PI2C) ) {
			err = 1;continue;}
		I2CWriteByte(E2PI2C, WriteAddr & 0x00FF);
		#else
		#error "请选择正确的配置"
		#endif
	}while((err == 1) && (i++ < 20));
		
	if(i >= 20)
		goto stop;
	
	if( RESET == I2CSlaveAck(E2PI2C) ) {
		err = 1; goto stop;} 
	i = 0;
	while(i < len)
	{
		I2CWriteByte(E2PI2C, buff[i++]);
		I2CSlaveAck(E2PI2C);
	}
	stop:I2CStop(E2PI2C); 
	__enable_irq();
	
	if(err)
		return RESET;
	else
		return SET;
}

#define PAGESIZE 	sEE_PAGESIZE


void E2WriteBuff(u8* buff, u16 WriteAddr, u16 len)
{
	#if (E2I2CMODE == 1)
	sEE_WriteBuffer(buff, WriteAddr, len);
	#else
	u16 i;
	
	u16 writed = 0;
	u16 pageNum = len / PAGESIZE + 1;
	s16 writeSize;
	
	for(i = 0; i < pageNum; i++)
	{
		
		writeSize = PAGESIZE - (WriteAddr % PAGESIZE);
		if(writeSize +  writed > len)
		{
			writeSize = len - writed;
		}
		if(PageWrite((u8*)(buff + writed), WriteAddr, writeSize) == RESET)
			break;
		writed += writeSize;
		WriteAddr += writeSize;
		delay_ms(20);
	}
	#endif
}



