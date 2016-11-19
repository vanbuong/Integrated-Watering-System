/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "TIMER/TIMER.h"
#include "LCD/LCD.h"
#include "INPUT165/INPUT165.h"
#include "DS1307/DS1307.h"
#include "GPIO/GPIO.h"
#include "EEPROM/EEPROM.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define     DISPLAY_TIME    0x50
#define     SHOW_SETTIME    0x51
#define     SHOW_WATERCALEN 0x52
#define			SHOW_LOADCALEN	0x53
#define			SHOW_CREATECALEN 0x54
#define     SETUP_TIME      0x55
#define 		LOAD_CALENDAR		0x56
#define 		CREATE_CALENDAR	0x57

#define     SETUP_DAY       0x10
#define     SETUP_HOUR      0x11
#define     SETUP_MINUTE    0x12
#define     SETUP_SECOND    0x13
#define     SETUP_MODE_TIME 0x14
#define     SETUP_MONTH     0x15
#define     SETUP_DATE      0x16
#define     SETUP_YEAR      0x17
#define     SETUP_SAVE      0x18
#define     SETUP_BACK      0x19

#define			MONDAY					0x1A
#define			TUESDAY					0x1B
#define			WEDNESDAY				0x1C
#define			THUSDAY					0x1D
#define			FRIDAY					0x1F
#define			SATURDAY				0x20
#define			SUNDAY					0x21

#define			TIMER1					0x22
#define			TIMER2					0x23
#define			TIMER3					0x24
#define			TIMER4					0x25
#define			TIMER5					0x26
#define			TIMER6					0x27
#define			TIMER7					0x28
#define			TIMER8					0x29
#define			TIMER9					0x2A
#define			TIMER10					0x2B
#define			TIMER11					0x2C
#define			TIMER12					0x2D
#define			TIMER13					0x2E
#define			TIMER14					0x2F
#define			TIMER15					0x30
#define			TIMER16					0x31

#define			SHOW_FULL_WEEK	0x32
#define			SHOW_ODD_DAYS		0x33
#define			SHOW_EVEN_DAYS	0x34
#define			SHOW_CHOOSE_DAYS	0x35
#define			FULL_WEEK				0x36
#define			ODD_DAYS				0x37
#define			EVEN_DAYS				0x38
#define			CHOOSE_DAYS			0x39

#define			CRC_4BYTES			0x3FFC

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t CRC_EEPROM 		= 0x5F29A0B2;
uint32_t CRCValue				= 0;
uint32_t CRCTemp				= 0;
uint8_t MainState 			= DISPLAY_TIME;
uint8_t SetupTimeState 	= SETUP_DAY;
uint8_t	TimerState			= TIMER1;
uint8_t	LoadDay					= MONDAY;
uint8_t CreateModeState = SHOW_FULL_WEEK;
uint8_t WriteEEPROMState= 0xff;
uint8_t time_delay 			= 0;
uint8_t isFlicker 			= 0;
uint8_t enableWriteEEPROM = 0;
uint8_t Monday[64] 			= {0};
uint8_t Tuesday[64] 		= {0};
uint8_t Wednesday[64] 	= {0};
uint8_t Thusday[64] 		= {0};
uint8_t Friday[64] 			= {0};
uint8_t Saturday[64] 		= {0};
uint8_t Sunday[64] 			= {0};

