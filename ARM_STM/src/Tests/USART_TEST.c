/*
 * USART_TEST.c
 *
 *  Created on: Apr 15, 2024
 *      Author: Lenovo
 */


#ifdef TEST_UARTTEST

#ifdef TEST_UART
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
#endif








#endif

