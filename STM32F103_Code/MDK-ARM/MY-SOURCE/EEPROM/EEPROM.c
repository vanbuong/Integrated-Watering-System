/* Includes ------------------------------------------------------------------*/
#include "EEPROM/EEPROM.h"
#include "TIMER/TIMER.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void EEPROM_Init(void)
{
	I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(EEP_I2C_SCL_GPIO_CLK | EEP_I2C_SDA_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(EEP_I2C_CLK, ENABLE);
	
	/*!< GPIO configuration */  
  /*!< Configure EEP_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = EEP_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(EEP_I2C_SCL_GPIO, &GPIO_InitStructure);
	
	/*!< Configure EEP_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = EEP_I2C_SDA_PIN;
  GPIO_Init(EEP_I2C_SDA_GPIO, &GPIO_InitStructure); 

	/*I2C configuration*/
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = EEP_I2C_ADDRESS;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = EEP_I2C_SPEED;
	
	I2C_Init(EEP_I2C, &I2C_InitStructure);
	I2C_Cmd(EEP_I2C, ENABLE);
}

void EEPROM_WriteByte(uint16_t address, uint8_t data)
{
 /*!< While the bus is busy */
  while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(EEP_I2C, ENABLE);
	
  /*!< Send START condition */
  I2C_GenerateSTART(EEP_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(EEP_I2C, EEP_I2C_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(EEP_I2C, (uint8_t)((address & 0xFF00) >> 8));
	
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(EEP_I2C, (uint8_t)(address & 0x00FF));
	
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, data);
	
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_TXE) == RESET);
	I2C_GenerateSTOP(EEP_I2C, ENABLE);
	_delay_ms(3);
}

uint8_t EEPROM_ReadByte(uint16_t address)
{
//	static uint32_t timeOut = 0;
	uint8_t data;
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(EEP_I2C,ENABLE);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(EEP_I2C, EEP_I2C_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	/*!< Send the EEPROM's internal address to read from: MSB of the address first */
	I2C_SendData(EEP_I2C, (uint8_t)((address & 0xFF00) >> 8));   
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, (uint8_t)(address & 0x00FF));  
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BTF) == RESET);
	
	/*!< Send STRAT condition a second time */  
	I2C_GenerateSTART(EEP_I2C, ENABLE);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(EEP_I2C, EEP_I2C_ADDRESS, I2C_Direction_Receiver);  
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_ADDR) == RESET);
	I2C_AcknowledgeConfig(EEP_I2C, DISABLE);
	(void) EEP_I2C->SR2;
	
	I2C_GenerateSTOP(EEP_I2C, ENABLE);
	
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_RXNE) == RESET);
	data = I2C_ReceiveData(EEP_I2C);
 
	while(EEP_I2C->CR1 & I2C_CR1_STOP);
	
	I2C_AcknowledgeConfig(EEP_I2C, ENABLE);    
	return data;
}

void EEPROM_WriteInt(uint16_t address, uint16_t data)
{
	/*!< While the bus is busy */
  while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(EEP_I2C, ENABLE);
	
  /*!< Send START condition */
  I2C_GenerateSTART(EEP_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(EEP_I2C, EEP_I2C_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(EEP_I2C, (uint8_t)((address & 0xFF00) >> 8));
	
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(EEP_I2C, (uint8_t)(address & 0x00FF));
	
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, data/256);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, data%256);
	
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_TXE) == RESET);
	I2C_GenerateSTOP(EEP_I2C, ENABLE);
	_delay_ms(3);
}

uint16_t EEPROM_ReadInt(uint16_t address)
{
//	static uint32_t timeOut = 0;
	uint8_t dataHigh, dataLow;
	uint16_t data;
	dataHigh = EEPROM_ReadByte(address);
	dataLow = EEPROM_ReadByte(address+1);
	data = (int16_t)dataHigh * 256 + dataLow;
	return data;
}

