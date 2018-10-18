/*
* ----------------------------------------------------------------------------
* “BUY ME A BEER LICENSE”:
* This library is created by Alpagut Sencer KARACA(alpaguty@gmail.com)
*If you have any question, feel free to post me!
*As long as you retain my name,you can do whatever you like in this file.
*If it has solved your problems or if you use it in any project maybe you can buy me a beer someday for return :)
"delay.c"---09.08.2018----
*/

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"

void _delay_ms(uint32_t ms)
{
        volatile uint32_t nCount;
        RCC_ClocksTypeDef RCC_Clocks;
        RCC_GetClocksFreq (&RCC_Clocks);

        nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
        for (; nCount!=0; nCount--);
}


void _delay_us(uint32_t mc)
{
        volatile uint32_t nCount;
        RCC_ClocksTypeDef RCC_Clocks;
        RCC_GetClocksFreq (&RCC_Clocks);

        nCount=(RCC_Clocks.HCLK_Frequency/10000000)*mc;
        for (; nCount!=0; nCount--);
}


