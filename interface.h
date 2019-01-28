#ifndef INTERFACE_H_
#define INTERFACE_H_

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
#include "stdbool.h"

#define MAX_MSG_LEN 100

extern volatile uint8_t packetbuffer[MAX_MSG_LEN];
extern volatile uint8_t newMessageFlag;
extern volatile uint8_t packetbufferIndex;
extern volatile uint8_t timeoutCnt;


void put32b(uint32_t val);
void packetHandler();

#endif 