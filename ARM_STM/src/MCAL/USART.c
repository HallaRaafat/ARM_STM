/*
 * USART.c
 *
 *  Created on: Apr 13, 2024
 *      Author: Halla Raafat
 */

#include "MCAL/USART.h"
#include "MCAL/USART_Cfg.h"
#include "LIB/STD_Types.h"


#define USART_OVS_MASK    0x00008000
#define USART_ENABLE_MASK 0X00002000 //Bit 13
#define STATE_READY       0X00000000
#define STATE_BUSY        0X00000001

#define USART_TX_ENABLE_MASK  0X00000008
#define USART_TXE_ENABLE_MASK 0X00000080
#define USART_RX_ENABLE_MASK  0X00000004
#define USART_RXE_ENABLE_MASK 0X00000020

#define USART_RXNE_FLAG_MASK       0X00000020
#define USART_TXE_FLAG_MASK        0X00000080

typedef struct
{
    uint32 SR;
    uint32 DR;
    uint32 BRR;
    uint32 CR1;
    uint32 CR2;
    uint32 CR3;
    uint32 GTPR;

} USART_t;

typedef struct
{
    uint8 *data;
    uint32 Pos;
    uint32 Size;
    uint8 state;
    CB_t CB;
} USART_Req_t;


extern const  USART_Cfg_t USART_ARR[_USART_Num] ;
static USART_Req_t TxReq[_USART_Num]={};
static USART_Req_t RxReq[_USART_Num]={};





USART_Error_t USART_Init (void)
{
	USART_Error_t Local_ErrorState = USART_NOK;



    uint32   Loc_CR1Value = 0;
    uint32   Loc_CR2Value = 0;


    uint32 Loc_DIV_Fraction = 0;

    uint32 Loc_DIV_Mantissa = 0;


for (USART_Names_t Idx=0 ;Idx<_USART_Num;Idx++ )
{





  // Div value calculate consider float

	Loc_DIV_Mantissa=(FCPU/(USART_ARR[Idx].BaudRate*USART_ARR[Idx].OverSamplingMode));
	// To round up fraction X+(y-1)/y
	Loc_DIV_Fraction=(	((FCPU+(USART_ARR[Idx].BaudRate-1))/USART_ARR[Idx].BaudRate)-(Loc_DIV_Mantissa*USART_ARR[Idx].OverSamplingMode)  );
    // overflow happened
    if (Loc_DIV_Fraction == USART_ARR[Idx].OverSamplingMode)
    {
    	Loc_DIV_Fraction=0;
    	Loc_DIV_Mantissa++;
    }

    //Oversampling bit

     // if 8 set bit 15 if 16 clear bit 15 in CR1
   	if (USART_ARR[Idx].OverSamplingMode== USART_OVS_8)
   	  {
   		  Loc_CR1Value|= USART_OVS_MASK;
   	  }

   	Loc_CR1Value= USART_ENABLE_MASK | USART_ARR[Idx].WordLength | USART_ARR[Idx].ParityControl| USART_ARR[Idx].ParityType;
   	Loc_CR2Value= USART_ARR[Idx].StopBits;

	//assign local variables in main register

	((volatile USART_t * ) (USART_ARR[Idx].USART_ID ))-> BRR = (Loc_DIV_Mantissa << 4) | Loc_DIV_Fraction;

	((volatile USART_t * ) (USART_ARR[Idx].USART_ID ))-> CR1 = Loc_CR1Value;
	((volatile USART_t * ) (USART_ARR[Idx].USART_ID ))-> CR2 = Loc_CR2Value;





}


	return Local_ErrorState;

}

USART_Error_t USART_SendByte(User_Request_t * Ptr_Request)
{
	USART_Error_t Local_ErrorState = USART_NOK;
   if (Ptr_Request==NULL)
   { // Error status as it is

   }
   else
   {

	   volatile USART_t *USART_Ptr = (volatile USART_t *)(USART_ARR[Ptr_Request->USART_Num].USART_ID);
	   if ( TxReq[Ptr_Request->USART_Num].state== STATE_READY)
	   {
		   volatile uint16 Timer =3000; // Time out

		   TxReq[Ptr_Request->USART_Num].state=STATE_BUSY;



		   USART_Ptr-> DR  =*(Ptr_Request->Ptr_Buffer);
		   USART_Ptr-> CR1 |= USART_TX_ENABLE_MASK;

		   while( (USART_Ptr->SR & USART_TXE_FLAG_MASK)== 0  && Timer)
		   {
			   Timer--;
		   }
		   // if timer =0 then problem
		   if (Timer==0)
		   {
			   Local_ErrorState = USART_NOK;
		   }
		   else
		   {

			   Local_ErrorState = USART_OK;
		   }
		     USART_Ptr-> CR1 &=~ USART_TX_ENABLE_MASK;// TEST IT
		   TxReq[Ptr_Request->USART_Num].state= STATE_READY;

	   }
	   else
	   {

		   TxReq[Ptr_Request->USART_Num].state= STATE_BUSY;
	   }

   }


	return Local_ErrorState;

}
USART_Error_t USART_GetByte(User_Request_t * Ptr_Request)
{
	USART_Error_t Local_ErrorState = USART_NOK;
	   if (Ptr_Request==NULL)
	   { // Error status as it is

	   }
	   else
	   {

		   volatile USART_t *USART_Ptr = (volatile USART_t *)(USART_ARR[Ptr_Request->USART_Num].USART_ID);
		   if ( RxReq[Ptr_Request->USART_Num].state== STATE_READY)
		   {
			   volatile uint16 Timer =3000; // Time out

			   RxReq[Ptr_Request->USART_Num].state=STATE_BUSY;
			   USART_Ptr-> CR1 |= USART_RX_ENABLE_MASK;

			   while( (USART_Ptr->SR & USART_RXNE_FLAG_MASK)== 0  && Timer)
			   			   {
			   				   Timer--;
			   			   }


			   // if timer =0 then problem
			   if (Timer==0)
			   {
				   Local_ErrorState = USART_NOK;
			   }
			   else
			   {
				   *(Ptr_Request->Ptr_Buffer)=USART_Ptr-> DR  ;
				   Local_ErrorState = USART_OK;
			   }

			   USART_Ptr-> CR1 &=~ USART_RX_ENABLE_MASK;

			   RxReq[Ptr_Request->USART_Num].state= STATE_READY;

		   }
		   else
		   {

			   TxReq[Ptr_Request->USART_Num].state= STATE_BUSY;
		   }

	   }


		return Local_ErrorState;


}
USART_Error_t USART_SendBufferAsync(User_Request_t * Ptr_Request)
{
	USART_Error_t Local_ErrorState = USART_NOK;

	 if (Ptr_Request==NULL)
	   { // Error status as it is

	   }
	   else
	   {

		   Local_ErrorState = USART_OK;
	   }

	return Local_ErrorState;

}
USART_Error_t USART_ReceiveBufferAsync(User_Request_t * Ptr_Request)
{
	USART_Error_t Local_ErrorState = USART_NOK;

	 if (Ptr_Request==NULL)
	   { // Error status as it is

	   }
	   else
	   {

		   Local_ErrorState = USART_OK;
	   }

	return Local_ErrorState;

}



