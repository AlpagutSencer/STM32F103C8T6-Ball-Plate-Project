#include "FreeRTOS.h"
#include <stm32f10x.h>
#include <stdio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "queue.h"
#include <misc.h>

#include "semphr.h"
#include "task.h"

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure; 

static xSemaphoreHandle trg = NULL;


void triggertask(void *pvParameters)
{
	
    while(1) {
        xSemaphoreGive(trg);
			vTaskDelay(1000); 
		
		}
}

void sendertask(void *pvParameters)
{
    while(1) {
			
				if(xSemaphoreTake(trg,portMAX_DELAY)){	
					
			  GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
        vTaskDelay(1000/portTICK_RATE_MS);
  
        GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
        vTaskDelay(1000/portTICK_RATE_MS);
				}
				
        
 }
}


//-----------------------------------------
int main()
{ 
 SystemInit();
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
 
 //GPIO
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIOA pin 0 output push pull
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

	vSemaphoreCreateBinary(trg);
	
 //Create task to blink gpio
 xTaskCreate(triggertask, (const char *)"triggertask", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
 xTaskCreate(sendertask, (const char *)"sendertask", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
 //Start the scheduler
 vTaskStartScheduler();
 
 //Should never reach here
 while(10);
}