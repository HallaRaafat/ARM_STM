

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"
//#include "HAL/LED.h"
#include "HAL/LCD.h"
#include "LIB/STD_Types.h"
#include "MCAL/RCC.h"
#include "OS/Sched.h"
#include "APP/Traffic.h"
// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace-impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
//#define TEST_LED
//#define TEST_SYSTICK
//#define TEST_SCHED

#include <stdio.h>






int
main(int argc, char* argv[])
{


#ifdef TEST_SCHED

	RCC_ManageClock(RCC_CLOCK_HSI,RCC_STATE_ENABLE);
	RCC_SelectSystemClock(RCC_CLOCK_HSI);

    //Traffic_Init();



	SCHED_Init();
	SCHED_Start();





#endif


	RCC_ManageClock(RCC_CLOCK_HSI,RCC_STATE_ENABLE);
	RCC_SelectSystemClock(RCC_CLOCK_HSI);

	RCC_ControlPeripheral(RCC_AHB1,GPIOA,RCC_STATE_ENABLE);

    LCD_InitAsync();
	//LCD_WriteChar('H');

	SCHED_Error_t SCHED_Init();

	SCHED_Error_t SCHED_Start();
	// LCD_WriteChar( 'H');



    }




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
