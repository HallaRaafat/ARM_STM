/*
 * USART.c
 *
 *  Created on: Apr 13, 2024
 *      Author: Halla Raafat
 */

#include "MCAL/USART.h"
#include "MCAL/USART_Cfg.h"
#include "LIB/STD_Types.h"

#define USART1_G_IDX     0
#define USART2_G_IDX     1
#define USART6_G_IDX     2

#define USART_OVS_MASK    0x00008000
#define USART_ENABLE_MASK 0X00002000 //Bit 13
#define STATE_READY       0X00000000
#define STATE_BUSY        0X00000001

#define USART_TX_ENABLE_MASK  0X00000008 //CR1
#define USART_TXE_FLAG_MASK 0X00000080  //7 SR

#define USART_RX_ENABLE_MASK  0X00000004 //CR1
#define USART_RXNE_FLAG_MASK  0X00000020 //5 SR

#define USART_RXNEIE_ENABLE_MASK       0X00000020   //CR1
#define USART_TXIE_ENABLE_MASK         0X00000080   //CR1

#define USART_TARGET_NUM    3 // 1,2,6
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
    uint8  state;
    CB_t   CB;
} USART_Req_t;


extern const  USART_Cfg_t USART_ARR[_USART_Num] ;
static USART_Req_t TxReq[_USART_Num]={};
static USART_Req_t RxReq[_USART_Num]={};
static uint8 G_USART_IDX[USART_TARGET_NUM]={0};



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


   	Loc_CR1Value= USART_ENABLE_MASK | USART_ARR[Idx].WordLength | USART_ARR[Idx].ParityControl| USART_ARR[Idx].ParityType;
    //After CR1
      	if (USART_ARR[Idx].OverSamplingMode== USART_OVS_8)
      	  {
      		  Loc_CR1Value|= USART_OVS_MASK;
      	  }
   	Loc_CR2Value= USART_ARR[Idx].StopBits;

	//assign local variables in main register

	((volatile USART_t * ) (USART_ARR[Idx].USART_ID ))-> BRR = (Loc_DIV_Mantissa << 4) | Loc_DIV_Fraction;

	((volatile USART_t * ) (USART_ARR[Idx].USART_ID ))-> CR1 = Loc_CR1Value;
	((volatile USART_t * ) (USART_ARR[Idx].USART_ID ))-> CR2 = Loc_CR2Value;


	if (USART_ARR[Idx].USART_ID==USART1)
	{
		G_USART_IDX[USART1_G_IDX ]=Idx;
	}
	else if (USART_ARR[Idx].USART_ID==USART2)
	{
		G_USART_IDX[USART2_G_IDX]=Idx;
	}
	else if (USART_ARR[Idx].USART_ID==USART6)
	{
		G_USART_IDX[USART6_G_IDX]=Idx;
	}
	/*
	switch(USART_ARR[Idx].USART_ID )
				{

				case USART1:
					G_USART_IDX[USART1_G_IDX ]=Idx;
					break;
				case USART2:
					G_USART_IDX[USART2_G_IDX]=Idx;
					break;
				case USART6:
					G_USART_IDX[USART6_G_IDX]=Idx;
					break;
				default :
					break ;

				}




*/


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

		   USART_Ptr-> CR1 |= USART_TX_ENABLE_MASK;

		   USART_Ptr-> DR  =*(Ptr_Request->Ptr_Buffer);


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
		   //  USART_Ptr-> CR1 &=~ USART_TX_ENABLE_MASK;// TEST IT
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

			   while( ((USART_Ptr->SR & USART_RXNE_FLAG_MASK)== 0 ) && Timer)
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

			   RxReq[Ptr_Request->USART_Num].state= STATE_BUSY;
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



		   volatile USART_t *USART_Ptr = (volatile USART_t *)(USART_ARR[Ptr_Request->USART_Num].USART_ID);


		  	   if ( TxReq[Ptr_Request->USART_Num].state== STATE_READY)
		  	   {



		  		   TxReq[Ptr_Request->USART_Num].state=STATE_BUSY;
		  	if(Ptr_Request->CallBack !=NULL) { TxReq[Ptr_Request->USART_Num].CB= Ptr_Request->CallBack;}
				 TxReq[Ptr_Request->USART_Num].Pos=0;
				 TxReq[Ptr_Request->USART_Num].Size=Ptr_Request->Length;
				 TxReq[Ptr_Request->USART_Num].data=Ptr_Request->Ptr_Buffer;
		  		   USART_Ptr-> CR1 |= USART_TX_ENABLE_MASK;
                   USART_Ptr-> CR1 |= USART_TXIE_ENABLE_MASK;
		  		   USART_Ptr-> DR  =(TxReq[Ptr_Request->USART_Num].data[0]);


		  	   }
		  	   else
		  	   {

		  		   TxReq[Ptr_Request->USART_Num].state= STATE_BUSY;
		  	   }
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


		   volatile USART_t *USART_Ptr = (volatile USART_t *)(USART_ARR[Ptr_Request->USART_Num].USART_ID);




				   if ( RxReq[Ptr_Request->USART_Num].state== STATE_READY)
				   {


					   RxReq[Ptr_Request->USART_Num].state=STATE_BUSY;
					   if(Ptr_Request->CallBack !=NULL) { RxReq[Ptr_Request->USART_Num].CB= Ptr_Request->CallBack;}
						 RxReq[Ptr_Request->USART_Num].Pos=0;
						 RxReq[Ptr_Request->USART_Num].Size=Ptr_Request->Length;
						 RxReq[Ptr_Request->USART_Num].data=Ptr_Request->Ptr_Buffer;
                         //enable masks
                         USART_Ptr-> CR1 |= USART_RX_ENABLE_MASK;
                         USART_Ptr-> CR1 |= USART_RXNEIE_ENABLE_MASK;

                         //recieve 1st byte to generate interuupt

                         RxReq[Ptr_Request->USART_Num].data[0]=(uint8)USART_Ptr-> DR  ;


						   Local_ErrorState = USART_OK;




				   }
				   else
				   {

					   RxReq[Ptr_Request->USART_Num].state= STATE_BUSY;
				   }
		   Local_ErrorState = USART_OK;
	   }

	return Local_ErrorState;

}


