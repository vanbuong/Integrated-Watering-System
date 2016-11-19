/* Includes ------------------------------------------------------------------*/
#include "TIMER/TIMER.h"
#include "INPUT165/INPUT165.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t delay_timeout = 0;
volatile uint32_t _delay_timeout = 0;

volatile unsigned int timer2_value = 0;
volatile unsigned int timer2_cnt = 0;
unsigned int timer2_MUL = 0;
unsigned char timer2_flag = 0;

volatile unsigned int timer7_cnt1 = 0;
unsigned int timer7_MUL1 = 0;
unsigned char timer7_flag1 = 0;

volatile unsigned int timer7_cnt2 = 0;
unsigned int timer7_MUL2 = 0;
unsigned char timer7_flag2 = 0;

volatile unsigned int timer7_cnt3 = 0;
unsigned int timer7_MUL3 = 0;
unsigned char timer7_flag3 = 0;

/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
*	TIM2-7 configuration:
********************************************************************************
*	TIM Clock: 36Mhz
*	TIM_Prescaler: If (APB1 prescaler = 1) x1 else x2
*	TIM_Period: Times TIM counter up or down or ...
*	TIM_ClockDivision: ...
*	TIM_CounterMode: Config TIM counter up or down or ...
...
********************************************************************************
*	TIM1|8 configuration:
********************************************************************************
*	TIM Clock: 72Mhz
*	TIM_Prescaler: If (APB2 prescaler = 1) x1 else x2
*	TIM_Period: Times TIM counter up or down or...
*	TIM_ClockDivision: ...
*	TIM_CounterMode: Config TIM counter up or down or ...
...
********************************************************************************
* For example like TIM2_Init:
* - Use TIM2 -> clock = 36Mhz
* - Set Prescaler = ... = 71 (When calc use 72 because count from zero '0')
*		+	Clock will be x2 because APB1 Pres > 1
*		+ TIM2_Clock = 72Mhz
*
*	-	Period = 1000 - 1 (When calc use 1000 because count from zero '0')
* - Mode = Counter up
* - So, we have:
*		+ 1 Clock TIM for counter up 1 unit need = 1/(TIM_CLOCK) x Prescaler (s)
*		+ ---> 1/(TIM_CLOCK = 72Mhz) x 72 = 1 (us)
*		+ Period = 1000
*		+ ---> TIM need 1000 x 1 (us) = 1ms to overflow!
*
...
********************************************************************************
*	--- Summary we have a TIM Formula:
*	--- Time_Cycle = (1/(TIM_CLK)) x (TIM_Pres + 1)  x (TIM_Period + 1) x  (TIM_Repetition + 1) (s)
* --- OKAY, Let's use TIMERRRRRRRR
*******************************************************************************/
/*******************************************************************************
* Function Name  : TIM2_Init
* Description    : Configures TIM2 for 1ms interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void TIM2_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1)/1000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : TIM7_Init
* Description    : Configures TIM7 for 10ms interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1)/1000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	/* TIM IT enable */
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	
	/* TIM2 enable counter */
	TIM_Cmd(TIM7, ENABLE);

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : TIM_IRQ_Handler
* Description    : TIM IRQ Handler
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void TIM2_IRQ_Handler(void)
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	if (delay_timeout > 0)
    delay_timeout--;
	
	timer2_cnt++;
  if (timer2_cnt >= timer2_MUL)
  {
		timer2_cnt = 0;
		timer2_flag = 1;
  }
	
}

void TIM7_IRQ_Handler(void)
{
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	
	timer7_cnt1++;
  if (timer7_cnt1 >= timer7_MUL1)
  {
		timer7_cnt1 = 0;
		timer7_flag1 = 1;
  }
	
	timer7_cnt2++;
  if (timer7_cnt2 >= timer7_MUL2)
  {
		timer7_cnt2 = 0;
		timer7_flag2 = 1;
  }
	
	timer7_cnt3++;
  if (timer7_cnt3 >= timer7_MUL3)
  {
		timer7_cnt3 = 0;
		timer7_flag3 = 1;
  }
	
}

/*******************************************************************************
* Function Name  : Delay ms
* Description    : ...
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void delay_ms(uint32_t time)
{
	delay_timeout = time;
	while (delay_timeout > 0);
}

void delay_us(unsigned int value)
{
	unsigned int i,j;
	for (i = 0; i < value; i++)
		for (j = 0; j < 10; j++);
}

void _delay_ms(uint32_t time)
{
	_delay_timeout = time;
	while (_delay_timeout > 0);
}

/*******************************************************************************
* Function Name  : TIM2 Flag Functions
* Description    : ...
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_Setms(unsigned int ms)
{
	timer2_MUL = ms;
	timer2_cnt = 0;
	timer2_flag = 0;
}

uint8_t TIM2_Flag(void)
{
	if (timer2_flag)
	{
		timer2_flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

void TIM7_SetmsFlag1(unsigned int ms)
{
	timer7_MUL1 = ms;
	timer7_cnt1 = 0;
	timer7_flag1 = 0;
}

uint8_t TIM7_Flag1(void)
{
	if (timer7_flag1)
	{
		timer7_flag1 = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

void TIM7_SetmsFlag2(unsigned int ms)
{
	timer7_MUL2 = ms;
	timer7_cnt2 = 0;
	timer7_flag2 = 0;
}

uint8_t TIM7_Flag2(void)
{
	if (timer7_flag2)
	{
		timer7_flag2 = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

void TIM7_SetmsFlag3(unsigned int ms)
{
	timer7_MUL3 = ms;
	timer7_cnt3 = 0;
	timer7_flag3 = 0;
}

uint8_t TIM7_Flag3(void)
{
	if (timer7_flag3)
	{
		timer7_flag3 = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
/*******************************************************************************
* Function Name  : SysTick 1ms functions
* Description    : ...
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);	//1ms
	NVIC_SetPriority(SysTick_IRQn,6);
}

void SysTick_IRQ_Handler(void)
{
	if (_delay_timeout > 0)
    _delay_timeout--;
}
/* Private functions ---------------------------------------------------------*/
