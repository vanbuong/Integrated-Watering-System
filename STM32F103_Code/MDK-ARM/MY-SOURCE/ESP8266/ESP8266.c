/* Includes ------------------------------------------------------------------*/
#include "ESP8266.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile ESP_RxDataTypedef ESP_RxData;
volatile uint8_t ESP_RxBuffer[ESP_DATA_BFRSIZE];
char ESP_DATA[ESP_DATA_BFRSIZE];

volatile unsigned int esp_tim_cnt1 = 0;
unsigned int esp_tim_MUL1 = 0;
unsigned char esp_tim_flag1 = 0;

volatile unsigned int esp_tim_cnt2 = 0;
unsigned int esp_tim_MUL2 = 0;
unsigned char esp_tim_flag2 = 0;

volatile unsigned int esp_tim_cnt3 = 0;
unsigned int esp_tim_MUL3 = 0;
unsigned char esp_tim_flag3 = 0;

char ESP_STATUS;

int SMARTHOME_CURRENT_TEMP;
char SMARTHOME_CURRENT_LED1;
char SMARTHOME_CURRENT_LED2;
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : ESP8266_Init
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ESP8266_Init(void)
{
	ESP_UartInit();
	ESP_TimerInit();
	
	ESP_STATUS = ESPSTT_POWER_ON;
	while (ESP_STATUS == ESPSTT_POWER_ON)
		ESP8266_CheckAlive();
}

