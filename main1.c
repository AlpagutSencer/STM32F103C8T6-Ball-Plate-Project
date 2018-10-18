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
#include "twilcd.h"

#define SYSCLK 72000000
#define PRESCALER 72

TIM_TimeBaseInitTypeDef timer;
TIM_OCInitTypeDef timerPWM;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure; 
NVIC_InitTypeDef NVIC_InitStructure;

static xQueueHandle queue_handle = NULL;
static unsigned int LEDState = 0;
void servo_init(void);
int TIM_Pulse;
char str[20];




void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        if((char)USART_ReceiveData(USART3) == '1')
            LEDState = 2;
 
        if((char)USART_ReceiveData(USART3) == '0')
            LEDState = 1;
    }
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}



static xSemaphoreHandle trg = NULL;


static void USART_SendString(USART_TypeDef* USARTx, char* s)
{
    while(*s)
    {
        while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));
        USART_SendData(USARTx, *s);
        s++;
    }
}


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

			
		
    		//USART_SendString(USART3,buffer);
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
 
	
	
//----------------------------------------------------------------	
	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	

	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);
   
 
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_Parity                = USART_Parity_No;
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                  = USART_Mode_Tx | USART_Mode_Rx;
 
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
 
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	
	
	
	

 
 servo_init();
 
 
 
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


