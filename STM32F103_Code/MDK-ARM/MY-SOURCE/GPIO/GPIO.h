#ifndef __GPIO_H
#define __GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BTN1										1
#define BTN2										2

/* Output */
// LED
#define LED_GPIO             		GPIOB
#define LED_GPIO_CLK         		RCC_APB2Periph_GPIOB
#define LED_Pin1            		GPIO_Pin_0
#define LED_Pin2            		GPIO_Pin_1

/* Input */
// BTN
#define BTN_GPIO_1             	GPIOB
#define BTN_GPIO_CLK_1         	RCC_APB2Periph_GPIOB
#define BTN_Pin1            		GPIO_Pin_2

#define BTN_GPIO_2             	GPIOC
#define BTN_GPIO_CLK_2         	RCC_APB2Periph_GPIOC
#define BTN_Pin2            		GPIO_Pin_13

/* Private macro -------------------------------------------------------------*/
#define LED1(x)       					x?GPIO_SetBits(LED_GPIO, LED_Pin1):GPIO_ResetBits(LED_GPIO, LED_Pin1);
#define LED2(x)      						x?GPIO_SetBits(LED_GPIO, LED_Pin2):GPIO_ResetBits(LED_GPIO, LED_Pin2);


/* Private variables ---------------------------------------------------------*/
/* Single BUTTON */
extern uint32_t btn_code[2];

/* Private function prototypes -----------------------------------------------*/
void GPIOx_Init(void);
void GPIO_ToogleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void Button_Scan(void);
char Button_Pressed(char chr);
char Button_LongPressed(char chr);

/* Private functions ---------------------------------------------------------*/

#endif