void ESP8266_InternetInit(void)
{
	ESP_STATUS = ESPSTT_CHECK_ALIVE;
	while (ESP_STATUS != ESPSTT_AVAILABLE)
	{
		switch(ESP_STATUS)
		{
			case ESPSTT_CHECK_ALIVE:
				if (ESP8266_SendCommand("AT\r\n", "OK", 500))
				{
					ESP_STATUS = ESPSTT_INTERNET_QUIT_AP;
					if (ESP_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT OK          ");
				}
				break;
				
			case ESPSTT_INTERNET_QUIT_AP:
				if (ESP8266_SendCommand("AT+CWQAP\r\n", "OK", 500))
				{
					ESP_STATUS = ESPSTT_INTERNET_MODE;
					if (ESP_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+CWQAP OK    ");
				}
				break;
				
			case ESPSTT_INTERNET_MODE:
				if (ESP8266_SendCommand("AT+CWMODE=3\r\n", "OK", 500))
				{
					ESP_STATUS = ESPSTT_INTERNET_SET_AP;
					if (ESP_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+CWMODE=3 OK ");
				}
				break;
				
			case ESPSTT_INTERNET_SET_AP:
				if (ESP8266_SendCommand(ESPCMD_SET_AP, "OK", 500))
				{
					ESP_STATUS = ESPSTT_INTERNET_JOIN_AP;
					if (ESP_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+CWSAP OK    ");
				}
				break;
				
			case ESPSTT_INTERNET_JOIN_AP:
				if (ESP8266_SendCommand(ESPCMD_JOIN_AP, "OK", 2000))
				{
					ESP_STATUS = ESPSTT_MUL_CONNECTION;
					if (ESP_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+CWJAP OK    ");
				}
				break;
				
			case ESPSTT_MUL_CONNECTION:
				if (ESP8266_SendCommand("AT+CIPMUX=1\r\n", "OK", 500))
				{
					ESP_STATUS = ESPSTT_INTERNET_OK;
					if (ESP_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+CIPMUX=1 OK ");
					delay_ms(500);
				}
				break;
				
			case ESPSTT_INTERNET_OK:
				ESP_STATUS = ESPSTT_AVAILABLE;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Internet OK      ");
				break;
				
			default:
				break;
		}
	}
}

uint16_t ESP8266_GetLengthDataFrom(char* DATA)
{
	char* tmp_data;
	uint16_t length_index_data = 0;
	
	tmp_data = (char*)DATA;
	while (*tmp_data)
	{
		*tmp_data++;
		length_index_data++;
	}
	/*!< Exclude \r and \n */
	return length_index_data - 2;
}

/*******************************************************************************
* Function Name  : ESP8266_SendChar | ESP8266_SendString | ESP8266_SendCommand
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ESP8266_SendChar(char chr)
{
	USART_SendData(ESP_UART_CHANNEL, chr);    
	/* Loop until USARTy DR register is empty */ 
	while(USART_GetFlagStatus(ESP_UART_CHANNEL, USART_FLAG_TXE) == RESET);
}

void ESP8266_SendString(char* str)
{
	while(*str)	
	{
    ESP8266_SendChar(*str++);
	}
}

char ESP8266_SendCommand(char* AT_CMD, char* Result, unsigned int Timeout)
{
	ESP8266_SendString(AT_CMD);
	return ESP8266_isCorrectResult(Result, Timeout);
}

/*******************************************************************************
* Function Name  : ESP8266_IRQ_Handler | ESP8266_DataAvailable | ESP8266_ReceiveData
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ESP8266_DATA_IRQHandler(void)
{
	if (USART_GetITStatus(ESP_UART_CHANNEL, USART_IT_RXNE) == SET)
	{	  	   
		ESP_RxBuffer[ESP_RxData.Index_Write] = USART_ReceiveData(ESP_UART_CHANNEL);
		
		ESP_RxData.Index_Write = (ESP_RxData.Index_Write + 1) % ESP_DATA_BFRSIZE;
		if (++ESP_RxData.Available == ESP_DATA_BFRSIZE)
		{
			ESP_RxData.Available = 0;
			ESP_RxData.Overflow = 1;
		}
	}
}

uint16_t ESP8266_DataAvailable(void)
{
	return ESP_RxData.Available;
}

char* ESP8266_ReceiveData(void)
{
	int Index = 0;
	int i = 0;
	char* tmp;
	
	while (ESP8266_DataAvailable())
	{
		if (ESP_RxBuffer[ESP_RxData.Index_Read] != 0)
		{	
			ESP_DATA[Index] = ESP_RxBuffer[ESP_RxData.Index_Read];

			Index = (Index + 1) % ESP_DATA_BFRSIZE;
			ESP_RxData.Index_Read = (ESP_RxData.Index_Read + 1) % ESP_DATA_BFRSIZE;
			ESP_RxData.Available--;
			if (ESP_RxData.Overflow)
			{
				ESP_RxData.Overflow = 0;
			}
			if (ESP_RxData.Index_Read == ESP_RxData.Index_Write && ESP_RxData.Available)
			{
				ESP_RxData.Available = 0;
			}
		}		
	}
	
	tmp = (char*)ESP_DATA;
	
	for (i = Index; i <= ESP_DATA_BFRSIZE; i++)
	{
		ESP_DATA[Index] = '\0';
	}
	
	return tmp;
}

/*******************************************************************************
* Function Name  : ESP8266_isCorrectResult
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
char ESP8266_isCorrectResult(char* Result, unsigned int Timeout)
{
	char* tmp_result;
	
	delay_ms(Timeout);	
	if (ESP8266_DataAvailable())
	{
		tmp_result = ESP8266_ReceiveData();
		tmp_result = (char*)strstr(tmp_result, (char*)Result);			
		Timeout = 0;		
		if (tmp_result)
			return 1;
	}
		
	return 0;
}

/* Private functions ---------------------------------------------------------*/
void ESP_UartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure UART Tx as alternate function push-pull */
	RCC_APB2PeriphClockCmd(ESP_GPIO_Tx_CLK | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ESP_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(ESP_GPIO_Tx, &GPIO_InitStructure);

	/* Configure UART Rx as input floating */
	RCC_APB2PeriphClockCmd(ESP_GPIO_Rx_CLK | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ESP_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(ESP_GPIO_Rx, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(ESP_UART_CLOCK, ENABLE);
	/* USARTx configured as follow:
			- BaudRate = 115200 baud  
			- Word Length = 8 Bits
			- One Stop Bit
			- No parity
			- Hardware flow control disabled (RTS and CTS signals)
			- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(ESP_UART_CHANNEL, &USART_InitStructure);

	/* Enable UART Receive and Transmit interrupts */
	USART_ITConfig(ESP_UART_CHANNEL, USART_IT_RXNE, ENABLE);

	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = ESP_UART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(ESP_UART_CHANNEL, ENABLE);
}

void ESP_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(ESP_TIMER_CLOCK, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1)/1000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(ESP_TIMER_CHANNEL, &TIM_TimeBaseStructure);
	
	/* TIM IT enable */
	TIM_ITConfig(ESP_TIMER_CHANNEL, TIM_IT_Update, ENABLE);
	
	/* TIM enable counter */
	TIM_Cmd(ESP_TIMER_CHANNEL, ENABLE);

	/* Enable the TIM global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = ESP_TIMER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
}

unsigned long ESP_pow_of(int A, int n)
{
	char i;
	unsigned long temp = 1;
	
  for (i = 0; i < n; i++)
	{
		temp *= A;
	}
    
	return temp;
}

void ESP8266_SendNum(int num)
{
	char i;
	char num_flag = 0;
	
	if (num == 0)
	{
		ESP8266_SendChar('0');
		return;
	}
	if(num < 0)
	{
		ESP8266_SendChar('-');
		num *= -1;
	}

	for ( i = 7; i > 0; i--)
	{
		if ((num / ESP_pow_of(10, i-1)) != 0)
		{
			num_flag = 1;
			ESP8266_SendChar(num/ESP_pow_of(10, i-1) + '0');
		}
		else
		{
			if (num_flag != 0)
			{
				ESP8266_SendChar('0');
			}
		}
		
		num %= ESP_pow_of(10, i-1);
	}
}

void ESP8266_TIMER_IRQHandler(void)
{
	TIM_ClearITPendingBit(ESP_TIMER_CHANNEL, TIM_IT_Update);
	
	esp_tim_cnt1++;
  if (esp_tim_cnt1 >= esp_tim_MUL1)
  {
		esp_tim_cnt1 = 0;
		esp_tim_flag1 = 1;
  }
	
	esp_tim_cnt2++;
  if (esp_tim_cnt2 >= esp_tim_MUL2)
  {
		esp_tim_cnt2 = 0;
		esp_tim_flag2 = 1;
  }
	
	esp_tim_cnt3++;
  if (esp_tim_cnt3 >= esp_tim_MUL3)
  {
		esp_tim_cnt3 = 0;
		esp_tim_flag3 = 1;
  }
}

void ESP8266_SetTimer1(unsigned int ms)
{
	esp_tim_MUL1 = ms;
	esp_tim_cnt1 = 0;
	esp_tim_flag1 = 0;
}

char ESP8266_isTimer1OVF(void)
{
	if (esp_tim_flag1)
	{
		esp_tim_flag1 = 0;
		return 1;
	}
	else
		return 0;
}

void ESP8266_SetTimer2(unsigned int ms)
{
	esp_tim_MUL2 = ms;
	esp_tim_cnt2 = 0;
	esp_tim_flag2 = 0;
}

char ESP8266_isTimer2OVF(void)
{
	if (esp_tim_flag2)
	{
		esp_tim_flag2 = 0;
		return 1;
	}
	else
		return 0;
}

void ESP8266_SetTimer3(unsigned int ms)
{
	esp_tim_MUL3 = ms;
	esp_tim_cnt3 = 0;
	esp_tim_flag3 = 0;
}

char ESP8266_isTimer3OVF(void)
{
	if (esp_tim_flag3)
	{
		esp_tim_flag3 = 0;
		return 1;
	}
	else
		return 0;
}

void ESP8266_CheckAlive(void)
{
	if (ESP_STATUS == ESPSTT_AVAILABLE || ESP_STATUS == ESPSTT_POWER_ON)
	{
		if (ESP8266_SendCommand("AT\r\n", "OK", 250))
		{
			ESP_STATUS = ESPSTT_AVAILABLE;
			if (ESP_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "[WIFI] AT OK   ");
		}
		else
		{
			if (ESP_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "[WIFI] AT ERROR");
		}
	}
}

void ESP8266_CheckInternet(void)
{
	if (ESP_STATUS == ESPSTT_AVAILABLE)
		ESP_STATUS = ESPSTT_CHECK_AP;
	switch (ESP_STATUS)
	{
		case ESPSTT_CHECK_AP:
			if (ESP8266_SendCommand("AT+CWJAP?\r\n", "+CWJAP:", 250))
			{
				ESP_STATUS = ESPSTT_AVAILABLE;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Internet OK    ");
			}
			else
			{
				ESP_STATUS = ESPSTT_RECONNECT_AP;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Internet ERROR ");
			}
			break;
			
		case ESPSTT_RECONNECT_AP:
			if (ESP8266_SendCommand(ESPCMD_JOIN_AP, "OK", 500))
			{
				ESP_STATUS = ESPSTT_AVAILABLE;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Internet OK    ");
			}
			break;
		
		default:
			break;
	}
}

void ESP8266_WeatherForecast(void)
{
	char* esp_data;
	unsigned i;
	
	switch (ESP_STATUS)
	{
		case ESPSTT_WEATHER_REQUEST:			
			ESP_STATUS = ESPSTT_WEATHER_START_CONNECT;
			break;
		
		case ESPSTT_WEATHER_START_CONNECT:
			if (ESP_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "WeatherForecast");
			ESP8266_SendCommand("AT+CIPMUX=1\r\n", "OK", 500);
			if (ESP8266_SendCommand(ESPCMD_API_THINGSPEAK, "CONNECT", 500))
			{
				ESP_STATUS = ESPSTT_WEATHER_SEND_REQUEST;	
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Connecting.    ");				
			}
			break;
			
		case ESPSTT_WEATHER_SEND_REQUEST:	
			if (ESP8266_SendCommand(ESPCMD_WEATHER_SEND_REQUEST, ">", 500))
			{
				ESP_STATUS = ESPSTT_WEATHER_SEND;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Connecting..   ");	
			}
			break;
			
		case ESPSTT_WEATHER_SEND:		
		
			if (ESP8266_SendCommand(ESPCMD_WEATHER_SEND, "+IPD", 500))
			{
				ESP_STATUS = ESPSTT_WEATHER_DATA_PROCESS;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Connecting...  ");
			}
			break;
			
		case ESPSTT_WEATHER_DATA_PROCESS:
			ESP_STATUS = ESPSTT_AVAILABLE;
			if (ESP_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "WorldWeatherOnl");
			
			LCD2004_PrintString(2, 0, "                    ");
			LCD2004_PrintString(3, 0, "                    ");
			
			LCD2004_PrintString(2, 0, "Temp:   C");
			LCD2004_PrintString(2, 7, "\xdf");			
			LCD2004_PrintString(2, 10, "Humid:   %");
			LCD2004_PrintString(3, 0, "ClCv:  %");
			LCD2004_PrintString(3, 9, "Vision:  Km");
			
			// Show temperature
			esp_data = (char*)(ESP_DATA);
			esp_data = (char*)strstr(ESP_DATA, "temperature");
			for (i = 0; i < 13; i++)
				esp_data++;
			
			for (i = 0; i < 2; i++)
				LCD2004_PrintChar(2, i+5, *esp_data++);
			
			// Show Humidity
			esp_data = (char*)(ESP_DATA);
			esp_data = (char*)strstr(ESP_DATA, "100%");
			if (!esp_data)
				LCD2004_PrintChar(2, 19, ' ');
			
			esp_data = (char*)(ESP_DATA);
			esp_data = (char*)strstr(ESP_DATA, "Humidity:");
			for (i = 0; i < 20; i++)
				esp_data++;
			
			for (i = 0; i < 3; i++)
				LCD2004_PrintChar(2, i+16, *esp_data++);
			
			// Show Cloud Cover
			esp_data = (char*)(ESP_DATA);
			esp_data = (char*)strstr(ESP_DATA, "Cloud Cover:");
			for (i = 0; i < 23; i++)
				esp_data++;
			
			for (i = 0; i < 2; i++)
				LCD2004_PrintChar(3, i+5, *esp_data++);
				
			// Show Visibility
			esp_data = (char*)(ESP_DATA);
			esp_data = (char*)strstr(ESP_DATA, "Visibility:");
			for (i = 0; i < 22; i++)
				esp_data++;
			
			for (i = 0; i < 2; i++)
				LCD2004_PrintChar(3, i+16, *esp_data++);
			
			break;

		default:
			break;
	}
}

void ESP8266_Smarthome_Ctrl(void)
{
	char* esp_data;
	unsigned i;
	
	switch (ESP_STATUS)
	{
		/*!< READ and PROCESS data from WEB */
		case ESPSTT_SMARTHOME_REQUEST:
			ESP_STATUS = ESPSTT_SMARTHOME_START_CONNECT;
			break;
		
		case ESPSTT_SMARTHOME_START_CONNECT:
			LCD2004_PrintString(1, 5, "SmartHomeStatus");
			ESP8266_SendCommand("AT+CIPMUX=1\r\n", "OK", 500);
			if (ESP8266_SendCommand(ESPCMD_API_ITS4U, "CONNECT", 500))
			{
				ESP_STATUS = ESPSTT_SMARTHOME_SEND_REQUEST;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Connecting.    ");
			}
			break;
			
		case ESPSTT_SMARTHOME_SEND_REQUEST:
			if (ESP8266_SendCommand(ESPCMD_SHOME_SEND_READ_REQUEST, ">", 500))
			{
				ESP_STATUS = ESPSTT_SMARTHOME_SEND;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Connecting..   ");			
			}
			else
			{
				ESP_STATUS = ESPSTT_SMARTHOME_START_CONNECT;
			}
			break;
			
		case ESPSTT_SMARTHOME_SEND:
			if (ESP8266_SendCommand(ESPCMD_SHOME_SEND_READ, "+IPD", 500))
			{
				ESP_STATUS = ESPSTT_SMARTHOME_DATA_PROCESS;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Connecting...  ");
			}
			else
			{
				ESP_STATUS = ESPSTT_SMARTHOME_START_CONNECT;
			}
			break;
			
		case ESPSTT_SMARTHOME_DATA_PROCESS:
			ESP_STATUS = ESPSTT_SMARTHOME_WEB_REQUEST;
			if (ESP_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "SmartHome Ctrl!");
			
			LCD2004_PrintString(2, 0, "Temp:   C           ");
			LCD2004_PrintString(2, 7, "\xdf");			

			LCD2004_PrintString(3, 0, "LED1:     ");
			LCD2004_PrintString(3, 10, "LED2:     ");
			
			// Show Temperature
			esp_data = (char*)(ESP_DATA);
			esp_data = (char*)strstr(ESP_DATA, "Temp");
			for (i = 0; i < 7; i++)
				esp_data++;
			
			for (i = 0; i < 2; i++)
			{
				LCD2004_PrintChar(2, i+5, *esp_data++);
			}
			
			// Show LED1 Status
			esp_data = (char*)(ESP_DATA);
			esp_data = (char*)strstr(ESP_DATA, "Led1");
			for (i = 0; i < 7; i++)
				esp_data++;
			
			if ((char)*esp_data == '1')
			{
				LCD2004_PrintString(3, 5, "ON   ");
				LED1(1);
				SMARTHOME_CURRENT_LED1 = 1;
				nRF24L01_Send(nRF24_CMD_LED1_ON);
			}
			else
			{
				LCD2004_PrintString(3, 5, "OFF  ");
				LED1(0);
				SMARTHOME_CURRENT_LED1 = 0;
				nRF24L01_Send(nRF24_CMD_LED1_OFF);
			}
				
			// Show LED2 Status
			esp_data = (char*)(ESP_DATA);
			esp_data = (char*)strstr(ESP_DATA, "Led2");
			for (i = 0; i < 7; i++)
				esp_data++;
			
			if ((char)*esp_data == '1')
			{
				LCD2004_PrintString(3, 15, "ON   ");
				LED2(1);
				SMARTHOME_CURRENT_LED2 = 1;
				nRF24L01_Send(nRF24_CMD_LED2_ON);
			}
			else
			{
				LCD2004_PrintString(3, 15, "OFF  ");
				LED2(0);
				SMARTHOME_CURRENT_LED2 = 0;
				nRF24L01_Send(nRF24_CMD_LED2_OFF);
			}		
			break;

		/*!< SEND data to WEB */
		case ESPSTT_SMARTHOME_WEB_REQUEST:
			ESP_STATUS = ESPSTT_SMARTHOME_WEB_START_CONNECT;
			break;
		
		case ESPSTT_SMARTHOME_WEB_START_CONNECT:
			LCD2004_PrintString(1, 5, "SmartHomeUpdate");
			ESP8266_SendCommand("AT+CIPMUX=1\r\n", "OK", 500);
			if (ESP8266_SendCommand(ESPCMD_API_ITS4U, "CONNECT", 500))
			{
				ESP_STATUS = ESPSTT_SMARTHOME_WEB_SEND_REQUEST;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Sending.       ");
			}
			break;
		
		case ESPSTT_SMARTHOME_WEB_SEND_REQUEST:	
			if (ESP8266_SendCommand(ESPCMD_SHOME_SEND_WRITE_REQUEST, ">", 500))
			{
				ESP_STATUS = ESPSTT_SMARTHOME_WEB_SEND;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "Sending..      ");			
			}
			else
			{
				ESP_STATUS = ESPSTT_SMARTHOME_WEB_START_CONNECT;
			}
			break;
			
		case ESPSTT_SMARTHOME_WEB_SEND:
			LCD2004_PrintString(1, 5, "Sending...     ");	
			ESP8266_SendString(ESPCMD_SHOME_SEND_WRITE);		
			ESP8266_SendString(ESPCMD_SHOME_TEMPERATURE);
			ESP8266_SendNum(SMARTHOME_CURRENT_TEMP);
			ESP8266_SendString(ESPCMD_SHOME_LED1);
			ESP8266_SendNum(SMARTHOME_CURRENT_LED1);
			ESP8266_SendString(ESPCMD_SHOME_LED2);
			ESP8266_SendNum(SMARTHOME_CURRENT_LED2);
		
			if (ESP8266_SendCommand("\r\n", "+IPD", 500))
			{
				ESP_STATUS = ESPSTT_AVAILABLE;
				if (ESP_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "SmartHome Sent!");
			}
			else
			{
				ESP_STATUS = ESPSTT_SMARTHOME_WEB_START_CONNECT;
			}
			break;
			
		default:
			break;
	}
}
