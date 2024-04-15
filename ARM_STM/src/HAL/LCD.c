/*
 *  File      : LCD.c
 *
 *  Created on: Mar 21, 2024
 *      Author: Halla Raafat
 */
#ifdef LCD_HALLA

#include "HAL/CFG/LCD_Cfg.h"
#include "HAL/LCD.h"
#include "MCAL/GPIO.h"





#define CONCAT_HELPER(d7,d6,d5,d4,d3,d2,d1,d0)       0b##d7##d6##d5##d4##d3##d2##d1##d0
#define CONCAT(d7,d6,d5,d4,d3,d2,d1,d0)              CONCAT_HELPER(d7,d6,d5,d4,d3,d2,d1,d0)



#if (LCD_BIT_MODE == LCD_8_BIT_MODE)
#define LCD_FUNCTION_SET        CONCAT(0,0,1,1,LCD_NUMBER_LINES,LCD_FONT_SIZE,0,0)


#elif (LCD_BIT_MODE == LCD_4_BIT_MODE)
#define LCD_FUNCTION_SET        CONCAT(0,0,1,0,LCD_NUMBER_LINES,LCD_FONT_SIZE,0,0)

#endif

#define LCD_DISPLAY_CONTROL     CONCAT(0,0,0,0,1,LCD_DISPLAY,LCD_CURSOR,LCD_BLINK)

#define LCD_ENTRY_MODE          CONCAT(0,0,0,0,0,1,LCD_INC_DEC,LCD_SHIFT)


#define LCD_CLEAR               0x01


#define HIGH_BITS       4
#define LOW_BITS       0

#if  (LCD_BIT_MODE == LCD_8_BIT_MODE)
#define NUMBER_PINS  11
#define CONTROL_ENABLE_COUNT   2  // first time to enable high 2nd to make it low
#elif (BIT_MODE == LCD_4_BIT_MODE)
#define NUMBER_PINS  7
#define CONTROL_ENABLE_COUNT   4 // high ,low then high again to send lower nibble then low

#endif

typedef void (*LCD_CallBack_t)(void);

typedef struct
{
	uint8 RequestType;  // write or clear or set cursor
	uint8 State;
	LCD_CallBack_t CallBack; //callback

}LCD_Request_t;

typedef struct
{
     uint8 * Data;
    uint8 Length;
    uint8 Index;
}LCD_WriteRequest_t;

typedef enum
{

    Write,
    Clear_Screen,
    Set_CursorPosition,
	Nth
}LCD_RequestTypes_t;

typedef enum
{
    Available,
    Busy,
}LCD_RequestStates_t;

typedef enum{
	PowerOn,
	FunctionSet,
	DisplayOnOff,
	DisplayClear,
	EntryModeSet,
	EndInit
}InitState_t;

typedef enum{
	LCD_Off,
	LCD_Init,
	LCD_Operation
}LCD_State_t;



static LCD_State_t LCD_State = LCD_Off;

static LCD_Request_t Request;
static LCD_WriteRequest_t WriteRequest;




extern LCD_PinCfg_t LCD_Cfg;



static void LCD_WriteCommand(uint8 Cmd);
static void LCD_WriteChar( uint8 data);
static void LCD_InitStateMachine();
static void LCD_SendByte(uint8 Byte  );
static void  LCD_OperationProcess();
static void Write_String();


static void LCD_WriteCommand(uint8 Cmd)
{
	GPIO_SetPinValue(LCD_Cfg.RegSelect.Port,LCD_Cfg.RegSelect.Pin,GPIO_OUTPUT_LOW);
    GPIO_SetPinValue(LCD_Cfg.Read_Write.Port,LCD_Cfg.Read_Write.Pin,GPIO_OUTPUT_LOW);
    LCD_SendByte(Cmd);


}
static void LCD_WriteChar( uint8 data)
{
	GPIO_SetPinValue(LCD_Cfg.RegSelect.Port,LCD_Cfg.RegSelect.Pin,GPIO_OUTPUT_HIGH);
	GPIO_SetPinValue(LCD_Cfg.Read_Write.Port,LCD_Cfg.Read_Write.Pin,GPIO_OUTPUT_LOW);
	LCD_SendByte(data);

}


static void LCD_SendByte(uint8 Byte  ){

	  static uint8 LCD_Enable_Pin = 	GPIO_OUTPUT_LOW; //make sure it is low


	    static uint8 Send_4_bits = HIGH_BITS;

	    if (LCD_Enable_Pin==GPIO_OUTPUT_LOW)
	    {
	    	   Pin_Cfg_t * Ptr_LCDCfg = ( Pin_Cfg_t *)& LCD_Cfg;

	    		for (uint8 Idx =0 ; Idx<LCD_BIT_MODE;Idx++)
	    		{
						#if LCD_BIT_MODE == LCD_8_BIT_MODE

	    		        	GPIO_SetPinValue(Ptr_LCDCfg[Idx].Port,Ptr_LCDCfg[Idx].Pin,(Byte>> (LCD_BIT_MODE-1-Idx))& 0x01);

						#elif    LCD_BIT_MODE == LCD_4_BIT_MODE

	    		        	GPIO_SetPinValue(Ptr_LCDCfg[Idx].Port,Ptr_LCDCfg[Idx].Pin,(Byte>> (Send_4_bits + LCD_BIT_MODE-1-Idx) )&0x01);

						#endif


	    		}

	    	LCD_Enable_Pin=	GPIO_OUTPUT_HIGH;
	    }
	    else
	    {
	    	LCD_Enable_Pin=	GPIO_OUTPUT_LOW;


	        #if    LCD_BIT_MODE == LCD_4_BIT_MODE
	        if(Send_4_bits==HIGH_BITS)
	    	Send_4_bits=LOW_BITS;
	        else
	        Send_4_bits=HIGH_BITS;
		    #endif
	    }

	    GPIO_SetPinValue(LCD_Cfg.Enable.Port,LCD_Cfg.Enable.Pin,LCD_Enable_Pin);

}


