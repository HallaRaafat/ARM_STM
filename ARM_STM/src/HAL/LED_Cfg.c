/*
 * LED_Cfg.c
 *
 *  Created on: Feb 19, 2024
 *      Author: Halla Raafat
 */
#include "MCAL/GPIO.h"
#include "HAL/LED_Cfg.h"
#include "HAL/LED.h"


/*
 * 	 Pin	Port
	 Connection, InitState
 */
/*
 * @brief    : array to configure LED connection
 * @configure : .Port
 *               .Pin
 *               .Connection -> Led
 *               .InitState

 */

const LED_Cfg_t LEDS_ARR [_LED_NUMBERS_]=
{
[LED_Green]={ .Port= GPIO_PORTA,
		            .Pin= GPIO_PIN1,
					.Connection= LED_CONN_FORWARD,
					.InitState=LED_STATE_OFF},

[LED_Yellow]={
					.Port= GPIO_PORTA,
					.Pin= GPIO_PIN2,
					.Connection= LED_CONN_FORWARD,
					.InitState=LED_STATE_OFF},

[LED_Red]={
					.Port= GPIO_PORTA,
					.Pin= GPIO_PIN3,
					.Connection= LED_CONN_FORWARD,
					.InitState=LED_STATE_OFF}

};



//array for leds

