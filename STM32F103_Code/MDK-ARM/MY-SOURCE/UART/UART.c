/* Includes ------------------------------------------------------------------*/
#include "UART.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile USART_RxDataTypedef Rx1_Data;
volatile USART_RxDataTypedef Rx2_Data;
volatile USART_RxDataTypedef Rx3_Data;
volatile USART_RxDataTypedef Rx4_Data;
volatile USART_RxDataTypedef Rx5_Data;

volatile uint8_t Rx1_CircularBuffer[RX1_BUFFER_SIZE];
volatile uint8_t Rx2_CircularBuffer[RX2_BUFFER_SIZE];
volatile uint8_t Rx3_CircularBuffer[RX3_BUFFER_SIZE];
volatile uint8_t Rx4_CircularBuffer[RX4_BUFFER_SIZE];
volatile uint8_t Rx5_CircularBuffer[RX5_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*******************************************************************************
* Function Name  : USARTx_Configuration
* Description    : Configures GPIO for UART1, UART operation mode and start.
* Input          : Speed, length, stop bit, parity...
* Output         : None
* Return         : None
*******************************************************************************/	
void USART1_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(USART1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	
	/* Configure USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART1_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
	
	/* Configure USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART1_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
	
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
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}

void USART2_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(USART2_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	
	/* Configure USART2 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART2_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART2_GPIO, &GPIO_InitStructure);
	
	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART2_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_GPIO, &GPIO_InitStructure);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
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
	USART_Init(USART2, &USART_InitStructure);
	
	/* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
}

void USART3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(USART3_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	
	/* Configure USART3 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART3_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART3_GPIO, &GPIO_InitStructure);
	
	/* Configure USART3 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART3_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART3_GPIO, &GPIO_InitStructure);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
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
	USART_Init(USART3, &USART_InitStructure);
	
	/* Enable USART3 Receive and Transmit interrupts */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
}

void UART4_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(UART4_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure UART4 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = UART4_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART4_GPIO, &GPIO_InitStructure);

	/* Configure UART4 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = UART4_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART4_GPIO, &GPIO_InitStructure);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

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
	USART_Init(UART4, &USART_InitStructure);

	/* Enable UART4 Receive and Transmit interrupts */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(UART4, ENABLE);
}

void UART5_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(UART5_GPIO_Tx_CLK | RCC_APB2Periph_AFIO, ENABLE);
	
	/* Configure UART5 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = UART5_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART5_GPIO_Tx, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(UART5_GPIO_Rx_CLK | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure UART5 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = UART5_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART5_GPIO_Rx, &GPIO_InitStructure);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

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
	USART_Init(UART5, &USART_InitStructure);

	/* Enable UART5 Receive and Transmit interrupts */
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(UART5, ENABLE);
}

/*******************************************************************************
* Function Name  : USART1_SendChar
* Description    : Send a character to Tx buffer of USART1 (blocking).
* Input          : 8 bits character
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_SendChar(char chr)
{
	USART_SendData(USART1, chr);    
	/* Loop until USARTy DR register is empty */ 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
}

void USART2_SendChar(char chr)
{
	USART_SendData(USART2, chr);    
	/* Loop until USARTy DR register is empty */ 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
	}
}

void USART3_SendChar(char chr)
{
	USART_SendData(USART3, chr);    
	/* Loop until USARTy DR register is empty */ 
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
	{
	}
}

void UART4_SendChar(char chr)
{
	USART_SendData(UART4, chr);    
	/* Loop until USARTy DR register is empty */ 
	while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
	{
	}
}

