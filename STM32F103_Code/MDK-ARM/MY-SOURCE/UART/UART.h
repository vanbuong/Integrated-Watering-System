#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "stdio.h"

/* Private typedef -----------------------------------------------------------*/
#define RX1_BUFFER_SIZE 	10
#define RX2_BUFFER_SIZE 	200
#define RX3_BUFFER_SIZE 	200
#define RX4_BUFFER_SIZE 	200
#define RX5_BUFFER_SIZE 	200

typedef struct{
	uint16_t Index_Read;
	uint16_t Index_Write;
	uint16_t Available;
	uint8_t Overflow;
} USART_RxDataTypedef;

/* Private define ------------------------------------------------------------*/
#define USART1_GPIO             GPIOA
#define USART1_GPIO_CLK         RCC_APB2Periph_GPIOA
#define USART1_RxPin            GPIO_Pin_10
#define USART1_TxPin            GPIO_Pin_9

#define USART2_GPIO             GPIOA
#define USART2_GPIO_CLK         RCC_APB2Periph_GPIOA
#define USART2_RxPin            GPIO_Pin_3
#define USART2_TxPin            GPIO_Pin_2

#define USART3_GPIO             GPIOB
#define USART3_GPIO_CLK         RCC_APB2Periph_GPIOB
#define USART3_RxPin            GPIO_Pin_11
#define USART3_TxPin            GPIO_Pin_10

#define UART4_GPIO							GPIOC
#define UART4_GPIO_CLK					RCC_APB2Periph_GPIOC
#define UART4_RxPin							GPIO_Pin_11
#define UART4_TxPin							GPIO_Pin_10

#define UART5_GPIO_Tx						GPIOC
#define UART5_GPIO_Rx						GPIOD
#define UART5_GPIO_Tx_CLK				RCC_APB2Periph_GPIOC
#define UART5_GPIO_Rx_CLK				RCC_APB2Periph_GPIOD
#define UART5_RxPin							GPIO_Pin_2
#define UART5_TxPin							GPIO_Pin_12

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void USART1_IRQ_Handler(void);
void USART1_Init(void);
void USART1_SendChar(char chr);
void USART1_SendString(char* str);
void USART1_SendNum(int num);
uint8_t USART1_DataAvailable(void);
uint8_t USART1_numDataAvailable(void);
uint8_t USART1_GetChar(void);

void USART2_IRQ_Handler(void);
void USART2_Init(void);
void USART2_SendChar(char chr);
void USART2_SendString(char* str);
void USART2_SendNum(int num);
uint8_t USART2_DataAvailable(void);
uint8_t USART2_numDataAvailable(void);
uint8_t USART2_GetChar(void);

void USART3_IRQ_Handler(void);
void USART3_Init(void);
void USART3_SendChar(char chr);
void USART3_SendString(char* str);
void USART3_SendNum(int num);
uint8_t USART3_DataAvailable(void);
uint8_t USART3_numDataAvailable(void);
uint8_t USART3_GetChar(void);

void UART4_IRQ_Handler(void);
void UART4_Init(void);
void UART4_SendChar(char chr);
void UART4_SendString(char* str);
void UART4_SendNum(int num);
uint8_t UART4_DataAvailable(void);
uint8_t UART4_numDataAvailable(void);
uint8_t UART4_GetChar(void);

void UART5_IRQ_Handler(void);
void UART5_Init(void);
void UART5_SendChar(char chr);
void UART5_SendString(char* str);
void UART5_SendNum(int num);
uint8_t UART5_DataAvailable(void);
uint8_t UART5_numDataAvailable(void);
uint8_t UART5_GetChar(void);


void USART1_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3);
void USART2_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3);
void USART3_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3);
void UART4_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3);
void UART5_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3);

/* Private functions ---------------------------------------------------------*/
unsigned long usart_pow_of(int A, int n);

#endif
