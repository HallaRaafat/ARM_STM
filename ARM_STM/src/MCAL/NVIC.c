/*
 * NVIC.c
 *
 *  Created on: Mar 5, 2024
 *      Author: Halla Raafat
 */



#include "MCAL/NVIC.h"
#include "LIB/STD_Types.h"
#include "MCAL/STM32F401C_IRQ.h"

#define REG_SIZE              32
#define NVIC_BASE_ADDRESS   (0xE000E100)
#define IRQ_LOW_BOUND_LIMIT 0
#define IRQ_HIGH_BOUND_LIMIT 240
typedef struct
{
  uint32 ISER [8];
  uint32 Reserved1 [24];
  uint32 ICER [8];
  uint32 Reserved2 [24];
  uint32 ISPR [8];
  uint32 Reserved3 [24];
  uint32 ICPR [8];
  uint32 Reserved4 [24];
  uint32 IABR [8];
  uint32 Reserved5 [56];
  uint32 IPR  [60];
  uint32 Reserved5 [580];
  uint32 STIR;

}NVIC_t;

volatile NVIC_t * const NVIC =(volatile NVIC_t *const) NVIC_BASE_ADDRESS  ;


/*
 * a summary for prog. manual
 *
 * Nested vectored interrupt controller (NVIC)
 *
 *
 *
 * IRQn from reference manual
 *

 *
 *  (NVIC_IABRx) Interrupt active bit register
 *   NVIC_IABRx -> read only
 *   A bit reads as 1
 *   if the status of the corresponding interrupt is active
 *    or active and pending.
 *   Bits 16 to 31 of the NVIC_IABR7 register are reserved.
 *
 *   Interrupt priority register x (NVIC_IPRx)
 *   NVIC_IPR0 -> IP0,IP1,IP2,IP3   each int have 1 byte to define its priority
 *      till NVIC_IPR59
 *
 *
 *Software trigger interrupt register (NVIC_STIR)
 *  write only in 8 bits
 *  Write to the STIR to generate a Software Generated Interrupt (SGI). The value to be written is
 *  the Interrupt ID of the required SGI, in the range 0-239. For example, a value of 0x03 specifies interrupt IRQ3.
 *
 * Pending if detected high / rising
 * or SW by -> ISPR (int set pending reg)or STIR (SW trigger int reg.)
 *
 * not pending ->
 * for level sensitive if signal asserted pending and re enter isr
 * for pulse ????????????????????????
 *
 * sw-> icpr int clear pending reg
 *
 *
 *
 */


const uint32_t bit_masks[REG_SIZE] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800,
    0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000,
    0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000
};

NVIC_Error_t NVIC_Control_IRQ(uint8 NVIC_IRQ,uint8 State)
{

	NVIC_Error_t Local_ErrorState= NVIC_NOK;
/*
 *
* NVIC_ISER-> rs read and set
*  0 to 7
*  NVIC_ISER0 from 0to 31
*  NVIC_ISER1 drom 32 to 63
*  write 0 -> no effect , 1 -> enable Interrupt
*  Read 0 disabled int , read 1 int enabled
*  Bits 16 to 31 of the NVIC_ISER7 register are reserved.
*
*  NVIC_ICER0->
*  write 1 disable interrupt
*
*  read it same as ISER
*
*  Bits 16 to 31 of the NVIC_ISER7 register are reserved.
 */



/*
 * for example 193 193/32 = 6 register 193%32 = 1 pin
 */
uint8 Local_RegisterNumber= NVIC_IRQ/REG_SIZE;
uint8 Local_PinNumber= NVIC_IRQ%REG_SIZE;

switch (State)
{
case NVIC_ENABLE :
					NVIC->ISER[Local_RegisterNumber]= bit_masks[Local_PinNumber];
					Local_ErrorState= NVIC_OK;
					break;
case NVIC_DISABLE :
					NVIC->ICER[Local_RegisterNumber]= bit_masks[Local_PinNumber];
					Local_ErrorState= NVIC_OK;
	                break;
default :
	                Local_ErrorState= NVIC_NOK;
	                break;

}




	return Local_ErrorState;
}

/*
 * set/clear pending status of interrupt
 *
 */
NVIC_Error_t NVIC_ControlPendingIRQ(uint8 NVIC_IRQ,,uint8 State)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;
/*
*
*  Interrupt set-pending register x (NVIC_ISPRx)
*  write 1 -> interrupt state to pending
*  NVIC_ICPRx-> write 1 clear pending
*/

	uint8 Local_RegisterNumber= NVIC_IRQ/REG_SIZE;
	uint8 Local_PinNumber= NVIC_IRQ%REG_SIZE;

	switch (State)
	{
	case NVIC_ENABLE :
						NVIC->ISPR[Local_RegisterNumber]= bit_masks[Local_PinNumber];
						Local_ErrorState= NVIC_OK;
						break;
	case NVIC_DISABLE :
						NVIC->ICPR[Local_RegisterNumber]= bit_masks[Local_PinNumber];
						Local_ErrorState= NVIC_OK;
		                break;
	default :
		                Local_ErrorState= NVIC_NOK;
		                break;

	}




	return Local_ErrorState;


}
/*
 * Reads the pending status of interrupt or
exception. This function returns non-zero value
 if the pending status is set to 1.
 */
NVIC_Error_t NVIC_GetPendingIRQ(uint8 NVIC_IRQ,uint8 * Pending_Value)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;
	 if (NVIC_IRQ < IRQ_HIGH_BOUND_LIMIT && NVIC_IRQ >= IRQ_LOW_BOUND_LIMIT)
	 {
	uint8 Local_RegisterNumber= NVIC_IRQ/REG_SIZE;
	uint8 Local_PinNumber= NVIC_IRQ%REG_SIZE;

	*Pending_Value = ((NVIC->ISPR[Local_RegisterNumber]& bit_masks[Local_PinNumber])>> Local_PinNumber );
	Local_ErrorState= NVIC_OK;
	 }

	return Local_ErrorState;
}
/*
 * Sets the priority of an interrupt or
 * exception with configurable priority level to 1.
 */
NVIC_Error_t NVIC_SetPirority(uint8 NVIC_IRQ)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;





	return Local_ErrorState;
}

/*
 * Reads the priority of an interrupt or
 * exception with configurable priority level.
 * This function return the current priority level.
 */


NVIC_Error_t NVIC_GetPirority(uint8 NVIC_IRQ)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;





	return Local_ErrorState;
}
/*
 * Active or not active (current IRQ )
 */
NVIC_Error_t NVIC_GetStatus(uint8 NVIC_IRQ)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;





	return Local_ErrorState;
}




NVIC_Error_t NVIC_GenerateSWInterrupt(uint8 NVIC_IRQ)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;

 if (NVIC_IRQ < IRQ_HIGH_BOUND_LIMIT && NVIC_IRQ >= IRQ_LOW_BOUND_LIMIT)
 { NVIC->STIR = NVIC_IRQ;
 Local_ErrorState= NVIC_OK;
 }

	return Local_ErrorState;
}

