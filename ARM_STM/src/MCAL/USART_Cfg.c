/*
 * USART_Cfg.c
 *
 *  Created on: Apr 14, 2024
 *      Author: Halla Raafat
 */

#include "MCAL/USART_Cfg.h"
#include "MCAL/USART.h"


const  USART_Cfg_t USART_ARR[_USART_Num] =
{
 [USART_1]={
    .USART_ID=USART1,
    .BaudRate=9600,
    .WordLength=USART_WL_8BIT_DATA,
    .ParityControl=USART_PARITY_DISABLE,
    .ParityType=0,
    .StopBits=USART_STOP_BIT_1,
    .OverSamplingMode=USART_OVS_16},

	[USART_2]={
	    .USART_ID=USART2,
	    .BaudRate=9600,
	    .WordLength=USART_WL_8BIT_DATA,
	    .ParityControl=USART_PARITY_DISABLE,
	    .ParityType=0,
	    .StopBits=USART_STOP_BIT_1,
	    .OverSamplingMode=USART_OVS_16}

};
