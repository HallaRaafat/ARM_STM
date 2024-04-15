

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"
#include "HAL/LED.h"
#include "HAL/LCD.h"
#include "LIB/STD_Types.h"
#include "MCAL/RCC.h"
#include "OS/Sched.h"
#include "APP/Traffic.h"
#include "MCAL/GPIO.h"
#include "MCAL/USART.h"
#include "HAL/CLCD.h"
//#include "HAL/HSWITCH.h"
#include "HAL/Switch.h"
#include "MCAL/STM32F401C_IRQ.h"
#include "MCAL/NVIC.h"
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

void Set_Led()
{
	LED_SetState(LED_Red,LED_STATE_ON);
}
void Set_Led2()
{
	LED_SetState(LED_Yellow,LED_STATE_ON);
}
typedef struct
{
	uint8 Day;
	uint8 Month;
	uint32 Year;

}Date_t;
typedef struct {
	uint8 minute ;
	uint8 second ;
	uint8 hour;
	uint8 partofsecond;
	}Time_t;


	Time_t currentTime={.minute=11,.hour=11,.second=0,.partofsecond=10};
    Time_t Stopwatch={.minute=0,.hour=0,.second=0,.partofsecond=0};
    Date_t Date ={.Year=1999,.Month=11,.Day=29};


	uint8 Time [12];
void Increment_Date(Date_t * Date )
{
	 uint32 daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


	        // Check for end of month
	        if (Date->Day > daysInMonth[Date->Month - 1])
	        {
	            Date->Day = 1;
	            Date->Month++;

	            // Check for end of year
	            if (Date->Month > 12)
	            {
	                Date->Month = 1;
	                Date->Year++;
	            }
	        }



}
void Increment_Time(Time_t * Timer,Date_t * Date)
	{

	Timer->partofsecond+=10;
    if (Timer->partofsecond>=100)
    {
    	Timer->partofsecond = 0;
    	Timer->second++;
        if (Timer->second>= 60)
        {
        	Timer->second= 0;
        	Timer->minute++;
        }
        if (Timer->minute >= 60)
        {
        	Timer->minute = 0;
        	Timer->hour++;
        }
        if (Timer->hour >= 24)
        {
            Timer->hour = 0;
            Date->Day++;
        }
    }

    Increment_Date(Date);

	}
void ConvertDigitToChar(uint8 digit, uint8 *buffer) {
    buffer[0] = (digit / 10) + '0';
    buffer[1] = (digit % 10) + '0';//convert to ascii
}

void FormatTimeToString(Time_t time, uint8 *buffer) {
    ConvertDigitToChar(time.hour, buffer);
    buffer[2] = ':';
    ConvertDigitToChar(time.minute, buffer + 3);
    buffer[5] = ':';
    ConvertDigitToChar(time.second, buffer + 6);
    buffer[8] = '.';
    ConvertDigitToChar(time.partofsecond, buffer + 9);
    buffer[11] = '\0'; // Null terminator
}


void ConvertDateToString(Date_t date, uint8 *buffer)
{
    ConvertDigitToChar(date.Year / 1000, buffer);
    ConvertDigitToChar((date.Year / 100) % 10, buffer + 2);
    ConvertDigitToChar((date.Year / 10) % 10, buffer + 4);
    ConvertDigitToChar(date.Year % 10, buffer + 6);
    buffer[4] = '-'; // Separator between year and month
    ConvertDigitToChar(date.Month, buffer + 8);
    buffer[10] = '-'; // Separator between month and day
    ConvertDigitToChar(date.Day, buffer + 11);
    buffer[13] = '\0'; // Null-terminate the string
}
void APP_Runnable(void)
{
   //CLCD_ClearDisplayAsynch();

	 // uint8 dateString[11]; // Buffer to hold the date string (YYYY-MM-DD\0)
      uint8 Timestring[12];
	   // ConvertDateToString(Date, dateString);
	    FormatTimeToString(currentTime, Timestring);

	   // CLCD_WriteStringAsynch(dateString,11);
	    CLCD_SetCursorAsynch(LINE2,COL1);
	   // CLCD_WriteStringAsynch(Timestring,11);


	    CLCD_WriteStringAsynch("W",1);

	 //uint8 FIRST_SWITCH_STATUS=0;
	// SW_MUSIC_PLAY
	//HSWITCH_GetStatus(HSWITCH_1,&FIRST_SWITCH_STATUS);
/*

	 SW_GetState(SW_MUSIC_PLAY,&FIRST_SWITCH_STATUS);
	if (FIRST_SWITCH_STATUS==HSWITCH_PRESSED)
	   		{
	  		    CLCD_WriteStringAsynch("WELCOME HOME",12);
		LED_SetState(LED_Red,LED_STATE_ON);
		  CLCD_ClearDisplayAsynch();

	   		}
	  	else if (FIRST_SWITCH_STATUS==HSWITCH_RELEASED)
	  	{
	  	   CLCD_ClearDisplayAsynch();
	  		LED_SetState(LED_Red,LED_STATE_OFF);

	  	}
*/

	/*
    CLCD_ClearDisplayAsynch();
    CLCD_SetCursorAsynch(LINE2,COL1);
    FormatTimeToString(currentTime, Time);
	    CLCD_WriteStringAsynch(Time,8);
	    */
}


void Runnable_Timer()
{
//	currentTime.partofsecond+=10;
	//Stopwatch.partofsecond+=10;
	Increment_Time(&currentTime,&Date);


}
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
/*	uint8 data ='H';

	RCC_ManageClock(RCC_CLOCK_HSI,RCC_STATE_ENABLE);
	RCC_SelectSystemClock(RCC_CLOCK_HSI);

	RCC_ControlPeripheral(RCC_AHB1,GPIOA,RCC_STATE_ENABLE);

    LCD_InitAsync();

	SCHED_Init();


	SCHED_Start();
	LCD_WriteStringAsync(&data,1,0,0);
	// LCD_WriteChar( 'H');

*/



RCC_ManageClock(RCC_CLOCK_HSI,RCC_STATE_ENABLE);
RCC_SelectSystemClock(RCC_CLOCK_HSI);
RCC_ControlPeripheral(RCC_AHB1,GPIOA,RCC_STATE_ENABLE);
RCC_ControlPeripheral(RCC_AHB1,GPIOB,RCC_STATE_ENABLE);


SCHED_Init();
CLCD_InitAsynch();
HSWITCH_Init();
LED_Init();
SCHED_Start();



    }




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
