/* Includes ------------------------------------------------------------------*/
#include "SIM900.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile SIM_RxDataTypedef SIM_RxData;
volatile uint8_t SIM_RxBuffer[SIM_DATA_BFRSIZE];
char SIM_DATA[SIM_DATA_BFRSIZE];

volatile unsigned int sim_tim_cnt1 = 0;
unsigned int sim_tim_MUL1 = 0;
unsigned char sim_tim_flag1 = 0;

volatile unsigned int sim_tim_cnt2 = 0;
unsigned int sim_tim_MUL2 = 0;
unsigned char sim_tim_flag2 = 0;

char SIM_STATUS;
char SIM_INDEX_SMS;
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : SIM900_Init
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SIM900_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* PWRKEY Configuration */
	RCC_APB2PeriphClockCmd(SIM_PWRKEY_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = SIM_PWRKEY_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SIM_PWRKEY_GPIO, &GPIO_InitStructure);
	
	SIM900_PWRKEY(0);
	delay_ms(10);
	SIM900_PWRKEY(1);
	delay_ms(4000);
	
	SIM_UartInit();
	SIM_TimerInit();
	
	SIM_STATUS = SIMSTT_POWER_ON;
	while (SIM_STATUS == SIMSTT_POWER_ON)
		SIM900_CheckAlive();
}

void SIM900_GPRSInit(void)
{
	SIM_STATUS = SIMSTT_CHECK_ALIVE;
	while (SIM_STATUS != SIMSTT_AVAILABLE)
	{
		switch(SIM_STATUS)
		{
			case SIMSTT_CHECK_ALIVE:
				if (SIM900_SendCommand("AT\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_GPRS_NETWORK_REG;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT OK          ");
				}
				break;
				
			case SIMSTT_GPRS_NETWORK_REG:
				if (SIM900_SendCommand("AT+CREG=1\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_GPRS_DET_ATT_FROM;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+CREG=1 OK   ");
				}
				break;
				
			case SIMSTT_GPRS_DET_ATT_FROM:
				if (SIM900_SendCommand("AT+CGATT=1\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_GPRS_BEARER_CONFIG;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+CGATT=1 OK  ");
				}
				break;
				
			case SIMSTT_GPRS_BEARER_CONFIG:
				if (SIM900_SendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_GPRS_OPEN_CONTEXT;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+SAPBR=3,1 OK");
				}
				break;
				
			case SIMSTT_GPRS_OPEN_CONTEXT:
				if (SIM900_SendCommand("AT+SAPBR=1,1\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_GPRS_HTTP_INIT;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+SAPBR=1,1 OK");
				}
				break;
				
			case SIMSTT_GPRS_HTTP_INIT:
				if (SIM900_SendCommand("AT+HTTPINIT\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_GPRS_HTTP_PARAM;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+HTTPINIT OK ");
				}
				break;
				
			case SIMSTT_GPRS_HTTP_PARAM:
				if (SIM900_SendCommand("AT+HTTPPARA=\"CID\",1\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_GPRS_OK;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+HTTPPARA OK ");
				}
				break;
				
			case SIMSTT_GPRS_OK:
				SIM_STATUS = SIMSTT_AVAILABLE;
				if (SIM_LCD_SUPPORT)
					LCD2004_PrintString(1, 5, "GPRS OK        ");
				break;
				
			default:
				break;
		}
	}
}

void SIM900_SMSInit(void)
{
	SIM_STATUS = SIMSTT_CHECK_ALIVE;
	while (SIM_STATUS != SIMSTT_AVAILABLE)
	{
		switch (SIM_STATUS)
		{
			case SIMSTT_CHECK_ALIVE:
				if (SIM900_SendCommand("AT\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_SMS_TEXT_MODE;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT OK          ");
				}
				break;
				
			case SIMSTT_SMS_TEXT_MODE:
				if (SIM900_SendCommand("AT+CMGF=1\r\n", "OK", 500))
				{
					SIM_STATUS = SIMSTT_SMS_OK;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "AT+CMGF=1 OK   ");
				}
				break;
				
			case SIMSTT_SMS_OK:
					SIM_STATUS = SIMSTT_AVAILABLE;;
					if (SIM_LCD_SUPPORT)
						LCD2004_PrintString(1, 5, "SMS OK         ");
				break;
					
			default:
				break;	
		}
	}
}

uint16_t SIM900_GetLengthDataFrom(char* DATA)
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

