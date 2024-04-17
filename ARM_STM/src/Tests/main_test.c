/*
 * main_test.c
 *
 *  Created on: Apr 16, 2024
 *      Author: Lenovo
 */

#ifdef main_test


#ifdef TEST_1_H
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
#include "HAL/HSWITCH.h"
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
    Date_t Date ={.Year=2024,.Month=4,.Day=17};


	uint8 Time [12];
uint8	Increment_Day_Flag=0;
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


void Increment_Timer(Time_t * Timer)
{

	Timer->partofsecond+=1;

    if (Timer->partofsecond>=100)
    {
    	Timer->partofsecond = 0;
    	Timer->second++;


    }
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

         }
}
void Increment_StopWatch(Time_t * Timer)
{
Increment_Timer(Timer);

}
void Increment_Clock(Time_t * Timer,Date_t * Date)
	{
    Increment_Timer(Timer);

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
   // ConvertDigitToChar(date.Year / 1000, buffer);
    buffer[0]=date.Year / 1000+'0';
    buffer[1]=(date.Year / 100) % 10+'0';
    buffer[2]=(date.Year / 10) % 10+'0';
    buffer[3]=date.Year % 10+'0';
   // ConvertDigitToChar((date.Year / 100) % 10, buffer + 2);
   // ConvertDigitToChar((date.Year / 10) % 10, buffer + 4);
  //  ConvertDigitToChar(date.Year % 10, buffer + 6);
    buffer[4] = '/'; // Separator between year and month
    ConvertDigitToChar(date.Month, buffer + 5);
    buffer[7] = '/'; // Separator between month and day
    ConvertDigitToChar(date.Day, buffer + 8);
    buffer[10] = '\0'; // Null-terminate the string
}

uint8 Clockstring[12];
uint8 StopWatchString[12];

uint8 dateString[11];

/*
void APP_Runnable(void)

{
      CLCD_ClearDisplayAsynch();
      uint8 FIRST_SWITCH_STATUS=0;
     HSWITCH_GetStatus(HSWITCH_1,&FIRST_SWITCH_STATUS);

	 // uint8 dateString[11]; // Buffer to hold the date string (YYYY-MM-DD\0)
      ConvertDateToString(Date, dateString);
          CLCD_WriteStringAsynch(dateString,10);
      if (FIRST_SWITCH_STATUS==HSWITCH_PRESSED)
      {
    	 /* FormatTimeToString(currentTime, Clockstring);
    	  	   CLCD_SetCursorAsynch(LINE2,COL1);
    	  	   CLCD_WriteStringAsynch(Clockstring,11);


         Stopwatch.hour=0;
         Stopwatch.minute=0;
         Stopwatch.second=0;
         Stopwatch.partofsecond=0;

      }
	  /* FormatTimeToString(currentTime, Clockstring);
	   CLCD_SetCursorAsynch(LINE2,COL1);
	   CLCD_WriteStringAsynch(Clockstring,11);


      else if (FIRST_SWITCH_STATUS==HSWITCH_RELEASED)
      {
      FormatTimeToString(Stopwatch, StopWatchString);
     	   CLCD_SetCursorAsynch(LINE2,COL1);
     	   CLCD_WriteStringAsynch(StopWatchString,11);
      }
	//   CLCD_WriteStringAsynch(Timestring,11);


	   // CLCD_WriteStringAsynch("W",1);


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

}


void Runnable_Timer()
{
//	currentTime.partofsecond+=10;
	//Stopwatch.partofsecond+=10;
	Increment_Clock(&currentTime,&Date);
    Increment_StopWatch(&Stopwatch);

}
*/

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


