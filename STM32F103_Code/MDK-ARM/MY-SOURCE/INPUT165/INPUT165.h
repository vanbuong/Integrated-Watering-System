#ifndef __INPUT165_H
#define __INPUT165_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Output */
// LOAD
#define INPUT165_LOAD_GPIO             	GPIOC
#define INPUT165_LOAD_GPIO_CLK         	RCC_APB2Periph_GPIOC
#define INPUT165_LOAD_Pin            		GPIO_Pin_2

// CLK
#define INPUT165_CLK_GPIO             	GPIOC
#define INPUT165_CLK_GPIO_CLK         	RCC_APB2Periph_GPIOC
#define INPUT165_CLK_Pin            		GPIO_Pin_1

/* Input */
// DATA
#define INPUT165_DATA_GPIO             	GPIOA
#define INPUT165_DATA_GPIO_CLK         	RCC_APB2Periph_GPIOA
#define INPUT165_DATA_Pin            		GPIO_Pin_8

/* Private macro -------------------------------------------------------------*/
#define pin165_LOAD(x)       						x?GPIO_SetBits(INPUT165_LOAD_GPIO, INPUT165_LOAD_Pin):GPIO_ResetBits(INPUT165_LOAD_GPIO, INPUT165_LOAD_Pin);
#define pin165_CLK(x)       						x?GPIO_SetBits(INPUT165_CLK_GPIO, INPUT165_CLK_Pin):GPIO_ResetBits(INPUT165_CLK_GPIO, INPUT165_CLK_Pin);
#define pin165_DATA       							GPIO_ReadInputDataBit(INPUT165_DATA_GPIO, INPUT165_DATA_Pin)

/* Private variables ---------------------------------------------------------*/
extern uint16_t key_code[16];

/* Private function prototypes -----------------------------------------------*/
void Keyboard_Init(void);
void Keyboard_Scan(void);
char Keyboard_Pressed(char chr);
char Keyboard_LongPressed(char chr);
/* Private functions ---------------------------------------------------------*/

void Input165_DelayUs(unsigned int value);
uint16_t Input165_Read(void);

#endif
