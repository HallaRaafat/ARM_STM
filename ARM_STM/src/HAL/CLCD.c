/*
 * LCD.c
 *
 *  Created on: Mar 21, 2024
 *      Author: ahmed tarek
 */
#include "HAL/CLCD.h"
#include "MCAL/GPIO.h"

//#define NULL ((void*)0)

#define CLCD_ENABLE_OFF  0x00
#define CLCD_ENABLE_ON   0x01

#define CLCD_LOW   0x00000000
#define CLCD_HIGH  0x00000001
#define BUFFER_SIZE  150

#define EIGHT_BIT_MODE 0x00000008
#define FOUR_BIT_MODE  0x00000004

#define CLCD_FUNCTION_SET_8BIT               0x38                //0b0011 1000
#define CLCD_FUNCTION_SET_4BIT               0x20                //0b0000100000
#define CLCD_CLEAR_DISPLAY                   0x01                //0b0000000001
#define CLCD_RETURN_HOME                     0x03                //0b0000000011
#define CLCD_DISPLAY_ON_CURSOR_ON            0x0F                //0b0000001111     //display on with cursor enable and blinking
#define CLCD_DISPLAY_ON_CURSOR_OFF           0x0C                //0b0000001100     //display on with cursor Disabled
#define CLCD_DISPLAY_OFF                     0x08                //0b0000001000
#define CONTROL_PINS                         0x03
#define ENTRY_MODE_MASK                      0x06

#define INDEX_FOR_LINE2                    0x40

typedef enum
{
	CLCD_Init,
	CLCD_Operation,
	CLCD_Off
}CLCD_State_t;

typedef enum

{
	CLCD_POWER_ON,
	CLCD_Functional_Set,
	CLCD_Dispaly_ON,
	CLCD_Display_Clear,
	CLCD_Entry_Mode,
	CLCD_ENDInit

}CLCD_InitMode_t;

typedef enum
{
    CLCD_Ready,
    CLCD_Busy
} CLCD_UserReqState_t;
typedef enum
{
    CLCD_NoReq,
    CLCD_Write,
    CLCD_Clear,
    CLCD_SetCursor,
	CLCD_WriteNumber
} CLCD_UserReqType_t;

typedef enum
{

    CLCD_WriteStart,
	CLCD_WriteCharacter,
	CLCD_WriteDone
} CLCD_WriteState_t;

typedef enum
{
	CLCD_WriteNumStart,
	CLCD_WriteNum,
}CLCD_WriteNumState_t;

typedef enum
{

    CLCD_SetPosStart,
	CLCD_SetPos,
	CLCD_SetPosEnd
} CLCD_SetPosState_t;

typedef struct
{
    uint8 LinePos;
    uint8 ColPos;

} CLCD_Write_t;
typedef struct
{
	uint8 Num_Buff[16];
}Num_Buff_t;
 typedef struct
{

	const uint8 *String;
	uint16 Length;
	CLCD_UserReqState_t state;
	CLCD_UserReqType_t Type;
	CLCD_Write_t CurrPos;
	Num_Buff_t Num_Buffer;
} CLCD_UserRequest_t;

extern CLCD_CFG_t LCDS[_LCD_NUM];

/**************************Global Variables*****************************/
static uint32 CLCD_EnablePinState = CLCD_ENABLE_OFF;
static CLCD_State_t CLCDState = CLCD_Off;
static CLCD_InitMode_t InitMode = CLCD_POWER_ON;
static CLCD_WriteState_t WriteState = CLCD_WriteStart;
static CLCD_WriteNumState_t WriteNumState = CLCD_WriteNumStart;
static CLCD_SetPosState_t PositionState=CLCD_SetPosStart;
static CLCD_Write_t Current_Write_Pos;
CLCD_UserRequest_t UserRequest[BUFFER_SIZE];
static uint8 UserCurrentIdx=0;

/**************************HELPER FUNCTIONS*****************************/

