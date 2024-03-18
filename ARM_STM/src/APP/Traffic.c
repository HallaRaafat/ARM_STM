/*
 * Traffic.c
 *
 *  Created on: Mar 18, 2024
 *      Author: Halla Raafat
 */

#include "HAL/LED.h"
#include "MCAL/RCC.h"



#define TICK_MS     2
void Traffic_Init()
{
	RCC_ControlPeripheral(RCC_AHB1,GPIOA,RCC_STATE_ENABLE);

	LED_Init();


}
void Traffic_Runnable()
{
    static uint8 Counter=0;
	static uint8 State ='G';//Green is initial value for state machine
	static uint8 Prev='R';
	Counter+=TICK_MS;
	switch(State)
	{
	case'G':
		     LED_SetState(LED_Green,LED_STATE_ON);
		     LED_SetState(LED_Yellow,LED_STATE_OFF);
		     LED_SetState(LED_Red,LED_STATE_OFF);

             if (Counter==6)
             {   Counter=0;
            	 Prev='G';
            	 State='Y';

             }
		     break;

	case'Y':
			LED_SetState(LED_Green,LED_STATE_OFF);
			LED_SetState(LED_Yellow,LED_STATE_ON);
			LED_SetState(LED_Red,LED_STATE_OFF);
			 if (Counter==2 )
			 {
				 if (Prev=='G')
				 {
					 State='R';
				 }
				 else if (Prev=='R')
				 {
					 State='G';
				 }

				 Counter=0;
				 Prev='Y';

			 }

			     break;
	case'R':
			LED_SetState(LED_Green,LED_STATE_OFF);
			LED_SetState(LED_Yellow,LED_STATE_OFF);
			LED_SetState(LED_Red,LED_STATE_ON);
			if (Counter==20)
			             {   Counter=0;
			            	 Prev='R';
			            	 State='Y';

			             }
			     break;
	default:
		     break;





	}


}


