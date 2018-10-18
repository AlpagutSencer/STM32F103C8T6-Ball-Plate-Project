/*
* ----------------------------------------------------------------------------
* “BUY ME A BEER LICENSE”:
* This library is created by Alpagut Sencer KARACA(alpaguty@gmail.com)
*If you have any question, feel free to post me!
*As long as you retain my name,you can do whatever you like in this file.
*If it has solved your problems or if you use it in any project maybe you can buy me a beer someday for return :)
"twilcd.h"---09.08.2018----
*/

#ifndef TWILCD_H_
#define TWILCD_H_
#include "stm32f10x_i2c.h"
#include "misc.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "twilcd.h"
void createChar(uint8_t location, uint8_t charmap[]);
void I2C1_init(void);
void I2C_SendByteByADDR(unsigned char c,unsigned char addr);
void lcd_init(void);
void setpos(unsigned char x, unsigned y);
void str_lcd (char str1[]);
void clearlcd(void);
void sendcharlcd(unsigned char c);







#endif 
