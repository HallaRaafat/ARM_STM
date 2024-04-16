/*
 * Sched_Runnables.h
 *
 *  Created on: Mar 17, 2024
 *      Author: Halla Raafat
 */

#ifndef SCHED_RUNNABLES_H_
#define SCHED_RUNNABLES_H_

//Change the ticktime for the value of schedular in (ms)
// max 1000 ms min 1 ms

#define SCHED_TICK_TIME 1


typedef enum
{
	//Runnable_Traffic,
   Runnable_LCD,
   Runnable_APP,
   Runnable_SWITCH,
   Runnable__Timer_Update,
  _Num_Runnables
}Runnables;

#endif /* SCHED_RUNNABLES_H_ */
