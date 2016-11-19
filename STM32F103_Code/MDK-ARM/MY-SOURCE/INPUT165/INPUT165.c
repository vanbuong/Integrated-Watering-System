/* Includes ------------------------------------------------------------------*/
#include "INPUT165.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t key_mask[16] = {0x0001, 0x0002, 0x0004, 0x0008,
												 0x0010, 0x0020, 0x0040, 0x0080,
												 0x0100, 0x0200, 0x0400, 0x0800,
												 0x1000, 0x2000, 0x4000, 0x8000};

uint16_t key_code[16] = {0, 0, 0, 0,
												 0, 0, 0, 0,
												 0, 0, 0, 0,
												 0, 0, 0, 0};

/* Private function prototypes -----------------------------------------------*/
void Keyboard_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* OUTPUT: CLK */
	RCC_APB2PeriphClockCmd(INPUT165_CLK_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = INPUT165_CLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(INPUT165_CLK_GPIO, &GPIO_InitStructure);
	
	/* OUTPUT: LOAD */
	RCC_APB2PeriphClockCmd(INPUT165_LOAD_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = INPUT165_LOAD_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(INPUT165_LOAD_GPIO, &GPIO_InitStructure);
	
	/* INPUT: DATA */
	RCC_APB2PeriphClockCmd(INPUT165_DATA_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = INPUT165_DATA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(INPUT165_DATA_GPIO, &GPIO_InitStructure);
	
	pin165_LOAD(0);
	pin165_CLK(0);
}

uint16_t Input165_Read(void)
{
	uint16_t temp = 0x0000;
	char i;
	pin165_LOAD(1);
	
	for (i = 0; i < 16; i++)
	{
		temp = temp | (pin165_DATA << (15-i));
		pin165_CLK(1);
		//Input165_DelayUs(1);
		pin165_CLK(0);
	}
	
	pin165_LOAD(0);
	temp = (temp >> 8)|(temp << 8);
	return ~temp & 0xFFFF;
}

void Keyboard_Scan(void)
{
	char x;
	uint16_t temp = 0x0000;
	
	temp = Input165_Read();
	for (x = 0; x < 16; x++)
	{
		if ((temp & key_mask[x]) >> x)
		{
			key_code[x]++;
		}
		else
		{
			key_code[x] = 0;
		}
	}
}

int KB_GetIndex(char chr)
{
	switch (chr)
	{
		case '1':
			return 0;
		case '2':
			return 1;
		case '3':
			return 2;
		case 'A':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case 'B':
			return 7;
		case '7':
			return 8;
		case '8':
			return 9;
		case '9':
			return 10;
		case 'C':
			return 11;
		case '*':
			return 12;
		case '0':
			return 13;
		case '#':
			return 14;
		case 'D':
			return 15;
		default:
			return -1;
	}
}

char Keyboard_Pressed(char chr)
{
	int key_index;
	key_index = KB_GetIndex(chr);
	
	if (key_index != -1)
	{
		if (key_code[key_index] > 5 && key_code[key_index] < 15)
			return 1;
		else
			return 0;
	}
	return 0;
}

char Keyboard_LongPressed(char chr)
{
	int key_index;
	key_index = KB_GetIndex(chr);
	
	if (key_index != -1)
	{
		if ((key_code[key_index] > 100) && (key_code[key_index]%100 > 50))
			return 1;
		else
			return 0;
	}
	return 0;
}

void Input165_DelayUs(unsigned int value)
{
	unsigned int i,j;
	for (i = 0; i < value; i++)
		for (j = 0; j < 6; j++);
}
/* Private functions ---------------------------------------------------------*/