/* Private function prototypes -----------------------------------------------*/
void InitSystem(void);
void DisplayFlicker(void);
void SetupTime(void);
void CreateCalendar(void);
void LoadCalendar(void);
void LoadTimer(void);
void PrintTimer(uint8_t row, uint8_t col, uint8_t num);
void WriteCalendarEEPROM(void);
void InitCalendarEEPROM(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  /* Add your application code here
     */
	InitSystem();
	InitCalendarEEPROM();
	_delay_ms(1000);

  /* Infinite loop */
  while (1)
  {
		_delay_ms(10);
		LCD1602_ClearBFR();
		Keyboard_Scan();
		
		switch (MainState) 
			{
					case DISPLAY_TIME: 
							DisplayTime();
							if (isNextButton())
							{
									MainState = SHOW_SETTIME;
									time_delay = 0;
									isFlicker = 0;
							}
							if (isPreviousButton())
									MainState = SHOW_WATERCALEN;
							break;
					case SHOW_SETTIME:
							LCD1602_ClearBFR();
							LCD1602_PrintStringBFR(0,0,"<  SETUP TIME  >");
							if (isNextButton()) 
									MainState = SHOW_WATERCALEN;
							if (isPreviousButton())
									MainState = DISPLAY_TIME;
							if (isEnterButton())
							{
									MainState = SETUP_TIME;
									time_delay = 0;
									isFlicker = 0;
							}
							if (isBackButton())
									MainState = DISPLAY_TIME;
							break;
					case SHOW_WATERCALEN:
							LCD1602_ClearBFR();
							LCD1602_PrintStringBFR(0,0,"<WATER CALENDAR>");
							if (isNextButton())
									MainState = DISPLAY_TIME;
							if (isPreviousButton())
									MainState = SHOW_SETTIME;
							if (isEnterButton())
									MainState = SHOW_LOADCALEN;
							if (isBackButton())
									MainState = DISPLAY_TIME;
							break;
					case SHOW_LOADCALEN:
							LCD1602_ClearBFR();
							LCD1602_PrintStringBFR(0,0,"< LOAD CALENDAR>");
							if (isNextButton())
									MainState = SHOW_CREATECALEN;
							if (isPreviousButton())
									MainState = SHOW_CREATECALEN;
							if (isBackButton())
									MainState = SHOW_WATERCALEN;
							if (isEnterButton())
									MainState = LOAD_CALENDAR;
							break;
					case SHOW_CREATECALEN:
							LCD1602_ClearBFR();
							LCD1602_PrintStringBFR(0,0,"<CREATE CALENDA>");
							if (isNextButton())
									MainState = SHOW_LOADCALEN;
							if (isPreviousButton())
									MainState = SHOW_LOADCALEN;
							if (isBackButton())
									MainState = SHOW_WATERCALEN;
							if (isEnterButton())
									MainState = CREATE_CALENDAR;
							break;
					case SETUP_TIME:
							LCD1602_ClearBFR();
							SetupTime();
							break;
					case LOAD_CALENDAR:
							LCD1602_ClearBFR();
							LoadCalendar();
							break;
					case CREATE_CALENDAR:
							LCD1602_ClearBFR();
							CreateCalendar();
							break;
					default:
							break;
			}
			WriteCalendarEEPROM();
			LCD1602_Display();
  }
}