/*
RCC_ManageClock(RCC_CLOCK_HSI,RCC_STATE_ENABLE);
RCC_SelectSystemClock(RCC_CLOCK_HSI);
RCC_ControlPeripheral(RCC_AHB1,GPIOA,RCC_STATE_ENABLE);
RCC_ControlPeripheral(RCC_AHB1,GPIOB,RCC_STATE_ENABLE);


SCHED_Init();
CLCD_InitAsynch();
HSWITCH_Init();
LED_Init();
SCHED_Start();
*/

	RCC_ManageClock(RCC_CLOCK_HSI,RCC_STATE_ENABLE);
		RCC_SelectSystemClock(RCC_CLOCK_HSI);

		RCC_ControlPeripheral(RCC_AHB1,GPIOA,RCC_STATE_ENABLE);
		RCC_ControlPeripheral( RCC_APB2, RCC_USART1 , RCC_STATE_ENABLE);
		RCC_ControlPeripheral( RCC_APB1, RCC_USART2 , RCC_STATE_ENABLE);


		uint8 x='T';
		uint8 z=0;
		uint8 Arr[5]={'T','a','r','e','K'};
		GPIO_Pin_Cfg_t UART_PINS[4]={
				//tx
				[0]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN9,.Speed=GPIO_OUTPUT_SPEED_HIGH},
				//rx
				[1]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN10,.Speed=GPIO_OUTPUT_SPEED_HIGH},
			//	[2]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN2,.Speed=GPIO_OUTPUT_SPEED_HIGH},
							//rx
				//[3]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN3,.Speed=GPIO_OUTPUT_SPEED_HIGH}
		};

		GPIO_InitPin(&UART_PINS[0]);
		GPIO_InitPin(&UART_PINS[1]);
		GPIO_CFG_AF(UART_PINS[0].Port,UART_PINS[0].Pin,GPIO_AF7);
		GPIO_CFG_AF(UART_PINS[1].Port,UART_PINS[1].Pin,GPIO_AF7);
		NVIC_Control_IRQ(NVIC_USART1,NVIC_ENABLE);

	/*	GPIO_InitPin(&UART_PINS[2]);
			GPIO_InitPin(&UART_PINS[3]);
			GPIO_CFG_AF(UART_PINS[2].Port,UART_PINS[2].Pin,GPIO_AF7);
			GPIO_CFG_AF(UART_PINS[3].Port,UART_PINS[3].Pin,GPIO_AF7);
			*/
	/*
	 * {
	 USART_Names_t  	USART_Num; // only in configured names in USART_Cfg.h
	 uint8 *	        Ptr_Buffer;
	 uint32             Length;
	 CB_t               CallBack; // called after finishing Request
	}User_Request_t;
	 *
	 */
	/*
	User_Request_t Tx;
	Tx.Ptr_Buffer=&x;
	Tx.USART_Num=USART_1;
	*/

		User_Request_t Tx;
		Tx.Ptr_Buffer=Arr;
		Tx.USART_Num=USART_1;
		Tx.CallBack=Set_Led;
	    Tx.Length=5;
	uint8 Rx_Arr[5]={0};



	 User_Request_t Rx;
	 Rx.Ptr_Buffer=Rx_Arr;
	 	Rx.USART_Num=USART_1;
	 	Rx.CallBack=Set_Led2;
	     Rx.Length=5;

	LED_Init();
		USART_Init();
		USART_SendByte(&Tx);
		USART_GetByte(&Rx);
	USART_SendBufferAsync(&Tx);
	USART_ReceiveBufferAsync(&Rx);
		//USART_SendByte(&Tx);


	while(1)
	{

		if (Rx_Arr[0]=='a')
			{
				LED_SetState(LED_Green,LED_STATE_ON);

			}
		else
		{
			LED_SetState(LED_Green,LED_STATE_OFF);
		}


	}


    }




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
#endif

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
#include "HAL/HSWITCH.h"
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

#define CLOCK_MODE     0
#define STOPWATCH_MODE 1


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
    Date_t Date ={.Year=2024,.Month=4,.Day=17};


	uint8 Time [12];
uint8	Increment_Day_Flag=0;
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


