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
uint8 Received_Data=9;
uint8 Pressed_Key_Flag=0;
uint8 Pressed_Key=0;
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
		[Sec_Idx]=0,
		[Ms_Idx]=0,



};
uint64 StopWatch[]={
		[SW_Hr_Idx]=0,
		[SW_Min_Idx]=0,
		[SW_Sec_Idx]=0,
		[SW_Ms_Idx]=0,


};
typedef struct
{
uint8 Line;
uint8 Col;
}Cursor_t;
Cursor_t Cursor_Update[7]=
{
		[Year_Idx]={.Line=0,.Col=0},
			[Month_Idx]={.Line=0,.Col=5},
			[Day_Idx]={.Line=0,.Col=8},

			[Hr_Idx]={.Line=1,.Col=0},
			[Min_Idx]={.Line=1,.Col=3},
			[Sec_Idx]={.Line=1,.Col=6},
			[Ms_Idx]={.Line=1,.Col=9},

};
//extern uint8 HSWITCH_status[_HSWITCH_NUM];
void Convert_TimeStamptoDate(uint64 ms, uint64 *timeArray) ;
void Convert_TimestamptoStopwatch(uint64_t ms, uint64_t *timeArray) ;
void Set_Led()
{
	LED_SetState(LED_Red,LED_STATE_ON);
}

