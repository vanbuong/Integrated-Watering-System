/* Includes ------------------------------------------------------------------*/
#include "PWM.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : PWM_Init
* Description    : Configures TIM3_CH3 PWM
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* GPIO Config */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* TIM3 Config */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1)/1000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* TIM3 Chanel 3 PWM Config */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_CCxCmd(TIM3, TIM_Channel_3, TIM_CCx_Enable);
	TIM_Cmd(TIM3, ENABLE);
}

/*******************************************************************************
* Function Name  : PWM_Init
* Description    : Configures TIM3_CH3 PWM
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void PWM_SetPercent(int8_t percent)
{
	if (percent < 0)
		percent = 0;
	else if (percent > 100)
		percent = 100;
	
	TIM3->CCR3 = (percent*20000)/100;
}
/* Private functions ---------------------------------------------------------*/
