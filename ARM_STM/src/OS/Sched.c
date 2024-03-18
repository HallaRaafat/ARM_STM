/*
 * Sched.c
 *
 *  Created on: Mar 15, 2024
 *      Author: Halla Raafat
 */
#include "OS/Sched.h"
#include "OS/Sched_Runnables.h"
#include "MCAL/Systick.h"





typedef struct
{
	Runnable_t * Runnable;
	uint32  RemainTime_ms;
}Runnable_Info_t;




static uint8 Pending_Ticks=0;

extern const  Runnable_t Runnables_Array[_Num_Runnables];

Runnable_Info_t RunnableInfo_Array[_Num_Runnables];


static void Sched();
static void Tick_CallBack();




/*
 * Initialize Systick by
 * 1. set systick tick time
 * 2. set systick call back function
 * 3. initalize the runnable info array by delay value
 */
SCHED_Error_t SCHED_Init()
{
	SCHED_Error_t Local_ErrorState= SCHED_NOK;
	SYSTICK_Error_t Local_SystickError=SYSTICK_NOK;

 // initalize remaintime by delay in Runnables array
	for(Runnables Index =0 ; Index<_Num_Runnables;Index++)
	{
		RunnableInfo_Array[Index].Runnable= & Runnables_Array[Index];
		RunnableInfo_Array[Index].RemainTime_ms= Runnables_Array[Index].Delay_ms;

	}

    Local_SystickError=SYSTICK_SetTimeMS(SCHED_TICK_TIME);
    Local_SystickError=SYSTICK_SetCallBack(Tick_CallBack) ;



  if (Local_SystickError==SYSTICK_OK) Local_ErrorState=SCHED_OK;

  return Local_ErrorState;
}
/*
 *
 */

SCHED_Error_t SCHED_Start()
{
	SCHED_Error_t Local_ErrorState= SCHED_NOK;
	SYSTICK_Error_t Local_SystickError=SYSTICK_NOK;
	 Local_SystickError=SYSTICK_Start(SYSTICK_CLK_AHB_NO_DEV);

while(1)
{
  if (Pending_Ticks)
  {
	  Pending_Ticks--;
	  Sched();
  }
}
if (Local_SystickError==SYSTICK_OK) Local_ErrorState=SCHED_OK;
return Local_ErrorState;

}

static void Sched()
{

	for(Runnables  Index =0 ; Index<_Num_Runnables;Index++)
	{
      if (  RunnableInfo_Array[Index].RemainTime_ms==0&& RunnableInfo_Array[Index].Runnable->Callback )
      {
    	  RunnableInfo_Array[Index].RemainTime_ms=RunnableInfo_Array[Index].Runnable->Periodicity_ms;
    	  RunnableInfo_Array[Index].Runnable->Callback();

      }
      else
      {
    	  RunnableInfo_Array[Index].RemainTime_ms-= SCHED_TICK_TIME;
      }
	}


}
static void Tick_CallBack()
{
  Pending_Ticks++;
}


