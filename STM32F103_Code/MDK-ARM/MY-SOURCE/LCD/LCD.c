/* Includes ------------------------------------------------------------------*/
#include "LCD/LCD.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int current_row, current_col;
char statusLCD = LCD_SCREEN_0;

unsigned char LCD_SIZE;
	
unsigned char LcdScreen1602[2][16] = {
																			0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,
																			0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00
																		 };

unsigned char LcdScreen2004[4][20] = {
																			0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,
																			0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,
																			0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,
																			0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,
																		 };

/* 8 user defined characters to be loaded into CGRAM (used for bargraph)      */
const unsigned char UserFont[8][8] = {
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
  { 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10 },
  { 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 },
  { 0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C },
  { 0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E },
  { 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F },
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : LCD_DATA_DIR_IN | LCD_DATA_DIR_OUT | LCD_ALL_DIR_OUT
* Description    : Functions for config I/O
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DATA_DIR_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	/* LCD1602 Pins Configuration -> IN */
	// DB4
	RCC_APB2PeriphClockCmd(DB4_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DB4_GPIO_PORT, &GPIO_InitStructure);
	// DB5
	RCC_APB2PeriphClockCmd(DB5_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DB5_GPIO_PORT, &GPIO_InitStructure);
	// DB6
	RCC_APB2PeriphClockCmd(DB6_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DB6_GPIO_PORT, &GPIO_InitStructure);
	// DB7
	RCC_APB2PeriphClockCmd(DB7_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DB7_GPIO_PORT, &GPIO_InitStructure);
}

void LCD_DATA_DIR_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* LCD1602 Pins Configuration -> OUT */
	// DB4
	RCC_APB2PeriphClockCmd(DB4_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DB4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DB4_GPIO_PORT, &GPIO_InitStructure);
	// DB5
	RCC_APB2PeriphClockCmd(DB5_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DB5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DB5_GPIO_PORT, &GPIO_InitStructure);
	// DB6
	RCC_APB2PeriphClockCmd(DB6_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DB6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DB6_GPIO_PORT, &GPIO_InitStructure);
	// DB7
	RCC_APB2PeriphClockCmd(DB7_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DB7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DB7_GPIO_PORT, &GPIO_InitStructure);
}

void LCD_ALL_DIR_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* LCD1602 Data Pins Configuration -> OUT */
	LCD_DATA_DIR_OUT();
	
	/* LCD1602 Option Pins Configuration -> OUT */
	// EN
	RCC_APB2PeriphClockCmd(EN_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EN_GPIO, &GPIO_InitStructure);
	// RW
	RCC_APB2PeriphClockCmd(RW_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = RW_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RW_GPIO, &GPIO_InitStructure);
	// RS
	RCC_APB2PeriphClockCmd(RS_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = RS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS_GPIO, &GPIO_InitStructure);	
}

unsigned char PINS_DATA(void)
{
	unsigned char pins_data = 0x00;
	
	if (DB7)
		pins_data = (pins_data | 0x08);
	if (DB6)
		pins_data = (pins_data | 0x04);
	if (DB5)
		pins_data = (pins_data | 0x02);
	if (DB4)
		pins_data = (pins_data | 0x01);
	
	return pins_data;
}

unsigned char LCD_DATA_IN(void)
{
	return PINS_DATA();
}

void LCD_DATA_OUT(unsigned char data)
{	
	if ((data & 0x08) == 0x08)
		GPIO_WriteBit(DB7_GPIO_PORT, DB7_PIN, Bit_SET);
	else
		GPIO_WriteBit(DB7_GPIO_PORT, DB7_PIN, Bit_RESET);
	
	if ((data & 0x04) == 0x04)
		GPIO_WriteBit(DB6_GPIO_PORT, DB6_PIN, Bit_SET);
	else
		GPIO_WriteBit(DB6_GPIO_PORT, DB6_PIN, Bit_RESET);
	
	if ((data & 0x02) == 0x02)
		GPIO_WriteBit(DB5_GPIO_PORT, DB5_PIN, Bit_SET);
	else
		GPIO_WriteBit(DB5_GPIO_PORT, DB5_PIN, Bit_RESET);
	
	if ((data & 0x01) == 0x01)
		GPIO_WriteBit(DB4_GPIO_PORT, DB4_PIN, Bit_SET);
	else
		GPIO_WriteBit(DB4_GPIO_PORT, DB4_PIN, Bit_RESET);

}