static CLCD_enuErrorStatus_t CLCD_SendCommand(uint8 Command);
static CLCD_enuErrorStatus_t CLCD_SendData(uint8 Data);
static CLCD_enuErrorStatus_t CLCD_CntrlEnablePin(uint32 EnablePins_State);
static CLCD_enuErrorStatus_t CLCD_CLEAR(void);
static CLCD_enuErrorStatus_t CLCD_SetPosition_Helper(uint8 *Ptr_PositionDDRAM);
/************************Process functions******************************/
static CLCD_enuErrorStatus_t CLCD_PowerOn_proc(void);
static CLCD_enuErrorStatus_t CLCD_InitState(void);
static CLCD_enuErrorStatus_t CLCD_Write_Proc(void);
static CLCD_enuErrorStatus_t CLCD_SetPosition_Proc(void);
static CLCD_enuErrorStatus_t CLCD_WriteNum_Proc(void);




//Num_Buff_t Num_Buffer[BUFFER_SIZE];
//static uint8 Num_Buff[16];





/*each 2ms*/
void RunnableLCD(void)
{
	switch (CLCDState)
	{
	   case CLCD_Init:
		   CLCD_InitState();
		   break;

	   case CLCD_Operation:
		   if (UserRequest[UserCurrentIdx].state==CLCD_Busy)
		   {
			   switch (UserRequest[UserCurrentIdx].Type)
			   {
			      case CLCD_Write:
			    	  CLCD_Write_Proc();
			    	  break;
			      case CLCD_Clear:
			    	  CLCD_CLEAR();
			    	  break;
			      case CLCD_SetCursor:
			    	  CLCD_SetPosition_Proc();
			    	  break;
			      case CLCD_WriteNumber:
			    	  CLCD_WriteNum_Proc();
			    	  break;

			      default:
			    	  break;
			   }

		   }
		   else
		   {
			   UserCurrentIdx++;
			   if (UserCurrentIdx==BUFFER_SIZE)
			   {
				   UserCurrentIdx=0;
			   }
		   }
		   break;

	   case CLCD_Off: break;


        default: break;
	}

}


/*********************************HELPER FUNCTIONS**************************************/
static CLCD_enuErrorStatus_t CLCD_PowerOn_proc(void)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;
	UserRequest[UserCurrentIdx].state=CLCD_Busy;
	uint8 Loc_index;
	GPIO_Pin_Cfg_t clcd_pin;
	clcd_pin.Mode=GPIO_OUTPUT_PP;
	clcd_pin.Speed=GPIO_OUTPUT_SPEED_HIGH;

	for (Loc_index=0;Loc_index<_LCD_NUM;Loc_index++)
	{
		clcd_pin.Port=LCDS[Loc_index].Port;
	    clcd_pin.Pin=LCDS[Loc_index].Pin;
	    Ret_enuErrorStatusCLCD= GPIO_InitPin(&clcd_pin);
	}


return Ret_enuErrorStatusCLCD;

}

/*  GPIO_ErrorStatus_t GPIO_InitPin(GPIO_Pin_Cfg_t * Ptr_PinObject);

GPIO_ErrorStatus_t GPIO_SetPinValue ( void * Port ,uint32 Pin ,uint32 Value);

GPIO_ErrorStatus_t GPIO_GetPinValue ( void * Port ,uint32 Pin,uint8 * Get_Value);
GPIO_ErrorStatus_t GPIO_CFG_AF ( void * Port ,uint32 Pin,uint32 AF_Num);*/



static CLCD_enuErrorStatus_t CLCD_SendCommand(uint8 Command)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;
    uint8 Loc_index;
    uint8 Loc_Buffer;

    if (CLCD_EnablePinState== CLCD_ENABLE_OFF)
 {
    Ret_enuErrorStatusCLCD=GPIO_SetPinValue(LCDS[RS].Port,LCDS[RS].Pin,CLCD_LOW);
    Ret_enuErrorStatusCLCD=GPIO_SetPinValue(LCDS[RW].Port,LCDS[RW].Pin,CLCD_LOW);

    for (Loc_index=0;Loc_index<_LCD_NUM-CONTROL_PINS;Loc_index++)
    {
    	Loc_Buffer=((Command>>Loc_index)&0x01);
    	Ret_enuErrorStatusCLCD=GPIO_SetPinValue(LCDS[Loc_index].Port,LCDS[Loc_index].Pin,Loc_Buffer);

    }
    CLCD_EnablePinState= CLCD_ENABLE_ON;
    CLCD_CntrlEnablePin(CLCD_EnablePinState);

 }
    else
    {
    	CLCD_EnablePinState=CLCD_ENABLE_OFF;
    	CLCD_CntrlEnablePin(CLCD_EnablePinState);

    }