void USART1_IRQHandler(void)
{
//IF state is busy then check on the buffer and the txe flag

	 uint8 REQ_IDX= G_USART_IDX[USART1_G_IDX ]; //INDEX in request array

	// if (TxReq[REQ_IDX])
	   volatile USART_t *USART_Ptr = (volatile USART_t *)(USART1);
	   // which is the reason of interuupt tx or rxne?

	   //TX Flag
	   if (USART_Ptr->SR & USART_TXE_FLAG_MASK)
		{
		   if(TxReq[REQ_IDX].state== STATE_BUSY)
		   {
			   if (TxReq[REQ_IDX].Pos< TxReq[REQ_IDX].Size)
			   {
				   //send current pos
				   USART_Ptr-> DR  =(TxReq[REQ_IDX].data[TxReq[REQ_IDX].Pos]);
				   //increment pos
				   TxReq[REQ_IDX].Pos++;
			   }
			   else
			   {

				   //disable inturrpt
				   USART_Ptr->CR1&=~ USART_TXIE_ENABLE_MASK;
				   // TXreq ready
				   TxReq[REQ_IDX].state=STATE_READY;
				   //call callback func
					if(TxReq[REQ_IDX].CB!=NULL)
					{
						TxReq[REQ_IDX].CB();
					}

			   }

		   }
		   else
		   {

		   }

		}




	   //RX Flag
	 	   if (USART_Ptr->SR & USART_RXNE_FLAG_MASK)
	 		{
	 		   if(RxReq[REQ_IDX].state== STATE_BUSY)
	 		   {
	 			   if (RxReq[REQ_IDX].Pos< RxReq[REQ_IDX].Size)
	 			   {
	 				   //send current pos
	 				 RxReq[REQ_IDX].data[RxReq[REQ_IDX].Pos]=(uint8)USART_Ptr-> DR  ;
	 				   //increment pos
	 				   RxReq[REQ_IDX].Pos++;
	 			   }
	 			   else
	 			   {

	 				   //disable inturrpt
	 				   USART_Ptr->CR1&=~ USART_RXNEIE_ENABLE_MASK;
	 				   // TXreq ready
	 				   RxReq[REQ_IDX].state=STATE_READY;
	 				   //call callback func
	 					if(RxReq[REQ_IDX].CB!=NULL)
	 					{
	 						RxReq[REQ_IDX].CB();
	 					}

	 			   }

	 		   }
	 		   else
	 		   {

	 		   }

	 		}



}


















