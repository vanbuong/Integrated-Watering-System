/* Includes ------------------------------------------------------------------*/
#include "BLE.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile BLE_RxDataTypedef BLE_RxData;
volatile uint8_t BLE_RxBuffer[BLE_DATA_BFRSIZE];
char BLE_DATA[BLE_DATA_BFRSIZE];

volatile unsigned int ble_tim_cnt1 = 0;
unsigned int ble_tim_MUL1 = 0;
unsigned char ble_tim_flag1 = 0;

volatile unsigned int ble_tim_cnt2 = 0;
unsigned int ble_tim_MUL2 = 0;
unsigned char ble_tim_flag2 = 0;

char BLE_STATUS;

/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : BLEHM10_Init
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BLEHM10_Init(void)
{
	BLE_UartInit();
	BLE_TimerInit();
	
	BLE_STATUS = BLESTT_POWER_ON;
	while (BLE_STATUS == BLESTT_POWER_ON)
		BLEHM10_CheckAlive();
	
	BLEHM10_SendCommand("AT+NAMEGideon", "OK", 500);
}

uint16_t BLEHM10_GetLengthDataFrom(char* DATA)
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
* Function Name  : BLEHM10_SendChar | BLEHM10_SendString | BLEHM10_SendCommand
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BLEHM10_SendChar(char chr)
{
	USART_SendData(BLE_UART_CHANNEL, chr);    
	/* Loop until USARTy DR register is empty */ 
	while(USART_GetFlagStatus(BLE_UART_CHANNEL, USART_FLAG_TXE) == RESET);
}

void BLEHM10_SendString(char* str)
{
	while(*str)	
	{
    BLEHM10_SendChar(*str++);
	}
}

char BLEHM10_SendCommand(char* AT_CMD, char* Result, unsigned int Timeout)
{
	BLEHM10_SendString(AT_CMD);
	return BLEHM10_isCorrectResult(Result, Timeout);
}