return Ret_enuErrorStatusCLCD;

}

static CLCD_enuErrorStatus_t CLCD_SendData(uint8 Data)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;
    uint8 Loc_index;
    uint8 Loc_Buffer;

    if (CLCD_EnablePinState== CLCD_ENABLE_OFF)
 {
    Ret_enuErrorStatusCLCD=GPIO_SetPinValue(LCDS[RS].Port,LCDS[RS].Pin,CLCD_HIGH);
    Ret_enuErrorStatusCLCD=GPIO_SetPinValue(LCDS[RW].Port,LCDS[RW].Pin,CLCD_LOW);

    for (Loc_index=0;Loc_index<_LCD_NUM-CONTROL_PINS;Loc_index++)
    {
    	Loc_Buffer=((Data>>Loc_index)&0x01);
    	Ret_enuErrorStatusCLCD=GPIO_SetPinValue(LCDS[Loc_index].Port,LCDS[Loc_index].Pin,Loc_Buffer);

    }
    CLCD_EnablePinState=CLCD_ENABLE_ON;
    CLCD_CntrlEnablePin(CLCD_EnablePinState);

 }
    else
    {
    	CLCD_EnablePinState=CLCD_ENABLE_OFF;
    	CLCD_CntrlEnablePin(CLCD_EnablePinState);

    }

return Ret_enuErrorStatusCLCD;

}

static CLCD_enuErrorStatus_t CLCD_CntrlEnablePin(uint32 EnablePins_State)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	Ret_enuErrorStatusCLCD= GPIO_SetPinValue(LCDS[EN].Port,LCDS[EN].Pin,EnablePins_State);
    return Ret_enuErrorStatusCLCD;
}

static CLCD_enuErrorStatus_t CLCD_CLEAR(void)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	CLCD_SendCommand(CLCD_CLEAR_DISPLAY);

	if (CLCD_EnablePinState== CLCD_ENABLE_OFF)
	{
		 UserRequest[UserCurrentIdx].state=CLCD_Ready;
		 UserRequest[UserCurrentIdx].Type=CLCD_NoReq;

	}
return Ret_enuErrorStatusCLCD;

}

static CLCD_enuErrorStatus_t CLCD_SetPosition_Helper(uint8 *Ptr_PositionDDRAM)

{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	uint8 Loc_LocationOnDDRAM=0;

	switch (UserRequest[UserCurrentIdx].CurrPos.LinePos)
	{
	case LINE1 :
		Loc_LocationOnDDRAM= UserRequest[UserCurrentIdx].CurrPos.ColPos; break;
	case LINE2 :
		Loc_LocationOnDDRAM= ((UserRequest[UserCurrentIdx].CurrPos.ColPos)+INDEX_FOR_LINE2); break;

    default :  Ret_enuErrorStatusCLCD = CLCD_enuNOK;
	break;

	}

   *Ptr_PositionDDRAM=Loc_LocationOnDDRAM;

   return Ret_enuErrorStatusCLCD;

}

/************************************************************************************/

static CLCD_enuErrorStatus_t CLCD_InitState(void)
{

	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	switch(InitMode)
	{
	static uint8 counts=0;
	case CLCD_POWER_ON:
		counts++;
		if (counts==20)
		{
		Ret_enuErrorStatusCLCD=CLCD_PowerOn_proc();
		InitMode=CLCD_Functional_Set;
		}
		break;

	case CLCD_Functional_Set:
		Ret_enuErrorStatusCLCD=CLCD_SendCommand(CLCD_FUNCTION_SET_8BIT);
		if (CLCD_EnablePinState==CLCD_ENABLE_OFF)
		{
			InitMode=CLCD_Dispaly_ON;
		}
		break;

	case CLCD_Dispaly_ON:

		Ret_enuErrorStatusCLCD=CLCD_SendCommand(CLCD_DISPLAY_ON_CURSOR_ON);
		if (CLCD_EnablePinState==CLCD_ENABLE_OFF)
		{
			InitMode=CLCD_Display_Clear;
		}
		break;

	case CLCD_Display_Clear:

		Ret_enuErrorStatusCLCD = CLCD_SendCommand(CLCD_CLEAR_DISPLAY);
		if (CLCD_EnablePinState==CLCD_ENABLE_OFF)
		{
			InitMode=CLCD_Entry_Mode;
		}
				break;

	case CLCD_Entry_Mode:

		Ret_enuErrorStatusCLCD = CLCD_SendCommand(ENTRY_MODE_MASK);
		if (CLCD_EnablePinState==CLCD_ENABLE_OFF)
				{
			InitMode=CLCD_ENDInit;
				}
		break;


	case CLCD_ENDInit:

		UserRequest[UserCurrentIdx].state=CLCD_Ready;
		CLCDState=CLCD_Operation;
		break;

	default:
		 Ret_enuErrorStatusCLCD = CLCD_enuNOK;
		 break;



	}

	return Ret_enuErrorStatusCLCD;
}

