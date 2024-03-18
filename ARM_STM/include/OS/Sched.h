/*
 * Sched.h
 *
 *  Created on: Mar 15, 2024
 *      Author: Halla Raafat
 */

#ifndef SCHED_H_
#define SCHED_H_

#include "OS/Sched_Runnables.h"
#include "LIB/STD_Types.h"
#include "MCAL/Systick.h"



typedef enum
{
	SCHED_OK,
	SCHED_NOK

}SCHED_Error_t;

typedef void (* Callback_t)(void);

typedef struct
{

   uint32 Periodicity_ms;//in millisec
   Callback_t    Callback;
   uint32   Delay_ms;

}Runnable_t;


SCHED_Error_t SCHED_Init();

SCHED_Error_t SCHED_Start();

#endif /* SCHED_H_ */
