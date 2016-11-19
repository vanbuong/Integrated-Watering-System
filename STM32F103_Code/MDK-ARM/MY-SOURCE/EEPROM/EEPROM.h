#ifndef __EEPROM_H
#define __EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EEP_I2C									I2C1
#define EEP_I2C_CLK							RCC_APB1Periph_I2C1

#define EEP_I2C_SCL_GPIO_CLK    RCC_APB2Periph_GPIOB	
#define EEP_I2C_SCL_GPIO			  GPIOB                       /* GPIOB */
#define EEP_I2C_SCL_PIN					GPIO_Pin_6 									/* PB.06 */		

#define EEP_I2C_SDA_GPIO_CLK    RCC_APB2Periph_GPIOB
#define	EEP_I2C_SDA_GPIO   			GPIOB                       /* GPIOB */
#define EEP_I2C_SDA_PIN         GPIO_Pin_7                  /* PB.07 */

#define EEP_I2C_SPEED           400000
#define EEP_I2C_ADDRESS      		0xA0
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void EEPROM_Init(void);
void EEPROM_WriteByte(uint16_t address, uint8_t data);
uint8_t EEPROM_ReadByte(uint16_t address);
void EEPROM_WriteInt(uint16_t address, uint16_t data);
uint16_t EEPROM_ReadInt(uint16_t address);
void EEPROM_WriteLong(uint16_t address, uint32_t data);
uint32_t EEPROM_ReadLong(uint16_t address);
void EEPROM_WritePage(uint16_t address, uint8_t* data);
void EEPROM_ReadPage(uint16_t address, uint8_t* data);
/* Private functions ---------------------------------------------------------*/

#endif
