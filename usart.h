#ifndef USART_H_
#define USART_H_

#include <stm32f10x.h>
#include <stdio.h>
#include "stm32f10x_usart.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "misc.h"
#include "stm32f10x_tim.h"
#include "interface.h"
#include "stdbool.h"

void packetHandler();
void processPacket(void);
void USART_init(void);
void USART_SendString(USART_TypeDef* USARTx, char* s);
void USART1_IRQHandler(void);





#endif