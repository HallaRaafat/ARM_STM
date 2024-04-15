/*
 * HSWITCH.h
 *
 *  Created on: Apr 13, 2024
 *      Author: ahmed tarek
 */

#ifndef HSWITCH_H_
#define HSWITCH_H_

#include "LIB/STD_Types.h"
#include "HAL/CFG/HSWITCH_Config.h"

#define HSWITCH_PRESSED  0x00000001
#define HSWITCH_RELEASED 0x00000000

typedef struct
{
	uint32   PIN;
	void* PORT;
	uint32 Connection;

}HSWITCH_CFG_t;

typedef enum

{
	HSWITCH_enuOK,
	HSWITCH_enuNOK


}HSWITCH_enuErrorStatus_t;


/*
 * @brief    : function to initialize the HSWITCH based on user configuration
 * @param[in]: none (void)
 * @return   : Error status (HSWITCH_enuOk,HSWITCH_enuNOK)
 * @details  : This function initializes a HSWITCH pin according to the provided configuration.
 */
HSWITCH_enuErrorStatus_t HSWITCH_Init(void);

/*
 * @brief    : Function to set the value of a specified SWITCH to a specific state
 * @param[in]: HSWITCH NAME , HSWITCHState(PRESSED OR RELEASED)
 * @return   : Error status (HSWITCH_enuOk,HSWITCH_enuNOK)
 * @details  : Function to set the value of a specified HSWITCH to a specific state either HIGH OR LOW
 */

HSWITCH_enuErrorStatus_t HSWITCH_GetStatus(HSWITCHES_t HSWITCH_NAME, uint8 * ptr_TO_HSWITCHStatus);


#endif /* HSWITCH_H_ */