/*******************************************************************************
* Function Name  : LCD_Delay | LCD_ReadStatus | LCD_WaitWhileBusy | LCD_Write4bit
									| LCD_WriteCMD | LCD_WriteData | LCD_PutChar
* Description    : Delay in while loop cycles
* Input          : cnt -> number of while cycles to LCD_Delay
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Delay (int cnt)
{
  cnt <<= DELAY_2N;

  while (cnt--);
}

unsigned char LCD_ReadStatus(void)
{
  unsigned char status;

  LCD_DATA_DIR_IN();
  LCD_RS(0)
  LCD_RW(1)
  LCD_Delay(10);
  LCD_EN(1)
  LCD_Delay(10);
  status = LCD_DATA_IN() << 4;
  LCD_EN(0)
  LCD_Delay(10);
  LCD_EN(1)
  LCD_Delay(10);
  status |= LCD_DATA_IN();
  LCD_EN(0)
  LCD_DATA_DIR_OUT();
  return (status);
}

unsigned char LCD_WaitWhileBusy(void)
{
  unsigned char status;

  do  {
    status = LCD_ReadStatus();
		//USB_Send_Data(&status,1);
  }  while (status & 0x80);             /* Wait for busy flag                 */

  return (status);
}

void LCD_Write4bit(unsigned char c)
{
  LCD_RW(0)
  LCD_EN(1)
  LCD_DATA_OUT(c&0x0F);
  LCD_Delay(10);
  LCD_EN(0)
  LCD_Delay(10);
}

void LCD_WriteCMD(unsigned char c)
{
  LCD_WaitWhileBusy();

  LCD_RS(0)
  LCD_Write4bit(c>>4);
  LCD_Write4bit(c);
}

void LCD_WriteData(unsigned char c)
{
  LCD_WaitWhileBusy();

  LCD_RS(1)
  LCD_Write4bit(c>>4);
  LCD_Write4bit(c);
}

void LCD_PutChar(char c)
{ 
  LCD_WriteData(c);
}


/*******************************************************************************
* Function Name  : LCD_Init
* Description    : Configures GPIO for LCD1602 2004, LCD mode 4bits
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void LCD_Init(unsigned char lcd_size)
{
	int i;
  unsigned char const *p;

	if (lcd_size == LCD_1602)
		LCD_SIZE = LCD_1602;
	else if (lcd_size == LCD_2004)
		LCD_SIZE = LCD_2004;
	
	/* Remap config for pin D5(PB4-NJTRST) & D6(PB3_JTDO) & D7(PA15-JTDI)  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);											//	JTAG-DP Disabled and SW-DP Enabled
	
	/* Config all pins for LCD as outputs */
	LCD_ALL_DIR_OUT();
	
	LCD_Delay(15000);
  LCD_RS(0)
  LCD_Write4bit(0x3);                  /* Select 4-bit interface             */
  LCD_Delay(4100);
  LCD_Write4bit(0x3);
  LCD_Delay(100);
  LCD_Write4bit(0x3);
  LCD_Write4bit(0x2);

  LCD_WriteCMD(0x28);                  /* 2 lines, 5x8 character matrix      */
  LCD_WriteCMD(0x0C);                  /* Display ctrl:Disp=ON,Curs/Blnk=OFF */
  LCD_WriteCMD(0x06);                  /* Entry mode: Move right, no shift   */

  /* Load user-specific characters into CGRAM */
  LCD_WriteCMD(0x40);                  /* Set CGRAM address counter to 0     */
  p = &UserFont[0][0];
  for (i = 0; i < sizeof(UserFont); i++, p++)
    LCD_PutChar(*p);

  LCD_WriteCMD(0x80);                  /* Set DDRAM address counter to 0     */
	
}

