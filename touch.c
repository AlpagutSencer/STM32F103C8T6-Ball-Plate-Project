/************ 4-Wire Resistive Touch Screen Driver  *************
 * touch.c
 *
 *  Created on: April 14, 2019
 *      Author: Alpagut Sencer KARACA(alpaguty@gmail.com)
 */



#include <stm32f10x.h>
#include <stdio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "stm32f10x_adc.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "interface.h"
#include "stdbool.h"
#include "usart.h"
#include "touch.h"


TouchScreenErrorCodes adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;



	GPIO_Init(GPIOA,&GPIO_InitStructure);
	RCC_ADCCLKConfig (RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	// we work in continuous sampling mode
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1,ADC_SampleTime_28Cycles5); // define regular conversion config
	ADC_Init ( ADC1, &ADC_InitStructure);	//set config of ADC1

	// enable ADC
	ADC_Cmd (ADC1,ENABLE);	//enable ADC1

	//	ADC calibration (optional, but recommended at power on)
	ADC_ResetCalibration(ADC1);	// Reset previous calibration
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);	// Start new calibration (ADC must be off at that time)
	while(ADC_GetCalibrationStatus(ADC1));

	// start conversion
	ADC_Cmd (ADC1,ENABLE);	//enable ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// start conversion (will be endless as we are in continuous mode)



  return TOUCH_ERR_NONE;
}