void UART5_SendChar(char chr)
{
	USART_SendData(UART5, chr);    
	/* Loop until USARTy DR register is empty */ 
	while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
	{
	}
}
/*******************************************************************************
* Function Name  : USART1_SendString
* Description    : Send a string to Tx buffer of USART1 (blocking).
* Input          : 8 bits character
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_SendString(char* str)
{
	while(*str)	
	{
		USART1_SendChar(*str++);
	}
}

void USART2_SendString(char* str)
{
	while(*str)	
	{
    USART2_SendChar(*str++);
	}
}

void USART3_SendString(char* str)
{
	while(*str)	
	{
		USART3_SendChar(*str++);
	}
}

void UART4_SendString(char* str)
{
	while(*str)	
	{
		UART4_SendChar(*str++);
	}
}

void UART5_SendString(char* str)
{
	while(*str)	
	{
		UART5_SendChar(*str++);
	}
}

/*******************************************************************************
* Function Name  : USART1_IRQ_Handler
* Description    : Receive interrupts IRQHandle
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQ_Handler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{	  	   
		Rx1_CircularBuffer[Rx1_Data.Index_Write] = USART_ReceiveData(USART1);
		
		Rx1_Data.Index_Write = (Rx1_Data.Index_Write + 1) % RX1_BUFFER_SIZE;	
		if (++Rx1_Data.Available == RX1_BUFFER_SIZE)
		{
			Rx1_Data.Available = 0;
			Rx1_Data.Overflow = 1;
		}
	}	
}

void USART2_IRQ_Handler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{	  	   
		Rx2_CircularBuffer[Rx2_Data.Index_Write] = USART_ReceiveData(USART2);
		
		Rx2_Data.Index_Write = (Rx2_Data.Index_Write + 1) % RX2_BUFFER_SIZE;	
		if (++Rx2_Data.Available == RX2_BUFFER_SIZE)
		{
			Rx2_Data.Available = 0;
			Rx2_Data.Overflow = 1;
		}
	} 
}

void USART3_IRQ_Handler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{	  	   
		Rx3_CircularBuffer[Rx3_Data.Index_Write] = USART_ReceiveData(USART3);
		
		Rx3_Data.Index_Write = (Rx3_Data.Index_Write + 1) % RX3_BUFFER_SIZE;
		if (++Rx3_Data.Available == RX3_BUFFER_SIZE)
		{
			Rx3_Data.Available = 0;
			Rx3_Data.Overflow = 1;
		}
	}
}

void UART4_IRQ_Handler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{	  	   
		Rx4_CircularBuffer[Rx4_Data.Index_Write] = USART_ReceiveData(UART4);
		
		Rx4_Data.Index_Write = (Rx4_Data.Index_Write + 1) % RX4_BUFFER_SIZE;
		if (++Rx4_Data.Available == RX4_BUFFER_SIZE)
		{
			Rx4_Data.Available = 0;
			Rx4_Data.Overflow = 1;
		}
	}
}

void UART5_IRQ_Handler(void)
{
	if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{	  	   
		Rx5_CircularBuffer[Rx5_Data.Index_Write] = USART_ReceiveData(UART5);
		
		Rx5_Data.Index_Write = (Rx5_Data.Index_Write + 1) % RX5_BUFFER_SIZE;
		if (++Rx5_Data.Available == RX5_BUFFER_SIZE)
		{
			Rx5_Data.Available = 0;
			Rx5_Data.Overflow = 1;
		}
	}
}

/*******************************************************************************
* Function Name  : USART1_GetChar
* Description    : Get 1 byte to Rx buffer of USART1 (blocking).
* Input          : 8 bits character
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t USART1_GetChar(void)
{
  uint8_t data = 0;
  if (USART1_DataAvailable())
  {
    if (Rx1_CircularBuffer[Rx1_Data.Index_Read] != 0)
    {
      data = Rx1_CircularBuffer[Rx1_Data.Index_Read];
      Rx1_Data.Index_Read = (Rx1_Data.Index_Read + 1)%(RX1_BUFFER_SIZE);
      Rx1_Data.Available--;
      if (Rx1_Data.Overflow)
      {
        Rx1_Data.Overflow = 0;
      }
    }
    if (Rx1_Data.Index_Read == Rx1_Data.Index_Write && Rx1_Data.Available)
    {
      Rx1_Data.Available = 0;
    }
    return data;
  }
  return 0;
}

uint8_t USART2_GetChar(void)
{
  uint8_t data = 0;
  if (USART2_DataAvailable())
  {
    if (Rx2_CircularBuffer[Rx2_Data.Index_Read] != 0)
    {
      data = Rx2_CircularBuffer[Rx2_Data.Index_Read];
      Rx2_Data.Index_Read = (Rx2_Data.Index_Read + 1)%(RX2_BUFFER_SIZE);
      Rx2_Data.Available--;
      if (Rx2_Data.Overflow)
      {
        Rx2_Data.Overflow = 0;
      }
    }
    if (Rx2_Data.Index_Read == Rx2_Data.Index_Write && Rx2_Data.Available)
    {
      Rx2_Data.Available = 0;
    }
    return data;
  }
  return 0;
}

uint8_t USART3_GetChar(void)
{
  uint8_t data = 0;
  if (USART3_DataAvailable())
  {
    if (Rx3_CircularBuffer[Rx3_Data.Index_Read] != 0)
    {
      data = Rx3_CircularBuffer[Rx3_Data.Index_Read];
      Rx3_Data.Index_Read = (Rx3_Data.Index_Read + 1)%(RX3_BUFFER_SIZE);
      Rx3_Data.Available--;
      if (Rx3_Data.Overflow)
      {
        Rx3_Data.Overflow = 0;
      }
    }
    if (Rx3_Data.Index_Read == Rx3_Data.Index_Write && Rx3_Data.Available)
    {
      Rx3_Data.Available = 0;
    }
    return data;
  }
  return 0;
}

uint8_t UART4_GetChar(void)
{
  uint8_t data = 0;
  if (UART4_DataAvailable())
  {
    if (Rx4_CircularBuffer[Rx4_Data.Index_Read] != 0)
    {
      data = Rx4_CircularBuffer[Rx4_Data.Index_Read];
      Rx4_Data.Index_Read = (Rx4_Data.Index_Read + 1)%(RX4_BUFFER_SIZE);
      Rx4_Data.Available--;
      if (Rx4_Data.Overflow)
      {
        Rx4_Data.Overflow = 0;
      }
    }
    if (Rx4_Data.Index_Read == Rx4_Data.Index_Write && Rx4_Data.Available)
    {
      Rx4_Data.Available = 0;
    }
    return data;
  }
  return 0;
}

uint8_t UART5_GetChar(void)
{
  uint8_t data = 0;
  if (UART5_DataAvailable())
  {
    if (Rx5_CircularBuffer[Rx5_Data.Index_Read] != 0)
    {
      data = Rx5_CircularBuffer[Rx5_Data.Index_Read];
      Rx5_Data.Index_Read = (Rx5_Data.Index_Read + 1)%(RX5_BUFFER_SIZE);
      Rx5_Data.Available--;
      if (Rx5_Data.Overflow)
      {
        Rx5_Data.Overflow = 0;
      }
    }
    if (Rx5_Data.Index_Read == Rx5_Data.Index_Write && Rx5_Data.Available)
    {
      Rx5_Data.Available = 0;
    }
    return data;
  }
  return 0;
}

/*******************************************************************************
* Function Name  : USART1_DataAvailable
* Description    : Status about data of Rx1
* Input          : None
* Output         : None
* Return         : 0 -> Empty | num -> numData available
*******************************************************************************/
uint8_t USART1_DataAvailable(void)
{
	return Rx1_Data.Available;
}

