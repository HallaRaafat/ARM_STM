/*
 * NVIC.h
 *
 *  Created on: Mar 5, 2024
 *      Author: Halla Raafat
 */

#ifndef NVIC_H_
#define NVIC_H_

#define NVIC_ENABLE        0x1

#define NVIC_DISABLE       0x0

typedef enum
{

NVIC_OK,
NVIC_NOK

}NVIC_Error_t;
/*
 * Enable/ disable interrupt or exception
 */
NVIC_Error_t NVIC_Control_IRQ(uint8 NVIC_IRQ, uint8 State);
/*
 * set/clear pending status of interrupt
 *
 */
NVIC_Error_t NVIC_ControlPendingIRQ(uint8 NVIC_IRQ,uint8 State);
/*
 * Reads the pending status of interrupt or
exception. This function returns non-zero value
 if the pending status is set to 1.
 */
NVIC_Error_t NVIC_GetPendingIRQ(uint8 NVIC_IRQ,uint8 * Pending_Value);
/*
 * Sets the priority of an interrupt or
 * exception with configurable priority level to 1.
 */
NVIC_Error_t NVIC_SetPirority(uint8 NVIC_IRQ);

/*
 * Reads the priority of an interrupt or
 * exception with configurable priority level.
 * This function return the current priority level.
 */


NVIC_Error_t NVIC_GetPirority(uint8 NVIC_IRQ);
/*
 * Active or not active (current IRQ )
 */
NVIC_Error_t NVIC_GetStatus(uint8 NVIC_IRQ);




NVIC_Error_t NVIC_GenerateSWInterrupt(uint8 NVIC_IRQ);




#endif /* NVIC_H_ */
