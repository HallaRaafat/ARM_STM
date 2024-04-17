/*
 * HSWITCH_Config.h
 *
 *  Created on: Apr 13, 2024
 *      Author: ahmed tarek
 */

#ifndef HSWITCH_CONFIG_H_
#define HSWITCH_CONFIG_H_

/*USER should configure the SWITCHES name in this enum*/
typedef enum

{
    HSWITCH_Up,
    HSWITCH_Down,
	HSWITCH_Right,
	HSWITCH_Left,

	HSWITCH_Edit_Reset,
	HSWITCH_OK_Start,
	HSWITCH_Mode,


   /*configure the desired number of HSWITCHES*/

   _HSWITCH_NUM


}HSWITCHES_t;


#endif /* HSWITCH_CONFIG_H_ */
