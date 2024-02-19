/*
 * GPIO.h
 *
 *  Created on: Feb 18, 2024
 *      Author: Halla Raafat
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "LIB/STD_Types.h"

/******************************************************************************************/
/*                                  Ports                                                 */
/******************************************************************************************/

#define GPIO_PORTH     ( void *)0x40021C00
#define GPIO_PORTE     ( void *)0x40021000
#define GPIO_PORTD     ( void *)0x40020C00

#define GPIO_PORTC     ( void *)0x40020800
#define GPIO_PORTB     ( void *)0x40020400
#define GPIO_PORTA     (void *)0x40020000
/******************************************************************************************/
/*                                  Pins                                                  */
/******************************************************************************************/

#define GPIO_PIN0       0x00000000
#define GPIO_PIN1       0x00000001
#define GPIO_PIN2       0x00000002
#define GPIO_PIN3       0x00000003
#define GPIO_PIN4       0x00000004
#define GPIO_PIN5       0x00000005
#define GPIO_PIN6       0x00000006
#define GPIO_PIN7       0x00000007
#define GPIO_PIN8       0x00000008
#define GPIO_PIN9       0x00000009
#define GPIO_PIN10      0x0000000A
#define GPIO_PIN11      0x0000000B
#define GPIO_PIN12      0x0000000C
#define GPIO_PIN13      0x0000000D
#define GPIO_PIN14      0x0000000E
#define GPIO_PIN15      0x0000000F

/******************************************************************************************/
/*                                  Modes                                                 */
/******************************************************************************************/


/*
 * Sequence                   PUPDR/OTYPER/MODER
 * Output pullup push pull     01      0    01  -> 0x9
 *
*/

#define GPIO_OUTPUT_PP          0x00000001
#define GPIO_OUTPUT_PP_PU       0x00000009
#define GPIO_OUTPUT_PP_PD       0x00000011

#define GPIO_OUTPUT_OD          0x00000005
#define GPIO_OUTPUT_OD_PU       0x0000000D
#define GPIO_OUTPUT_OD_PD       0x00000015

#define GPIO_AF_PP              0x00000002
#define GPIO_AF_PP_PU           0x0000000A
#define GPIO_AF_PP_PD           0x00000012

#define GPIO_AF_OD              0x00000006
#define GPIO_AF_OD_PU           0x0000000E
#define GPIO_AF_OD_PD           0x00000016

#define GPIO_INPUT_FLOAT        0x00000000
#define GPIO_INPUT_PU           0x00000008
#define GPIO_INPUT_PD           0x00000010

#define GPIO_INPUT_ANALOG       0x00000003
#define GPIO_OUTPUT_ANALOG      0x00000003

/******************************************************************************************/
/*                                  Values                                                */
/******************************************************************************************/
#define GPIO_OUTPUT_HIGH        0x00000001
#define GPIO_OUTPUT_LOW         0x00000000

/******************************************************************************************/
/*                           Output Speeds                                                */
/******************************************************************************************/
#define GPIO_OUTPUT_SPEED_LOW   0x00000000
#define GPIO_OUTPUT_SPEED_MED   0x00000001
#define GPIO_OUTPUT_SPEED_HIGH  0x00000002
#define GPIO_OUTPUT_SPEED_VHIGH 0x00000003

/******************************************************************************************/

typedef struct
{
  uint32 Pin;
  uint32 Mode ;
  uint32 Speed;
  void * Port;
}GPIO_Pin_Cfg_t;

typedef enum
{
	GPIO_OK,
	GPIO_NOK
}GPIO_ErrorStatus_t;

GPIO_ErrorStatus_t GPIO_InitPin(GPIO_Pin_Cfg_t * Ptr_PinObject);

GPIO_ErrorStatus_t GPIO_SetPinValue ( void * Port ,uint32 Pin ,uint32 Value);

GPIO_ErrorStatus_t GPIO_GetPinValue ( void * Port ,uint32 Pin,uint8 * Get_Value);
#endif /* GPIO_H_ */
