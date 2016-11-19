/* Includes ------------------------------------------------------------------*/
#include "DS18B20.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
void OW_SetInput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(OW_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  OW_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(OW_GPIO, &GPIO_InitStructure);
}

void OW_SetOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(OW_GPIO_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = OW_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(OW_GPIO, &GPIO_InitStructure);
}

unsigned char OW_ReadBit(void)
{
	unsigned char z = 0;
	OW_SetOutput();
	OW_DQ(0);
	delay_us(6);
//	CLR_OP_1WIRE();// set pin low
//	delay_us(6);
	OW_SetInput();
	//SET_OP_1WIRE();//set pin high
	delay_us(8);
	//ds18b20_in();//set pin in

	z = OQ_DQ_Read();// read pin
	//delay_us(60);
	

//	SET_OP_1WIRE(); //set pin high
//	delay_us(10);
	return z;
}

void OW_WriteBit0(void)
{
	OW_SetOutput();
	OW_DQ(0);
	delay_us(65);
	OW_SetInput();
	delay_us(5);
}

void OW_WriteBit1(void)
{
	OW_SetOutput();
	OW_DQ(0);
	delay_us(6);
	OW_SetInput();
	delay_us(64);
	
}

unsigned char OW_ReadByte(void)
{
	char i, tmp = 1, data = 0;
//	SET_OP_1WIRE();// set pin high
//	delay_us(2);
	for(i = 0; i < 8; i++)
	{
		if (OW_ReadBit())
			data += tmp;
		
		tmp *= 2;
		delay_us(60);
	}
	return (data);
}

void OW_WriteByte(unsigned char data)
{
	char i, temp;
//	ds18b20_out();// pin output
	for(i = 0; i < 8; i++)
	{
		temp = data & 0x01;
		if(temp)
			OW_WriteBit1();
		else 
			OW_WriteBit0();
		
		data >>= 1;
	}
}

void OW_Reset(void)
{
	OW_SetOutput();
	OW_DQ(0);
	delay_us(250);
	delay_us(230);
	OW_SetInput();
	delay_us(70);
	delay_us(410);
	
}

int DS18B20_ReadTemp()
{
	int temperature;
	uint16_t ulTemp = 0;
	
	OW_Reset();
 	OW_WriteByte(0xCC);
  OW_WriteByte(0x44);
	delay_us(10);
	OW_Reset();
 	OW_WriteByte(0xCC);
	OW_WriteByte(0xBE);
	ulTemp = OW_ReadByte();
	ulTemp |= (OW_ReadByte() << 8);
	temperature = (6*ulTemp) + ulTemp / 4;
	temperature = temperature/100;
	
	/*if (ulTemp > 2097)
    {
        ulTemp = 65536 - ulTemp;
        tem = -(((ulTemp & 0x7F0) >> 4)*1.0 + (ulTemp & 0xf)*0.0625);
    }
    else
    {
        tem = ((ulTemp & 0x7F0) >> 4)*1.0 + (ulTemp & 0xf)*0.0625;
    }
	*/
	
	return temperature ;
}
