/* Includes ------------------------------------------------------------------*/
#include "ADC.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint16_t ADC1ConvertedValue;
/* Private function prototypes -----------------------------------------------*/
void ADCx_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	ADC_InitTypeDef	ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/* ADCCLK = PCLK2/4 */
  //RCC_ADCCLKConfig(RCC_PCLK2_Div4); 

  /* Configure PC0, PC1, PC2 and PC3 (ADC Channel1x as analog inputs */	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* ADC1 configuration */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC1ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;        
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                     
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
	
  /* ADC1 regular channels configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
	
	/* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibration register */
  ADC_ResetCalibration(ADC1);
	
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
	
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
	
	/* Start ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	/* Enable ADC1 EOC interrupt */
  //ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
}

uint16_t ADCx_GetConvertedValue(uint8_t channel)
{
	switch(channel)
	{
		case 14:
			return ADC1ConvertedValue;
		default:
			return 0;
	}
}
//void ADC1_IRQ_Handler(void)
//{
//	ADC1ConvertedValue = ADC_GetConversionValue(ADC1);
//}
/* Private functions ---------------------------------------------------------*/
