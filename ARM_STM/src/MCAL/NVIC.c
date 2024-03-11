/*
 * NVIC.c
 *
 *  Created on: Mar 5, 2024
 *      Author: Halla Raafat
 */



#include "MCAL/NVIC.h"
#include "LIB/STD_Types.h"
#include "MCAL/STM32F401C_IRQ.h"

#define NVIC_BASE_ADDRESS               (0xE000E100)
#define SCB_AIRCR                       (*((volatile uint32 *)0xE000ED0C))


#define SCB_AIRCR_SUB_GROUP_MASK        0x00000700
#define SHIFT_TWOBITS_HEX               0x00000100
#define EIGHT_BITS                       8
#define BIT_3_MASK                      0x00000004
#define FOUR_BITS_MASK                  0x0000000f
#define POWER2                           1

#define NVIC_PRI_IRQ_PER_REG 4
#define LOWER_FOUR_BITS      4
#define REG_SIZE              32

#define IRQ_LOW_BOUND_LIMIT 0
#define IRQ_HIGH_BOUND_LIMIT 240
#define BYTE_IN_BITS          8

#define PRIORITY_HIGH_BOUND  15
#define PRIORITY_LOW_BOUND    0
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
  uint32 Reserved6 [580];
  uint32 STIR;

}NVIC_t;

volatile NVIC_t * const NVIC =(volatile NVIC_t *const) NVIC_BASE_ADDRESS  ;

static volatile  uint32 Global_SubBits = NVIC_SUB_BITS_NONE;
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


const uint32 bit_masks[REG_SIZE] = {
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

	 if (NVIC_IRQ < IRQ_HIGH_BOUND_LIMIT && NVIC_IRQ >= IRQ_LOW_BOUND_LIMIT)
	 {
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


	 }

	return Local_ErrorState;
}

/*
 * set/clear pending status of interrupt
 *
 */
NVIC_Error_t NVIC_ControlPendingIRQ(uint8 NVIC_IRQ,uint8 State)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;
/*
*
*  Interrupt set-pending register x (NVIC_ISPRx)
*  write 1 -> interrupt state to pending
*  NVIC_ICPRx-> write 1 clear pending
*/

	 if (NVIC_IRQ < IRQ_HIGH_BOUND_LIMIT && NVIC_IRQ >= IRQ_LOW_BOUND_LIMIT)
	 {

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




NVIC_Error_t NVIC_SetPirorityGroup(uint32 SubBits)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;
/*
Application interrupt and reset control register (AIRCR) in SCB
Bits 10:8 PRIGROUP: Interrupt priority grouping field
x for group priority y for sub priority

                       Group               Sub
0b0xx 0bxxxx    [7:4]    4 bits = 16
0b100 0bxxx.y   7:5      3 =8            [4] 1 bit=2
0b101 0bxx.yy
0b110 0bx.yyy
0b111 0b.yyyy   none                       7:4
 */

if (    SubBits == NVIC_SUB_BITS_EIGHT ||
		SubBits == NVIC_SUB_BITS_FOUR||
		SubBits == NVIC_SUB_BITS_NONE||
		SubBits == NVIC_SUB_BITS_SIXTEEN||
		SubBits == NVIC_SUB_BITS_TWO)
{
uint32 Local_AIRCR =SCB_AIRCR ;

Local_AIRCR&= ~SCB_AIRCR_SUB_GROUP_MASK;
Local_AIRCR =SubBits;
SCB_AIRCR=Local_AIRCR;
Global_SubBits=SubBits;
Local_ErrorState=NVIC_OK;
}


	return Local_ErrorState;



}

/*
 * Interrupt priority grouping
 interrupt priority register entry into two fields:

• An upper field that defines the group priority
• A lower field that defines a subpriority within the group.

Only the group priority determines preemption of interrupt exceptions.
 another interrupt with the same group priority does not preempt the handler,
If multiple pending interrupts have the same group priority, the subpriority field determines the order in which they are processed.
If multiple pending interrupts have the same group priority and subpriority, the interrupt with the lowest IRQ number is processed first.

 */


