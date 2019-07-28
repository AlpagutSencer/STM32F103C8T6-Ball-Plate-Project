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
#include "stm32f10x_dma.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "interface.h"
#include "stdbool.h"
#include "usart.h"
#include "touch.h"
#include "delay.h"


ADC_InitTypeDef ADC_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;

volatile uint16_t ADCBuffer[2] = {0xFFFF, 0xFFFF};


TouchScreenErrorCodes adc_init(void)
{
	
	
	
   
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);


    
   RCC->APB2ENR |= 0x00000004; //RCC_APB2ENR_IOPAEN;
   






     /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);*/
	RCC_ADCCLKConfig (RCC_PCLK2_Div6);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	// we work in continuous sampling mode
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1,ADC_SampleTime_1Cycles5);// define regular conversion config
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 1,ADC_SampleTime_1Cycles5);// define regular conversion config
	 // define regular conversion config
	ADC_Init ( ADC1, &ADC_InitStructure);
	ADC_Cmd (ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);	// Reset previous calibration
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);	// Start new calibration (ADC must be off at that time)
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_Cmd (ADC1,ENABLE);	//enable ADC1
	
	 

  return TOUCH_ERR_NONE;
}



uint16_t readX (void){

	uint16_t adc_value;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
	
    GPIOA->CRL &=0x0; //~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= 0x00004303;//GPIO_CRL_MODE0;

    GPIOA->ODR |= 0x00000001;

    ADC1->SQR1 = 0x00000000;
    ADC1->SQR3 |= (1<<0);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    /*_delay_ms(500);

    GPIOA->ODR &= ~(0x0000000F);
    
	_delay_ms(500);*/
    //ADC1->SQR3 = 1;
    
	// A1 declared as analog input
	
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1,ADC_SampleTime_1Cycles5);// define regular conversion config
  
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);


    adc_value = ADC_GetConversionValue(ADC1);
	
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	


return adc_value;
	
}


uint16_t readY (void) {

	uint16_t adc_value2;
	
	GPIOA->CRL &=0x0; //~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= 0x00003034;//GPIO_CRL_MODE0;

	
    GPIOA->ODR |= 0x00000002;
    
	ADC1->SQR3 = 2;

  // A0 and A2 pins declared as output
	/*RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // A3 declared as Hi-Z(tri-state)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
*/
	
    
   
	// A1 declared as analog input

	
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 1,ADC_SampleTime_1Cycles5);// define regular conversion config
  
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);


    adc_value2 = ADC_GetConversionValue(ADC1);
	
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    
	return adc_value2;
}






























