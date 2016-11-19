/* Includes ------------------------------------------------------------------*/
#include "RS485.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : RS485_Init
* Description    : Configures GPIO for RS485 Communication (USART3 | DirEn_Pin)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void RS485_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* OUTPUT: Dir Enable Pin */
	RCC_APB2PeriphClockCmd(RS485_DirEN_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = RS485_DirEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS485_DirEN_GPIO, &GPIO_InitStructure);
	
	USART3_Init();
	RS485_DirEN(RS485Receive);
}

void RS485_DirEN(RS485_Status RS485_Stt)
{
	if (RS485_Stt == RS485Transmit)
	{
		GPIO_SetBits(RS485_DirEN_GPIO, RS485_DirEN_PIN);
	}
	else
	{
		GPIO_ResetBits(RS485_DirEN_GPIO, RS485_DirEN_PIN);
	}
}

void RS485_SendChar(char chr)
{
	RS485_DirEN(RS485Transmit);
	USART3_SendChar(chr);
	RS485_DirEN(RS485Receive);
}

void RS485_SendString(char* str)
{
	RS485_DirEN(RS485Transmit);
	USART3_SendString(str);
	USART3_SendChar('@');
	RS485_DirEN(RS485Receive);
}

uint8_t RS485_DataAvailable(void)
{
	uint8_t temp = USART3_DataAvailable();
	return temp;
}

uint8_t RS485_GetChar(void)
{
	uint8_t temp = USART3_GetChar();
	return temp;
}

/* Private functions ---------------------------------------------------------*/