void ConvertDigitToChar(uint8 digit, uint8 *buffer) {
    buffer[0] = (digit / 10) + '0';
    buffer[1] = (digit % 10) + '0';//convert to ascii
}
void FormatTimeToString(uint64_t *time, uint8_t *buffer) {
    ConvertDigitToChar(time[Hr_Idx], buffer);
    buffer[2] = ':';
    ConvertDigitToChar(time[Min_Idx], buffer + 3);
    buffer[5] = ':';
    ConvertDigitToChar(time[Sec_Idx], buffer + 6);
    buffer[8] = '.';
    ConvertDigitToChar(time[Ms_Idx], buffer + 9);
    buffer[11] = '\0'; // Null terminator
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
void UART_Runnable()
{


uint8 Send_Data=99;
User_Request_t Tx;
		Tx.Ptr_Buffer=&Send_Data;
		Tx.USART_Num=USART_1;
		Tx.CallBack=Set_Led;
	    Tx.Length=1;

for(uint8 Ind=0;Ind<_HSWITCH_NUM;Ind++)
{
	uint8 value=0;
	HSWITCH_GetStatus(Ind,&value);
	if (value==HSWITCH_PRESSED)
	{
		Send_Data=Ind+'0';
		//LED_SetState(LED_Red,LED_STATE_ON);

		USART_SendBufferAsync(&Tx);
	}

}

	 User_Request_t Rx;
	 Rx.Ptr_Buffer=&Received_Data;
	 	Rx.USART_Num=USART_1;
	 	Rx.CallBack=Set_Led;
	     Rx.Length=1;

	 USART_ReceiveBufferAsync(&Rx);

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


	//uint8 Rx_Arr[5]={0};



//	 User_Request_t Rx;
//	 Rx.Ptr_Buffer=Rx_Arr;
//	 	Rx.USART_Num=USART_1;
//	 	Rx.CallBack=Set_Led2;
//	     Rx.Length=5;



	//	USART_SendByte(&Tx);
		//USART_GetByte(&Rx);

//	USART_ReceiveBufferAsync(&Rx);
		//USART_SendByte(&Tx);



}
void Input_Runnable()
{

//	for(uint8 Ind=0;Ind<_HSWITCH_NUM;Ind++)
//	{
//		//uint8 value=0;
//		//HSWITCH_GetStatus(Ind,&value);
//		if (Received_Data==_HSWITCH_NUM)
//		{
//			//Send_Data=Ind;
//			//LED_SetState(LED_Red,LED_STATE_ON);
//
//			//USART_SendBufferAsync(&Tx);
//
//        CLCD_WriteNumAsynch(_HSWITCH_NUM);
//		}
//
//	}

	Received_Data-='0';
if (Received_Data<7&&Received_Data>=0)
{
	//CLCD_WriteNumAsynch(Received_Data);
	Pressed_Key=Received_Data;
	Pressed_Key_Flag=1;

}
Received_Data=9;

uint8 c=':';



//if(Pressed_Key_Flag==1)
//{
//switch(Pressed_Key)
//{
//case HSWITCH_Up:
//	               //check which mode if clock then check if edit mode then increment
//
//	              //upcursor func
//	                break;
//case HSWITCH_Down:
//	//check which mode if clock then check if edit mode then DEC
//
//
//
//}
//
//}

	// CLCD_WriteNumAsynch(Received_Data-'0');
}


void APP_Runnable(void)
{
	//CLCD_ClearDisplayAsynch();
static uint8 flag=0;
//CLCD_WriteNumAsynch(100);
	if(flag==0)
	{
	CLCD_SetCursorAsynch(0,1);
	CLCD_WriteNumAsynch(90);
	CLCD_WriteStringAsynch("Halla",5);
	flag+=1;
	}
	//CLCD_ClearDisplayAsynch();
	//static last_up_time=0;
//	for(uint8 ind=0;ind<1;ind++)
//	{ //if(Current_Date[ind]!=Prev_Date[ind])
//	{
//	CLCD_SetCursorAsynch(Cursor_Update[ind].Line,Cursor_Update[ind].Col);
//	CLCD_WriteNumAsynch(Current_Date[ind]);
//	CLCD_SetCursorAsynch(Cursor_Update[ind].Line,Cursor_Update[ind].Col+4);
//	CLCD_WriteStringAsynch(&c,1);
//	}
//	Prev_Date[ind]=Current_Date[ind];
//	}
	/*
static uint8 ind=0;

	//uint8 string[2];

ind++;
if(ind==7){ind=0;}

*/
//for(uint8 ind=0;ind<7;ind++){
//	if(Current_Date[ind]!=Prev_Date[ind])
//	{
//	//ConvertDigitToChar(Current_Date[ind],string);
//	//ConvertDateToString(Current_Date,string);
//// 3: 4: 5:6
////11:11:11:1
////01:34:56:8
//
//	CLCD_SetCursorAsynch(Cursor_Update[ind].Line,Cursor_Update[ind].Col);
//	//CLCD_WriteStringAsynch(string,2);
//	CLCD_WriteNumAsynch(Current_Date[ind]);
//		 Prev_Date[ind]=Current_Date[ind];
//	}
//}
}

int
main(int argc, char* argv[])
{
	RCC_ManageClock(RCC_CLOCK_HSI,RCC_STATE_ENABLE);
	RCC_SelectSystemClock(RCC_CLOCK_HSI);
	RCC_ControlPeripheral(RCC_AHB1,GPIOA,RCC_STATE_ENABLE);
	RCC_ControlPeripheral(RCC_AHB1,GPIOB,RCC_STATE_ENABLE);

	RCC_ControlPeripheral( RCC_APB2, RCC_USART1 , RCC_STATE_ENABLE);
	NVIC_Control_IRQ(NVIC_USART1,NVIC_ENABLE);
	GPIO_Pin_Cfg_t UART_PINS[4]={
			//tx
			[0]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN9,.Speed=GPIO_OUTPUT_SPEED_HIGH},
			//rx
			[1]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN10,.Speed=GPIO_OUTPUT_SPEED_HIGH},

	};

	GPIO_InitPin(&UART_PINS[0]);
	GPIO_InitPin(&UART_PINS[1]);
	GPIO_CFG_AF(UART_PINS[0].Port,UART_PINS[0].Pin,GPIO_AF7);
	GPIO_CFG_AF(UART_PINS[1].Port,UART_PINS[1].Pin,GPIO_AF7);

	SCHED_Init();
	CLCD_InitAsynch();
	HSWITCH_Init();
	USART_Init();
	LED_Init();
	SCHED_Start();
}
//
//#include <stdio.h>
//
//void Convert_TimeStamptoDate(uint64 ms, uint64 *timeArray) {
//    // Constants for time conversion
//    const long long msPerSecond = 1000;
//    const long long msPerMinute = 60 * msPerSecond;
//    const long long msPerHour = 60 * msPerMinute;
//    const long long msPerDay = 24 * msPerHour;
//    const long long msPerYear = 365 * msPerDay; // Approximation
//
//    // Maximum values for each time unit
//     int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Days in each month
//
//    // Extract values from the array
//    int years = timeArray[0];
//    int months = timeArray[1];
//    int days = timeArray[2];
//    int hours = timeArray[3];
//    int minutes = timeArray[4];
//    int seconds = timeArray[5];
//
//    // Increment the values
//    years += ms / msPerYear;
//    ms %= msPerYear;
//
//    int remainingDays = ms / msPerDay;
//
//    // Adjust for leap years
//    int leapYear = (years % 4 == 0 && (years % 100 != 0 || years % 400 == 0));
//    if (leapYear)
//        daysInMonth[1] = 29;
//
//    // Calculate months and days
//    months += remainingDays / daysInMonth[months - 1];
//    days += remainingDays % daysInMonth[months - 1];
//
//    // Adjust for overflow
//    if (days >= daysInMonth[months - 1]) {
//        days -= daysInMonth[months - 1];
//        months += 1;
//    }
//
//    // Adjust for overflow
//    if (months > 12) {
//        months -= 12;
//        years += 1;
//    }
//
//    // Convert remaining milliseconds to hours, minutes, and seconds
//    ms %= msPerDay;
//    hours += ms / msPerHour;
//    ms %= msPerHour;
//    minutes += ms / msPerMinute;
//    ms %= msPerMinute;
//    seconds += ms / msPerSecond;
//
//    // Adjust values if they exceed their maximum
//    if (seconds >= 60) {
//        minutes += seconds / 60;
//        seconds %= 60;
//    }
//
//    if (minutes >= 60) {
//        hours += minutes / 60;
//        minutes %= 60;
//    }
//
//    if (hours >= 24) {
//        days += hours / 24;
//        hours %= 24;
//    }
//
//    // Update milliseconds in the array, capped at 999
//    timeArray[6] = (int)((ms % msPerSecond) / 100);
//    if (timeArray[6] > 9)
//        timeArray[6] = 9;
//
//    // Update the values in the array
//    timeArray[0] = years;
//    timeArray[1] = months;
//    timeArray[2] = days;
//    timeArray[3] = hours;
//    timeArray[4] = minutes;
//    timeArray[5] = seconds;
//}
//
//void Convert_TimestamptoStopwatch(uint64_t ms, uint64_t *timeArray) {
//    // Create a separate array to hold the updated values
//    uint64_t updatedTime[4] = {timeArray[SW_Hr_Idx], timeArray[SW_Min_Idx], timeArray[SW_Sec_Idx], timeArray[SW_Ms_Idx]};
//
//    // Constants for time conversion
//    const uint64_t msPerSecond = 1000;
//    const uint64_t msPerMinute = 60 * msPerSecond;
//    const uint64_t msPerHour = 60 * msPerMinute;
//
//    // Increment the values
//    updatedTime[SW_Hr_Idx] += ms / msPerHour;
//    ms %= msPerHour;
//
//    updatedTime[SW_Min_Idx] += ms / msPerMinute;
//    ms %= msPerMinute;
//
//    updatedTime[SW_Sec_Idx] += ms / msPerSecond;
//    ms %= msPerSecond;
//
//    // Update milliseconds in the array, capped at 999
//    updatedTime[SW_Ms_Idx] += (ms ) / 10; // Rounding up to the nearest tenth
//    if (updatedTime[SW_Ms_Idx] > 99)
//        updatedTime[SW_Ms_Idx] = 99;
//
//    // Handle overflow for seconds, minutes, and hours
//    if (updatedTime[SW_Sec_Idx] >= 60) {
//        updatedTime[SW_Min_Idx] += updatedTime[SW_Sec_Idx] / 60;
//        updatedTime[SW_Sec_Idx] %= 60;
//    }
//
//    if (updatedTime[SW_Min_Idx] >= 60) {
//        updatedTime[SW_Hr_Idx] += updatedTime[SW_Min_Idx] / 60;
//        updatedTime[SW_Min_Idx] %= 60;
//    }
//
//    if (updatedTime[SW_Hr_Idx] >= 24) {
//        // Handle overflow for hours exceeding 24
//        updatedTime[SW_Hr_Idx] %= 24;
//    }
//
//    // Copy the updated values back to the original array
//    for (int i = 0; i < 4; i++) {
//        timeArray[i] = updatedTime[i];
//    }
//}

