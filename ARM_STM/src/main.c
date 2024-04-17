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



//time date array
/*

#define HOUR_TENTH  0
#define HOUR_UNIT   1


#define MIN_TENTH   3
#define MIN_UNIT    4


#define SEC_TENTH   6
#define SEC_UNIT    7

#define MS_UNIT     9
*/

#define Year_Idx  0
#define Month_Idx 1
#define Day_Idx   2
#define Hr_Idx    3
#define Min_Idx   4
#define Sec_Idx   5
#define Ms_Idx    6


#define SW_Hr_Idx    0
#define SW_Min_Idx   1
#define SW_Sec_Idx   2
#define SW_Ms_Idx    3

#define CLOCK_MODE     0
#define STOPWATCH_MODE 1

uint8 Current_Mode=CLOCK_MODE;

uint64 timestamp=0;
uint64 timestampSW=0;
uint8 Stopwatch_Resume_Start=0;


uint64 Current_Date[7]={
		[Year_Idx]=2024,
		[Month_Idx]=4,
		[Day_Idx]=17,
		[Hr_Idx]=11,
		[Min_Idx]=11,
		[Sec_Idx]=11,
		[Ms_Idx]=0,



};
uint64 Prev_Date[7]={
		[Year_Idx]=0,
		[Month_Idx]=0,
		[Day_Idx]=0,
		[Hr_Idx]=0,
		[Min_Idx]=0,
		[Sec_Idx]=11,
		[Ms_Idx]=0,



};
uint64 StopWatch[]={
		[SW_Hr_Idx]=0,
		[SW_Min_Idx]=0,
		[SW_Sec_Idx]=0,
		[SW_Ms_Idx]=0,


};
uint8 string[11];
void Convert_TimeStamptoDate(uint64 ms, uint64 *timeArray) ;
void Convert_TimestamptoStopwatch(uint64_t ms, uint64_t *timeArray) ;
void ConvertDigitToChar(uint8 digit, uint8 *buffer) {
    buffer[0] = (digit / 10) + '0';
    buffer[1] = (digit % 10) + '0';//convert to ascii
}
void ConvertDateToString(uint64_t *date, uint8_t *buffer) {
    buffer[0] = date[Year_Idx] / 1000 + '0';
    buffer[1] = (date[Year_Idx] / 100) % 10 + '0';
    buffer[2] = (date[Year_Idx] / 10) % 10 + '0';
    buffer[3] = date[Year_Idx] % 10 + '0';
    buffer[4] = '/'; // Separator between year and month
    ConvertDigitToChar(date[Month_Idx], buffer + 5);
    buffer[7] = '/'; // Separator between month and day
    ConvertDigitToChar(date[Day_Idx], buffer + 8);
    buffer[10] = '\0'; // Null-terminate the string
}
void Runnable_Timer()
{
timestamp++;
if(Stopwatch_Resume_Start==1)
	{
	    timestampSW++;
	}
}

void Runnable_Clock_Update()
{
	Convert_TimeStamptoDate(timestamp,Current_Date);
	if(Stopwatch_Resume_Start==1)
	{
	Convert_TimestamptoStopwatch(timestampSW,StopWatch);
	}
}

void APP_Runnable(void)
{
	//CLCD_ClearDisplayAsynch();
for(uint8 ind =0; ind<3;ind++)
{
if (Current_Date[ind]!= Prev_Date[ind])
{
	ConvertDateToString(Current_Date,string);
		 CLCD_SetCursorAsynch(LINE2,COL1);
		 CLCD_WriteStringAsynch(string,10);
		 Prev_Date[ind]=Current_Date[ind];
}

}


}

