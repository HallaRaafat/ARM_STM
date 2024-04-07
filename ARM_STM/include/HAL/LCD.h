/*
 * LCD.h
 *
 *  Created on: Mar 21, 2024
 *      Author: Halla Raafat
 */

#ifndef LCD_H_
#define LCD_H_

#include  "HAL/CFG/LCD_Cfg.h"
#include "LIB/STD_Types.h"


#define LCD_ENABLE    1
#define LCD_DISABLE   0
#define LCD_TWO_LINES         1
#define LCD_ONE_LINE          0
#define LCD_INCREMENT  0
#define LCD_DECREMENT  1
#define LCD_4_BIT_MODE      4
#define LCD_8_BIT_MODE      8
#define LCD_5X7           0
#define LCD_5X10           1
typedef enum
{

	LCD_OK,
	LCD_NOK
}LCD_Error_t;


LCD_Error_t LCD_InitAsync();
LCD_Error_t LCD_GetState(uint8 * Ptr_LCDState);
LCD_Error_t LCD_ClearScreenAsync();
LCD_Error_t LCD_WriteStringAsync(uint8 * String , uint8 Length ,uint8 PosX ,uint8 PosY );


LCD_Error_t LCD_Write_NumberAsync(uint32 Number);


#endif /* LCD_H_ */
