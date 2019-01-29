#include "stm32f10x.h"
#include "delay.h"
#include <stm32f10x_usart.h>
#include "stdbool.h"
#include "interface.h"
#include "usart.h"


void put32b(uint32_t val) // message train to the pc 
{
  uint8_t tmpBuf[4];
  tmpBuf[0] = (val & 0x000000FF) >>  0;
  tmpBuf[1] = (val & 0x0000FF00) >>  8;
  tmpBuf[2] = (val & 0x00FF0000) >> 16;
  tmpBuf[3] = (val & 0xFF000000) >> 24;

  USART_SendData(USART1,tmpBuf[0]);
  USART_SendData(USART1,tmpBuf[1]);
  USART_SendData(USART1,tmpBuf[2]);
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

