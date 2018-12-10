#include <stm32f10x.h>
#include <stdio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "delay.h"
#include "interface.h"
#include "twilcd.h"
#include "stdbool.h"
#include "usart.h"

#define SYSCLK 72000000
#define PRESCALER 72

TIM_TimeBaseInitTypeDef timer;
TIM_OCInitTypeDef timerPWM;
GPIO_InitTypeDef GPIO_InitStructure;


static xQueueHandle queue_handle = NULL;
void servo_init(void);
int TIM_Pulse;
char str[20];
static xSemaphoreHandle trg = NULL;





void triggertask(void *pvParameters)
{
	int txBuffer[3]={650,1500,2200};
	
    while(1) {
			
				
		
		for(int j=0; j<3;j++){
	       vTaskDelay(750/portTICK_RATE_MS);
			xQueueSend(queue_handle,&txBuffer[j],1000);
		}
		
		 
		//vTaskDelay(1000/portTICK_RATE_MS);
			}
		
			//vTaskDelete(NULL);
		}



void sendertask(void *pvParameters)
{
	
	
	int rxBuffer[3];
	char buffer[16];
	TIM_Pulse = timerPWM.TIM_Pulse;
	TIM2->CCR4=2000;

    while(1) {
		
		for(int i=0; i<3;i++){
	       vTaskDelay(750/portTICK_RATE_MS);
			xQueueReceive(queue_handle,&rxBuffer[i],1000);
			TIM2->CCR4=rxBuffer[i];
			clearlcd();
			sprintf(buffer, "Servo :%d us ", rxBuffer[i]);
			setpos(0,0);
			str_lcd(buffer);

			
		
    		
		}
				
        
 }
//vTaskDelete(NULL);
}

//-----------------------------------------
int main(){
	
	
    SystemInit();
    

	I2C1_init();
	lcd_init();
	clearlcd();
    servo_init();
 	USART_init();
    USART_SendString(USART1,"hh");
 
 queue_handle = xQueueCreate(3,sizeof(int));

	vSemaphoreCreateBinary(trg);
	
 //Create task to blink gpio
 xTaskCreate(triggertask, (const char *)"triggertask", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
 xTaskCreate(sendertask, (const char *)"sendertask", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
 //Start the scheduler
 vTaskStartScheduler();
 
 //Should never reach here
 while(10);
}


void servo_init(void) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&timer);
	timer.TIM_Prescaler = PRESCALER;
	timer.TIM_Period = SYSCLK / PRESCALER / 50;
	timer.TIM_ClockDivision = 0;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &timer);

	TIM_OCStructInit(&timerPWM);
	timerPWM.TIM_Pulse = 1000;
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM2, &timerPWM);

    TIM_Cmd(TIM2, ENABLE);
}


