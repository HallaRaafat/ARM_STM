/*
 * Switch.c
 *
 *  Created on: Feb 25, 2024
 *      Author: Halla Raafat
 */


#include "HAL/Switch.h"
#include "HAL/Switch_Cfg.h"
#include "MCAL/GPIO.h"

const SW_Cfg_t SW_ARR [SW_NUMBERS_];
SW_ErrorStatus_t SW_Init(void)
{
	SW_ErrorStatus_t Local_ErrorState= SW_NOK;
	GPIO_Pin_Cfg_t Switch ;

    Switch.Speed = GPIO_OUTPUT_SPEED_MED;
	for (int i =0 ;i <SW_NUMBERS_;i++)
	{
		Switch.Mode= SW_ARR[i].ActiveState;
		Switch.Port= SW_ARR[i].Port;
		Switch.Pin= SW_ARR[i].Pin;


        GPIO_ErrorStatus_t Local_GPIO_Error= GPIO_InitPin(&Switch);
        if (Local_GPIO_Error== GPIO_OK)
        	Local_ErrorState= SW_OK;
        else
        	Local_ErrorState= SW_NOK;

	}


    return Local_ErrorState;


}

SW_ErrorStatus_t SW_GetState(SW_Names_t Switch ,uint8 *PtrState)

{
	SW_ErrorStatus_t Local_ErrorState= SW_NOK;
    uint8 Local_SW_Value;
	GPIO_ErrorStatus_t Local_GPIO_Error= GPIO_GetPinValue(SW_ARR[Switch].Port,SW_ARR[Switch].Pin,&Local_SW_Value);

	/*
	 *                   local   notpres
	 *  pu    pressed       0       1     1000
 	 *  pd    pressed       1       0    10000
	 */
     *PtrState = !(Local_SW_Value^(SW_ARR[Switch].ActiveState >>4));


	          if (Local_GPIO_Error== GPIO_OK)
	          	Local_ErrorState= SW_OK;
	          else
	          	Local_ErrorState= SW_NOK;


		return Local_ErrorState;


}