/*******************************************************************************
* Function Name  : LCD_SetCursorPos | LCD_Print | LCD_PowerOf | LCD_PrintNum
									| LCD_PrintCharBFR
* Description    : Set cursor position on LCD display
* Input          : Row -> row position
									 Column -> column position
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SetCursorPos(unsigned char row, unsigned char column)
{
  unsigned char address;
	
	switch (row)
	{
		case 0: address = 0x80; break;		// address if 1st line
		case 1: address = 0xC0; break;		// address if 1st line
		case 2: address = 0x94; break;		// address if 1st line
		case 3: address = 0xD4; break;		// address if 1st line
		default: break;
	}
	
	address += column;
  LCD_WriteCMD(address);               /* Set DDRAM address counter to 0     */											  
}

void LCD_Print(unsigned char const *string)
{
  while (*string)
	{
    LCD_PutChar(*string++);
  }
}

unsigned long LCD_PowerOf(int A, int x) {
	char i;
	unsigned long temp = 1;
	for(i = 0; i < x; i++)
		temp *= A;
	return temp;
}

void LCD_PrintNum(long num) {
	char num_flag = 0;
	char i;
	
	if(num == 0)
	{
		LCD_Print("0");
		return;
	}
	
	if(num < 0)
	{
		LCD_PutChar('-');
		num *= -1;
	}
	//else
	//	LCD_PutChar(' ');
		
	for(i = 10; i > 0; i--) 
	{
		if((num / LCD_PowerOf(10, i-1)) != 0) 
		{
			num_flag = 1;
			LCD_PutChar(num/LCD_PowerOf(10, i-1) + '0');
		}
		else
		{
			if(num_flag != 0)
				LCD_PutChar('0');
		}
		num %= LCD_PowerOf(10, i-1);
	}	
}

void LCD_PrintCharBFR(unsigned char c)
{
	if (LCD_SIZE == LCD_1602)
		LcdScreen1602[current_row][ current_col] = c;
	else if (LCD_SIZE == LCD_2004)
		LcdScreen2004[current_row][ current_col] = c;
	current_col ++;
}

/*******************************************************************************
* Function Name  : LCD1602_Clear | LCD1602_PrintChar | LCD1602_PrintNum | LCD1602_PrintString
								 | LCD2004_Clear | LCD2004_PrintChar | LCD2004_PrintNum | LCD2004_PrintString
* Description    : Print sting to LCD display
* Input          : row, col, string
* Output         : None
* Return         : None
*******************************************************************************/
void LCD1602_Clear(void)
{
  LCD_WriteCMD(0x01);                  /* Display clear                      */
  LCD_SetCursorPos(0, 0);
}

void LCD1602_PrintChar(unsigned char row, unsigned char column, char c)
{
	row = row%2;
	column = column%16;
	LCD_SetCursorPos(row, column);
	LCD_PutChar(c);
}

void LCD1602_PrintNum(unsigned char row, unsigned char column, long num)
{
	row = row%2;
	column = column%16;
	LCD_SetCursorPos(row,column);
	LCD_PrintNum(num);
}

void LCD1602_PrintString(unsigned char row, unsigned char column, unsigned char const * string)
{
	row = row%2;
	column = column%16;
	LCD_SetCursorPos(row,column);
	LCD_Print(string);
}

void LCD2004_Clear(void)
{
  LCD_WriteCMD(0x01);                  /* Display clear                      */
  LCD_SetCursorPos(0, 0);
}

void LCD2004_PrintChar(unsigned char row, unsigned char column, char c)
{
	row = row%4;
	column = column%20;
	LCD_SetCursorPos(row, column);
	LCD_PutChar(c);
}

