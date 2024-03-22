/*
 *  File      : LCD.c
 *
 *  Created on: Mar 21, 2024
 *      Author: Halla Raafat
 */

#include "HAL/LCD.h"
#include "HAL/LCD_Cfg.h"
#include "MCAL/GPIO.h"


#define FUNC_SET_EIGHT_BIT           0b00111100
#define DISPLAY_ON                   0b00001110
#define DISPLAY_CLEAR                0b00000001
#define ENTRY_MODE_SET_NOSHIFT       0b00000100

#if  (BIT_MODE == EIGHT_BIT_MODE)
#define NUMBER_PINS  11
#elif (BIT_MODE == FOUR_BIT_MODE)
#define NUMBER_PINS  7
#endif

extern LCD_PinCfg_t LCD_Cfg;

void _delay_ms(uint32 milliseconds) {
    volatile uint32 i, j;

    for (i = 0; i < milliseconds; i++) {
        for (j = 0; j < 16000; j++) { // Adjusted for 16 MHz clock frequency
            __asm__("nop"); // This assembly instruction is a no-operation, ensuring accurate timing
        }
    }
}

static void LCD_SendCommand(uint8 Cmd);
void LCD_WriteChar( uint8 data);

LCD_Error_t LCD_InitAsync()
{

	LCD_Error_t Local_ErrorState=LCD_NOK;
	//init output pins of lcd
	GPIO_Pin_Cfg_t LCD_Pin; // create object of GPIO_pin
    LCD_Pin.Mode=GPIO_OUTPUT_PP; // set mode
    LCD_Pin.Speed=GPIO_OUTPUT_SPEED_MED;
    Pin_Cfg_t * Ptr_LCDCfg = & LCD_Cfg;

 for (uint8 Idx =0 ; Idx<BIT_MODE;Idx++)
 {
	LCD_Pin.Pin= Ptr_LCDCfg[Idx].Pin;
	LCD_Pin.Pin= Ptr_LCDCfg[Idx].Port;
   GPIO_InitPin(&LCD_Pin);

 }

// init states of LCD
_delay_ms(30);

LCD_SendCommand(FUNC_SET_EIGHT_BIT);
_delay_ms(1);

LCD_SendCommand(DISPLAY_ON);
_delay_ms(1);

LCD_SendCommand(DISPLAY_CLEAR);

_delay_ms(2);

LCD_SendCommand(ENTRY_MODE_SET_NOSHIFT);


	return Local_ErrorState;

}
LCD_Error_t LCD_GetState()
{
	LCD_Error_t Local_ErrorState=LCD_NOK;
		return Local_ErrorState;
}
LCD_Error_t LCD_ClearScreenAsync()
{
	LCD_Error_t Local_ErrorState=LCD_NOK;
		return Local_ErrorState;
}
LCD_Error_t LCD_WriteStringAsync(uint8 * String , uint8 Length ,uint8 PosX ,uint8 PosY )
{
	LCD_Error_t Local_ErrorState=LCD_NOK;


		return Local_ErrorState;

}

void LCD_WriteChar(uint8 data)
{
	 GPIO_SetPinValue(LCD_Cfg.RegSelect.Port,LCD_Cfg.RegSelect.Pin,GPIO_OUTPUT_HIGH);
	 GPIO_SetPinValue(LCD_Cfg.Read_Write.Port,LCD_Cfg.Read_Write.Pin,GPIO_OUTPUT_LOW);
	 Pin_Cfg_t * Ptr_LCDCfg = & LCD_Cfg;

	for (uint8 Idx =0 ; Idx<BIT_MODE;Idx++)
	{

		 GPIO_SetPinValue(Ptr_LCDCfg[Idx].Port,Ptr_LCDCfg[Idx].Pin,(data>> BIT_MODE-1-Idx)&0x01);

	}
	GPIO_SetPinValue(LCD_Cfg.Enable.Port,LCD_Cfg.Enable.Pin,GPIO_OUTPUT_HIGH);
	_delay_ms(2);
	GPIO_SetPinValue(LCD_Cfg.Enable.Port,LCD_Cfg.Enable.Pin,GPIO_OUTPUT_LOW);

}

static void LCD_SendCommand(uint8 Cmd)
{
 GPIO_SetPinValue(LCD_Cfg.RegSelect.Port,LCD_Cfg.RegSelect.Pin,GPIO_OUTPUT_LOW);
 GPIO_SetPinValue(LCD_Cfg.Read_Write.Port,LCD_Cfg.Read_Write.Pin,GPIO_OUTPUT_LOW);
 Pin_Cfg_t * Ptr_LCDCfg = & LCD_Cfg;

for (uint8 Idx =0 ; Idx<BIT_MODE;Idx++)
{

	 GPIO_SetPinValue(Ptr_LCDCfg[Idx].Port,Ptr_LCDCfg[Idx].Pin,(Cmd>> BIT_MODE-1-Idx)&0x01);

}
GPIO_SetPinValue(LCD_Cfg.Enable.Port,LCD_Cfg.Enable.Pin,GPIO_OUTPUT_HIGH);
_delay_ms(2);
GPIO_SetPinValue(LCD_Cfg.Enable.Port,LCD_Cfg.Enable.Pin,GPIO_OUTPUT_LOW);


}



void LCD_Runnable()
{


}


/*
 *
 * Summary LCD Datasheet
 *
 * IR -> inst reg
 *  write only
 *  1. inst codes
 *  2. addresses display data ram or CGRAM
 * DR -> data reg
 *   Read/write
 *   for DDRAM or CGRAM
 *
 *
 *   REG SELECT  | R/W
 *
 *       0       |  0             inst to IR
 *       0       |  1             //read busy flag DB7
 *       1       |  0             DATA Write  to DR
 *       1       |  1             DATA Read from DR
 *
 *       enable -> high read , at falling engde ->write
 *
 *       when in 4 bit upper bits send first then lower
 *
 *       8 bit
 *
 *       power on
 *       wait 30 ms
 *                          RS   RW D7 6 5 4 3 2 1 0
 *       function set        0   0   0 0 1 1 n f x x   N : line display , f:0:5x7 dot char font
 *
 *
 *
 */