uint16_t SIM900_GetIndexSMS(char* SMS)
{
	uint16_t length_index_sms = 0;
	uint16_t index_sms = 0;
	unsigned char i;
	char* tmp_sms;
	
	length_index_sms = SIM900_GetLengthDataFrom(SMS);	
	tmp_sms = (char*)SMS;
	for (i = 0; i < length_index_sms; i++)
	{
		index_sms = index_sms + ((char)(*tmp_sms++)-48) * sim_pow_of(10, length_index_sms - i - 1);
	}
	return index_sms;
}

/*******************************************************************************
* Function Name  : SIM900_SendChar | SIM900_SendString | SIM900_SendCommand
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SIM900_SendChar(char chr)
{
	USART_SendData(SIM_UART_CHANNEL, chr);    
	/* Loop until USARTy DR register is empty */ 
	while(USART_GetFlagStatus(SIM_UART_CHANNEL, USART_FLAG_TXE) == RESET);
}

void SIM900_SendString(char* str)
{
	while(*str)	
	{
    SIM900_SendChar(*str++);
	}
}

char SIM900_SendCommand(char* AT_CMD, char* Result, unsigned int Timeout)
{
	SIM900_SendString(AT_CMD);
	return SIM900_isCorrectResult(Result, Timeout);
}

/*******************************************************************************
* Function Name  : SIM900_IRQ_Handler | SIM900_DataAvailable | SIM900_ReceiveData
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SIM900_DATA_IRQHandler(void)
{
	if (USART_GetITStatus(SIM_UART_CHANNEL, USART_IT_RXNE) == SET)
	{	  	   
		SIM_RxBuffer[SIM_RxData.Index_Write] = USART_ReceiveData(SIM_UART_CHANNEL);
		
		SIM_RxData.Index_Write = (SIM_RxData.Index_Write + 1) % SIM_DATA_BFRSIZE;
		if (++SIM_RxData.Available == SIM_DATA_BFRSIZE)
		{
			SIM_RxData.Available = 0;
			SIM_RxData.Overflow = 1;
		}
	}
}

uint16_t SIM900_DataAvailable(void)
{
	return SIM_RxData.Available;
}

char* SIM900_ReceiveData(void)
{
	int Index = 0;
	int i = 0;
	char* tmp;
	
	while (SIM900_DataAvailable())
	{
		if (SIM_RxBuffer[SIM_RxData.Index_Read] != 0)
		{	
			SIM_DATA[Index] = SIM_RxBuffer[SIM_RxData.Index_Read];

			Index = (Index + 1) % SIM_DATA_BFRSIZE;
			SIM_RxData.Index_Read = (SIM_RxData.Index_Read + 1) % SIM_DATA_BFRSIZE;
			SIM_RxData.Available--;
			if (SIM_RxData.Overflow)
			{
				SIM_RxData.Overflow = 0;
			}
			if (SIM_RxData.Index_Read == SIM_RxData.Index_Write && SIM_RxData.Available)
			{
				SIM_RxData.Available = 0;
			}
		}		
	}
	
	tmp = (char*)SIM_DATA;
	
	for (i = Index; i <= SIM_DATA_BFRSIZE; i++)
	{
		SIM_DATA[Index] = '\0';
	}
	
	return tmp;
}

/*******************************************************************************
* Function Name  : SIM900_isCorrectResult
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
char SIM900_isCorrectResult(char* Result, unsigned int Timeout)
{
	char* tmp_result;
	
	delay_ms(Timeout);	
	if (SIM900_DataAvailable())
	{
		tmp_result = SIM900_ReceiveData();
		tmp_result = (char*)strstr(tmp_result, (char*)Result);			
		Timeout = 0;		
		if (tmp_result)
			return 1;
	}
		
	return 0;
}

/* Private functions ---------------------------------------------------------*/
void SIM_UartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure UART Tx as alternate function push-pull */
	RCC_APB2PeriphClockCmd(SIM_GPIO_Tx_CLK | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SIM_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SIM_GPIO_Tx, &GPIO_InitStructure);

	/* Configure UART Rx as input floating */
	RCC_APB2PeriphClockCmd(SIM_GPIO_Rx_CLK | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SIM_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SIM_GPIO_Rx, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(SIM_UART_CLOCK, ENABLE);
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
	USART_Init(SIM_UART_CHANNEL, &USART_InitStructure);

	/* Enable UART Receive and Transmit interrupts */
	USART_ITConfig(SIM_UART_CHANNEL, USART_IT_RXNE, ENABLE);

	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = SIM_UART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(SIM_UART_CHANNEL, ENABLE);
}

