#ifndef __LCD1602_H
#define __LCD1602_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_SCREEN_0		0
#define LCD_SCREEN_1		1
#define LCD_SCREEN_2    2
#define LCD_SCREEN_3		3
#define LCD_SCREEN_4		4
#define LCD_SCREEN_5		5
#define LCD_SCREEN_6		6
#define LCD_SCREEN_7		7

#define LCD_1602				0
#define LCD_2004				1

/*********************** Hardware specific configuration **********************/

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency lcd_delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N     8

/*------------------------- Text LCD size definitions ------------------------*/

#define LineLen     16                  /* Width (in characters)              */
#define NumLines     2                  /* Hight (in lines)                   */

/*-------------------- LCD interface hardware definitions --------------------*/

#define DB4_PIN                         GPIO_Pin_5
#define DB4_GPIO_PORT                   GPIOB
#define DB4_GPIO_CLK                    RCC_APB2Periph_GPIOB 

#define DB5_PIN                         GPIO_Pin_4
#define DB5_GPIO_PORT                   GPIOB
#define DB5_GPIO_CLK                    RCC_APB2Periph_GPIOB 

#define DB6_PIN                         GPIO_Pin_3
#define DB6_GPIO_PORT                   GPIOB
#define DB6_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define DB7_PIN                         GPIO_Pin_15
#define DB7_GPIO_PORT                   GPIOA
#define DB7_GPIO_CLK                    RCC_APB2Periph_GPIOA 

#define DB4															GPIO_ReadInputDataBit(DB4_GPIO_PORT, DB4_PIN)
#define DB5															GPIO_ReadInputDataBit(DB5_GPIO_PORT, DB5_PIN)
#define DB6															GPIO_ReadInputDataBit(DB6_GPIO_PORT, DB6_PIN)
#define DB7															GPIO_ReadInputDataBit(DB7_GPIO_PORT, DB7_PIN)

#define EN_GPIO                   			GPIOC
#define EN_GPIO_CLK                    	RCC_APB2Periph_GPIOC
#define EN_PIN                         	GPIO_Pin_7

#define RW_GPIO			                   	GPIOC
#define RW_GPIO_CLK                    	RCC_APB2Periph_GPIOC
#define RW_PIN                         	GPIO_Pin_9

#define RS_GPIO			                   	GPIOA
#define RS_GPIO_CLK                    	RCC_APB2Periph_GPIOA
#define RS_PIN                         	GPIO_Pin_1

/* Private macro -------------------------------------------------------------*/
#define LCD_EN(x)             x?GPIO_SetBits(EN_GPIO, EN_PIN):GPIO_ResetBits(EN_GPIO, EN_PIN);
#define LCD_RW(x)             x?GPIO_SetBits(RW_GPIO, RW_PIN):GPIO_ResetBits(RW_GPIO, RW_PIN);
#define LCD_RS(x)             x?GPIO_SetBits(RS_GPIO, RS_PIN):GPIO_ResetBits(RS_GPIO, RS_PIN);

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void LCD_Init(unsigned char lcd_size);
void LCD1602_Clear(void);
void LCD2004_Clear(void);

void LCD1602_PrintChar(unsigned char row, unsigned char column, char c);
void LCD1602_PrintNum(unsigned char row, unsigned char column, long num);
void LCD1602_PrintString(unsigned char row, unsigned char column, unsigned char const * string);

void LCD2004_PrintChar(unsigned char row, unsigned char column, char c);
void LCD2004_PrintNum(unsigned char row, unsigned char column, long num);
void LCD2004_PrintString(unsigned char row, unsigned char column, unsigned char const * string);

void LCD1602_ClearBFR(void);
void LCD1602_PrintCharBFR(unsigned char row, unsigned char column,unsigned char c);
void LCD1602_PrintNumBFR(unsigned char row, unsigned char column, long num);
void LCD1602_PrintStringBFR(unsigned char row, unsigned char column, const unsigned char* string);

void LCD2004_ClearBFR(void);
void LCD2004_PrintCharBFR(unsigned char row, unsigned char column,unsigned char c);
void LCD2004_PrintNumBFR(unsigned char row, unsigned char column, long num);
void LCD2004_PrintStringBFR(unsigned char row, unsigned char column, const unsigned char* string);

/* Private functions ---------------------------------------------------------*/
unsigned char PINS_DATA(void);
unsigned char LCD_DATA_IN(void);
void LCD_DATA_OUT(unsigned char data);

void LCD_WriteData(unsigned char c);
void LCD_Write4bit(unsigned char c);
void LCD_WriteCMD(unsigned char c);
unsigned char LCD_WaitWhileBusy(void);
unsigned char LCD_ReadStatus(void);
void LCD_Delay (int cnt);

void LCD_PutChar(char c);
void LCD_PrintNum(long num);
void LCD_SetCursorPos(unsigned char row, unsigned char column);
void LCD_Print(unsigned char const *string);

void LCD_PrintCharBFR(unsigned char c);

unsigned long LCD_PowerOf(int A, int x);

void LCD2004_Display(void);
void LCD1602_Display(void);

#endif
