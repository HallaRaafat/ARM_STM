/*
 * Systick.h
 *
 *  Created on: Mar 11, 2024
 *      Author: Halla Raafat
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_
#include "LIB/STD_Types.h"


#define SYSTICK_CLK_AHB_NO_DEV        0x00000007            //AHB BUS NO DEVISION
#define SYSTICK_CLK_AHB_DEV_8         0x00000003                      //AHB/8

typedef void (*Systick_CallBackFunc_t) (void); // void cause it can be casted to any data type

typedef enum
{

SYSTICK_OK,
SYSTICK_NOK

}SYSTICK_Error_t;


SYSTICK_Error_t  SYSTICK_Start(uint32 SYSTICK_CLK);

SYSTICK_Error_t  SYSTICK_Stop();

SYSTICK_Error_t  SYSTICK_SetTimeMS(uint32 TimeMS);




SYSTICK_Error_t  SYSTICK_SetCallBack(Systick_CallBackFunc_t Function );
#endif /* SYSTICK_H_ */