void SIM_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(SIM_TIMER_CLOCK, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1)/1000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(SIM_TIMER_CHANNEL, &TIM_TimeBaseStructure);
	
	/* TIM IT enable */
	TIM_ITConfig(SIM_TIMER_CHANNEL, TIM_IT_Update, ENABLE);
	
	/* TIM enable counter */
	TIM_Cmd(SIM_TIMER_CHANNEL, ENABLE);

	/* Enable the TIM global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = SIM_TIMER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
}

unsigned long sim_pow_of(int A, int n)
{
	char i;
	unsigned long temp = 1;
	
  for (i = 0; i < n; i++)
	{
		temp *= A;
	}
    
	return temp;
}

void SIM900_SendNum(int num)
{
	char i;
	char num_flag = 0;
	
	if (num == 0)
	{
		SIM900_SendChar('0');
		return;
	}
	if(num < 0)
	{
		SIM900_SendChar('-');
		num *= -1;
	}

	for ( i = 7; i > 0; i--)
	{
		if ((num / sim_pow_of(10, i-1)) != 0)
		{
			num_flag = 1;
			SIM900_SendChar(num/sim_pow_of(10, i-1) + '0');
		}
		else
		{
			if (num_flag != 0)
			{
				SIM900_SendChar('0');
			}
		}
		
		num %= sim_pow_of(10, i-1);
	}
}

void SIM900_TIMER_IRQHandler(void)
{
	TIM_ClearITPendingBit(SIM_TIMER_CHANNEL, TIM_IT_Update);
	
	sim_tim_cnt1++;
  if (sim_tim_cnt1 >= sim_tim_MUL1)
  {
		sim_tim_cnt1 = 0;
		sim_tim_flag1 = 1;
  }
	
	sim_tim_cnt2++;
  if (sim_tim_cnt2 >= sim_tim_MUL2)
  {
		sim_tim_cnt2 = 0;
		sim_tim_flag2 = 1;
  }
}

void SIM900_SetTimer1(unsigned int ms)
{
	sim_tim_MUL1 = ms;
	sim_tim_cnt1 = 0;
	sim_tim_flag1 = 0;
}

char SIM900_isTimer1OVF(void)
{
	if (sim_tim_flag1)
	{
		sim_tim_flag1 = 0;
		return 1;
	}
	else
		return 0;
}

void SIM900_SetTimer2(unsigned int ms)
{
	sim_tim_MUL2 = ms;
	sim_tim_cnt2 = 0;
	sim_tim_flag2 = 0;
}

char SIM900_isTimer2OVF(void)
{
	if (sim_tim_flag2)
	{
		sim_tim_flag2 = 0;
		return 1;
	}
	else
		return 0;
}

void SIM900_CheckAlive(void)
{
	if (SIM_STATUS == SIMSTT_AVAILABLE || SIM_STATUS == SIMSTT_POWER_ON)
	{
		if (SIM900_SendCommand("AT\r\n", "OK", 500))
		{
			SIM_STATUS = SIMSTT_AVAILABLE;
			if (SIM_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "[SIM] AT OK    ");
		}
		else
		{
			if (SIM_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "[SIM] AT ERROR ");
		}
	}
}
			
void SIM900_SpamMissCall(void)
{
	if (SIM_STATUS != SIMSTT_MAKE_CALL)
	{
		if (SIM900_SendCommand("ATD0906604388;\r\n", "OK", 1000))
		{
			if (SIM_LCD_SUPPORT)
				LCD2004_PrintString(2, 0, "Calling");
			
			SIM_STATUS = SIMSTT_MAKE_CALL;
		}				
	}
	else
	{
		if (SIM900_SendCommand("ATH\r\n", "OK", 500))
		{
			if (SIM_LCD_SUPPORT)
				LCD2004_PrintString(2, 0, "Ending.");
			
			SIM_STATUS = SIMSTT_END_CALL;
			
			if (SIM900_SendCommand("ATD0906604388;\r\n", "OK", 500))
			{
				if (SIM_LCD_SUPPORT)
					LCD2004_PrintString(2, 0, "Calling");
				
				SIM_STATUS = SIMSTT_MAKE_CALL;
			}	
		}	
	}
}

void SIM900_DeleteSMS(void)
{
	unsigned int i;
	for (i = 1; i <= 30; i++)
	{
		SIM900_SendString("AT+CMGD=");
		SIM900_SendNum(i);
		SIM900_SendString("\r\n");
		delay_ms(500);
	}
}
