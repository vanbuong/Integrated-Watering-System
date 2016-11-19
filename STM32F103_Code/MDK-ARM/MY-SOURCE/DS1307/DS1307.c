#include <stm32f10x.h>
#include "DS1307/DS1307.h"
#include "TIMER/TIMER.h"
#include "LCD/LCD.h"
#include "INPUT165/INPUT165.h"

uint8_t hour = 12, minute = 45, second = 12;
uint8_t day = 4;
uint8_t date = 12, month = 11, year = 16;

void DS1307_Init(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*!< DS1307_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
	RCC_APB2PeriphClockCmd(DS1307_I2C_SCL_GPIO_CLK | DS1307_I2C_SDA_GPIO_CLK, ENABLE);
	
	/*!< DS1307_I2C Periph clock enable */
	RCC_APB1PeriphClockCmd(DS1307_I2C_CLK, ENABLE);
	
	/* GPIO configuration */
	/* Configuration DS1307_I2C pins: SCL */
	GPIO_InitStructure.GPIO_Pin = DS1307_I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DS1307_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	
	/* Configuration DS1307_I2C pins: SDA */
	GPIO_InitStructure.GPIO_Pin = DS1307_I2C_SDA_PIN;
	GPIO_Init(DS1307_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	/* I2C Configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = DS1307_I2C_SPEED;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = DS1307_I2C_OWNADDRESS1;
	
	I2C_Init(DS1307_I2C, &I2C_InitStructure);
	I2C_Cmd(DS1307_I2C, ENABLE);

}

uint8_t BCD2Dec(uint8_t BCD){
	uint8_t L, H;
	L = BCD & 0x0F;
	H = (BCD>>4)*10;
	return (H+L);
}
uint8_t Dec2BCD(uint8_t Dec){
	uint8_t L, H;
	L=Dec % 10;
	H=(Dec/10)<<4;
	return (H+L);
}

void write_ds1307(uint8_t address, uint8_t data)
{
 /*!< While the bus is busy */
  while(I2C_GetFlagStatus(DS1307_I2C, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(DS1307_I2C, ENABLE);
  /*!< Send START condition */
  I2C_GenerateSTART(DS1307_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /*!< Send DS1307ROM address for write */
  I2C_Send7bitAddress(DS1307_I2C, DS1307_I2C_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(DS1307_I2C, address);
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(DS1307_I2C, Dec2BCD(data));
	
	while(I2C_GetFlagStatus(DS1307_I2C, I2C_FLAG_TXE) == RESET);
	I2C_GenerateSTOP(DS1307_I2C, ENABLE);
	_delay_ms(3);
}

uint8_t read_ds1307(uint8_t address)
{
//	static uint32_t timeOut = 0;
	uint8_t data;
	while(I2C_GetFlagStatus(DS1307_I2C, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(DS1307_I2C,ENABLE);
	while(!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(DS1307_I2C, DS1307_I2C_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(DS1307_I2C, address);  
	while(I2C_GetFlagStatus(DS1307_I2C, I2C_FLAG_BTF) == RESET);
	
	/*!< Send STRAT condition a second time */  
	I2C_GenerateSTART(DS1307_I2C, ENABLE);
	while(!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(DS1307_I2C, DS1307_I2C_ADDRESS, I2C_Direction_Receiver);  
	while(I2C_GetFlagStatus(DS1307_I2C, I2C_FLAG_ADDR) == RESET);
	I2C_AcknowledgeConfig(DS1307_I2C, DISABLE);
	(void) DS1307_I2C->SR2;
	
	I2C_GenerateSTOP(DS1307_I2C, ENABLE);
	
	while(I2C_GetFlagStatus(DS1307_I2C, I2C_FLAG_RXNE) == RESET);
	data = I2C_ReceiveData(DS1307_I2C);
 
	while(DS1307_I2C->CR1 & I2C_CR1_STOP);
	
	I2C_AcknowledgeConfig(DS1307_I2C, ENABLE);  
	data = BCD2Dec(data);	
	return data;
}

void SetupTimeForRealTime(void)
{
	write_ds1307(ADDRESS_SECOND, second);
	write_ds1307(ADDRESS_MINUTE, minute);
	write_ds1307(ADDRESS_HOUR, hour);
	write_ds1307(ADDRESS_DAY, day);
	write_ds1307(ADDRESS_DATE, date);
	write_ds1307(ADDRESS_MONTH, month);
	write_ds1307(ADDRESS_YEAR, year);
}

void ReadTime(void)
{
	hour = read_ds1307(ADDRESS_HOUR);
	minute = read_ds1307(ADDRESS_MINUTE);
	second = read_ds1307(ADDRESS_SECOND);
	day = read_ds1307(ADDRESS_DAY);
	date = read_ds1307(ADDRESS_DATE);
	month = read_ds1307(ADDRESS_MONTH);
	year = read_ds1307(ADDRESS_YEAR);
}
void DisplayTimeOnLCD1602(void)
{
    //////day
    switch(day)
    {
        case 1:
            LCD1602_PrintStringBFR(0,0,"SUN");
            break;
        case 2:
            LCD1602_PrintStringBFR(0,0,"MON");
            break;
        case 3:
            LCD1602_PrintStringBFR(0,0,"TUE");
            break;
        case 4: 
            LCD1602_PrintStringBFR(0,0,"WED");
            break;
        case 5: 
            LCD1602_PrintStringBFR(0,0,"THU");
            break;
        case 6:
            LCD1602_PrintStringBFR(0,0,"FRI");
            break;
        case 7:
            LCD1602_PrintStringBFR(0,0,"SAT");
            break;
        default:
            LCD1602_PrintStringBFR(0,0,"ERR");
            break;
    }
    LCD1602_PrintNumBFR(0,4,hour/10);
    LCD1602_PrintNumBFR(0,5,hour%10);
    LCD1602_PrintStringBFR(0,6,":");
    LCD1602_PrintNumBFR(0,7,minute/10);
    LCD1602_PrintNumBFR(0,8,minute%10);
    LCD1602_PrintStringBFR(0,9,":");
    LCD1602_PrintNumBFR(0,10,second/10);
    LCD1602_PrintNumBFR(0,11,second%10);
    
    if ((hour & 0x40) != 0) 
    {
        if ((hour & 0x20) == 0)
            LCD1602_PrintStringBFR(0,14,"AM");
        else
            LCD1602_PrintStringBFR(0,14,"PM");
    }
    else
        LCD1602_PrintStringBFR(0,14,"  ");
    
    switch(month)
    {
        case 1:
            LCD1602_PrintStringBFR(1,2,"JAN");
            break;
        case 2:
            LCD1602_PrintStringBFR(1,2,"FEB");
            break;
        case 3:
            LCD1602_PrintStringBFR(1,2,"MAR");
            break;
        case 4:
            LCD1602_PrintStringBFR(1,2,"APR");
            break;
        case 5:
            LCD1602_PrintStringBFR(1,2,"MAY");
            break;
        case 6:
            LCD1602_PrintStringBFR(1,2,"JUN");
            break;
        case 7:
            LCD1602_PrintStringBFR(1,2,"JUL");
            break;
        case 8:
            LCD1602_PrintStringBFR(1,2,"AUG");
            break;
        case 9:
            LCD1602_PrintStringBFR(1,2,"SEP");
            break;
        case 10:
            LCD1602_PrintStringBFR(1,2,"OCT");
            break;
        case 11:
            LCD1602_PrintStringBFR(1,2,"NOV");
            break;
        case 12:
            LCD1602_PrintStringBFR(1,2,"DEC");
            break;
        default:
            LCD1602_PrintStringBFR(1,2,"ERR");
            break;
    }

    LCD1602_PrintStringBFR(1,5," ");
		LCD1602_PrintNumBFR(1,6,date/10);
		LCD1602_PrintNumBFR(1,7,date%10);
		
    LCD1602_PrintStringBFR(1,8," ");
    LCD1602_PrintNumBFR(1,9,20);
    LCD1602_PrintNumBFR(1,11,year/10);
		LCD1602_PrintNumBFR(1,12,year%10);
}
void DisplayTime(void)
{
	ReadTime();
	DisplayTimeOnLCD1602();
}

uint8_t isUpButton(void)
{
    if (key_code[1] == 1)
    {
        return 1;
    }
    else
        return 0;
}

uint8_t isDownButton(void)
{
    if (key_code[9] == 1)
    {
        return 1;
    }
    else
        return 0;
}
uint8_t isNextButton(void)
{
	if (key_code[6] == 1)
    {
        return 1;
    }
    else
        return 0;
}
uint8_t isPreviousButton(void)
{
    if (key_code[4] == 1)
    {
        return 1;
    }
    else
        return 0;
}

uint8_t isSwitchButton(void)
{
    if (key_code[3] == 1)
    {
        return 1;
    }
    else
        return 0;
}

uint8_t isBackButton(void)
{
    if (key_code[11] == 1)
       {
        return 1;
    }
    else
        return 0;
}

uint8_t isEnterButton(void)
{
    if (key_code[5] == 1)
    {
        return 1; 
    }
    else
        return 0;
}

uint8_t isSaveButton(void)
{
    if (key_code[7] == 1)
    {
        return 1;
    }
    else
        return 0;
}

uint8_t isLeapYear(int8_t year)
{
    if (year == 0 || year%4 == 0)
        return 1;
    else
        return 0;
}