NVIC_Error_t NVIC_SetPirority(uint8 NVIC_IRQ,uint8 GroupPriority ,uint8 SubPriority)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;

   /*


 * processor implements only bits[7:4] of each field, bits[3:0] read
as zero and ignore writes.



Application interrupt and reset control register (AIRCR) in SCB
Bits 10:8 PRIGROUP: Interrupt priority grouping field
x for group priority y for sub priority

                       Group               Sub
0b0xx 0bxxxx    [7:4]    4 bits = 16         0
0b100 0bxxx.y   7:5      3 =8             4  1 bit=2   0 or 1
0b101 0bxx.yy                             5  2 bits=4  0,1,2,3
0b110 0bx.yyy                             6  3 bits=8  0,1,2,3..7
0b111 0b.yyyy   none                      7: 4     16   0....15


	*
	*
	*/

 if (NVIC_IRQ < IRQ_HIGH_BOUND_LIMIT && NVIC_IRQ >= IRQ_LOW_BOUND_LIMIT)
 {
		/*
		*
		*   Interrupt priority register x (NVIC_IPRx)
		*   NVIC_IPR0 -> IP0,IP1,IP2,IP3   each int have 1 byte to define its priority
		*      till NVIC_IPR59
		*      so /4 to get register offset and %4 to get to which byte
		*
		*
		*/
				 uint8 Local_IRQ_Register = NVIC_IRQ/NVIC_PRI_IRQ_PER_REG;
				 uint8 Local_IRQ_BYTE_offset=NVIC_IRQ%NVIC_PRI_IRQ_PER_REG;
		 if (GroupPriority>=PRIORITY_LOW_BOUND && GroupPriority <= PRIORITY_HIGH_BOUND)

		 {

				 if (Global_SubBits==NVIC_SUB_BITS_NONE)

				 {
					//do nth
				 }

				 else
				 {
					  // Shift sub bits 2 hex 0x700 -> 0x7 then clear bit 3  0b111->0b011  +1 so to get value of bits
							 uint8 Local_SubBits=((Global_SubBits>>EIGHT_BITS)&~ BIT_3_MASK)+1;

					 // 1<< local_subbits = if sub bits =2 then 1<<2 = 4 then highest prioity <4 0,1,2,3
							if( SubPriority < ( POWER2<<Local_SubBits ) && SubPriority>= PRIORITY_LOW_BOUND)
							{

									GroupPriority<<= Local_SubBits ; //shift them with number of sub priority bits
									GroupPriority|=SubPriority;  // add sub priority to the group priority variable


							}
				 }

				 GroupPriority&=FOUR_BITS_MASK; // for safety not exceed four bits
				uint32 Local_IPR= NVIC->IPR[Local_IRQ_Register];
				Local_IPR&=~ (FOUR_BITS_MASK<< ((Local_IRQ_BYTE_offset * BYTE_IN_BITS)+LOWER_FOUR_BITS));
				Local_IPR=  GroupPriority << ((Local_IRQ_BYTE_offset * BYTE_IN_BITS)+LOWER_FOUR_BITS);
				 NVIC->IPR[Local_IRQ_Register]= Local_IPR; // priority starts from bit 4:7



				 Local_ErrorState=NVIC_OK;
		 }

}



	return Local_ErrorState;
}

/*
 * Reads the priority of an interrupt or
 * exception with configurable priority level.
 * This function return the current priority level.
 */


NVIC_Error_t NVIC_GetPirority(uint8 NVIC_IRQ,uint8 * Group_Priority)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;


 if (NVIC_IRQ < IRQ_HIGH_BOUND_LIMIT && NVIC_IRQ >= IRQ_LOW_BOUND_LIMIT)
 {
	 uint8 Local_IRQ_Register = NVIC_IRQ/NVIC_PRI_IRQ_PER_REG;
	 uint8 Local_IRQ_BYTE_offset=NVIC_IRQ%NVIC_PRI_IRQ_PER_REG;

	 uint32 Local_IPR= NVIC->IPR[Local_IRQ_Register];

	   *Group_Priority=(uint8)(Local_IPR>>  ((Local_IRQ_BYTE_offset * BYTE_IN_BITS)+LOWER_FOUR_BITS));


 }

	return Local_ErrorState;
}
/*
 * Active or not active (current IRQ )
 */
NVIC_Error_t NVIC_GetStatus(uint8 NVIC_IRQ, uint8 *State)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;

	/*
	*
	*
	*  (NVIC_IABRx) Interrupt active bit register
	*   NVIC_IABRx -> read only
	*   A bit reads as 1
	*   if the status of the corresponding interrupt is active
	*    or active and pending.
	*/



	if (NVIC_IRQ < IRQ_HIGH_BOUND_LIMIT && NVIC_IRQ >= IRQ_LOW_BOUND_LIMIT)
	 {

		uint8 Local_RegisterNumber= NVIC_IRQ/REG_SIZE;
		uint8 Local_PinNumber= NVIC_IRQ%REG_SIZE;

			*State = (uint8)((NVIC->IABR[Local_RegisterNumber]& bit_masks[Local_PinNumber])>> Local_PinNumber );

	 Local_ErrorState= NVIC_OK;
	 }

	return Local_ErrorState;
}




NVIC_Error_t NVIC_GenerateSWInterrupt(uint8 NVIC_IRQ)
{
	NVIC_Error_t Local_ErrorState= NVIC_NOK;
	/*
	 *Software trigger interrupt register (NVIC_STIR)
	 *  write only in 8 bits
	 *  Write to the STIR to generate a Software Generated Interrupt (SGI). The value to be written is
	 *  the Interrupt ID of the required SGI, in the range 0-239. For example, a value of 0x03 specifies interrupt IRQ3.
     */
 if (NVIC_IRQ < IRQ_HIGH_BOUND_LIMIT && NVIC_IRQ >= IRQ_LOW_BOUND_LIMIT)
 { NVIC->STIR = NVIC_IRQ;
 Local_ErrorState= NVIC_OK;
 }

	return Local_ErrorState;
}