/* Private functions ---------------------------------------------------------*/
void InitSystem(void) 
{
	SysTick_Init();
	LCD_Init(LCD_1602);
	LCD1602_Clear();
	LCD1602_ClearBFR();
	Keyboard_Init();
	DS1307_Init();
	EEPROM_Init();
	
	/* Enable CRC clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	
}

void DisplayFlicker(void)
{
    switch (SetupTimeState)
    {
        case SETUP_DAY:
            time_delay = time_delay + 1;
            if (isFlicker == 0)
            {
                LCD1602_PrintStringBFR(0,0,"   ");
            }
            if (time_delay >= 10) 
            {
                time_delay = 0;
                if (isFlicker == 0)
                    isFlicker = 1;
                else 
                    isFlicker = 0;
            }
            break;
        case SETUP_HOUR:
            time_delay = time_delay + 1;
            if (isFlicker == 0)
            {
                LCD1602_PrintStringBFR(0,4,"  ");
            }
            if (time_delay >= 10) 
            {
                time_delay = 0;
                if (isFlicker == 0)
                    isFlicker = 1;
                else 
                    isFlicker = 0;
            }
            break;
        case SETUP_MINUTE:
            time_delay = time_delay + 1;
            if (isFlicker == 0)
            {
                LCD1602_PrintStringBFR(0,7,"  ");
            }
            if (time_delay >= 10) 
            {
                time_delay = 0;
                if (isFlicker == 0)
                    isFlicker = 1;
                else 
                    isFlicker = 0;
            }
            break;
        case SETUP_SECOND:
            time_delay = time_delay + 1;
            if (isFlicker == 0)
            {
                LCD1602_PrintStringBFR(0,10,"  ");
            }
            if (time_delay >= 10) 
            {
                time_delay = 0;
                if (isFlicker == 0)
                    isFlicker = 1;
                else 
                    isFlicker = 0;
            }
            break;
        case SETUP_MODE_TIME:
            time_delay = time_delay + 1;
            if (isFlicker == 0)
            {
                LCD1602_PrintStringBFR(0,14,"  ");
            }
            if (time_delay >= 10) 
            {
                time_delay = 0;
                if (isFlicker == 0)
                    isFlicker = 1;
                else 
                    isFlicker = 0;
            }
            break;
        case SETUP_YEAR:
            time_delay = time_delay + 1;
            if (isFlicker == 0)
            {
                LCD1602_PrintStringBFR(1,9,"    ");
            }
            if (time_delay >= 10) 
            {
                time_delay = 0;
                if (isFlicker == 0)
                    isFlicker = 1;
                else 
                    isFlicker = 0;
            }
            break;
        case SETUP_MONTH:
            time_delay = time_delay + 1;
            if (isFlicker == 0)
            {
                LCD1602_PrintStringBFR(1,2,"   ");
            }
            if (time_delay >= 10) 
            {
                time_delay = 0;
                if (isFlicker == 0)
                    isFlicker = 1;
                else 
                    isFlicker = 0;
            }
            break;
        case SETUP_DATE:
            time_delay = time_delay + 1;
            if (isFlicker == 0)
            {
                LCD1602_PrintStringBFR(1,6,"  ");
            }
            if (time_delay >= 10) 
            {
                time_delay = 0;
                if (isFlicker == 0)
                    isFlicker = 1;
                else 
                    isFlicker = 0;
            }
            break;
        default:
            break;
        
    }
}

void SetupTime(void)
{
    switch (SetupTimeState)
    {
        case SETUP_DAY:
            if (isUpButton())
            {
                day = day + 1;
                if (day == 8)
                    day = 1;
            }
            if (isDownButton())
            {
                day = day - 1;
                if (day == 0)
                    day = 7;
            }
            if (isSwitchButton())
            {
                SetupTimeState = SETUP_HOUR;
                time_delay = 0;
            }
            if (isBackButton()) 
                SetupTimeState = SETUP_BACK;
            if (isSaveButton())
                SetupTimeState = SETUP_SAVE;
            break;
        case SETUP_HOUR:
            if (isUpButton())
            {
                hour = hour + 1;
                if (hour == 24)
                    hour = 0;
            }
            if (isDownButton())
            {
                if (hour == 0)
                    hour = 24;
                hour = hour - 1;
                
            }
            if (isSwitchButton())
            {
                SetupTimeState = SETUP_MINUTE;
                time_delay = 0;
            }
            if (isBackButton()) 
                SetupTimeState = SETUP_BACK;
            if (isSaveButton())
                SetupTimeState = SETUP_SAVE;
            break;
        case SETUP_MINUTE:
            if (isUpButton())
            {
                minute = minute + 1;
                if (minute == 60)
                    minute = 0;
            }
            if (isDownButton())
            {
                if (minute == 0)
                    minute = 60;
                minute = minute - 1; 
            }
            if (isSwitchButton())
            {
                SetupTimeState = SETUP_SECOND;
                time_delay = 0;
            }
            if (isBackButton()) 
                SetupTimeState = SETUP_BACK;
            if (isSaveButton())
                SetupTimeState = SETUP_SAVE;
            break;
        case SETUP_SECOND:
            if (isUpButton())
            {
                second = second + 1;
                if (second == 60)
                    second = 0;
            }
            if (isDownButton())
            {
                if (second == 0)
                    second = 60;
                second = second - 1; 
            }
            if (isSwitchButton())
            {
                SetupTimeState = SETUP_YEAR;
                time_delay = 0;
            }
            if (isBackButton()) 
                SetupTimeState = SETUP_BACK;
            if (isSaveButton())
                SetupTimeState = SETUP_SAVE;
            break;
        case SETUP_MODE_TIME:
            if (isSwitchButton())
            {
                SetupTimeState = SETUP_YEAR;
                time_delay = 0;
            }
            if (isBackButton()) 
                SetupTimeState = SETUP_BACK;
            if (isSaveButton())
                SetupTimeState = SETUP_SAVE;
            break;
        case SETUP_YEAR:
            if (isUpButton())
            {
                year = year + 1;
                if (year == 100)
                    year = 0;
            }
            if (isDownButton())
            {
                if (year == 0)
                    year = 100;
                year = year - 1; 
            }
            if (isSwitchButton())
            {
                SetupTimeState = SETUP_MONTH;
                time_delay = 0;
            }
            if (isBackButton()) 
                SetupTimeState = SETUP_BACK;
            if (isSaveButton())
                SetupTimeState = SETUP_SAVE;
            break;
        case SETUP_MONTH:
            if (isUpButton())
            {
                month = month + 1;
                if (month == 13)
                    month = 1;
            }
            if (isDownButton())
            {
                month = month - 1; 
                if (month == 0)
                    month = 12;
            }
            if (isSwitchButton())
            {
                SetupTimeState = SETUP_DATE;
                time_delay = 0;
            }
            if (isBackButton()) 
                SetupTimeState = SETUP_BACK;
            if (isSaveButton())
                SetupTimeState = SETUP_SAVE;
            break;
        case SETUP_DATE:
            if (isUpButton())
            {
                date = date + 1;
                switch (month)
                {
                    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                        if (date == 32)
                            date = 1;
                        break;
                    case 4: case 6: case 9: case 11:
                        if (date == 31)
                            date = 1;
                        break;
                    case 2:
                        if (isLeapYear(year))
                        {
                            if (date == 30)
                                date = 1;
                        }
                        else
                        {
                            if (date == 29)
                                date = 1;
                        }
                        break;
                    default:
                        break;
                }
            }
            if (isDownButton())
            {
                date = date - 1;
                switch (month)
                {
                    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                        if (date == 0)
                            date = 31;
                        break;
                    case 4: case 6: case 9: case 11:
                        if (date == 0)
                            date = 30;
                        break;
                    case 2:
                        if (isLeapYear(year))
                        {
                            if (date == 0)
                                date = 29;
                        }
                        else
                        {
                            if (date == 0)
                                date = 28;
                        }
                        break;
                    default:
                        break;
                }
            }
            if (isSwitchButton())
            {
                SetupTimeState = SETUP_DAY;
                time_delay = 0;
            }
            if (isBackButton()) 
                SetupTimeState = SETUP_BACK;
            if (isSaveButton())
                SetupTimeState = SETUP_SAVE;
            break;
        case SETUP_SAVE:
            SetupTimeForRealTime();
            MainState = SHOW_SETTIME;
            SetupTimeState = SETUP_DAY;
            break;
        case SETUP_BACK:
            MainState = SHOW_SETTIME;
            SetupTimeState = SETUP_DAY;
            break;
        default:
            break;
    }
    DisplayTimeOnLCD1602();
    DisplayFlicker();
}

void InitCalendarEEPROM(void)
{
	/* Compute the CRC of "CRC_EEPROM" */
  CRCValue = CRC_CalcCRC(CRC_EEPROM);
	
	/* Read four byte at the last memory page of EEPROM */
	CRCTemp = EEPROM_ReadLong(CRC_4BYTES);
	
	/* Reset CRC DR Register */
	CRC_ResetDR();
	
	/* Compute the CRC of "CRCTemp" */
	CRCTemp = CRC_CalcCRC(CRCTemp);
	
	if (CRCValue == CRCTemp) 
	{
		/* Read data from EEPROM */
		EEPROM_ReadPage(0x0000, Monday);
		EEPROM_ReadPage(0x0040, Tuesday);
		EEPROM_ReadPage(0x0080, Wednesday);
		EEPROM_ReadPage(0x00C0, Thusday);
		EEPROM_ReadPage(0x0100, Friday);
		EEPROM_ReadPage(0x0140, Saturday);
		EEPROM_ReadPage(0x0180, Sunday);
	}
	else
	{
		/* Init data and write to EEPROM */
		uint8_t i;
		for (i=0;i<64;i++)
		{
			Monday[i] 		= 0;
			Tuesday[i] 		= 0;
			Wednesday[i] 	= 0;
			Thusday[i] 		= 0;
			Friday[i] 		= 0;
			Saturday[i] 	= 0;
			Sunday[i] 		= 0;
		}
		EEPROM_WritePage(0x0000, Monday);
		_delay_ms(3);
		EEPROM_WritePage(0x0040, Tuesday);
		_delay_ms(3);
		EEPROM_WritePage(0x0080, Wednesday);
		_delay_ms(3);
		EEPROM_WritePage(0x00C0, Thusday);
		_delay_ms(3);
		EEPROM_WritePage(0x0100, Friday);
		_delay_ms(3);
		EEPROM_WritePage(0x0140, Saturday);
		_delay_ms(3);
		EEPROM_WritePage(0x0180, Sunday);
		_delay_ms(3);
		EEPROM_WriteLong(0x3FFC, CRC_EEPROM);
	}
	
}

