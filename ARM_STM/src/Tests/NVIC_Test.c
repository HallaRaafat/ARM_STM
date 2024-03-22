/*
 * NVIC_Test.c
 *
 *  Created on: Mar 18, 2024
 *      Author: Halla Raafat
 */

#ifdef TEST_NVIC
#include "HAL/LED.h"

#include "LIB/STD_Types.h"
#include "MCAL/RCC.h"


#include "MCAL/STM32F401C_IRQ.h"
#include "MCAL/NVIC.h"


void  EXTI0_IRQHandler()
{
	 LED_SetState(LED_Green,LED_STATE_ON);
}



int
main(int argc, char* argv[])
{




	RCC_ManageClock(RCC_CLOCK_HSI,RCC_STATE_ENABLE);
	RCC_SelectSystemClock(RCC_CLOCK_HSI);

   // Traffic_Init();

	RCC_ControlPeripheral(RCC_AHB1,GPIOA,RCC_STATE_ENABLE);

	LED_Init();

	//SCHED_Init();
//	SCHED_Start();
NVIC_Control_IRQ(NVIC_EXTI0,NVIC_ENABLE);
NVIC_GenerateSWInterrupt(NVIC_EXTI0);


while(1){}





    }

#endif
