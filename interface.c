#include "stm32f10x.h"
#include "delay.h"
#include <stm32f10x_usart.h>
#include "stdbool.h"
#include "interface.h"
#include "usart.h"


void put32b(uint32_t val) // message train to the pc 
{
  uint16_t tmpBuf[2];
  tmpBuf[0] = (val & 0x000000FF) >>  0;
  tmpBuf[1] = (val & 0x0000FF00) >>  8;
  tmpBuf[2] = (val & 0x00FF0000) >> 16;
  tmpBuf[3] = (val & 0xFF000000) >> 24;

while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
USART_SendData(USART1,tmpBuf[0]);
while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
USART_SendData(USART1,tmpBuf[1]);
while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
USART_SendData(USART1,tmpBuf[2]);
while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
USART_SendData(USART1,tmpBuf[3]);
}

uint32_t make32b(uint8_t* buff, int32_t offset)
{
  uint32_t rv = 0;
  rv += buff[offset+0] <<  0;
  rv += buff[offset+1] <<  8;
  rv += buff[offset+2] << 16;
  rv += buff[offset+3] << 24;
  return rv;
}

void buzzerInit (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIOA pin 0 output push pull
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