void WriteCalendarEEPROM(void)
{
	switch (WriteEEPROMState)
	{
		case MONDAY:
			EEPROM_WritePage(0x0000, Monday);
			WriteEEPROMState = TUESDAY;
			break;
		case TUESDAY:
			EEPROM_WritePage(0x0040, Tuesday);
			WriteEEPROMState = WEDNESDAY;
			break;
		case WEDNESDAY:
			EEPROM_WritePage(0x0080, Wednesday);
			WriteEEPROMState = THUSDAY;
			break;
		case THUSDAY:
			EEPROM_WritePage(0x00C0, Thusday);
			WriteEEPROMState = FRIDAY;
			break;
		case FRIDAY:
			EEPROM_WritePage(0x0100, Friday);
			WriteEEPROMState = SATURDAY;
			break;
		case SATURDAY:
			EEPROM_WritePage(0x0140, Saturday);
			WriteEEPROMState = SUNDAY;
			break;
		case SUNDAY:
			EEPROM_WritePage(0x0180, Sunday);
			WriteEEPROMState = 0xff;
			break;
		default:
			break;
	}
}
void PrintTimer(uint8_t row, uint8_t col, uint8_t num)
{
	LCD1602_PrintNumBFR(row, col, num/10);
	LCD1602_PrintNumBFR(row, col+1, num%10);
}

