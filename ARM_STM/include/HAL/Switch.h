/*
 * Switch.h
 *
 *  Created on: Feb 25, 2024
 *      Author: Halla Raafat
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "LIB/STD_Types.h"
#include "HAL/Switch.h"

#include "HAL/Switch_Cfg.h"
#define SW_ACTIVESTATE_PU           0x00000008                //LED CONNECTION FORWARD
#define SW_ACTIVESTATE_PD           0x00000010


#define SW_STATE_ON               0x00000001
#define SW_STATE_OFF              0x00000000



// struct for leds configuration

typedef struct
{
	uint32 Pin;
	void * Port ;
	uint32 ActiveState ;// pullup or pulldown


}SW_Cfg_t;

typedef enum
{
	SW_OK,
	SW_NOK

}SW_ErrorStatus_t;
/*
 * @brief: Initialize Switch pin directions
 * @return: SW error status ok,nok
 */


SW_ErrorStatus_t SW_Init(void);

/*
 * @brief: Get switch values depend on pullup or pulldown and put the value in state pointer
 * @inputs: Switch name , pointer to 1 byte memory
 * @return: error status ok,not ok (NOK)
 */
SW_ErrorStatus_t SW_GetState(SW_Names_t Switch ,uint8 *PtrState);



#endif /* SWITCH_H_ */
