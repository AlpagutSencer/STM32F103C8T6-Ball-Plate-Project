/************ 4-Wire Resistive Touch Screen Driver  *************
 * touch.c
 *
 *  Created on: April 14, 2019
 *      Author: Alpagut Sencer KARACA(alpaguty@gmail.com)
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#include <stm32f10x.h>
#include <stdio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "stm32f10x_adc.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "interface.h"
#include "stdbool.h"
#include "usart.h"

#define TOUCH_MODULE_SW_VERSION (1.0)




typedef enum
{
	TOUCH_ERR_NONE,
	TOUCH_ERR_UNKNOWN,
	TOUCH_ERR_HW_ERROR,
	TOUCH_ERR_WRONG_IOCTL_CMD,
}TouchScreenErrorCodes;





TouchScreenErrorCodes adc_init();



#endif