static CLCD_enuErrorStatus_t CLCD_Write_Proc(void)

{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	switch (WriteState)
	{
	case CLCD_WriteStart:
		WriteState=CLCD_WriteCharacter;
		Current_Write_Pos.ColPos=0;
		break;

	case CLCD_WriteCharacter:
		if(Current_Write_Pos.ColPos != UserRequest[UserCurrentIdx].Length)
		{
			CLCD_SendData(UserRequest[UserCurrentIdx].String[Current_Write_Pos.ColPos]);
			if (CLCD_EnablePinState==CLCD_ENABLE_OFF)
			{
				Current_Write_Pos.ColPos++;

			}
		}
		else
		{
			UserRequest[UserCurrentIdx].state=CLCD_Ready;
			UserRequest[UserCurrentIdx].Type=CLCD_NoReq;
			WriteState=CLCD_WriteStart;
		}	break;



	default: Ret_enuErrorStatusCLCD = CLCD_enuNOK;
	break;

}

return Ret_enuErrorStatusCLCD;

}

static CLCD_enuErrorStatus_t CLCD_WriteNum_Proc(void)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	switch (WriteNumState)
	{
	  case CLCD_WriteNumStart:
		  WriteNumState=CLCD_WriteNum;
			Current_Write_Pos.ColPos=0;
		  break;
	  case CLCD_WriteNum:
			if(Current_Write_Pos.ColPos != UserRequest[UserCurrentIdx].Length)
			{
				CLCD_SendData(UserRequest[UserCurrentIdx].String[Current_Write_Pos.ColPos]);
				if (CLCD_EnablePinState==CLCD_ENABLE_OFF)
				{
					Current_Write_Pos.ColPos++;

				}
			}
			else
			{
				UserRequest[UserCurrentIdx].state=CLCD_Ready;
				UserRequest[UserCurrentIdx].Type=CLCD_NoReq;
				WriteState=CLCD_WriteNumStart;

			}
		  break;
	  default:  break;

	}

return Ret_enuErrorStatusCLCD;
}

static CLCD_enuErrorStatus_t CLCD_SetPosition_Proc(void)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	static uint8 Loc_Location=0;

	switch (PositionState)
	{
	case CLCD_SetPosStart:
		CLCD_SetPosition_Helper(&Loc_Location);
		PositionState=CLCD_SetPos;
		break;

	case CLCD_SetPos:
		Ret_enuErrorStatusCLCD=CLCD_SendCommand((128+Loc_Location));

		if (CLCD_EnablePinState==CLCD_ENABLE_OFF)
		{

			UserRequest[UserCurrentIdx].state=CLCD_Ready;
			UserRequest[UserCurrentIdx].Type=CLCD_NoReq;
			PositionState=CLCD_SetPosStart;
		}
		break;


	default:  Ret_enuErrorStatusCLCD = CLCD_enuNOK;
	break;


	}

return Ret_enuErrorStatusCLCD;
}


/**************************************User Functions Asynch/sync*************************************/

CLCD_enuErrorStatus_t CLCD_InitAsynch(void)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	CLCDState=CLCD_Init;
	return Ret_enuErrorStatusCLCD;
}