void Increment_Timer(Time_t * Timer)
{

	Timer->partofsecond+=1;

    if (Timer->partofsecond>=100)
    {
    	Timer->partofsecond = 0;
    	Timer->second++;


    }
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

         }
}
void Increment_StopWatch(Time_t * Timer)
{
Increment_Timer(Timer);

}
void Increment_Clock(Time_t * Timer,Date_t * Date)
	{
    Increment_Timer(Timer);

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
   // ConvertDigitToChar(date.Year / 1000, buffer);
    buffer[0]=date.Year / 1000+'0';
    buffer[1]=(date.Year / 100) % 10+'0';
    buffer[2]=(date.Year / 10) % 10+'0';
    buffer[3]=date.Year % 10+'0';
   // ConvertDigitToChar((date.Year / 100) % 10, buffer + 2);
   // ConvertDigitToChar((date.Year / 10) % 10, buffer + 4);
  //  ConvertDigitToChar(date.Year % 10, buffer + 6);
    buffer[4] = '/'; // Separator between year and month
    ConvertDigitToChar(date.Month, buffer + 5);
    buffer[7] = '/'; // Separator between month and day
    ConvertDigitToChar(date.Day, buffer + 8);
    buffer[10] = '\0'; // Null-terminate the string
}



uint8 dateString[11]="2024/04/17";
uint8 Clockstring[11]="11:11:01:1";
uint8 StopWatchString[12]="00:00:00:0";
uint8 Cursor_idx=0;
uint8 FIRST_SWITCH_STATUS=0;
uint8 SECOND_SWITCH_STATUS=0;
uint8 THIRD_SWITCH_STATUS=0;


void APP_Runnable(void)

{
    //  CLCD_ClearDisplayAsynch();

	FormatTimeToString(currentTime, Clockstring);
	ConvertDateToString(Date, dateString);
     HSWITCH_GetStatus(HSWITCH_Mode,&FIRST_SWITCH_STATUS );
      HSWITCH_GetStatus(HSWITCH_Edit_Reset,&SECOND_SWITCH_STATUS);
      HSWITCH_GetStatus(HSWITCH_OK_Start,&THIRD_SWITCH_STATUS);

      static uint8 Mode_Pressed=0;
      static uint8 Change_Mode=CLOCK_MODE;

      static Time_t prevClock = {.minute = 11, .hour = 11, .second = 0, .partofsecond = 0};
      static Time_t prevStopwatch = {.minute = 0, .hour = 0, .second = 0, .partofsecond = 0};

      static Date_t prevDate  = {.Year = 2024, .Month = 4, .Day = 17};

      //   CLCD_ClearDisplayAsynch();

         // Check if the clock mode is active
         if (Change_Mode == CLOCK_MODE && Mode_Pressed == 0) {
        	 FormatTimeToString(currentTime, Clockstring);
        	 if(prevClock.hour != currentTime.hour)
        	 {
             ConvertDateToString(Date, dateString);
             CLCD_SetCursorAsynch(LINE1, COL1);
             CLCD_WriteStringAsynch(dateString, 10);
// 11:11:11:99
// 01:34:67:910

        	 }
             // Compare each digit of the current time with the previous time
             if (prevClock.hour != currentTime.hour) {
                 CLCD_SetCursorAsynch(LINE2, COL1);
                 CLCD_WriteStringAsynch(Clockstring, 3);
             }
             if (prevClock.minute != currentTime.minute) {
                 CLCD_SetCursorAsynch(LINE2, COL4);
                 CLCD_WriteStringAsynch(Clockstring + 3, 3);
             }
             if (prevClock.second != currentTime.second) {
                 CLCD_SetCursorAsynch(LINE2, COL7);
                 CLCD_WriteStringAsynch(Clockstring + 6, 3);
             }
             if (prevClock.partofsecond != currentTime.partofsecond) {
                 CLCD_SetCursorAsynch(LINE2, COL10);
                 CLCD_WriteStringAsynch(Clockstring + 9, 4);
             }

             // Update previous time
             prevClock = currentTime;
         }
         // Check if the stopwatch mode is active
         else if (Change_Mode == STOPWATCH_MODE && Mode_Pressed == 0) {
             FormatTimeToString(Stopwatch, StopWatchString);

             // Compare each digit of the stopwatch with the previous stopwatch time
             if (prevStopwatch.hour != Stopwatch.hour) {
                 CLCD_SetCursorAsynch(LINE2, COL1);
                 CLCD_WriteStringAsynch(StopWatchString, 2);
             }
             if (prevStopwatch.minute != Stopwatch.minute) {
                 CLCD_SetCursorAsynch(LINE2, COL4);
                 CLCD_WriteStringAsynch(StopWatchString + 3, 2);
             }
             if (prevStopwatch.second != Stopwatch.second) {
                 CLCD_SetCursorAsynch(LINE2, COL7);
                 CLCD_WriteStringAsynch(StopWatchString + 6, 2);
             }
             if (prevStopwatch.partofsecond != Stopwatch.partofsecond) {
                 CLCD_SetCursorAsynch(LINE2, COL10);
                 CLCD_WriteStringAsynch(StopWatchString + 9, 3);
             }

             // Update previous stopwatch time
             prevStopwatch = Stopwatch;
         }
     }



