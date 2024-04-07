/*
 * LCD_Cfg.h
 *
 *  Created on: Mar 21, 2024
 *      Author: Halla Raafat
 */

#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#include "LIB/STD_Types.h"
#include "HAL/LCD.h"



/*             Configure LCD                         */



/*
 * Configure Periodicity in ms
 */
#define LCD_PERIODICITY_MS            2
/*
 * Configure Mode  : LCD_8_BIT_MODE
 *                   LCD_4_BIT_MODE
 */
#define LCD_BIT_MODE   LCD_8_BIT_MODE

/*
 * Configure Display : LCD_ENABLE
 *                     LCD_DISABLE
 */
#define LCD_DISPLAY    LCD_ENABLE
/*
 * Configure Cursor  : LCD_ENABLE
 *                     LCD_DISABLE
 */

#define LCD_CURSOR     LCD_ENABLE
/*
 * Configure Blinking  : LCD_ENABLE
 *                     LCD_DISABLE
 */

#define LCD_BLINK           LCD_DISABLE

/*
 * Configure Number lines  : LCD_TWO_LINES
 *                           LCD_ONE_LINE
 */
#define LCD_NUMBER_LINES    LCD_TWO_LINES

/*
 * Configure SHIFTING   : LCD_INCREMENT
 *                        LCD_DECREMENT
 */

#define LCD_INC_DEC       LCD_INCREMENT
/*
 * Configure SHIFTING   : LCD_ENABLE
 *                        LCD_DISABLE
 */
#define LCD_SHIFT          LCD_DISABLE
/*
 * Configure Font size    : LCD_5X7
 *                        LCD_5X10
 */

#define LCD_FONT_SIZE       LCD_5X10

typedef struct
{

 uint32 Pin;
 void * Port;

}Pin_Cfg_t;


typedef struct
{

	Pin_Cfg_t DB7;
	Pin_Cfg_t DB6;
	Pin_Cfg_t DB5;
	Pin_Cfg_t DB4;

#if  (LCD_BIT_MODE == LCD_8_BIT_MODE)
	Pin_Cfg_t DB3;
	Pin_Cfg_t DB2;
	Pin_Cfg_t DB1;
	Pin_Cfg_t DB0;
#endif
	Pin_Cfg_t RegSelect;  // Register Select
	Pin_Cfg_t Read_Write; // Read / Write
	Pin_Cfg_t Enable;




}LCD_PinCfg_t;


#endif /* LCD_CFG_H_ */