CLCD_enuErrorStatus_t CLCD_ClearDisplayAsynch(void)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;
for(uint8 Idx=0;Idx<BUFFER_SIZE;Idx++)
{
	if(CLCDState==CLCD_Operation && UserRequest[Idx].state==CLCD_Ready)
	{

		UserRequest[Idx].state=CLCD_Busy;
		UserRequest[Idx].Type=CLCD_Clear;
        break;

	}

}

    return Ret_enuErrorStatusCLCD;
}

CLCD_enuErrorStatus_t CLCD_WriteStringAsynch(uint8 * Ptr_string,uint8 Length)

{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	if (Ptr_string==NULL)
	{
		Ret_enuErrorStatusCLCD = CLCD_enuNOK;

	}
	else
	{
		for(uint8 Idx=0;Idx<BUFFER_SIZE;Idx++)
		{
		if(CLCDState==CLCD_Operation && UserRequest[Idx].state==CLCD_Ready)
		{
		UserRequest[Idx].state=CLCD_Busy;
		UserRequest[Idx].Type=CLCD_Write;
		UserRequest[Idx].String=Ptr_string;
		UserRequest[Idx].Length=Length;
		break;

		}

		}
	}

    return Ret_enuErrorStatusCLCD;

}

CLCD_enuErrorStatus_t CLCD_WriteNumAsynch(uint32 NUM)
{
    CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

    uint8 Loc_Count=0;
    uint8 Loc_index=0;
    uint32 Loc_temp=NUM;
   ;

    // Clear the local buffer
   // memset(Num_Buff, 0, sizeof(Num_Buff));

    for(uint8 Idx=0; Idx<BUFFER_SIZE; Idx++)
    {
        if(CLCDState == CLCD_Operation && UserRequest[Idx].state == CLCD_Ready)
        {
            UserRequest[Idx].state = CLCD_Busy;
            UserRequest[Idx].Type = CLCD_Write;

            do{
            	UserRequest[Idx].Num_Buffer.Num_Buff[Loc_Count++] = (Loc_temp % 10) + '0';
                    Loc_temp = Loc_temp / 10;
                }while(Loc_temp);
            UserRequest[Idx].Num_Buffer.Num_Buff[Loc_Count]='\0';

            // Reverse the buffer to get the correct order of digits
            for (Loc_index = 0; Loc_index < Loc_Count / 2; Loc_index++)
            {
                uint8 temp = UserRequest[Idx].Num_Buffer.Num_Buff[Loc_index];
                UserRequest[Idx].Num_Buffer.Num_Buff[Loc_index] = UserRequest[Idx].Num_Buffer.Num_Buff[Loc_Count - Loc_index - 1];
                UserRequest[Idx].Num_Buffer.Num_Buff[Loc_Count - Loc_index - 1] = temp;
            }

            UserRequest[Idx].Length = Loc_Count;

            UserRequest[Idx].String = UserRequest[Idx].Num_Buffer.Num_Buff;

            break; // Exit the loop after processing one request
        }
    }

    return Ret_enuErrorStatusCLCD;
}
CLCD_enuErrorStatus_t CLCD_SetCursorAsynch(uint8 XPOS,uint8 YPOS)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	for(uint8 Idx=0;Idx<BUFFER_SIZE;Idx++)
{
	if ((CLCDState==CLCD_Operation) && (UserRequest[Idx].state==CLCD_Ready))
	{
		if ((XPOS>=MAX_NO_ROWS) || (YPOS>=MAX_NO_COL))
		{

			Ret_enuErrorStatusCLCD = CLCD_enuNOK;

		}
		else
		{
			UserRequest[Idx].state=CLCD_Busy;
			UserRequest[Idx].CurrPos.ColPos=YPOS;
			UserRequest[Idx].CurrPos.LinePos=XPOS;
			UserRequest[Idx].Type=CLCD_SetCursor;
			break;


		}

	}

}
	return Ret_enuErrorStatusCLCD;
}

CLCD_enuErrorStatus_t CLCD_GetStatus(uint8 *Ptr_LcdStatus)
{
	CLCD_enuErrorStatus_t Ret_enuErrorStatusCLCD = CLCD_enuOK;

	if (Ptr_LcdStatus==NULL)
	{

		Ret_enuErrorStatusCLCD=CLCD_enuNOK;

	}
	else
	{
		*Ptr_LcdStatus=CLCDState;

	}



return Ret_enuErrorStatusCLCD;

}



/***********************************************************************************************/
