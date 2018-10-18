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

    // enable clock on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,  ENABLE);

    // configure port A1 for driving an LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(GPIOB, &GPIO_InitStructure) ;             // initialize port

    // main loop
    while(1) {
        GPIO_SetBits(GPIOB, GPIO_Pin_14);    // turn the LED on
        _delay_ms(1000);

        GPIO_ResetBits(GPIOB, GPIO_Pin_14);  // turn the LED off
        _delay_ms(1000);
    }
}
