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

const LED_Cfg_t LEDS_ARR [LED_NUMBERS_]=
{
[LED_FRONT_RIGHT]={ .Port= GPIO_PORTA,
		            .Pin= GPIO_PIN0,
					.Connection= LED_CONN_FORWARD,
					.InitState=LED_STATE_ON},

[LED_FRONT_LEFT]={
					.Port= GPIO_PORTA,
					.Pin= GPIO_PIN8,
					.Connection= LED_CONN_FORWARD,
					.InitState=LED_STATE_ON}

};



//array for leds

