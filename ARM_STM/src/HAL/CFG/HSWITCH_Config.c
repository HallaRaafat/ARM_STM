/*
 * HSWITCH_Config.c
 *
 *  Created on: Apr 13, 2024
 *      Author: ahmed tarek
 */

#include "MCAL/GPIO.h"
#include "HAL/HSWITCH.h"


/*ARRAY FOR CONFIGURATION OF THE SWITCHES(pin,port,connection) based on the gpio driver*/

HSWITCH_CFG_t HSWITCHES [_HSWITCH_NUM]=

{
		[HSWITCH_Up]= {
				.PIN=GPIO_PIN12,
				.PORT=GPIO_PORTB,
				.Connection=GPIO_INPUT_PU

		},

		[HSWITCH_Down]= {
					.PIN=GPIO_PIN13,
					.PORT=GPIO_PORTB,
					.Connection=GPIO_INPUT_PU

			},

		[HSWITCH_Right]= {
						.PIN=GPIO_PIN14,
						.PORT=GPIO_PORTB,
						.Connection=GPIO_INPUT_PU

				},
		[HSWITCH_Left]= {
							.PIN=GPIO_PIN15,
							.PORT=GPIO_PORTB,
							.Connection=GPIO_INPUT_PU

					},

		[HSWITCH_Edit_Reset]= {
								.PIN=GPIO_PIN6,
								.PORT=GPIO_PORTB,
								.Connection=GPIO_INPUT_PU

						},
		[HSWITCH_OK_Start]= {
									.PIN=GPIO_PIN7,
									.PORT=GPIO_PORTB,
									.Connection=GPIO_INPUT_PU

							},
		[HSWITCH_Mode]= {
										.PIN=GPIO_PIN5,
										.PORT=GPIO_PORTB,
										.Connection=GPIO_INPUT_PU

								},

		/*
		,

		[HSWITCH_2]= {
				.PIN=GPIO_PIN6,
				.PORT=GPIO_PORTB,
				.Connection=GPIO_INPUT_PU

		       	},
		[HSWITCH_3]= {
						.PIN=GPIO_PIN7,
						.PORT=GPIO_PORTB,
						.Connection=GPIO_INPUT_PU

				       	}
*/

};