void LoadTimer(void)
{
	uint8_t TimerTemp[64];
	uint8_t i;
	switch (LoadDay)
	{
		case MONDAY:
			for (i = 0; i<64; i++) 
				TimerTemp[i] = Monday[i];
			break;
		case TUESDAY:
			for (i = 0; i<64; i++) 
				TimerTemp[i] = Tuesday[i];
			break;
		case WEDNESDAY:
			for (i = 0; i<64; i++) 
				TimerTemp[i] = Wednesday[i];
			break;
		case THUSDAY:
			for (i = 0; i<64; i++) 
				TimerTemp[i] = Thusday[i];
			break;
		case FRIDAY:
			for (i = 0; i<64; i++) 
				TimerTemp[i] = Friday[i];
			break;
		case SATURDAY:
			for (i = 0; i<64; i++) 
				TimerTemp[i] = Saturday[i];
			break;
		case SUNDAY:
			for (i = 0; i<64; i++) 
				TimerTemp[i] = Sunday[i];
			break;
		default:
			break;
	}
	
	switch (TimerState)
	{
		case TIMER1:
			LCD1602_PrintStringBFR(1,0,"TMR1 ");
			PrintTimer(0,11,TimerTemp[0]);
			PrintTimer(0,14,TimerTemp[1]);
			PrintTimer(1,11,TimerTemp[2]);
			PrintTimer(1,14,TimerTemp[3]);
			if (isDownButton())
				TimerState = TIMER2;
			if (isUpButton())
				TimerState = TIMER16;
			break;
		case TIMER2:
			LCD1602_PrintStringBFR(1,0,"TMR2 ");
			PrintTimer(0,11,TimerTemp[4]);
			PrintTimer(0,14,TimerTemp[5]);
			PrintTimer(1,11,TimerTemp[6]);
			PrintTimer(1,14,TimerTemp[7]);
			if (isDownButton())
				TimerState = TIMER3;
			if (isUpButton())
				TimerState = TIMER1;
			break;
		case TIMER3:
			LCD1602_PrintStringBFR(1,0,"TMR3 ");
			PrintTimer(0,11,TimerTemp[8]);
			PrintTimer(0,14,TimerTemp[9]);
			PrintTimer(1,11,TimerTemp[10]);
			PrintTimer(1,14,TimerTemp[11]);
			if (isDownButton())
				TimerState = TIMER4;
			if (isUpButton())
				TimerState = TIMER2;
			break;
		case TIMER4:
			LCD1602_PrintStringBFR(1,0,"TMR4 ");
			PrintTimer(0,11,TimerTemp[12]);
			PrintTimer(0,14,TimerTemp[13]);
			PrintTimer(1,11,TimerTemp[14]);
			PrintTimer(1,14,TimerTemp[15]);
			if (isDownButton())
				TimerState = TIMER5;
			if (isUpButton())
				TimerState = TIMER3;
			break;
		case TIMER5:
			LCD1602_PrintStringBFR(1,0,"TMR5 ");
			PrintTimer(0,11,TimerTemp[16]);
			PrintTimer(0,14,TimerTemp[17]);
			PrintTimer(1,11,TimerTemp[18]);
			PrintTimer(1,14,TimerTemp[19]);
			if (isDownButton())
				TimerState = TIMER6;
			if (isUpButton())
				TimerState = TIMER4;
			break;
		case TIMER6:
			LCD1602_PrintStringBFR(1,0,"TMR6 ");
			PrintTimer(0,11,TimerTemp[20]);
			PrintTimer(0,14,TimerTemp[21]);
			PrintTimer(1,11,TimerTemp[22]);
			PrintTimer(1,14,TimerTemp[23]);
			if (isDownButton())
				TimerState = TIMER7;
			if (isUpButton())
				TimerState = TIMER5;
			break;
		case TIMER7:
			LCD1602_PrintStringBFR(1,0,"TMR7 ");
			PrintTimer(0,11,TimerTemp[24]);
			PrintTimer(0,14,TimerTemp[25]);
			PrintTimer(1,11,TimerTemp[26]);
			PrintTimer(1,14,TimerTemp[27]);
			if (isDownButton())
				TimerState = TIMER8;
			if (isUpButton())
				TimerState = TIMER6;
			break;
		case TIMER8:
			LCD1602_PrintStringBFR(1,0,"TMR8 ");
			PrintTimer(0,11,TimerTemp[28]);
			PrintTimer(0,14,TimerTemp[29]);
			PrintTimer(1,11,TimerTemp[30]);
			PrintTimer(1,14,TimerTemp[31]);
			if (isDownButton())
				TimerState = TIMER9;
			if (isUpButton())
				TimerState = TIMER7;
			break;
		case TIMER9:
			LCD1602_PrintStringBFR(1,0,"TMR9 ");
			PrintTimer(0,11,TimerTemp[32]);
			PrintTimer(0,14,TimerTemp[33]);
			PrintTimer(1,11,TimerTemp[34]);
			PrintTimer(1,14,TimerTemp[35]);
			if (isDownButton())
				TimerState = TIMER10;
			if (isUpButton())
				TimerState = TIMER8;
			break;
		case TIMER10:
			LCD1602_PrintStringBFR(1,0,"TMR10");
			PrintTimer(0,11,TimerTemp[36]);
			PrintTimer(0,14,TimerTemp[37]);
			PrintTimer(1,11,TimerTemp[38]);
			PrintTimer(1,14,TimerTemp[39]);
			if (isDownButton())
				TimerState = TIMER11;
			if (isUpButton())
				TimerState = TIMER9;
			break;
		case TIMER11:
			LCD1602_PrintStringBFR(1,0,"TMR11");
			PrintTimer(0,11,TimerTemp[40]);
			PrintTimer(0,14,TimerTemp[41]);
			PrintTimer(1,11,TimerTemp[42]);
			PrintTimer(1,14,TimerTemp[43]);
			if (isDownButton())
				TimerState = TIMER12;
			if (isUpButton())
				TimerState = TIMER10;
			break;
		case TIMER12:
			LCD1602_PrintStringBFR(1,0,"TMR12");
			PrintTimer(0,11,TimerTemp[44]);
			PrintTimer(0,14,TimerTemp[45]);
			PrintTimer(1,11,TimerTemp[46]);
			PrintTimer(1,14,TimerTemp[47]);
			if (isDownButton())
				TimerState = TIMER13;
			if (isUpButton())
				TimerState = TIMER11;
			break;
		case TIMER13:
			LCD1602_PrintStringBFR(1,0,"TMR13");
			PrintTimer(0,11,TimerTemp[48]);
			PrintTimer(0,14,TimerTemp[49]);
			PrintTimer(1,11,TimerTemp[50]);
			PrintTimer(1,14,TimerTemp[51]);
			if (isDownButton())
				TimerState = TIMER14;
			if (isUpButton())
				TimerState = TIMER12;
			break;
		case TIMER14:
			LCD1602_PrintStringBFR(1,0,"TMR14");
			PrintTimer(0,11,TimerTemp[52]);
			PrintTimer(0,14,TimerTemp[53]);
			PrintTimer(1,11,TimerTemp[54]);
			PrintTimer(1,14,TimerTemp[55]);
			if (isDownButton())
				TimerState = TIMER15;
			if (isUpButton())
				TimerState = TIMER13;
			break;
		case TIMER15:
			LCD1602_PrintStringBFR(1,0,"TMR15");
			PrintTimer(0,11,TimerTemp[56]);
			PrintTimer(0,14,TimerTemp[57]);
			PrintTimer(1,11,TimerTemp[58]);
			PrintTimer(1,14,TimerTemp[59]);
			if (isDownButton())
				TimerState = TIMER16;
			if (isUpButton())
				TimerState = TIMER14;
			break;
		case TIMER16:
			LCD1602_PrintStringBFR(1,0,"TMR16");
			PrintTimer(0,11,TimerTemp[60]);
			PrintTimer(0,14,TimerTemp[61]);
			PrintTimer(1,11,TimerTemp[62]);
			PrintTimer(1,14,TimerTemp[63]);
			if (isDownButton())
				TimerState = TIMER1;
			if (isUpButton())
				TimerState = TIMER15;
			break;
		default: 
			break;
	}
	
}

