#ifndef __BLEHM10_H
#define __BLEHM10_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "TIMER/TIMER.h"
#include "UART/UART.h"
#include "LCD/LCD.h"
#include "GPIO/GPIO.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	uint16_t Index_Read;
	uint16_t Index_Write;
	uint16_t Available;
	uint8_t Overflow;
} BLE_RxDataTypedef;

/* Private define ------------------------------------------------------------*/
#define BLE_DATA_BFRSIZE 								100
#define BLE_LCD_SUPPORT									1

#define BLE_UART_CHANNEL								USART2
#define BLE_UART_CLOCK									RCC_APB1Periph_USART2
#define BLE_UART_IRQn										USART2_IRQn

#define BLE_TIMER_CHANNEL								TIM3
#define BLE_TIMER_CLOCK									RCC_APB1Periph_TIM3
#define BLE_TIMER_IRQn									TIM3_IRQn

#define BLE_GPIO_Tx											GPIOA
#define BLE_GPIO_Rx											GPIOA
#define BLE_GPIO_Tx_CLK									RCC_APB2Periph_GPIOA
#define BLE_GPIO_Rx_CLK									RCC_APB2Periph_GPIOA
#define BLE_TxPin												GPIO_Pin_2
#define BLE_RxPin												GPIO_Pin_3

#define BLESTT_DEAD											200
#define BLESTT_POWER_ON									201

#define BLESTT_BUSY											202
#define BLESTT_AVAILABLE								203
#define BLESTT_CHECK_ALIVE							204

//Your Application
#define BLESTT_EXAMPLE_REQUEST					205
#define BLESTT_EXAMPLE_DATA_PROCESS			206
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern char BLE_STATUS;
extern char BLE_DATA[BLE_DATA_BFRSIZE];
/* Private function prototypes -----------------------------------------------*/
void BLEHM10_Init(void);
void BLEHM10_SendChar(char chr);
void BLEHM10_SendString(char* str);
void BLEHM10_SendNum(int num);
char BLEHM10_SendCommand(char* AT_CMD, char* Result, unsigned int Timeout);

void BLEHM10_CheckAlive(void);

uint16_t BLEHM10_GetLengthDataFrom(char* data);

uint16_t BLEHM10_DataAvailable(void);
char BLEHM10_isCorrectResult(char* Result, unsigned int Timeout);
char* BLEHM10_ReceiveData(void);

void BLEHM10_DATA_IRQHandler(void);
void BLEHM10_TIMER_IRQHandler(void);

void BLEHM10_SetTimer1(unsigned int ms);
char BLEHM10_isTimer1OVF(void);

void BLEHM10_SetTimer2(unsigned int ms);
char BLEHM10_isTimer2OVF(void);

/* Private functions ---------------------------------------------------------*/
void BLE_UartInit(void);
void BLE_TimerInit(void);
unsigned long BLE_pow_of(int A, int n);
unsigned char BLE_receive_num(char sms);
#endif