uint8_t USART2_DataAvailable(void)
{
	return Rx2_Data.Available;
}

uint8_t USART3_DataAvailable(void)
{
	return Rx3_Data.Available;
}

uint8_t UART4_DataAvailable(void)
{
	return Rx4_Data.Available;
}

uint8_t UART5_DataAvailable(void)
{
	return Rx5_Data.Available;
}

/*******************************************************************************
* Function Name  : USARTx_SendGraphData
* Description    : Send data to graph
* Input          : None
* Output         : None
* Return         : 0 -> Empty | 1 -> Data available
*******************************************************************************/
void USART1_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3)
{	
	USART1_SendChar(0xFF);
	USART1_SendChar(Data1/256);
	USART1_SendChar(Data1%256);
	USART1_SendChar(Data2/256);
	USART1_SendChar(Data2%256);
	USART1_SendChar(Data3/256);
	USART1_SendChar(Data3%256);
	USART1_SendChar(0xFE);
}

void USART2_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3)
{	
	USART2_SendChar(0xFF);
	USART2_SendChar(Data1/256);
	USART2_SendChar(Data1%256);
	USART2_SendChar(Data2/256);
	USART2_SendChar(Data2%256);
	USART2_SendChar(Data3/256);
	USART2_SendChar(Data3%256);
	USART2_SendChar(0xFE);
}

void USART3_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3)
{	
	USART3_SendChar(0xFF);
	USART3_SendChar(Data1/256);
	USART3_SendChar(Data1%256);
	USART3_SendChar(Data2/256);
	USART3_SendChar(Data2%256);
	USART3_SendChar(Data3/256);
	USART3_SendChar(Data3%256);
	USART3_SendChar(0xFE);
}