void LoadCalendar(void)
{
	switch (LoadDay)
	{
		case MONDAY:
			LCD1602_ClearBFR();
			LCD1602_PrintStringBFR(0, 0, "MON  START 00:00");
			LCD1602_PrintStringBFR(1, 0, "TMR1   RUN 00:00");
			LoadTimer();
			if (isNextButton())
			{
				TimerState = TIMER1;
				LoadDay = TUESDAY;
			}			
			if (isPreviousButton())
			{
				TimerState = TIMER1;
				LoadDay = SUNDAY;
			}
			if (isBackButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
				MainState = SHOW_LOADCALEN;
			}
			break;
		case TUESDAY:
			LCD1602_ClearBFR();
			LCD1602_PrintStringBFR(0, 0, "TUE  START 00:00");
			LCD1602_PrintStringBFR(1, 0, "TMR1   RUN 00:00");
			LoadTimer();
			if (isNextButton())
			{
				TimerState = TIMER1;
				LoadDay = WEDNESDAY;
			}
			if (isPreviousButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
			}
			if (isBackButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
				MainState = SHOW_LOADCALEN;
			}
			break;
		case WEDNESDAY:
			LCD1602_ClearBFR();
			LCD1602_PrintStringBFR(0, 0, "WED  START 00:00");
			LCD1602_PrintStringBFR(1, 0, "TMR1   RUN 00:00");
			LoadTimer();
			if (isNextButton())
			{
				TimerState = TIMER1;
				LoadDay = THUSDAY;
			}
			if (isPreviousButton())
			{
				TimerState = TIMER1;
				LoadDay = TUESDAY;
			}
			if (isBackButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
				MainState = SHOW_LOADCALEN;
			}
			break;
		case THUSDAY:
			LCD1602_ClearBFR();
			LCD1602_PrintStringBFR(0, 0, "THU  START 00:00");
			LCD1602_PrintStringBFR(1, 0, "TMR1   RUN 00:00");
			LoadTimer();
			if (isNextButton())
			{
				TimerState = TIMER1;
				LoadDay = FRIDAY;
			}
			if (isPreviousButton())
			{
				TimerState = TIMER1;
				LoadDay = WEDNESDAY;
			}
			if (isBackButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
				MainState = SHOW_LOADCALEN;
			}
			break;
		case FRIDAY:
			LCD1602_ClearBFR();
			LCD1602_PrintStringBFR(0, 0, "FRI  START 00:00");
			LCD1602_PrintStringBFR(1, 0, "TMR1   RUN 00:00");
			LoadTimer();
			if (isNextButton())
			{
				TimerState = TIMER1;
				LoadDay = SATURDAY;
			}
			if (isPreviousButton())
			{
				TimerState = TIMER1;
				LoadDay = THUSDAY;
			}
			if (isBackButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
				MainState = SHOW_LOADCALEN;
			}
			break;
		case SATURDAY:
			LCD1602_ClearBFR();
			LCD1602_PrintStringBFR(0, 0, "SAT  START 00:00");
			LCD1602_PrintStringBFR(1, 0, "TMR1   RUN 00:00");
			LoadTimer();
			if (isNextButton())
			{
				TimerState = TIMER1;
				LoadDay = SUNDAY;
			}
			if (isPreviousButton())
			{
				TimerState = TIMER1;
				LoadDay = FRIDAY;
			}
			if (isBackButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
				MainState = SHOW_LOADCALEN;
			}
			break;
		case SUNDAY:
			LCD1602_ClearBFR();
			LCD1602_PrintStringBFR(0, 0, "SUN  START 00:00");
			LCD1602_PrintStringBFR(1, 0, "TMR1   RUN 00:00");
			LoadTimer();
			if (isNextButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
			}
			if (isPreviousButton())
			{
				TimerState = TIMER1;
				LoadDay = SATURDAY;
			}
			if (isBackButton())
			{
				TimerState = TIMER1;
				LoadDay = MONDAY;
				MainState = SHOW_LOADCALEN;
			}
			break;
		default:
			break;
	}
}

