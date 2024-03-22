/*
 * LCD_Cfg.h
 *
 *  Created on: Mar 21, 2024
 *      Author: Halla Raafat
 */

#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#include "LIB/STD_Types.h"

#define FOUR_BIT_MODE      4
#define EIGHT_BIT_MODE     8


/*
 * Configure which mode you want here
 */

#define BIT_MODE   EIGHT_BIT_MODE





typedef struct
{

 uint32 Pin;
 uint32 Port;

}Pin_Cfg_t;


typedef struct
{

	Pin_Cfg_t DB7;
	Pin_Cfg_t DB6;
	Pin_Cfg_t DB5;
	Pin_Cfg_t DB4;

#if  (BIT_MODE == EIGHT_BIT_MODE)
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