void UART4_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3)
{	
	UART4_SendChar(0xFF);
	UART4_SendChar(Data1/256);
	UART4_SendChar(Data1%256);
	UART4_SendChar(Data2/256);
	UART4_SendChar(Data2%256);
	UART4_SendChar(Data3/256);
	UART4_SendChar(Data3%256);
	UART4_SendChar(0xFE);
}

void UART5_SendGraphData(unsigned int Data1, unsigned int Data2, unsigned int Data3)
{	
	UART5_SendChar(0xFF);
	UART5_SendChar(Data1/256);
	UART5_SendChar(Data1%256);
	UART5_SendChar(Data2/256);
	UART5_SendChar(Data2%256);
	UART5_SendChar(Data3/256);
	UART5_SendChar(Data3%256);
	UART5_SendChar(0xFE);
}

/*******************************************************************************
* Function Name  : USARTx_SendNum
* Description    : Send number
* Input          : Number
* Output         : None
* Return         : None
*******************************************************************************/
unsigned long usart_pow_of(int A, int n)
{
	char i;
	unsigned long temp = 1;
	
  for (i = 0; i < n; i++)
	{
		temp *= A;
	}
    
	return temp;
}

void USART1_SendNum(int num)
{
	char i;
	char num_flag = 0;
	
	if (num == 0)
	{
		USART1_SendChar('0');
		return;
	}
	if(num < 0)
	{
		USART1_SendChar('-');
		num *= -1;
	}

	for ( i = 7; i > 0; i--)
	{
		if ((num / usart_pow_of(10, i-1)) != 0)
		{
			num_flag = 1;
			USART1_SendChar(num/usart_pow_of(10, i-1) + '0');
		}
		else
		{
			if (num_flag != 0)
			{
				USART1_SendChar('0');
			}
		}
		
		num %= usart_pow_of(10, i-1);
	}
}

void USART2_SendNum(int num)
{
	char i;
	char num_flag = 0;
	
	if (num == 0)
	{
		USART2_SendChar('0');
		return;
	}
	if(num < 0)
	{
		USART2_SendChar('-');
		num *= -1;
	}

	for ( i = 7; i > 0; i--)
	{
		if ((num / usart_pow_of(10, i-1)) != 0)
		{
			num_flag = 1;
			USART2_SendChar(num/usart_pow_of(10, i-1) + '0');
		}
		else
		{
			if (num_flag != 0)
			{
				USART2_SendChar('0');
			}
		}
		
		num %= usart_pow_of(10, i-1);
	}
}

void USART3_SendNum(int num)
{
	char i;
	char num_flag = 0;
	
	if (num == 0)
	{
		USART3_SendChar('0');
		return;
	}
	if(num < 0)
	{
		USART3_SendChar('-');
		num *= -1;
	}

	for ( i = 7; i > 0; i--)
	{
		if ((num / usart_pow_of(10, i-1)) != 0)
		{
			num_flag = 1;
			USART3_SendChar(num/usart_pow_of(10, i-1) + '0');
		}
		else
		{
			if (num_flag != 0)
			{
				USART3_SendChar('0');
			}
		}
		
		num %= usart_pow_of(10, i-1);
	}
}

void UART4_SendNum(int num)
{
	char i;
	char num_flag = 0;
	
	if (num == 0)
	{
		UART4_SendChar('0');
		return;
	}
	if(num < 0)
	{
		UART4_SendChar('-');
		num *= -1;
	}

	for ( i = 7; i > 0; i--)
	{
		if ((num / usart_pow_of(10, i-1)) != 0)
		{
			num_flag = 1;
			UART4_SendChar(num/usart_pow_of(10, i-1) + '0');
		}
		else
		{
			if (num_flag != 0)
			{
				UART4_SendChar('0');
			}
		}
		
		num %= usart_pow_of(10, i-1);
	}
}

void UART5_SendNum(int num)
{
	char i;
	char num_flag = 0;
	
	if (num == 0)
	{
		UART5_SendChar('0');
		return;
	}
	if(num < 0)
	{
		UART5_SendChar('-');
		num *= -1;
	}

	for ( i = 7; i > 0; i--)
	{
		if ((num / usart_pow_of(10, i-1)) != 0)
		{
			num_flag = 1;
			UART5_SendChar(num/usart_pow_of(10, i-1) + '0');
		}
		else
		{
			if (num_flag != 0)
			{
				UART5_SendChar('0');
			}
		}
		
		num %= usart_pow_of(10, i-1);
	}
}
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