int
main(int argc, char* argv[])
{
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
//
//#include <stdio.h>
//
void Convert_TimeStamptoDate(uint64 ms, uint64 *timeArray) {
    // Constants for time conversion
    const long long msPerSecond = 1000;
    const long long msPerMinute = 60 * msPerSecond;
    const long long msPerHour = 60 * msPerMinute;
    const long long msPerDay = 24 * msPerHour;
    const long long msPerYear = 365 * msPerDay; // Approximation

    // Maximum values for each time unit
     int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Days in each month

    // Extract values from the array
    int years = timeArray[0];
    int months = timeArray[1];
    int days = timeArray[2];
    int hours = timeArray[3];
    int minutes = timeArray[4];
    int seconds = timeArray[5];

    // Increment the values
    years += ms / msPerYear;
    ms %= msPerYear;

    int remainingDays = ms / msPerDay;

    // Adjust for leap years
    int leapYear = (years % 4 == 0 && (years % 100 != 0 || years % 400 == 0));
    if (leapYear)
        daysInMonth[1] = 29;

    // Calculate months and days
    months += remainingDays / daysInMonth[months - 1];
    days += remainingDays % daysInMonth[months - 1];

    // Adjust for overflow
    if (days >= daysInMonth[months - 1]) {
        days -= daysInMonth[months - 1];
        months += 1;
    }

    // Adjust for overflow
    if (months > 12) {
        months -= 12;
        years += 1;
    }

    // Convert remaining milliseconds to hours, minutes, and seconds
    ms %= msPerDay;
    hours += ms / msPerHour;
    ms %= msPerHour;
    minutes += ms / msPerMinute;
    ms %= msPerMinute;
    seconds += ms / msPerSecond;

    // Adjust values if they exceed their maximum
    if (seconds >= 60) {
        minutes += seconds / 60;
        seconds %= 60;
    }

    if (minutes >= 60) {
        hours += minutes / 60;
        minutes %= 60;
    }

    if (hours >= 24) {
        days += hours / 24;
        hours %= 24;
    }

    // Update milliseconds in the array, capped at 999
    timeArray[6] = (int)((ms % msPerSecond) / 10);
    if (timeArray[6] > 99)
        timeArray[6] = 99;

    // Update the values in the array
    timeArray[0] = years;
    timeArray[1] = months;
    timeArray[2] = days;
    timeArray[3] = hours;
    timeArray[4] = minutes;
    timeArray[5] = seconds;
}

void Convert_TimestamptoStopwatch(uint64_t ms, uint64_t *timeArray) {
    // Create a separate array to hold the updated values
    uint64_t updatedTime[4] = {timeArray[SW_Hr_Idx], timeArray[SW_Min_Idx], timeArray[SW_Sec_Idx], timeArray[SW_Ms_Idx]};

    // Constants for time conversion
    const uint64_t msPerSecond = 1000;
    const uint64_t msPerMinute = 60 * msPerSecond;
    const uint64_t msPerHour = 60 * msPerMinute;

    // Increment the values
    updatedTime[SW_Hr_Idx] += ms / msPerHour;
    ms %= msPerHour;

    updatedTime[SW_Min_Idx] += ms / msPerMinute;
    ms %= msPerMinute;

    updatedTime[SW_Sec_Idx] += ms / msPerSecond;
    ms %= msPerSecond;

    // Update milliseconds in the array, capped at 999
    updatedTime[SW_Ms_Idx] += (ms + 5) / 10; // Rounding up to the nearest tenth
    if (updatedTime[SW_Ms_Idx] > 99)
        updatedTime[SW_Ms_Idx] = 99;

    // Handle overflow for seconds, minutes, and hours
    if (updatedTime[SW_Sec_Idx] >= 60) {
        updatedTime[SW_Min_Idx] += updatedTime[SW_Sec_Idx] / 60;
        updatedTime[SW_Sec_Idx] %= 60;
    }

    if (updatedTime[SW_Min_Idx] >= 60) {
        updatedTime[SW_Hr_Idx] += updatedTime[SW_Min_Idx] / 60;
        updatedTime[SW_Min_Idx] %= 60;
    }

    if (updatedTime[SW_Hr_Idx] >= 24) {
        // Handle overflow for hours exceeding 24
        updatedTime[SW_Hr_Idx] %= 24;
    }

    // Copy the updated values back to the original array
    for (int i = 0; i < 4; i++) {
        timeArray[i] = updatedTime[i];
    }
}
