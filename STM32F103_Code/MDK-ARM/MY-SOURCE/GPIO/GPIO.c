/* Includes ------------------------------------------------------------------*/
#include "GPIO/GPIO.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Single BUTTON */
uint32_t btn_code[2] = {0, 0};

/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIOx_Init
* Description    : Configures GPIO for LED, SWDIP, BUTTON, BUZZER, I/O...
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void GPIOx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* OUTPUT: LED Configuration*/
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = LED_Pin1 | LED_Pin2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);
	
	/* INPUT: BUTTON Configuration*/
	RCC_APB2PeriphClockCmd(BTN_GPIO_CLK_1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BTN_Pin1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BTN_GPIO_1, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(BTN_GPIO_CLK_2, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BTN_Pin2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BTN_GPIO_2, &GPIO_InitStructure);
	
}

void GPIO_ToogleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if (GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin))
			{
				GPIO_ResetBits(GPIOx, GPIO_Pin);
			}
			else
			{
				GPIO_SetBits(GPIOx, GPIO_Pin);
			}
}
		

void Button_Scan(void)
{
	if (!GPIO_ReadInputDataBit(BTN_GPIO_1, BTN_Pin1))
		btn_code[0]++;
	else
		btn_code[0] = 0;
	
	if (!GPIO_ReadInputDataBit(BTN_GPIO_2, BTN_Pin2))
		btn_code[1]++;
	else
		btn_code[1] = 0;
}

char Button_Pressed(char chr)
{
	if (chr == BTN1)
	{
		if (btn_code[0] == 1)
			return 1;
		else
			return 0;
	}
	else if (chr == BTN2)
	{
		if (btn_code[1] == 1)
			return 1;
		else
			return 0;
	}
	return 0;
}

char Button_LongPressed(char chr)
{
	if (chr == BTN1)
	{
		if (btn_code[0] >= 10 && btn_code[0]%5 == 1)
			return 1;
		else
			return 0;
	}
	else if (chr == BTN2)
	{
		if (btn_code[1] >= 10 && btn_code[1]%5 == 1)
			return 1;
		else
			return 0;
	}
	return 0;
}
/* Private functions ---------------------------------------------------------*/