LCD_Error_t LCD_InitAsync()
{
	LCD_Error_t Local_ErrorState=LCD_NOK;
	//init output pins of lcd
	GPIO_Pin_Cfg_t LCD_Pin; // create object of GPIO_pin
    LCD_Pin.Mode=GPIO_OUTPUT_PP; // set mode
    LCD_Pin.Speed=GPIO_OUTPUT_SPEED_MED;
    Pin_Cfg_t * Ptr_LCDCfg =(Pin_Cfg_t * ) & LCD_Cfg;

 for (uint8 Idx =0 ; Idx<NUMBER_PINS;Idx++)
 {
	LCD_Pin.Pin= Ptr_LCDCfg[Idx].Pin;
	LCD_Pin.Port= Ptr_LCDCfg[Idx].Port;
     GPIO_InitPin(&LCD_Pin);
 }

 GPIO_SetPinValue(LCD_Cfg.Enable.Port,LCD_Cfg.Enable.Pin,GPIO_OUTPUT_LOW);
	LCD_State=LCD_Init;
	Local_ErrorState=LCD_OK;// fix error from GPIO

	return Local_ErrorState;

}




void LCD_Runnable()
{
	switch(LCD_State)
	{
	case LCD_Off:
		         // DO NTH
		          break;
	case LCD_Init:
		         //Init state machine
		        LCD_InitStateMachine();
		        break;
	case LCD_Operation:
		         //depend on request
                 LCD_OperationProcess();
		        break;
	default:
		      break;


	}


}


static void LCD_InitStateMachine()
{
static uint8  Counter=0;
static InitState_t InitState=PowerOn;

switch(InitState)
	{
			case PowerOn:
				        Counter+=LCD_PERIODICITY_MS;
				          /*Delay after power on = 30 ms,  36 just in case */
				          if (Counter>=36)
				          {
				        	  InitState=FunctionSet;
				        	  Counter=0;
				          }
						  break;

			case FunctionSet:
                          Counter++;

                         if(Counter==CONTROL_ENABLE_COUNT)
                         {   InitState=DisplayOnOff;
                        	 Counter=0;

                         }
                         LCD_WriteCommand(LCD_FUNCTION_SET);

				break;
			case DisplayOnOff:
							Counter++;

							if(Counter==CONTROL_ENABLE_COUNT)
							{   InitState=DisplayOnOff;
							 Counter=0;

							}
							LCD_WriteCommand(LCD_DISPLAY_CONTROL);
				break;
			case DisplayClear:
							Counter++;

							if(Counter==CONTROL_ENABLE_COUNT)
							{   InitState=DisplayOnOff;
							 Counter=0;

							}
							LCD_WriteCommand(LCD_CLEAR);
				break;
			case EntryModeSet:
							Counter++;

							if(Counter==CONTROL_ENABLE_COUNT)
							{   InitState=DisplayOnOff;
							 Counter=0;

							}
							LCD_WriteCommand(LCD_ENTRY_MODE);
				break;
			case EndInit:
				LCD_State=LCD_Operation;
				break;
			default:
				break;

	}
}

static void LCD_OperationProcess()
{
if (Request.RequestType== Write)
{
   Write_String();
}

}

static void Write_String()
{

if(WriteRequest.Index< WriteRequest.Length)
{
	LCD_WriteChar(WriteRequest.Data[WriteRequest.Index]);
	static Counter=0;
	Counter++;

	if (Counter ==CONTROL_ENABLE_COUNT)
	{  WriteRequest.Index++;
		Counter=0;

	}
}
else
{
	Request.State=Available;


}



}
LCD_Error_t LCD_GetState(uint8 * Ptr_LCDState)
{
	LCD_Error_t Local_ErrorState=LCD_NOK;


		return Local_ErrorState;

}
LCD_Error_t LCD_ClearScreenAsync()
{
	LCD_Error_t Local_ErrorState=LCD_NOK;


		return Local_ErrorState;

}
LCD_Error_t LCD_WriteStringAsync( uint8 * String , uint8 Length ,uint8 PosX ,uint8 PosY )
{
	LCD_Error_t Local_ErrorState=LCD_NOK;
   if (Request.State==Available)
   {
	   Request.State=Busy;
	   Request.RequestType=Write;
	   WriteRequest.Data=String;
	   WriteRequest.Length=Length;
	   WriteRequest.Index=0;
   }

		return Local_ErrorState;

}


LCD_Error_t LCD_Write_NumberAsync(uint32 Number)
{
	LCD_Error_t Local_ErrorState=LCD_NOK;


		return Local_ErrorState;

}


/*
LCD_Error_t LCD_InitAsync()
{



// init states of LCD
_delay_ms(50);

LCD_SendCommand(FUNC_SET_EIGHT_BIT);
_delay_ms(2);

LCD_SendCommand(DISPLAY_ON);
_delay_ms(2);

LCD_SendCommand(DISPLAY_CLEAR);

_delay_ms(2);

LCD_SendCommand(ENTRY_MODE_SET_NOSHIFT);


	return Local_ErrorState;




}
*/



/*
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
*/
/*
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
*/



#endif


