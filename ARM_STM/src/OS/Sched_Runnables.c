/*
 * Sched_Runnables.c
 *
 *  Created on: Mar 17, 2024
 *      Author: Halla Raafat
 */
#include "OS/Sched.h"
#include "OS/Sched_Runnables.h"

#define TRAFFIC_PERIODICITY_MS     2000
#define TRAFFIC_INITIAL_DELAY_MS   1000


void Traffic_Runnable();




/*
 * 1. Write the prototype of the call back function before the array just to compile
 * 2. Fill the array as the priorties depends on the enum sequence in .h file
 * 3.
 *   [Enum]={           .Periodicity_ms=(Periodic time of the runnable ),
		                .Delay_ms= (initial delay of the runnable),
		                .Callback=(Function )}
 */
const Runnable_t Runnables_Array[_Num_Runnables]=
{

[Runnable_Traffic]={
		                .Periodicity_ms=TRAFFIC_PERIODICITY_MS,
		                .Delay_ms= TRAFFIC_INITIAL_DELAY_MS,
		                .Callback=Traffic_Runnable,




                   }


};

