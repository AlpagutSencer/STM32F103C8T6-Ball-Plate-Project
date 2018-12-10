#include "stm32f10x.h"
#include "delay.h"
#include <stm32f10x_usart.h>
#include "stdbool.h"
#include "interface.h"
#include "usart.h"


volatile uint8_t timeoutCnt=250;
volatile uint8_t newMessageFlag=false;
volatile uint8_t packetbuffer[MAX_MSG_LEN];
volatile uint8_t packetbufferIndex;


/*
       oooOOOOOOOOOOO" cu-cu-cu-cu-düüüütt düüüttt BİLETLERRRRRRRRR
     o   ____          :::::::::::::::::: :::::::::::::::::: __|-----|__
     Y_,_|[]| --++++++ |[][][][][][][][]| |[][][][][][][][]| |  [] []  |
    {|_|_|__|;|______|;|________________|;|________________|;|_________|;
     /oo--OO   oo  oo   oo oo      oo oo   oo oo      oo oo   oo     oo
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

void put32b(uint32_t val) // message train to the pc 
{
  uint8_t tmpBuf[4];
  tmpBuf[0] = (val & 0x000000FF) >>  0;
  tmpBuf[1] = (val & 0x0000FF00) >>  8;
  tmpBuf[2] = (val & 0x00FF0000) >> 16;
  tmpBuf[3] = (val & 0xFF000000) >> 24;

  /*USART_SendString(tmpBuf[0]);
  USART_SendString(tmpBuf[1]);
  USART_SendString(tmpBuf[2]);
  USART_SendString(tmpBuf[3]);*/
}

void packetHandler (){ //state machine of the packet handler (Command handler)

switch(packetbuffer[1]){

	case 0: // test message 
	{
      
		USART_SendString(USART3,"Valid");

	}break;

	case 1: //Send values to the host
	{

		//put32b();
      
	}break;


 }
}