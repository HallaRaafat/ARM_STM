/*
 * LCD_CFG.h
 *
 *  Created on: Mar 21, 2024
 *      Author: ahmed tarek
 */

#ifndef CLCD_CFG_H_
#define CLCD_CFG_H_

#include "LIB/STD_Types.h"

typedef struct
{
	void* Port;
	uint32   Pin;


}CLCD_CFG_t;

typedef enum{
	D0,
	D1,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7,
	RS,
	RW,
	EN,
	_LCD_NUM
}LCD_enuDataPins_t;


#endif /* LCD_CFG_H_ */
