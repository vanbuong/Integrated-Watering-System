#ifndef __DS18B20_H
#define __DS18B20_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "TIMER/TIMER.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
#define OW_GPIO										GPIOC
#define OW_GPIO_CLK								RCC_APB2Periph_GPIOC
#define OW_Pin										GPIO_Pin_6
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define OW_DQ(x)									x?GPIO_SetBits(OW_GPIO, OW_Pin):GPIO_ResetBits(OW_GPIO, OW_Pin)
#define OQ_DQ_Read()							GPIO_ReadInputDataBit(OW_GPIO, OW_Pin)
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
int DS18B20_ReadTemp(void);
/* Private functions ---------------------------------------------------------*/
void OW_SetInput(void);
void OW_SetOutput(void);
unsigned char OW_ReadBit(void);
void OW_WriteBit1(void);
void OW_WriteBit0(void);
unsigned char OW_ReadByte(void);
void OW_WriteByte(unsigned char data);
void OW_Reset(void);
int OW_ReadTemp(void);

#endif
