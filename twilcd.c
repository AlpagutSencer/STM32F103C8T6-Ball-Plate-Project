/*
* ----------------------------------------------------------------------------
* “BUY ME A BEER LICENSE”:
* This library is created by Alpagut Sencer KARACA(alpaguty@gmail.com)
*If you have any question, feel free to post me!
*As long as you retain my name,you can do whatever you like in this file.
*If it has solved your problems or if you use it in any project maybe you can buy me a beer someday for return :)
"twilcd.c"---09.08.2018----
*/


#include "stm32f10x_i2c.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "twilcd.h"
#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup



#define e1   I2C_SendByteByADDR(portlcd|=0x04,0b01001110) 
#define e0   I2C_SendByteByADDR(portlcd&=~0x04,0b01001110)  
#define rs1    I2C_SendByteByADDR(portlcd|=0x01,0b01001110) 
#define rs0   I2C_SendByteByADDR(portlcd&=~0x01,0b01001110)  
#define setled()    I2C_SendByteByADDR(portlcd|=0x08,0b01001110) 
#define setwrite()   I2C_SendByteByADDR(portlcd&=~0x02,0b01001110) 

unsigned char portlcd = 0;

void I2C1_init(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
	
    /* Configure I2C_EE pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;

    /* I2C Peripheral Enable */
    I2C_Cmd(I2C1, ENABLE);
    /* Apply I2C configuration after enabling it */
    I2C_Init(I2C1, &I2C_InitStructure);
}



void I2C_SendByteByADDR(unsigned char c,unsigned char addr){
	
	I2C_GenerateSTART(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C1, c);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTOP(I2C1,ENABLE);

while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
		
}

//--------------------------------------------------------------
void sendhalfbyte(unsigned char c)
{
	c<<=4;
	e1; 
	_delay_us(50);
	
	I2C_SendByteByADDR(portlcd|c,SLAVE_ADDRESS_LCD );
	e0; 
	_delay_us(50);
}

//------------------------------------------------------------------


void sendbyte(unsigned char c, unsigned char mode)
{
	if (mode==0) rs0;
	else         rs1;
	unsigned char hc=0;
	hc=c>>4;
	sendhalfbyte(hc); 
	sendhalfbyte(c);
}

//---------------------------------------------------------------------

void sendcharlcd(unsigned char c)
{
	sendbyte(c,1);
}

//---------------------------------------------------------------------


void setpos(unsigned char x, unsigned y)
{
	switch(y)
	{
		case 0:
			sendbyte(x|0x80,0);
			break;
		case 1:
			sendbyte((0x40+x)|0x80,0);
			break;
		case 2:
			sendbyte((0x14+x)|0x80,0);
			break;
		case 3:
			sendbyte((0x54+x)|0x80,0);
			break;
	}
}

//-----------------------------------------------------------------------

void lcd_init(void)
{
	_delay_ms(15); 
	sendhalfbyte(0b00000011);
	_delay_ms(4);
	sendhalfbyte(0b00000011);
	_delay_us(100);
	sendhalfbyte(0b00000011);
	_delay_ms(1);
	sendhalfbyte(0b00000010);
	_delay_ms(1);
	sendbyte(0b00101000, 0); 
	_delay_ms(1);
	sendbyte(0b00001100, 0); 
	_delay_ms(1);
	sendbyte(0b00000110, 0); 
	_delay_ms(1);
	setled();
	setwrite();
}
//------------------------------------------------------------------------------
void clearlcd(void)
{
	sendbyte(0b00000001, 0);
	_delay_us(1500);
}
//------------------------------------------------------------------------------
void str_lcd (char str1[])
{
	__CHAR32_TYPE__ n;
	for(n=0;str1[n]!='\0';n++)
	sendcharlcd(str1[n]);
}

//------------------------------------------------------------------------------

void createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; 
  sendbyte(0x40 | (location << 3),0);
  for (int i=0; i<8; i++) {
    sendbyte(charmap[i],1);
  }
}
