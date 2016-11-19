#ifndef __SIM900_H
#define __SIM900_H

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
} SIM_RxDataTypedef;

/* Private define ------------------------------------------------------------*/
#define SIM_DATA_BFRSIZE 								1000
#define SIM_SMS_BFRSIZE 								25
#define SIM_LCD_SUPPORT									1

#define LINK_HOCHIMINH									"AT+HTTPPARA=\"URL\",\"http://api.worldweatheronline.com/premium/v1/tz.ashx?key=fb281d1c03354774b5e153314162705&q=HoChiMinh&format=json\"\r\n"
#define LINK_TOKYO											"AT+HTTPPARA=\"URL\",\"http://api.worldweatheronline.com/premium/v1/tz.ashx?key=fb281d1c03354774b5e153314162705&q=Tokyo&format=json\"\r\n"
#define LINK_NEWYORK										"AT+HTTPPARA=\"URL\",\"http://api.worldweatheronline.com/premium/v1/tz.ashx?key=fb281d1c03354774b5e153314162705&q=Newyork&format=json\"\r\n"
#define LINK_LONDON											"AT+HTTPPARA=\"URL\",\"http://api.worldweatheronline.com/premium/v1/tz.ashx?key=fb281d1c03354774b5e153314162705&q=London&format=json\"\r\n"

#define SIM_UART_CHANNEL								UART5
#define SIM_UART_CLOCK									RCC_APB1Periph_UART5
#define SIM_UART_IRQn										UART5_IRQn

#define SIM_TIMER_CHANNEL								TIM5
#define SIM_TIMER_CLOCK									RCC_APB1Periph_TIM5
#define SIM_TIMER_IRQn									TIM5_IRQn


#define SIM_PWRKEY_GPIO									GPIOC
#define SIM_PWRKEY_GPIO_CLK							RCC_APB2Periph_GPIOC
#define SIM_PWRKEY_Pin									GPIO_Pin_5

#define SIM_GPIO_Tx											GPIOC
#define SIM_GPIO_Rx											GPIOD
#define SIM_GPIO_Tx_CLK									RCC_APB2Periph_GPIOC
#define SIM_GPIO_Rx_CLK									RCC_APB2Periph_GPIOD
#define SIM_TxPin												GPIO_Pin_12
#define SIM_RxPin												GPIO_Pin_2

#define SIMSTT_DEAD											0
#define SIMSTT_POWER_ON									1

#define SIMSTT_BUSY											2
#define SIMSTT_AVAILABLE								3

#define SIMSTT_MAKE_CALL								4
#define SIMSTT_END_CALL									5

#define SIMSTT_CHECK_ALIVE							6
#define SIMSTT_GPRS_NETWORK_REG					7
#define SIMSTT_GPRS_DET_ATT_FROM				8
#define SIMSTT_GPRS_BEARER_CONFIG				9
#define SIMSTT_GPRS_OPEN_CONTEXT				10
#define SIMSTT_GPRS_HTTP_INIT						11
#define SIMSTT_GPRS_HTTP_PARAM					12
#define SIMSTT_GPRS_OK									13
#define SIMSTT_GPRS_HTTP_SEND_URL				14
#define SIMSTT_GPRS_HTTP_ACTION					15
#define SIMSTT_GPRS_HTTP_READ						16

#define SIMSTT_SMS_TEXT_MODE						17
#define SIMSTT_SMS_OK										18	
#define SIMSTT_SMS_READ									19
#define SIMSTT_SMS_DELETE								20
							

//Your Application
#define SIMSTT_GPRS_EXAMPLE_REQUEST			21
#define SIMSTT_GPRS_EXAMPLE_PROCESS			22

#define SIMSTT_SMS_EXAMPLE_REQUEST			23
#define SIMSTT_SMS_EXAMPLE_CHECK				24
#define SIMSTT_SMS_EXAMPLE_READ					25

/* Private macro -------------------------------------------------------------*/
#define SIM900_PWRKEY(x)				x?GPIO_SetBits(SIM_PWRKEY_GPIO, SIM_PWRKEY_Pin):GPIO_ResetBits(SIM_PWRKEY_GPIO, SIM_PWRKEY_Pin);

/* Private variables ---------------------------------------------------------*/
extern char SIM_STATUS;
extern char SIM_DATA[SIM_DATA_BFRSIZE];
extern char SIM_INDEX_SMS;
/* Private function prototypes -----------------------------------------------*/
void SIM900_Init(void);
void SIM900_SendChar(char chr);
void SIM900_SendString(char* str);
void SIM900_SendNum(int num);
char SIM900_SendCommand(char* AT_CMD, char* Result, unsigned int Timeout);
void SIM900_GPRSInit(void);
void SIM900_SMSInit(void);
void SIM900_CheckAlive(void);

uint16_t SIM900_GetIndexSMS(char* sms);
uint16_t SIM900_GetLengthDataFrom(char* data);

uint16_t SIM900_DataAvailable(void);
char SIM900_isCorrectResult(char* Result, unsigned int Timeout);
char* SIM900_ReceiveData(void);

void SIM900_DATA_IRQHandler(void);
void SIM900_TIMER_IRQHandler(void);

void SIM900_SetTimer1(unsigned int ms);
char SIM900_isTimer1OVF(void);

void SIM900_SetTimer2(unsigned int ms);
char SIM900_isTimer2OVF(void);

void SIM900_DeleteSMS(void);

/* Private functions ---------------------------------------------------------*/
void SIM_UartInit(void);
void SIM_TimerInit(void);
unsigned long sim_pow_of(int A, int n);
unsigned char sim_receive_num(char sms);
#endif