void CreateCalendar(void)
{
	switch (CreateModeState)
	{
		case SHOW_FULL_WEEK:
			LCD1602_PrintStringBFR(0,0,"<  FULL WEEK   >");
			if (isNextButton())
				CreateModeState = SHOW_ODD_DAYS;
			if (isPreviousButton())
				CreateModeState = SHOW_CHOOSE_DAYS;
			if (isBackButton())
			{
				CreateModeState = SHOW_FULL_WEEK;
				MainState = SHOW_CREATECALEN;
			}
			if (isEnterButton())
				CreateModeState = FULL_WEEK;
			break;
		case SHOW_ODD_DAYS:
			LCD1602_PrintStringBFR(0,0,"<   ODD DAYS   >");
			if (isNextButton())
				CreateModeState = SHOW_EVEN_DAYS;
			if (isPreviousButton())
				CreateModeState = SHOW_FULL_WEEK;
			if (isBackButton())
			{
				CreateModeState = SHOW_FULL_WEEK;
				MainState = SHOW_CREATECALEN;
			}
			if (isEnterButton())
				CreateModeState = ODD_DAYS;
			break;
		case SHOW_EVEN_DAYS:
			LCD1602_PrintStringBFR(0,0,"<   EVEN DAYS  >");
			if (isNextButton())
				CreateModeState = SHOW_CHOOSE_DAYS;
			if (isPreviousButton())
				CreateModeState = SHOW_ODD_DAYS;
			if (isBackButton())
			{
				CreateModeState = SHOW_FULL_WEEK;
				MainState = SHOW_CREATECALEN;
			}
			if (isEnterButton())
				CreateModeState = EVEN_DAYS;
			break;
		case SHOW_CHOOSE_DAYS:
			LCD1602_PrintStringBFR(0,0,"<  CHOOSE DAYS >");
			if (isNextButton())
				CreateModeState = SHOW_FULL_WEEK;
			if (isPreviousButton())
				CreateModeState = SHOW_EVEN_DAYS;
			if (isBackButton())
			{
				CreateModeState = SHOW_FULL_WEEK;
				MainState = SHOW_CREATECALEN;
			}
			if (isEnterButton())
				CreateModeState = CHOOSE_DAYS;
			break;
		case FULL_WEEK:
			LCD1602_ClearBFR();
			if (isBackButton())
				CreateModeState = SHOW_FULL_WEEK;
			break;
		case ODD_DAYS:
			LCD1602_ClearBFR();
			if (isBackButton())
				CreateModeState = SHOW_ODD_DAYS;
			break;
		case EVEN_DAYS:
			LCD1602_ClearBFR();
			if (isBackButton())
				CreateModeState = SHOW_EVEN_DAYS;
			break;
		case CHOOSE_DAYS:
			LCD1602_ClearBFR();
			if (isBackButton())
				CreateModeState = SHOW_CHOOSE_DAYS;
			break;
		default:
			break;
	}
}
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
