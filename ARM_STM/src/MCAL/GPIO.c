/*
 * GPIO.c
 *
 *  Created on: Feb 18, 2024
 *      Author: Halla Raafat
 */

#include "MCAL/GPIO.h"

/*
 *  GPIO
 *  4 registers for each port
 *   GPIOx_MODER /OTYPE/OSPEED/PUPDR
 *   2 registers
 *   IDR/ODR
 *
 *   BSBRR
 *   locking reg _LCKR
 *
 *   2 alternate function selection _AFRH and _AFRL
 *
 *   16 pin per reg
 *   output -> push-pull / open drain with pull up/down
 *
 *   Output data from output data register (GPIOx_ODR)
 *   or peripheral (alternate function output)
 *    Speed selection for each I/O
 *
   Input states -> floating, pull-up/down, analog

• Input data to input data register (GPIOx_IDR)
or peripheral (alternate function input)

• Bit set and reset register (GPIOx_BSRR) for bitwise write access to GPIOx_ODR

• Locking mechanism (GPIOx_LCKR) provided to freeze the I/O configuration

• Analog function

• Alternate function input/output selection registers (at most 16 AFs per I/O)

• Fast toggle capable of changing every two clock cycles

• Highly flexible pin multiplexing allows the use of I/O pins as GPIOs or as one of several
peripheral functions
 *
 *
 */

 /*
 * MODER-> each 2 bits
 * 00 for input
 * 01 General output
 * 10 Alternate func mode
 * 11 Analog mode
 *
 * OTYPER -> 0:15
 * bit 0 : 0 Output push-pull
 * / 1 open drain
 *
 * OSPEEDR->
 * 00 : low
 * 01 med speed
 * 10: high
 * 11 : very high
 *
 *PUPDR ->  2 bits for push and pull
 *  00 : no pull
 *  01:pull up
 *  10: pull down
 *  11: reserved
 *
 */



#define TWO_BIT_MASK 0x00000003
#define ONE_BIT_MASK 0x00000001
#define GPIO_AF_MASK 0x0000000f
#define MODE_MASK   0x00000003
#define OTYPE_MASK  0x00000004
#define PUPDR_MASK  0x00000018

#define TWO_BITS      2
#define PORT_SIZE   0x00000010
typedef struct
{
  uint32 MODER;
  uint32 OTYPER;
  uint32 OSPEEDR;
  uint32 PUPDR;
  uint32 IDR;
  uint32 ODR;
  uint32 BSRR;
  uint32 LCKR;
  uint32 AFRL;
  uint32 AFRH;
}GPIO_PORT_t;

/*                                pupd/otype/mode
 *         OUTPUT_PP_PU     0b01001 -> 0x9
 *                            11000
 * MODER-> each 2 bits
 * 00 for input
 * 01 General output
 * 10 Alternate func mode
 * 11 Analog mode
 *
 * OTYPER -> 0:15
 * bit 0 : 0 Output push-pull
 * / 1 open drain
 *
 * OSPEEDR->
 * 00 : low
 * 01 med speed
 * 10: high
 * 11 : very high
 *
 *PUPDR ->  2 bits for push and pull
 *  00 : no pull
 *  01:pull up
 *  10: pull down
 *  11: reserved
 *
 *
 *  IDR-> r only
 *  READ ONLY  from 0:15 bit
 *
 *  ODR -> rw
 *
 * for atomic set/reset  BSRR
 *
 * BSRR -> W only
 *31:16  write 1  reset corresponding bit
 *15:0   write 1 set corresponding bit
 *
 *  AFRL from0 to 7 each 4 bits to configure AF from MC datasheet
 * /AFRH from 8 to 15
 */