void USART2_IRQHandler(void)
{
	uint8 REQ_IDX= G_USART_IDX[USART2_G_IDX ];



		// if (TxReq[REQ_IDX])
		   volatile USART_t *USART_Ptr = (volatile USART_t *)(USART2);
		   // which is the reason of interuupt tx or rxne?

		   //TX Flag
		   if (USART_Ptr->SR & USART_TXE_FLAG_MASK)
			{
			   if(TxReq[REQ_IDX].state== STATE_BUSY)
			   {
				   if (TxReq[REQ_IDX].Pos< TxReq[REQ_IDX].Size)
				   {
					   //send current pos
					   USART_Ptr-> DR  =(TxReq[REQ_IDX].data[TxReq[REQ_IDX].Pos]);
					   //increment pos
					   TxReq[REQ_IDX].Pos++;
				   }
				   else
				   {

					   //disable inturrpt
					   USART_Ptr->CR1&=~ USART_TXIE_ENABLE_MASK;
					   // TXreq ready
					   TxReq[REQ_IDX].state=STATE_READY;
					   //call callback func
						if(TxReq[REQ_IDX].CB!=NULL)
						{
							TxReq[REQ_IDX].CB();
						}

				   }

			   }
			   else
			   {

			   }

			}




		   //RX Flag
		 	   if (USART_Ptr->SR & USART_RXNE_FLAG_MASK)
		 		{
		 		   if(RxReq[REQ_IDX].state== STATE_BUSY)
		 		   {
		 			   if (RxReq[REQ_IDX].Pos< RxReq[REQ_IDX].Size)
		 			   {
		 				   //send current pos
		 				 RxReq[REQ_IDX].data[RxReq[REQ_IDX].Pos]=(uint8)USART_Ptr-> DR  ;
		 				   //increment pos
		 				   RxReq[REQ_IDX].Pos++;
		 			   }
		 			   else
		 			   {

		 				   //disable inturrpt
		 				   USART_Ptr->CR1&=~ USART_RXNEIE_ENABLE_MASK;
		 				   // TXreq ready
		 				   RxReq[REQ_IDX].state=STATE_READY;
		 				   //call callback func
		 					if(RxReq[REQ_IDX].CB!=NULL)
		 					{
		 						RxReq[REQ_IDX].CB();
		 					}

		 			   }

		 		   }
		 		   else
		 		   {

		 		   }

		 		}


}

void USART6_IRQHandler(void)
{
	 uint8 REQ_IDX= G_USART_IDX[USART6_G_IDX ];
	   volatile USART_t *USART_Ptr = (volatile USART_t *)(USART6);

		   // which is the reason of interuupt tx or rxne?

		   //TX Flag
		   if (USART_Ptr->SR & USART_TXE_FLAG_MASK)
			{
			   if(TxReq[REQ_IDX].state== STATE_BUSY)
			   {
				   if (TxReq[REQ_IDX].Pos< TxReq[REQ_IDX].Size)
				   {
					   //send current pos
					   USART_Ptr-> DR  =(TxReq[REQ_IDX].data[TxReq[REQ_IDX].Pos]);
					   //increment pos
					   TxReq[REQ_IDX].Pos++;
				   }
				   else
				   {

					   //disable inturrpt
					   USART_Ptr->CR1&=~ USART_TXIE_ENABLE_MASK;
					   // TXreq ready
					   TxReq[REQ_IDX].state=STATE_READY;
					   //call callback func
						if(TxReq[REQ_IDX].CB!=NULL)
						{
							TxReq[REQ_IDX].CB();
						}

				   }

			   }
			   else
			   {

			   }

			}




		   //RX Flag
		 	   if (USART_Ptr->SR & USART_RXNE_FLAG_MASK)
		 		{
		 		   if(RxReq[REQ_IDX].state== STATE_BUSY)
		 		   {
		 			   if (RxReq[REQ_IDX].Pos< RxReq[REQ_IDX].Size)
		 			   {
		 				   //send current pos
		 				 RxReq[REQ_IDX].data[RxReq[REQ_IDX].Pos]=(uint8)USART_Ptr-> DR  ;
		 				   //increment pos
		 				   RxReq[REQ_IDX].Pos++;
		 			   }
		 			   else
		 			   {

		 				   //disable inturrpt
		 				   USART_Ptr->CR1&=~ USART_RXNEIE_ENABLE_MASK;
		 				   // TXreq ready
		 				   RxReq[REQ_IDX].state=STATE_READY;
		 				   //call callback func
		 					if(RxReq[REQ_IDX].CB!=NULL)
		 					{
		 						RxReq[REQ_IDX].CB();
		 					}

		 			   }

		 		   }
		 		   else
		 		   {

		 		   }

		 		}


}

/*
 *    //   G_USART_IDX[] // 0 -usart1,1 usart-2,2 usart-6
		  				switch(USART_Ptr)
		  					{

		  					case USART1:
		  						G_USART_IDX[USART1_G_IDX ]=Ptr_Request->USART_Num;
		  						break;
		  					case USART2:
		  						G_USART_IDX[USART2_G_IDX]=Ptr_Request->USART_Num;
		  						break;
		  					case USART6:
		  						G_USART_IDX[USART6_G_IDX]=Ptr_Request->USART_Num;
		  						break;
		  					default :
		  						break ;

		  					}
		  					*/

