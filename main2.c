#include <stm32f10x.h>
#include "misc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "twilcd.h"
#include "stdio.h"

#define SLAVE_ADDRESS_LCD 0x4E
#define SYSCLK 72000000
#define PRESCALER 72

int TIM_Pulse;


int main(void) {

    I2C1_init();
    lcd_init();
    clearlcd();

    setpos(3,0);
    str_lcd("Ball-Plate");
    setpos(3,1);
    str_lcd("Project");

    // GPIO structure for port initializations
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef timer;
    TIM_OCInitTypeDef timerPWM;

    // enable clock on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,  ENABLE);

    // configure port A1 for driving an LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(GPIOB, &GPIO_InitStructure) ;     

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


            // initialize port

    // main loop
    while(1) {



    TIM_Pulse = timerPWM.TIM_Pulse;
    TIM2->CCR4=2000;


        GPIO_SetBits(GPIOB, GPIO_Pin_14);    // turn the LED on
        _delay_ms(1000);

        GPIO_ResetBits(GPIOB, GPIO_Pin_14);  // turn the LED off
        _delay_ms(1000);
    }
}
