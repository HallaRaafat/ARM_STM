/*
 * LCD.h
 *
 *  Created on: Mar 21, 2024
 *      Author: Halla Raafat
 */

#ifndef LCD_H_
#define LCD_H_

#include  "HAL/LCD_Cfg.h"
#include "LIB/STD_Types.h"

typedef enum
{

	LCD_OK,
	LCD_NOK
}LCD_Error_t;


LCD_Error_t LCD_InitAsync();
LCD_Error_t LCD_GetState();
LCD_Error_t LCD_ClearScreenAsync();
LCD_Error_t LCD_WriteStringAsync(uint8 * String , uint8 Length ,uint8 PosX ,uint8 PosY );



#endif /* LCD_H_ */
