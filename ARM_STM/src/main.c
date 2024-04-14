

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
	RCC_ControlPeripheral( RCC_APB2, RCC_USART1 , RCC_STATE_ENABLE);
	RCC_ControlPeripheral( RCC_APB1, RCC_USART2 , RCC_STATE_ENABLE);


	uint8 x='a';
	uint8 z=0;
	GPIO_Pin_Cfg_t UART_PINS[4]={
			//tx
			[0]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN9,.Speed=GPIO_OUTPUT_SPEED_HIGH},
			//rx
			[1]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN10,.Speed=GPIO_OUTPUT_SPEED_HIGH},
			[2]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN2,.Speed=GPIO_OUTPUT_SPEED_HIGH},
						//rx
			[3]={.Mode=GPIO_AF_PP,.Port=GPIO_PORTA,.Pin=GPIO_PIN3,.Speed=GPIO_OUTPUT_SPEED_HIGH}
	};

	GPIO_InitPin(&UART_PINS[0]);
	GPIO_InitPin(&UART_PINS[1]);
	GPIO_CFG_AF(UART_PINS[0].Port,UART_PINS[0].Pin,GPIO_AF7);
	GPIO_CFG_AF(UART_PINS[1].Port,UART_PINS[1].Pin,GPIO_AF7);


	GPIO_InitPin(&UART_PINS[2]);
		GPIO_InitPin(&UART_PINS[3]);
		GPIO_CFG_AF(UART_PINS[2].Port,UART_PINS[2].Pin,GPIO_AF7);
		GPIO_CFG_AF(UART_PINS[3].Port,UART_PINS[3].Pin,GPIO_AF7);
/*
 * {
 USART_Names_t  	USART_Num; // only in configured names in USART_Cfg.h
 uint8 *	        Ptr_Buffer;
 uint32             Length;
 CB_t               CallBack; // called after finishing Request
}User_Request_t;
 *
 */

User_Request_t Tx;
Tx.Ptr_Buffer=&x;
Tx.USART_Num=USART_1;


 User_Request_t Rx;
Rx.Ptr_Buffer=&z;
Rx.USART_Num=USART_2;
LED_Init();
	USART_Init();
	USART_SendByte(&Tx);
	USART_GetByte(&Rx);
	if (z=='a')
	{
		LED_SetState(LED_Green,LED_STATE_ON);

	}
	//USART_SendByte(&Tx);


while(1)
{
 //  USART_SendByte(&Tx);

}
    }




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
