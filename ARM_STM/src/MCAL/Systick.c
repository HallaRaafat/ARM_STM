/*
 * Systick.c
 *
 *  Created on: Mar 11, 2024
 *      Author: Halla Raafat
 */
#include "MCAL/Systick.h"



#define SYSTICK_BASE_ADDRESS  (0xE000E010)
#define SYSTICK_CTRL_MASK      0x00000007


#define SYSTICK_MAX_VAL       (0x00ffffff)
#define SYSTICK_MIN_VAL       (0x00000001)

//#define SYS_CLOCK              400000   //HSI clock 16 no dev
#define SYS_CLOCK              16000000   //HSI clock 16 no dev
#define CLEAR_VAL              55         //any value to clear current
typedef struct
{
	uint32 CTRL;
	uint32 LOAD;
	uint32 VAL;
	uint32 CALIB;

}Systick_t;

volatile Systick_t * const STK = (volatile Systick_t * const)SYSTICK_BASE_ADDRESS;

static Systick_CallBackFunc_t  APP_CallBack;
/*
 * CTRL->
 * bit16 count flag rw
 * return 1 if timer to 0 since last read
 *
 * bit 2 CLKSOURCE 0-> AHB/8 , 1 ->AHB
 *
 * bit 1 TICKINT count to 0
 * write 0 -> no exception 1-> assert exception request
 *
 * bit 0 counter enable
 *  Enables the counter. When ENABLE is set to 1, the counter loads the RELOAD value from the
LOAD register and then counts down. On reaching 0, it sets the COUNTFLAG to 1 and
optionally asserts the SysTick depending on the value of TICKINT. It then loads the RELOAD
value again, and begins counting.
0: Counter disabled
1: Counter enabled

LOAD -> 23:0 bits
 in the range 0x00000001-0x00FFFFFF
 16,777,215
 n-1 ? 100 clk pulse set reload =99

 VAL-> 23:0 CURRENT
 read current value of sysytick counter
 any write  current =0 +clear count flag in CTRL

  1. Reload value
  2. clear current
  3. program conrol + status register

 */



SYSTICK_Error_t  SYSTICK_Start(uint32 SYSTICK_CLK)
{
	SYSTICK_Error_t  Local_Error = SYSTICK_NOK;

   if (SYSTICK_CLK==SYSTICK_CLK_AHB_NO_DEV ||SYSTICK_CLK==SYSTICK_CLK_AHB_DEV_8 )
   {
      uint32 Local_CTRL= STK->CTRL;
             Local_CTRL &= ~ SYSTICK_CTRL_MASK ; //clear ctrl bits 0,1,2
             Local_CTRL |= SYSTICK_CLK;
             STK->CTRL=Local_CTRL;

	   Local_Error = SYSTICK_OK;
   }
    return Local_Error;
}

SYSTICK_Error_t  SYSTICK_Stop()
{

	SYSTICK_Error_t  Local_Error = SYSTICK_OK;

	STK->CTRL&=~ SYSTICK_CTRL_MASK;    //clear control bits

	    return Local_Error;
}


SYSTICK_Error_t  SYSTICK_SetTimeMS(uint32 TimeMS)
{
	SYSTICK_Error_t  Local_Error = SYSTICK_NOK;
    uint32 Local_Time=( TimeMS*(SYS_CLOCK/1000))-1;
	if(Local_Time>0  && Local_Time< SYSTICK_MAX_VAL)
	{
       STK->LOAD=Local_Time;
       STK->VAL= CLEAR_VAL;


		Local_Error = SYSTICK_OK;
	}
	    return Local_Error;

}

SYSTICK_Error_t  SYSTICK_SetCallBack(Systick_CallBackFunc_t Function )
{SYSTICK_Error_t  Local_Error = SYSTICK_NOK;


if (Function)
{
	APP_CallBack=Function;
	Local_Error = SYSTICK_OK;
}
return Local_Error;
}


void SysTick_Handler()
{
if (APP_CallBack)
{
	APP_CallBack();
}
}










