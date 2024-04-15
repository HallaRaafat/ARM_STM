/*
 * LCD_CFG.c
 *
 *  Created on: Mar 21, 2024
 *      Author: ahmed tarek
 */
#include "HAL/CFG/CLCD_CFG.h"
#include "MCAL/GPIO.h"

CLCD_CFG_t LCDS[_LCD_NUM]=
{

				[D0]={.Port=GPIO_PORTA , .Pin=GPIO_PIN0} ,
				[D1]={.Port=GPIO_PORTA , .Pin=GPIO_PIN1} ,
				[D2]={.Port=GPIO_PORTA , .Pin=GPIO_PIN2} ,
				[D3]={.Port=GPIO_PORTA , .Pin=GPIO_PIN3} ,
				[D4]={.Port=GPIO_PORTA , .Pin=GPIO_PIN4} ,
				[D5]={.Port=GPIO_PORTA , .Pin=GPIO_PIN5} ,
				[D6]={.Port=GPIO_PORTA , .Pin=GPIO_PIN6} ,
				[D7]={.Port=GPIO_PORTA , .Pin=GPIO_PIN7} ,
				[RS]={.Port=GPIO_PORTA , .Pin=GPIO_PIN8} ,
				[RW]={.Port=GPIO_PORTB , .Pin=GPIO_PIN9} ,
				[EN]={.Port=GPIO_PORTB , .Pin=GPIO_PIN10} ,



};
