/*
 *  File      :LCD_Cfg.c
 *
 *  Created on: Mar 21, 2024
 *  Author    : Halla Raafat
 */

#include <HAL/CFG/LCD_Cfg.h>
#include "HAL/LCD.h"

#include "MCAL/GPIO.h"




LCD_PinCfg_t LCD_Cfg=
		{
				.RegSelect={ .Port=GPIO_PORTA,.Pin= GPIO_PIN0 },
				.Read_Write={ .Port=GPIO_PORTA,.Pin= GPIO_PIN1 },
				.Enable={ .Port=GPIO_PORTA,.Pin= GPIO_PIN10 },
#if  (LCD_BIT_MODE == LCD_8_BIT_MODE)
				.DB0={ .Port=GPIO_PORTA,.Pin= GPIO_PIN2 },
				.DB1={ .Port=GPIO_PORTA,.Pin= GPIO_PIN3 },
				.DB2={ .Port=GPIO_PORTA,.Pin= GPIO_PIN4 },
				.DB3={ .Port=GPIO_PORTA,.Pin= GPIO_PIN5 },
#endif

				.DB4={ .Port=GPIO_PORTA,.Pin= GPIO_PIN6 },
				.DB5={ .Port=GPIO_PORTA,.Pin= GPIO_PIN7 },
				.DB6={ .Port=GPIO_PORTA,.Pin= GPIO_PIN8 },
				.DB7={ .Port=GPIO_PORTA,.Pin= GPIO_PIN9 },

		};