/*
 * Sequence                   PUPDR/OTYPER/MODER
 * Output pullup push pull     01      0    01  -> 0x9
 *
*/
GPIO_ErrorStatus_t GPIO_InitPin(GPIO_Pin_Cfg_t * Ptr_PinObject)
{
	GPIO_ErrorStatus_t Local_ReturnErrorState=GPIO_NOK;

    uint32 Local_MODER= ((volatile GPIO_PORT_t  *) (Ptr_PinObject->Port))    -> MODER;
    uint32 Local_OTYPER= ((volatile GPIO_PORT_t *) (Ptr_PinObject->Port))    -> OTYPER;
    uint32 Local_PUPDR = ((volatile GPIO_PORT_t *)  (Ptr_PinObject->Port))   -> PUPDR;
    uint32 Local_OSPEEDR=((volatile GPIO_PORT_t *)  (Ptr_PinObject -> Port)) ->OSPEEDR;

    uint32 Local_ClearBits = 0xffffffff ;
    Local_ClearBits &=~ (TWO_BIT_MASK<<( (Ptr_PinObject->Pin) *TWO_BITS ) ); // shift 0x3 by (pin number * 2 )
    // to clear the corresponding 2 bits to the pin number
    Local_MODER &=Local_ClearBits;
    // set the moder by mode bits and shift them depend on pin number
    Local_MODER |= (((Ptr_PinObject->Mode)& MODE_MASK)<<(Ptr_PinObject->Pin)*TWO_BITS);

    Local_OTYPER &=~ (ONE_BIT_MASK<< Ptr_PinObject->Pin);
    Local_OTYPER |=  (((Ptr_PinObject->Mode)& OTYPE_MASK )<< (Ptr_PinObject->Pin));

    // to clear the corresponding 2 bits to the pin number
    Local_PUPDR &=Local_ClearBits; // shift 0x3 by (pin number * 2 )
      // set the moder by mode bits and shift them depend on pin number
    Local_PUPDR |= (((Ptr_PinObject->Mode)& PUPDR_MASK)<<(Ptr_PinObject->Pin)*TWO_BITS);


    // to clear the corresponding 2 bits to the pin number
    Local_OSPEEDR &=Local_ClearBits;
    //set value to ospeedr corresponding to pin number
    Local_OSPEEDR |= (Ptr_PinObject->Speed << Ptr_PinObject->Pin* TWO_BITS);

   //Assign values in the Registers
    ((volatile GPIO_PORT_t *)  (Ptr_PinObject->Port))    -> MODER  =  Local_MODER;
    ((volatile GPIO_PORT_t *)  (Ptr_PinObject->Port))    -> OTYPER =  Local_OTYPER ;
    ((volatile GPIO_PORT_t *)  (Ptr_PinObject->Port))    -> PUPDR  =  Local_PUPDR  ;
    ((volatile GPIO_PORT_t *)  (Ptr_PinObject->Port))    -> OSPEEDR= Local_OSPEEDR;

    Local_ReturnErrorState=GPIO_OK;

	return Local_ReturnErrorState;
}

GPIO_ErrorStatus_t GPIO_SetPinValue ( void * Port ,uint32 Pin ,uint32 Value)
{
	GPIO_ErrorStatus_t Local_ReturnErrorState=GPIO_NOK;
    switch(Value)
    {
    case GPIO_OUTPUT_HIGH :
    	                      ((volatile GPIO_PORT_t *)  Port) ->BSRR = (ONE_BIT_MASK<< Pin);
    	                       Local_ReturnErrorState=GPIO_OK; break;

    case GPIO_OUTPUT_LOW:
    	                      ((volatile GPIO_PORT_t *)  Port) ->BSRR = (ONE_BIT_MASK<< (Pin+ PORT_SIZE ));
    	                        Local_ReturnErrorState=GPIO_OK;  break;

    default : Local_ReturnErrorState=GPIO_NOK;break;
    }
	return Local_ReturnErrorState;
}

GPIO_ErrorStatus_t GPIO_GetPinValue ( void * Port ,uint32 Pin ,uint8 * Get_Value)
{
	GPIO_ErrorStatus_t Local_ReturnErrorState=GPIO_NOK;


    *Get_Value =(  ( ( (volatile GPIO_PORT_t *)  Port ) ->IDR >> Pin )& ONE_BIT_MASK  );
	return Local_ReturnErrorState;

}

GPIO_ErrorStatus_t GPIO_CFG_AF ( void * Port ,uint32 Pin,uint32 AF_Num)
{

	GPIO_ErrorStatus_t Local_ReturnErrorState=GPIO_NOK;
	uint32 Local_AFRValue=0;
if (Pin<= GPIO_PIN7)
{
	Local_AFRValue=((volatile GPIO_PORT_t *)  Port) ->AFRL;
	// Clear 4 bits of this pin
    Local_AFRValue&=~ (GPIO_AF_MASK<<(Pin*4));
    Local_AFRValue |= (AF_Num<<(Pin*4));
    ((volatile GPIO_PORT_t *)  Port) ->AFRL=Local_AFRValue;

	}
else
{
	Local_AFRValue=((volatile GPIO_PORT_t *)  Port) ->AFRH;

		// Clear 4 bits of this pin
	    Local_AFRValue&=~ (GPIO_AF_MASK<<((Pin-8)*4));
	    Local_AFRValue |= (AF_Num<<((Pin-8)*4));
	    ((volatile GPIO_PORT_t *)  Port) ->AFRH=Local_AFRValue;

	}
		return Local_ReturnErrorState;
}
