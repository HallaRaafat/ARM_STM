/*
 * Sched_Runnables.c
 *
 *  Created on: Mar 17, 2024
 *      Author: Halla Raafat
 */
#include "OS/Sched.h"
#include "OS/Sched_Runnables.h"

//#define TRAFFIC_PERIODICITY_MS     2000
//#define TRAFFIC_INITIAL_DELAY_MS   1000

#define LCD_PERIODICITY_MS     1
#define LCD_INITIAL_DELAY_MS   0

#define HSWITCH_PERIODICITY_MS 5

#define APP_PERIODICITY_MS     1000

#define TIMER_PERIODICITY_MS    1
#define TIMER_INITIAL_DELAY_MS   1



//void Traffic_Runnable();

//void LCD_Runnable();

void RunnableLCD();
void APP_Runnable();
void Runnable_HSWITCH();
void Runnable_Timer();
void Runnable_Clock_Update();
void UART_Runnable();
void Input_Runnable();
/*
 * 1. Write the prototype of the call back function before the array just to compile
 * 2. Fill the array as the priorties depends on the enum sequence in .h file
 * 3.
 *   [Enum]={           .Periodicity_ms=(Periodic time of the runnable ),
		                .Delay_ms= (initial delay of the runnable),
		                .Callback=(Function )}
 */
const Runnable_t Runnables_Array[_Num_Runnables]=
{
/*
[Runnable_Traffic]={
		                .Periodicity_ms=TRAFFIC_PERIODICITY_MS,
		                .Delay_ms= TRAFFIC_INITIAL_DELAY_MS,
		                .Callback= Traffic_Runnable,

                   }


[Runnable_LCD]={
						.Periodicity_ms=LCD_PERIODICITY_MS,
						.Delay_ms= LCD_INITIAL_DELAY_MS,
						.Callback= LCD_Runnable,

				   }

*/

[Runnable_LCD]={
						.Periodicity_ms=LCD_PERIODICITY_MS,
						.Delay_ms= LCD_INITIAL_DELAY_MS,
						.Callback= RunnableLCD,

				   },

[Runnable_APP]={
						.Periodicity_ms=APP_PERIODICITY_MS,
						.Delay_ms= LCD_INITIAL_DELAY_MS,
						.Callback= APP_Runnable,

				   },

[Runnable_SWITCH]={
						.Periodicity_ms=HSWITCH_PERIODICITY_MS,
						.Delay_ms= LCD_INITIAL_DELAY_MS,
						.Callback= Runnable_HSWITCH,

				   },



[Runnable_Timer_Update]={
						.Periodicity_ms=TIMER_PERIODICITY_MS,
						.Delay_ms= TIMER_INITIAL_DELAY_MS,
						.Callback= Runnable_Timer,

				   },



[Runnable_Clock_Stop_Update]={
						.Periodicity_ms=50,
						.Delay_ms= 1,
						.Callback= Runnable_Clock_Update,

				   },


[Runnable_UART]={
					.Periodicity_ms=50,
					.Delay_ms= 2,
					.Callback= UART_Runnable,

			   },
[Runnable_Input]={
		.Periodicity_ms=50,
		.Delay_ms= 2,
		.Callback= Input_Runnable,

   }

};

