/*
 * Switch_Cfg.c
 *
 *  Created on: Feb 25, 2024
 *      Author: Halla Raafat
 */

#include "MCAL/GPIO.h"
#include "HAL/Switch_Cfg.h"
#include "HAL/Switch.h"



const SW_Cfg_t SW_ARR [SW_NUMBERS_]=
{
[SW_MUSIC_PLAY]={ .Port = GPIO_PORTA,
		          .Pin = GPIO_PIN1,
				  .ActiveState = SW_ACTIVESTATE_PU
				}

};


