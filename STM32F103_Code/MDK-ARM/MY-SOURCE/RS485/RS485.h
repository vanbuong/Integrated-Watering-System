#ifndef __RS485_H
#define __RS485_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "UART/UART.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {RS485Transmit = 1, RS485Receive = 0} RS485_Status;

/* Private define ------------------------------------------------------------*/
#define RS485_DirEN_GPIO            		GPIOC
#define RS485_DirEN_GPIO_CLK        		RCC_APB2Periph_GPIOC
#define RS485_DirEN_PIN            			GPIO_Pin_5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void RS485_Init(void);
void RS485_DirEN(RS485_Status RS485_Stt);
void RS485_SendChar(char chr);
void RS485_SendString(char* str);

uint8_t RS485_DataAvailable(void);
uint8_t RS485_GetChar(void);
/* Private functions ---------------------------------------------------------*/

#endif