void EEPROM_WriteLong(uint16_t address, uint32_t data)
{
	/*!< While the bus is busy */
  while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(EEP_I2C, ENABLE);
	
  /*!< Send START condition */
  I2C_GenerateSTART(EEP_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(EEP_I2C, EEP_I2C_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(EEP_I2C, (uint8_t)((address & 0xFF00) >> 8));
	
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(EEP_I2C, (uint8_t)(address & 0x00FF));
	
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, data/65535/256);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, data/65535%256);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, data%65535/256);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, data%65535%256);
	
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_TXE) == RESET);
	I2C_GenerateSTOP(EEP_I2C, ENABLE);
	_delay_ms(3);
}

uint32_t EEPROM_ReadLong(uint16_t address)
{
//	static uint32_t timeOut = 0;
	uint8_t dataHighH, dataHighL, dataLowH, dataLowL;
	uint32_t data;
	dataHighH = EEPROM_ReadByte(address);
	dataHighL = EEPROM_ReadByte(address+1);
	dataLowH = EEPROM_ReadByte(address+2);
	dataLowL = EEPROM_ReadByte(address+3);
	data = (int32_t)((uint16_t)dataHighH * 256 + dataHighL) * 65535 + (uint16_t)dataLowH*256 + dataLowL;
	return data;
}

void EEPROM_WritePage(uint16_t address, uint8_t* data)
{
	uint8_t numData;
	/*!< While the bus is busy */
  while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(EEP_I2C, ENABLE);
	
  /*!< Send START condition */
  I2C_GenerateSTART(EEP_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(EEP_I2C, EEP_I2C_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(EEP_I2C, (uint8_t)((address & 0xFF00) >> 8));
	
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(EEP_I2C, (uint8_t)(address & 0x00FF));
	
  /*!< Test on EV8 and clear it */
	for (numData = 0; numData<64; numData++)
	{
		while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		I2C_SendData(EEP_I2C, data[numData]);
	}
	
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_TXE) == RESET);
	I2C_GenerateSTOP(EEP_I2C, ENABLE);
	//_delay_ms(3);
}
void EEPROM_ReadPage(uint16_t address, uint8_t* data)
{
	uint8_t numData;
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BUSY));
	
	// Enable Acknowledgement, clear POS flag

  I2C_AcknowledgeConfig(EEP_I2C, ENABLE);
  I2C_NACKPositionConfig(EEP_I2C, I2C_NACKPosition_Current);
	
	I2C_GenerateSTART(EEP_I2C,ENABLE);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(EEP_I2C, EEP_I2C_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	/*!< Send the EEPROM's internal address to read from: MSB of the address first */
	I2C_SendData(EEP_I2C, (uint8_t)((address & 0xFF00) >> 8));   
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(EEP_I2C, (uint8_t)(address & 0x00FF));  
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BTF) == RESET);
	
	/*!< Send STRAT condition a second time */  
	I2C_GenerateSTART(EEP_I2C, ENABLE);
	while(!I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(EEP_I2C, EEP_I2C_ADDRESS, I2C_Direction_Receiver);  
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_ADDR) == RESET);
	
	// Start read page data
	(void) EEP_I2C->SR2;
	numData = 64;
	
	while (numData-- != 3)
	{
		// EV7 -- cannot guarantee 1 transfer completion time, wait for BTF 
    //        instead of RXNE

	  while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BTF) == RESET); 
	  *data++ = I2C_ReceiveData(EEP_I2C);
	}
	
	while(I2C_GetFlagStatus(EEP_I2C, I2C_FLAG_BTF) == RESET);  

	// EV7_2 -- Figure 1 has an error, doesn't read N-2 !

	I2C_AcknowledgeConfig(EEP_I2C, DISABLE);           // clear ack bit

	*data++ = I2C_ReceiveData(EEP_I2C);             // receive byte N-2
	I2C_GenerateSTOP(EEP_I2C,ENABLE);                  // program stop

	*data++ = I2C_ReceiveData(EEP_I2C);             // receive byte N-1

	// wait for byte N

	while(I2C_CheckEvent(EEP_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED) == RESET); 
	*data++ = I2C_ReceiveData(EEP_I2C);

	numData = 0;
 
	while(EEP_I2C->CR1 & I2C_CR1_STOP);
	
	I2C_AcknowledgeConfig(EEP_I2C, ENABLE);    
}
/* Private functions ---------------------------------------------------------*/
