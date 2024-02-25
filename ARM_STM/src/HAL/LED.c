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
	GPIO_Pin_Cfg_t Led ;
	Led.Mode= GPIO_OUTPUT_PP;
    Led.Speed = GPIO_OUTPUT_SPEED_MED;
	for (int i =0 ;i <LED_NUMBERS_;i++)
	{

        Led.Port = LEDS_ARR[i].Port;
        Led.Pin = LEDS_ARR[i].Pin;

        GPIO_ErrorStatus_t Local_GPIO_Error= GPIO_InitPin(&Led);
        if (Local_GPIO_Error== GPIO_OK)
        	Local_ErrorState= LED_OK;
        else
        	Local_ErrorState= LED_NOK;

	}


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
    /*
     * xor
     *                             so if ON is 1 and off is 0   by doing xor
     *      00  0                 frw is 0     1            0
     *      01  1                 rvr is 1     0            1
     *      10  1
     *      11  0
     */
	GPIO_ErrorStatus_t Local_GPIO_Error= GPIO_SetPinValue(LEDS_ARR[Led].Port,LEDS_ARR[Led].Pin,(LEDS_ARR[Led].Connection^Status));

          if (Local_GPIO_Error== GPIO_OK)
          	Local_ErrorState= LED_OK;
          else
          	Local_ErrorState= LED_NOK;
	return Local_ErrorState;
}
