#ifndef __ESP8266_H
#define __ESP8266_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "TIMER/TIMER.h"
#include "UART/UART.h"
#include "LCD/LCD.h"
#include "GPIO/GPIO.h"
#include "nRF24L01/nRF24L01.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	uint16_t Index_Read;
	uint16_t Index_Write;
	uint16_t Available;
	uint8_t Overflow;
} ESP_RxDataTypedef;

/* Private define ------------------------------------------------------------*/
#define ESP_DATA_BFRSIZE 								2000
#define ESP_LCD_SUPPORT									1

#define ESPCMD_SET_AP										"AT+CWSAP=\"Gideon\",\"khongcopasshihi\",5,3\r\n"
#define ESPCMD_JOIN_AP0									"AT+CWJAP=\"GAAC\",\"bungbudeptrai\"\r\n"
#define ESPCMD_JOIN_AP1									"AT+CWJAP=\"iTLabs4U_OutRoom\",\"khongcopassnnn\"\r\n"
#define ESPCMD_JOIN_AP									"AT+CWJAP=\"The-Duy's iPhone\",\"duyduy123\"\r\n"

#define ESPCMD_CLOSE_CONNECTION					""
#define ESPCMD_API_THINGSPEAK						"AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80\r\n"
#define ESPCMD_WEATHER_SEND_REQUEST			"AT+CIPSEND=0,85\r\n"
#define ESPCMD_WEATHER_SEND							"GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=YQNDHH3I1JI9MBUB\r\n"

#define ESPCMD_API_ITS4U								"AT+CIPSTART=1,\"TCP\",\"its4u.vn\",80\r\n"
#define ESPCMD_SHOME_SEND_READ_REQUEST	"AT+CIPSEND=1,41\r\n"
#define ESPCMD_SHOME_SEND_READ					"GET http://its4u.vn/thuan/LVTN/getvalue\r\n"
#define ESPCMD_SHOME_SEND_WRITE_REQUEST	"AT+CIPSEND=1,63\r\n"
#define ESPCMD_SHOME_SEND_WRITE					"GET http://its4u.vn/thuan/LVTN/setvalue?"
#define ESPCMD_SHOME_TEMPERATURE				"temp="
#define ESPCMD_SHOME_LED1								"&led1="
#define ESPCMD_SHOME_LED2								"&led2="

#define ESP_UART_CHANNEL								UART4
#define ESP_UART_CLOCK									RCC_APB1Periph_UART4
#define ESP_UART_IRQn										UART4_IRQn

#define ESP_TIMER_CHANNEL								TIM4
#define ESP_TIMER_CLOCK									RCC_APB1Periph_TIM4
#define ESP_TIMER_IRQn									TIM4_IRQn

#define ESP_GPIO_Tx											GPIOC
#define ESP_GPIO_Rx											GPIOC
#define ESP_GPIO_Tx_CLK									RCC_APB2Periph_GPIOC
#define ESP_GPIO_Rx_CLK									RCC_APB2Periph_GPIOC
#define ESP_TxPin												GPIO_Pin_10
#define ESP_RxPin												GPIO_Pin_11

#define ESPSTT_DEAD											100
#define ESPSTT_POWER_ON									101

#define ESPSTT_BUSY											102
#define ESPSTT_AVAILABLE								103

#define ESPSTT_CHECK_ALIVE							104

#define ESPSTT_INTERNET_QUIT_AP					105
#define ESPSTT_INTERNET_MODE						106
#define ESPSTT_INTERNET_SET_AP					107
#define ESPSTT_INTERNET_JOIN_AP					108
#define ESPSTT_INTERNET_OK							109
#define ESPSTT_MUL_CONNECTION						110

#define ESPSTT_CHECK_AP									111
#define ESPSTT_RECONNECT_AP							112

//Your Application
#define ESPSTT_WEATHER_REQUEST									121
#define ESPSTT_WEATHER_START_CONNECT						122
#define ESPSTT_WEATHER_SEND_REQUEST							123
#define ESPSTT_WEATHER_SEND											124
#define ESPSTT_WEATHER_DATA_PROCESS							125

#define ESPSTT_SMARTHOME_REQUEST								126
#define ESPSTT_SMARTHOME_START_CONNECT					127
#define ESPSTT_SMARTHOME_SEND_REQUEST						128
#define ESPSTT_SMARTHOME_SEND										129
#define ESPSTT_SMARTHOME_DATA_PROCESS						130

#define ESPSTT_SMARTHOME_WEB_REQUEST						131
#define ESPSTT_SMARTHOME_WEB_START_CONNECT			132
#define ESPSTT_SMARTHOME_WEB_SEND_REQUEST				133
#define ESPSTT_SMARTHOME_WEB_SEND								134

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern char ESP_STATUS;
extern char ESP_DATA[ESP_DATA_BFRSIZE];

//Your Application
extern int SMARTHOME_CURRENT_TEMP;
extern char SMARTHOME_CURRENT_LED1;
extern char SMARTHOME_CURRENT_LED2;

/* Private function prototypes -----------------------------------------------*/
void ESP8266_Init(void);
void ESP8266_SendChar(char chr);
void ESP8266_SendString(char* str);
void ESP8266_SendNum(int num);
char ESP8266_SendCommand(char* AT_CMD, char* Result, unsigned int Timeout);
void ESP8266_InternetInit(void);

void ESP8266_CheckAlive(void);
void ESP8266_CheckInternet(void);

uint16_t ESP8266_GetLengthDataFrom(char* data);

uint16_t ESP8266_DataAvailable(void);
char ESP8266_isCorrectResult(char* Result, unsigned int Timeout);
char* ESP8266_ReceiveData(void);

void ESP8266_DATA_IRQHandler(void);
void ESP8266_TIMER_IRQHandler(void);

void ESP8266_SetTimer1(unsigned int ms);
char ESP8266_isTimer1OVF(void);

void ESP8266_SetTimer2(unsigned int ms);
char ESP8266_isTimer2OVF(void);

void ESP8266_SetTimer3(unsigned int ms);
char ESP8266_isTimer3OVF(void);

void ESP8266_WeatherForecast(void);
void ESP8266_Smarthome_Ctrl(void);

/* Private functions ---------------------------------------------------------*/
void ESP_UartInit(void);
void ESP_TimerInit(void);
unsigned long ESP_pow_of(int A, int n);
unsigned char ESP_receive_num(char sms);
#endif
