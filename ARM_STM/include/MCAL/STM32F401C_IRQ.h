/*
 * STM32F401C_IRQ.h
 *
 *  Created on: Mar 6, 2024
 *      Author: Halla Raafat
 */

#ifndef STM32F401C_IRQ_H_
#define STM32F401C_IRQ_H_


#define NVIC_WINDOW_WATCHDOG_TIMER		0
#define	NIVC_EXTERNAL_LINE16			1
#define	NIVC_PVD	            		1

#define	NIVC_EXTERNAL_LINE21			2
#define	NIVC_TAMPER_TIMESTAMP			2

#define	NIVC_EXTERNAL_LINE22			3
#define	NIVC_RTC_WAKEUP					3

#define	NIVC_FLASH		  				4
#define	NVIC_RCC        				5
#define NVIC_EXTI0						6
#define NVIC_EXTI1						7
#define NVIC_EXTI2						8
#define NVIC_EXTI3						9
#define NVIC_EXTI4		            	10
#define NVIC_DMA1_STREAM0				11
#define NVIC_DMA1_STREAM1				12
#define NVIC_DMA1_STREAM2				13
#define NVIC_DMA1_STREAM3				14
#define NVIC_DMA1_STREAM4				15
#define NVIC_DMA1_STREAM5				16
#define NVIC_DMA1_STREAM6               17
#define NVIC_ADC	                    18
#define NVIC_EXTI9                      23
#define NVIC_TIM1_BRK                   24
#define NVIC_TIM9_GLOBAL                24

#define NVIC_TIM1_UPDATE                25
#define NVIC_TIM10_GLOBAL               25

#define NVIC_TIMI1_TRG                  26
#define NVIC_COM_INTERRUPTS             26
#define NVIC_TIM11_GLOBAL               26

#define NVIC_TIM1_CC      			    27
#define NVIC_TIM2_GLOBAL                28
#define NVIC_TIM3_GLOBAL            	29
#define NVIC_TIM4_GLOBAL            	30
#define NVIC_I2C1_EV          			31
#define NVIC_I2C1_ER          			32
#define NVIC_I2C2_EV          			33
#define NVIC_I2C2_ER          			34
#define NVIC_SPI1             			35
#define NVIC_SPI2             			36
#define NVIC_USART1           			37
#define NVIC_USART2           			38
#define NVIC_EXTI15_10                  40

#define NVIC_EXTI17           			41
#define NVIC_RTC_ALARM         			41
#define NVIC_EXTI18           			42
#define NVIC_OTG_FS_WKUP       			42

#define NVIC_DMA1_STREAM7      			47
#define NVIC_SDIO             			49
#define NVIC_TIM5_GLOBAL             	50
#define NVIC_SPI3             			51
#define NVIC_DMA2_STREAM0     			56
#define NVIC_DMA2_STREAM1     			57
#define NVIC_DMA2_STREAM2     			58
#define NVIC_DMA2_STREAM3     			59
#define NVIC_DMA2_STREAM4     			60
#define NVIC_OTG_FS           			67
#define NVIC_DMA2_STREAM5     			68
#define NVIC_DMA2_STREAM6     			69
#define NVIC_DMA2_STREAM7     			70
#define NVIC_USART6           			71
#define NVIC_I2C3_EV          			72
#define NVIC_I2C3_ER          			73
#define NVIC_FPU              			81
#define NVIC_SPI4             			84


#endif /* STM32F401C_IRQ_H_ */
