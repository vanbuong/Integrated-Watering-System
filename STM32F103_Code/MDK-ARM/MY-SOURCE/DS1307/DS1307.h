/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DS1307_H_
#define __DS1307_H_

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/**
  * @brief  I2C DS1307 Interface pins
  */  
#define DS1307_I2C									I2C1
#define DS1307_I2C_CLK							RCC_APB1Periph_I2C1
#define DS1307_I2C_SCL_PIN					GPIO_Pin_6 									/* PB.06 */			
#define DS1307_I2C_SCL_GPIO_PORT   	GPIOB                       /* GPIOB */
#define DS1307_I2C_SCL_GPIO_CLK    	RCC_APB2Periph_GPIOB
#define DS1307_I2C_SDA_PIN         	GPIO_Pin_7                  /* PB.07 */
#define	DS1307_I2C_SDA_GPIO_PORT   	GPIOB                       /* GPIOB */
#define DS1307_I2C_SDA_GPIO_CLK    	RCC_APB2Periph_GPIOB
	

#define DS1307_I2C_SPEED            400000
#define DS1307_I2C_ADDRESS      		0xD0
#define DS1307_I2C_OWNADDRESS1			0xFF

extern uint8_t hour, minute, second;
extern uint8_t day;
extern uint8_t date, month, year;

#define ADDRESS_SECOND		0x00
#define ADDRESS_MINUTE		0x01
#define ADDRESS_HOUR			0x02
#define ADDRESS_DAY				0x03
#define ADDRESS_DATE			0x04
#define ADDRESS_MONTH			0x05
#define ADDRESS_YEAR			0x06

void DS1307_Init(void);
void write_ds1307(uint8_t address, uint8_t data);
uint8_t read_ds1307(uint8_t address);
void SetupTimeForRealTime(void);
void ReadTime(void);
void DisplayTime(void);
void DisplayTimeOnLCD1602(void);
uint8_t isUpButton(void);
uint8_t isDownButton(void);
uint8_t isNextButton(void);
uint8_t isPreviousButton(void);
uint8_t isSwitchButton(void);
uint8_t isBackButton(void);
uint8_t isEnterButton(void);
uint8_t isSaveButton(void);
uint8_t isLeapYear(int8_t year);

#endif
