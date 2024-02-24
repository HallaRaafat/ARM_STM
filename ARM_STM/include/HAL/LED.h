/*
 * LED.h
 *
 *  Created on: Feb 19, 2024
 *      Author: Halla Raafat
 */

#ifndef LED_H_
#define LED_H_

#include "HAL/LED_Cfg.h"
#include "LIB/STD_Types.h"

#define LED_CONN_FORWARD           0x00000000                //LED CONNECTION FORWARD
#define LED_CONN_REVERSED          0x00000001


#define LED_STATE_ON               0x00000001
#define LED_STATE_OFF              0x00000000



// struct for leds configuration

typedef struct
{
	uint32 Pin;
	void * Port ;
	uint32 Connection ;// forward or reversed
	uint32 InitState ;

}LED_Cfg_t;

typedef enum
{
	LED_OK,
	LED_NOK

}LED_ErrorStatus_t;

LED_ErrorStatus_t LED_Init(void);
// enter the led name defined in led_cfg file enum
// status ente LED_STATE_ON or LED_STATE_OFF

LED_ErrorStatus_t LED_SetState(LED_Names_t Led ,uint32 Status);


#endif /* LED_H_ */