void LCD2004_PrintNum(unsigned char row, unsigned char column, long num)
{
	row = row%4;
	column = column%20;
	LCD_SetCursorPos(row, column);
	LCD_PrintNum(num);
}

void LCD2004_PrintString(unsigned char row, unsigned char column, unsigned char const * string)
{
	row = row%4;
	column = column%20;
	LCD_SetCursorPos(row, column);
	LCD_Print(string);
}
/*******************************************************************************
* Function Name  : LCD1602_ClearBFR | LCD1602_PrintCharBFR | LCD1602_PrintNumBFR 
								 | LCD1602_PrintStringBFR
								 | LCD2004_ClearBFR | LCD2004_PrintCharBFR | LCD2004_PrintNumBFR 
								 | LCD2004_PrintStringBFR
* Description    : ...
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD1602_ClearBFR()
{
    char i;
    for (i = 0; i<16; i++)
    {
        LcdScreen1602[0][i] = ' ';
        LcdScreen1602[1][i] = ' ';
    }
}

void LCD1602_PrintCharBFR(unsigned char row, unsigned char column,unsigned char c)
{
	current_row = row%2;
	current_col = column%16;
	LcdScreen1602[current_row][current_col] = c;
}
void LCD1602_PrintNumBFR(unsigned char row, unsigned char column, long num)
{
	char num_flag = 0;
	char i;
	current_row = row%2;
	current_col = column%16;

	if(num == 0) 
	{
		LCD_PrintCharBFR('0');
		return;
	}
	if(num < 0)
	{
		LCD_PrintCharBFR('-');
		num *= -1;
	}
	//else
	//	LCD_PrintCharBFR(' ');

	for(i = 7; i > 0; i--)
	{
		if((num / LCD_PowerOf(10, i-1)) != 0)
		{
			num_flag = 1;
			LCD_PrintCharBFR(num/LCD_PowerOf(10, i-1) + '0');
		}
		else
		{
			if(num_flag != 0)
				LCD_PrintCharBFR('0');
		}
		num %= LCD_PowerOf(10, i-1);
	}
}

void LCD1602_PrintStringBFR(unsigned char row, unsigned char column, const unsigned char *string)
{
	current_row = row%2;
	current_col = column%16;
	while (*string)
	{
		LCD_PrintCharBFR (*string++);
	}
}

void LCD2004_ClearBFR()
{
    char i;
    for (i = 0; i<20; i++)
    {
			LcdScreen2004[0][i] = ' ';
			LcdScreen2004[1][i] = ' ';
			LcdScreen2004[2][i] = ' ';
			LcdScreen2004[3][i] = ' ';
    }
}

void LCD2004_PrintCharBFR(unsigned char row, unsigned char column,unsigned char c)
{
	current_row = row%4;
	current_col = column%20;
	LcdScreen2004[current_row][current_col] = c;
}

void LCD2004_PrintNumBFR(unsigned char row, unsigned char column, long num)
{
	char num_flag = 0;
	char i;
	current_row = row%4;
	current_col = column%20;

	if(num == 0) 
	{
		LCD_PrintCharBFR('0');
		return;
	}
	if(num < 0)
	{
		LCD_PrintCharBFR('-');
		num *= -1;
	}
	//else
	//	LCD_PrintCharBFR(' ');

	for(i = 7; i > 0; i--)
	{
		if((num / LCD_PowerOf(10, i-1)) != 0)
		{
			num_flag = 1;
			LCD_PrintCharBFR(num/LCD_PowerOf(10, i-1) + '0');
		}
		else
		{
			if(num_flag != 0)
				LCD_PrintCharBFR('0');
		}
		num %= LCD_PowerOf(10, i-1);
	}
}

void LCD2004_PrintStringBFR(unsigned char row, unsigned char column, const unsigned char *string)
{
	current_row = row%4;
	current_col = column%20;
	while (*string)
	{
		LCD_PrintCharBFR (*string++);
	}
}

/*******************************************************************************
* Function Name  : LCDxxxx_Display
* Description    : Display Functions
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD1602_Display(void)
{
	unsigned char i;
	switch (statusLCD)
	{
		case LCD_SCREEN_0:
				LCD_SetCursorPos(0,0);
				for (i = 0; i<8; i++)
				{
					LCD_PutChar(LcdScreen1602[0][i]);
				}
				statusLCD = LCD_SCREEN_1;
				break;
		
		 case LCD_SCREEN_1:
				LCD_SetCursorPos(0,8);
				for (i = 0; i<8; i++)
				{
					LCD_PutChar(LcdScreen1602[0][8 + i]);
				}
				statusLCD = LCD_SCREEN_2;
				break;
				
		 case LCD_SCREEN_2:
				LCD_SetCursorPos(1,0);
				for (i = 0; i<8; i++)
				{
					LCD_PutChar(LcdScreen1602[1][i]);
				}
				statusLCD = LCD_SCREEN_3;
				break;
				
		 case LCD_SCREEN_3:
				LCD_SetCursorPos(1,8);
				for (i = 0; i<8; i++)
				{
						LCD_PutChar(LcdScreen1602[1][8 + i]);
				}
				statusLCD = LCD_SCREEN_0;
				break;
				
		default:
				statusLCD = LCD_SCREEN_0;
				break;
	}
}

void LCD2004_Display(void)
{
	
	unsigned char i;
	switch (statusLCD)
	{
		case LCD_SCREEN_0:
				LCD_SetCursorPos(0,0);
				for (i = 0; i < 10; i++)
				{
					LCD_PutChar(LcdScreen2004[0][i]);
				}
				statusLCD = LCD_SCREEN_1;
				break;
		
		 case LCD_SCREEN_1:
				LCD_SetCursorPos(0,10);
				for (i = 0; i < 10; i++)
				{
					LCD_PutChar(LcdScreen2004[0][10 + i]);
				}
				statusLCD = LCD_SCREEN_2;
				break;
				
		 case LCD_SCREEN_2:
				LCD_SetCursorPos(1,0);
				for (i = 0; i < 10; i++)
				{
					LCD_PutChar(LcdScreen2004[1][i]);
				}
				statusLCD = LCD_SCREEN_3;
				break;
				
		 case LCD_SCREEN_3:
				LCD_SetCursorPos(1,10);
				for (i = 0; i < 10; i++)
				{
					LCD_PutChar(LcdScreen2004[1][10 + i]);
				}
				statusLCD = LCD_SCREEN_4;
				break;
				
		 case LCD_SCREEN_4:
				LCD_SetCursorPos(2,0);
				for (i = 0; i < 10; i++)
				{
					LCD_PutChar(LcdScreen2004[2][i]);
				}
				statusLCD = LCD_SCREEN_5;
				break;
				
		 case LCD_SCREEN_5:
				LCD_SetCursorPos(2,10);
				for (i = 0; i < 10; i++)
				{
					LCD_PutChar(LcdScreen2004[2][10+i]);
				}
				statusLCD = LCD_SCREEN_6;
				break;
			
		 case LCD_SCREEN_6:
				LCD_SetCursorPos(3,0);
				for (i = 0; i < 10; i++)
				{
					LCD_PutChar(LcdScreen2004[3][i]);
				}
				statusLCD = LCD_SCREEN_7;
				break;	
				
		 case LCD_SCREEN_7:
				LCD_SetCursorPos(3,10);
				for (i = 0; i < 10; i++)
				{
					LCD_PutChar(LcdScreen2004[3][10+i]);
				}
				statusLCD = LCD_SCREEN_0;
				break;

		default:
				statusLCD = LCD_SCREEN_0;
				break;
	}
}

/* Private functions ---------------------------------------------------------*/