void Convert_TimeStamptoDate(uint64 ms, uint64 *timeArray) {
    // Constants for time conversion
    const uint64 msPerSecond = 1000;
    const uint64 msPerMinute = 60 * msPerSecond;
    const uint64 msPerHour = 60 * msPerMinute;
    const uint64 msPerDay = 24 * msPerHour;
    const uint64 msPerYear = 365 * msPerDay; // Approximation

    // Extract values from the array
    uint64 years = timeArray[0];
    uint64 months = timeArray[1];
    uint64 days = timeArray[2];
    uint64 hours = timeArray[3];
    uint64 minutes = timeArray[4];
    uint64 seconds = timeArray[5];

    // Increment the values
    years += ms / msPerYear;
    ms %= msPerYear;

    uint64 remainingDays = ms / msPerDay;

    // Constants for date conversion
     uint32 daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Days in each month

    // Adjust for leap years
     uint32 leapYear = (years % 4 == 0 && (years % 100 != 0 || years % 400 == 0));
    if (leapYear)
        daysInMonth[1] = 29;

    // Calculate months and days
    months += remainingDays / daysInMonth[months - 1];
    days += remainingDays % daysInMonth[months - 1];

    // Adjust for overflow
    if (days > daysInMonth[months - 1]) {
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

    // Update the values in the array
    timeArray[0] = years;
    timeArray[1] = months;
    timeArray[2] = days;
    timeArray[3] = hours;
    timeArray[4] = minutes;
    timeArray[5] = seconds;
}

void Convert_TimestamptoStopwatch(uint64 ms, uint64 *timeArray) {
    // Constants for time conversion
    const uint64 msPerSecond = 1000;
    const uint64 msPerMinute = 60 * msPerSecond;
    const uint64 msPerHour = 60 * msPerMinute;

    // Compute hours, minutes, seconds, and milliseconds
    timeArray[SW_Hr_Idx] += ms / msPerHour;
    timeArray[SW_Min_Idx] += (ms / msPerMinute) % 60;
    timeArray[SW_Sec_Idx] += (ms / msPerSecond) % 60;
    timeArray[SW_Ms_Idx] += (ms % msPerSecond) / 10; // Rounding up to the nearest tenth
    if (timeArray[SW_Ms_Idx] > 99)
        timeArray[SW_Ms_Idx] = 99;

    // Handle overflow for seconds, minutes, and hours
    if (timeArray[SW_Sec_Idx] >= 60) {
        timeArray[SW_Min_Idx] += timeArray[SW_Sec_Idx] / 60;
        timeArray[SW_Sec_Idx] %= 60;
    }

    if (timeArray[SW_Min_Idx] >= 60) {
        timeArray[SW_Hr_Idx] += timeArray[SW_Min_Idx] / 60;
        timeArray[SW_Min_Idx] %= 60;
    }

    if (timeArray[SW_Hr_Idx] >= 24) {
        timeArray[SW_Hr_Idx] %= 24;
    }
}
