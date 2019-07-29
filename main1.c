#include <stm32f10x.h>
#include <stdio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_adc.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "delay.h"
#include "interface.h"
#include "twilcd.h"
#include "stdbool.h"
#include "usart.h"
#include "touch.h"

#define SYSCLK 72000000
#define PRESCALER 72

TIM_TimeBaseInitTypeDef timer;
TIM_OCInitTypeDef timerPWM;
GPIO_InitTypeDef GPIO_InitStructure;


static xQueueHandle queue_handle = NULL;
void servo_init(void);
uint32_t servoMap (uint8_t angle);
int TIM_Pulse;
char str[20];
static xSemaphoreHandle trg = NULL;





void triggertask(void *pvParameters)
{
	int txBuffer[3]={650,1500,2400};
	
            
            

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

    

         
		/*for(int i=0; i<3;i++){
	       vTaskDelay(750/portTICK_RATE_MS);
			xQueueReceive(queue_handle,&rxBuffer[i],1000);

           

			TIM2->CCR4=rxBuffer[i];
			
			clearlcd();
			sprintf(buffer, "Servo :%d us ", rxBuffer[i]);
			setpos(0,0);
			str_lcd(buffer);
			setpos(0,1);
			sprintf(buffer,"freq: %lu" ,get_cpuFreq());
			str_lcd(buffer);
         }*/
    	
    	


		 int new = readX();
		
		 int new2 = readY();
        sprintf(buffer, "x: %d y: %d ", new,new2);
    	setpos(0,0);
		str_lcd(buffer);
    	vTaskDelay(10/portTICK_RATE_MS);
    	//USART_SendString(USART1,buffer);
		
    

        /*for (int i=30;i<150;i++)
           {
             
            TIM2->CCR4= servoMap(i);
            setpos(0,0);
            sprintf(buffer, "Servo :%ld  ", servoMap(i));
			str_lcd(buffer);
			vTaskDelay(500/portTICK_RATE_MS);
			

           }*/

         }
}




void GUI (void *pvParameters){

   
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10;
	xLastWakeTime = xTaskGetTickCount();
	while(1){
		  vTaskDelayUntil(&xLastWakeTime,xFrequency);
          //processPacket();


	}

}

/*void IWDG_Reset (void *pvParameters){

   IWDG_ReloadCounter();
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1000;
	xLastWakeTime = xTaskGetTickCount();
	while(1){
		  vTaskDelayUntil(&xLastWakeTime,xFrequency);
			IWDG_ReloadCounter();
          //processPacket();


	}

}*/





//-----------------------------------------
int main(){

  /*GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_StructInit(&GPIO_InitStructure);*/

  /*GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA pin 0 output push pull
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/


               
   

    servo_init();
    SystemInit();
    I2C1_init();
	lcd_init();
	clearlcd();
    adc_init();
 	USART_init();
 	buzzerInit();

 


	

  /*IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_128); // 4, 8, 16 ... 256
  IWDG_SetReload(0x0FFF);//This parameter must be a number between 0 and 0x0FFF.
  IWDG_ReloadCounter();
  IWDG_Enable();*/







 	/*GPIO_WriteBit(GPIOB,GPIO_Pin_14, Bit_SET);
    _delay_ms(250);
    GPIO_WriteBit(GPIOB,GPIO_Pin_14, Bit_RESET);*/


 queue_handle = xQueueCreate(3,sizeof(int));

	vSemaphoreCreateBinary(trg);

 //Create task to blink gpio
 xTaskCreate(triggertask, (const char *)"triggertask", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
 xTaskCreate(sendertask, (const char *)"sendertask", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
 xTaskCreate(GUI, (const char *)"GUI", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
 //xTaskCreate(IWDG_Reset, (const char *)"IWDG_Reset", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY -1, NULL);
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
	GPIO_Init(GPIOA, &GPIO_InitStructure) ;


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

uint32_t servoMap (uint8_t angle){ //Lineer Interpolation for 500-2500 us driven servos

	return ((angle*11.11111111)+500);
}