/*******************************************************************************
* Function Name  : BLEHM10_IRQ_Handler | BLEHM10_DataAvailable | BLEHM10_ReceiveData
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BLEHM10_DATA_IRQHandler(void)
{
	if (USART_GetITStatus(BLE_UART_CHANNEL, USART_IT_RXNE) == SET)
	{	  	   
		BLE_RxBuffer[BLE_RxData.Index_Write] = USART_ReceiveData(BLE_UART_CHANNEL);
		
		BLE_RxData.Index_Write = (BLE_RxData.Index_Write + 1) % BLE_DATA_BFRSIZE;
		if (++BLE_RxData.Available == BLE_DATA_BFRSIZE)
		{
			BLE_RxData.Available = 0;
			BLE_RxData.Overflow = 1;
		}
	}
}

uint16_t BLEHM10_DataAvailable(void)
{
	return BLE_RxData.Available;
}

char* BLEHM10_ReceiveData(void)
{
	int Index = 0;
	int i = 0;
	char* tmp;
	
	while (BLEHM10_DataAvailable())
	{
		if (BLE_RxBuffer[BLE_RxData.Index_Read] != 0)
		{	
			BLE_DATA[Index] = BLE_RxBuffer[BLE_RxData.Index_Read];

			Index = (Index + 1) % BLE_DATA_BFRSIZE;
			BLE_RxData.Index_Read = (BLE_RxData.Index_Read + 1) % BLE_DATA_BFRSIZE;
			BLE_RxData.Available--;
			if (BLE_RxData.Overflow)
			{
				BLE_RxData.Overflow = 0;
			}
			if (BLE_RxData.Index_Read == BLE_RxData.Index_Write && BLE_RxData.Available)
			{
				BLE_RxData.Available = 0;
			}
		}		
	}
	
	tmp = (char*)BLE_DATA;
	
	for (i = Index; i <= BLE_DATA_BFRSIZE; i++)
	{
		BLE_DATA[Index] = '\0';
	}
	
	return tmp;
}

/*******************************************************************************
* Function Name  : BLEHM10_isCorrectResult
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
char BLEHM10_isCorrectResult(char* Result, unsigned int Timeout)
{
	char* tmp_result;
	
	delay_ms(Timeout);	
	if (BLEHM10_DataAvailable())
	{
		tmp_result = BLEHM10_ReceiveData();
		tmp_result = (char*)strstr(tmp_result, (char*)Result);			
		Timeout = 0;		
		if (tmp_result)
			return 1;
	}
		
	return 0;
}

/* Private functions ---------------------------------------------------------*/
void BLE_UartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure UART Tx as alternate function push-pull */
	RCC_APB2PeriphClockCmd(BLE_GPIO_Tx_CLK | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BLE_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(BLE_GPIO_Tx, &GPIO_InitStructure);

	/* Configure UART Rx as input floating */
	RCC_APB2PeriphClockCmd(BLE_GPIO_Rx_CLK | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BLE_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BLE_GPIO_Rx, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(BLE_UART_CLOCK, ENABLE);
	/* USARTx configured as follow:
			- BaudRate = 115200 baud  
			- Word Length = 8 Bits
			- One Stop Bit
			- No parity
			- Hardware flow control disabled (RTS and CTS signals)
			- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(BLE_UART_CHANNEL, &USART_InitStructure);

	/* Enable UART Receive and Transmit interrupts */
	USART_ITConfig(BLE_UART_CHANNEL, USART_IT_RXNE, ENABLE);

	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = BLE_UART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(BLE_UART_CHANNEL, ENABLE);
}

void BLE_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(BLE_TIMER_CLOCK, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1)/1000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(BLE_TIMER_CHANNEL, &TIM_TimeBaseStructure);
	
	/* TIM IT enable */
	TIM_ITConfig(BLE_TIMER_CHANNEL, TIM_IT_Update, ENABLE);
	
	/* TIM enable counter */
	TIM_Cmd(BLE_TIMER_CHANNEL, ENABLE);

	/* Enable the TIM global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = BLE_TIMER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
}

unsigned long BLE_pow_of(int A, int n)
{
	char i;
	unsigned long temp = 1;
	
  for (i = 0; i < n; i++)
	{
		temp *= A;
	}
    
	return temp;
}

void BLEHM10_SendNum(int num)
{
	char i;
	char num_flag = 0;
	
	if (num == 0)
	{
		BLEHM10_SendChar('0');
		return;
	}
	if(num < 0)
	{
		BLEHM10_SendChar('-');
		num *= -1;
	}

	for ( i = 7; i > 0; i--)
	{
		if ((num / BLE_pow_of(10, i-1)) != 0)
		{
			num_flag = 1;
			BLEHM10_SendChar(num/BLE_pow_of(10, i-1) + '0');
		}
		else
		{
			if (num_flag != 0)
			{
				BLEHM10_SendChar('0');
			}
		}
		
		num %= BLE_pow_of(10, i-1);
	}
}

void BLEHM10_TIMER_IRQHandler(void)
{
	TIM_ClearITPendingBit(BLE_TIMER_CHANNEL, TIM_IT_Update);
	
	ble_tim_cnt1++;
  if (ble_tim_cnt1 >= ble_tim_MUL1)
  {
		ble_tim_cnt1 = 0;
		ble_tim_flag1 = 1;
  }
	
	ble_tim_cnt2++;
  if (ble_tim_cnt2 >= ble_tim_MUL2)
  {
		ble_tim_cnt2 = 0;
		ble_tim_flag2 = 1;
  }
}

void BLEHM10_SetTimer1(unsigned int ms)
{
	ble_tim_MUL1 = ms;
	ble_tim_cnt1 = 0;
	ble_tim_flag1 = 0;
}

char BLEHM10_isTimer1OVF(void)
{
	if (ble_tim_flag1)
	{
		ble_tim_flag1 = 0;
		return 1;
	}
	else
		return 0;
}

void BLEHM10_SetTimer2(unsigned int ms)
{
	ble_tim_MUL2 = ms;
	ble_tim_cnt2 = 0;
	ble_tim_flag2 = 0;
}

char BLEHM10_isTimer2OVF(void)
{
	if (ble_tim_flag2)
	{
		ble_tim_flag2 = 0;
		return 1;
	}
	else
		return 0;
}

void BLEHM10_CheckAlive(void)
{
	if (BLE_STATUS == BLESTT_AVAILABLE || BLE_STATUS == BLESTT_POWER_ON)
	{
		if (BLEHM10_SendCommand("AT", "OK", 250))
		{
			BLE_STATUS = BLESTT_AVAILABLE;
			if (BLE_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "[BLE] AT OK    ");
		}
		else
		{
			if (BLE_LCD_SUPPORT)
				LCD2004_PrintString(1, 5, "[BLE] AT ERROR ");
		}
	}
}

