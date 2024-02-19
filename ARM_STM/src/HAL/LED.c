/*
 * LED.c
 *
 *  Created on: Feb 19, 2024
 *      Author: Halla Raafat
 */

#include "HAL/LED.h"
#include "MCAL/GPIO.h"
#include "HAL/LED_Cfg.h"

/*
 * @brief    : function to initialize the pin based on user configuration
 * @param[in]: none (void)
 * @return   : Error status (LED_Ok,LED_NOK)
 */

extern const LED_Cfg_t LEDS_ARR [LED_NUMBERS_];

LED_ErrorStatus_t LED_Init(void)
{
	LED_ErrorStatus_t Local_ErrorState= LED_NOK;



    return Local_ErrorState;
}

/*
 * @brief    : function to set led state on or off
 * @param[in]: Led name , State on or off
 * @return   : Error status (LED_Ok,LED_NOK)
 */
LED_ErrorStatus_t LED_SetState(LED_Names_t Led ,uint32 Status)
{
	LED_ErrorStatus_t Local_ErrorState= LED_NOK;



	return Local_ErrorState;
}
