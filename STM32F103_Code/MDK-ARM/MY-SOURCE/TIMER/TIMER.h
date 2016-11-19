#ifndef __TIMER_H
#define __TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void TIM2_Init(void);
void TIM2_IRQ_Handler(void);
void TIM2_Setms(unsigned int ms);
uint8_t TIM2_Flag(void);

void TIM7_Init(void);
void TIM7_IRQ_Handler(void);
void TIM7_SetmsFlag1(unsigned int ms);
void TIM7_SetmsFlag2(unsigned int ms);
void TIM7_SetmsFlag3(unsigned int ms);
uint8_t TIM7_Flag1(void);
uint8_t TIM7_Flag2(void);
uint8_t TIM7_Flag3(void);

void delay_ms(uint32_t time);
void _delay_ms(uint32_t time);

void delay_us(unsigned int value);

void SysTick_Init(void);
void SysTick_IRQ_Handler(void);

/* Private functions ---------------------------------------------------------*/

#endif
