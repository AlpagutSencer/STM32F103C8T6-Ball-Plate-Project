/*
* ----------------------------------------------------------------------------
* “BUY ME A BEER LICENSE”:
* This library is created by Alpagut Sencer KARACA(alpaguty@gmail.com)
*If you have any question, feel free to post me!
*As long as you retain my name,you can do whatever you like in this file.
*If it has solved your problems or if you use it in any project maybe you can buy me a beer someday for return :)
"usart.c"---03.12.2018----
*/

#include <stm32f10x.h>
#include <stdio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "misc.h"
#include "stm32f10x_tim.h"
#include "interface.h"
#include "stdbool.h"
#include "usart.h"
#define MAX_MSG_LEN 100
volatile uint8_t timeoutCnt=250;
volatile uint8_t newMessageFlag=false;
volatile uint8_t packetbuffer[MAX_MSG_LEN];
volatile uint8_t packetbufferIndex;

 uint32_t deneme = 123456789; //trying packet


void USART_init(void)

{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitTStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO ,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
  USART_Cmd(USART1,ENABLE);

  NVIC_InitTStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitTStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitTStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitTStructure);

}


 void USART1_IRQHandler(void)
{


    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        /*if((char)USART_ReceiveData(USART1) == '1'){
            USART_SendString(USART1,"Valid");}*/



      volatile uint8_t data = USART_ReceiveData(USART1);

      if(newMessageFlag == true){

      }

      else{

      	packetbuffer[packetbufferIndex]=data;
      	packetbufferIndex++;

      	 if(packetbufferIndex> MAX_MSG_LEN){ //buffer overflow control
      	 	packetbufferIndex=0;
      	 	newMessageFlag= false;

      	 }
      	 else if (packetbufferIndex==packetbuffer[0]){ //flag for main method to handle the incoming packet

      	 	packetbufferIndex=0;
      	 	newMessageFlag= true;

      	 }

      	 else{
      	 	timeoutCnt=200;
      	 }



    }
      USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }

}






void packetHandler (){


//state machine of the packet handler (Command handler)







switch(packetbuffer[1]){

  case 0: // test message
  {



  }break;

  case 1: //Send values to the host
  {


    //USART_SendString(USART1,"      ");


  put32b(deneme);
  put32b(deneme);
  put32b(deneme);





  }break;


 }
}

void processPacket(void){

  if(newMessageFlag==true){
    packetHandler();
    newMessageFlag= false;
  }
}


void USART_SendString(USART_TypeDef* USARTx, char* s)
 {
    while(*s)
    {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
        USART_SendData(USARTx, *s);
        s++;
    }
 }


/*void USART_SendInt(USART_TypeDef* USARTx, uint8_t *s)
 {
    while(*s)
    {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
        USART_SendData(USARTx, s);
        s++;
    }
 }*/