//
//	 // uint8 dateString[11]; // Buffer to hold the date string (YYYY-MM-DD\0)
//
//      if (FIRST_SWITCH_STATUS==HSWITCH_PRESSED)
//      {
//		 Mode_Pressed=1;
//      }
//      else if (FIRST_SWITCH_STATUS==HSWITCH_RELEASED&& Mode_Pressed==1)
//      {
//    	  if (Counter==0)
//    	  {
//    		  Counter=1;
//    	  }
//    	  else if (Counter==1)
//    	  {
//    		  Counter=0;
//    	  }
//          Mode_Pressed=0;
//
//      }
//
//      if (Counter==0 && Mode_Pressed==0)
//      {
//          ConvertDateToString(Date, dateString);
//
//               FormatTimeToString(currentTime, Clockstring);
//    		   CLCD_SetCursorAsynch(LINE1,COL1);
//    		   CLCD_WriteStringAsynch(dateString,10);
//    		   CLCD_SetCursorAsynch(LINE2,COL1);
//    		   CLCD_WriteStringAsynch(Clockstring,11);
//
//
//
//      }
//      else if (Counter==1 && Mode_Pressed==0)
//      {
//
//          FormatTimeToString(Stopwatch, StopWatchString);
//        		   CLCD_SetCursorAsynch(LINE2,COL1);
//        		   CLCD_WriteStringAsynch(StopWatchString,11);
//
//      }
//      if (SECOND_SWITCH_STATUS==HSWITCH_PRESSED)
//      {
//    	  Stopwatch.hour=0;
//		  Stopwatch.minute=0;
//		  Stopwatch.second=0;
//		  Stopwatch.partofsecond=0;
//
//      }
//
//      {
//			  Stopwatch.hour=0;
//			  Stopwatch.minute=0;
//			  Stopwatch.second=0;
//			  Stopwatch.partofsecond=0;
//      }
//      else if (THIRD_SWITCH_STATUS==HSWITCH_PRESSED)
//      {
//
//      }
//
//
//	//   CLCD_WriteStringAsynch(Timestring,11);
//
//
//	   // CLCD_WriteStringAsynch("W",1);
//
//
//	// SW_MUSIC_PLAY
//	//HSWITCH_GetStatus(HSWITCH_1,&FIRST_SWITCH_STATUS);
//
//
//	 SW_GetState(SW_MUSIC_PLAY,&FIRST_SWITCH_STATUS);
//	if (FIRST_SWITCH_STATUS==HSWITCH_PRESSED)
//	   		{
//	  		    CLCD_WriteStringAsynch("WELCOME HOME",12);
//		LED_SetState(LED_Red,LED_STATE_ON);
//		  CLCD_ClearDisplayAsynch();
//
//	   		}
//	  	else if (FIRST_SWITCH_STATUS==HSWITCH_RELEASED)
//	  	{
//	  	   CLCD_ClearDisplayAsynch();
//	  		LED_SetState(LED_Red,LED_STATE_OFF);
//
//	  	}
//
//
//
//    CLCD_ClearDisplayAsynch();
//    CLCD_SetCursorAsynch(LINE2,COL1);
//    FormatTimeToString(currentTime, Time);
//	    CLCD_WriteStringAsynch(Time,8);
//



void Runnable_Timer()
{
//	currentTime.partofsecond+=10;
	//Stopwatch.partofsecond+=10;
	Increment_Clock(&currentTime,&Date);

    Increment_StopWatch(&Stopwatch);

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

#endif




