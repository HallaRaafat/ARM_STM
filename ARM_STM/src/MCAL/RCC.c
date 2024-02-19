#include "MCAL/RCC.h"

#define RCC_BASE_ADDRESS 0x40023800

#define HSIRDY_MASK      0x00000002
#define HSERDY_MASK      0x00020000
#define PLLRDY_MASK      0x02000000

#define SW_SYSCLK_MASK   0x00000003
#define SW_HSE           0x00000001
#define SW_HSI           0x00000000
#define SW_PLL           0X00000002

#define SWS_SYSCLK_STATUS_MASK           0x0000000c
#define SWS_HSE                          0x00000004
#define SWS_HSI                          0x00000000
#define SWS_PLL                          0x00000008



#define PLL_MASK_PRES   0x0F037FFF
/*
#define PLL_PLLM        0x0000003F
#define PLL_PLLN        0x00007FC0
#define PLL_PLLP        0x00030000
#define PLL_PLLQ        0x0F000000
*/
#define PLL_SHIFT_PLLM        0
#define PLL_SHIFT_PLLN        6
#define PLL_SHIFT_PLLP        16
#define PLL_SHIFT_PLLQ        24

#define PLL_BOUNDARY1_PLLM        2
#define PLL_BOUNDARY2_PLLM        63

#define PLL_BOUNDARY1_PLLN       192
#define PLL_BOUNDARY2_PLLN       432

#define PLL_BOUNDARY1_PLLP       2
#define PLL_BOUNDARY2_PLLP       8

#define PLL_BOUNDARY1_PLLQ       2
#define PLL_BOUNDARY2_PLLQ       15
typedef struct {
	
	
	uint32  CR;
	uint32  PLLCFGR;
	uint32  CFGR;
	uint32  CIR;
	uint32  AHB1RSTR;
	uint32  AHB2RSTR;
	uint32  Reserved1;
	uint32  Reserved2;
	uint32  APB1RSTR;
	uint32  APB2RSTR;
	
	uint32  Reserved3;
	uint32  Reserved4;
	
	uint32  AHB1ENR;
	uint32  AHB2ENR;
	
	uint32  Reserved5;
	uint32  Reserved6;
	
	uint32  APB1ENR;
	uint32  APB2ENR;
	uint32  Reserved7;
	uint32  Reserved8;
	
	uint32  AHB1LPENR;
	uint32  AHB2LPENR;
	uint32  Reserved9;
	uint32  Reserved10;

	uint32  APB1LPENR;
	uint32  APB2LPENR;
	uint32  Reserved11;
	uint32  Reserved12;
	
	uint32  BDCR;
	uint32  CSR;
	uint32  Reserved13;
	uint32  Reserved14;	

	uint32  SSCGR;
	uint32  PLLI2SCFGR;
	uint32  Reserved15;
	uint32  DCKCFGR;	
	
	
	
}RCC_t;

//allocate the size of the pointer only depends on bus not the struct
// POINTEE (pointing to volatile (bring from memory dont optimaize)) * constant POINTER (RCC)
volatile RCC_t * const RCC = (volatile RCC_t * const) RCC_BASE_ADDRESS;


/*
 * HSE ext source must be provided
 * set HSEBYP ,HSEON IN CR FOR RC
 * for Crystal more accurate
 */


  /* HSI internal 16 MHz directly as
   *  system clock or PLL input
   * HSI RC using HSION bit
   * HSIRDY flag in CR if it is stable
   * HSICAL ?
   * HSITRIM?
   *
   */

/*
 * PLL configuration
 * both same (PLLM PLLSRC )
 * main PLL clocked by HSI or HSE
 * 1st output -> high speed sys clocks 84 mhz
 * 2nd output USB 48 MHz
 *
 * dedicated PLL PLLI2S high quality audio
 *
 * cannot change config of PLL once enabled
 * 1 select HSI or HSE
 * 2 config  (DEVISION )M P Q  (MULTI) N
 *
 *
 *
 */

/*
 * switch from clock source only if the target is ready or PLL is locked
 * CR which is ready and which is used as sysclk
 *
 *
 */
RCC_ErrorStatus_t    RCC_ManageClock(uint32 Clock ,uint32 STATE )
{
	RCC_ErrorStatus_t Local_ErrorState= RCC_NOK;


				switch(STATE)
				{

							case STATE_ENABLE:

											   switch (Clock)
											   {// set the bit in the CR register wait if isnt not ready
											   //do timeout
											   case RCC_CLOCK_HSI:         RCC ->CR |= Clock; while(!(RCC->CR & HSIRDY_MASK)){}; Local_ErrorState= RCC_OK; break;

											   case RCC_CLOCK_HSE_BYPASS:  RCC ->CR |= Clock; while(!(RCC->CR & HSERDY_MASK)){}; Local_ErrorState= RCC_OK; break;
											   case RCC_CLOCK_HSE_CRYSTAL: RCC ->CR |= Clock; while(!(RCC->CR & HSERDY_MASK)){}; Local_ErrorState= RCC_OK; break;
											   case RCC_CLOCK_PLL:         RCC ->CR |= Clock; while(!(RCC->CR & PLLRDY_MASK)){}; Local_ErrorState= RCC_OK; break;

											   default :
														 Local_ErrorState= RCC_NOK;break;
											   }


												 break;

							case STATE_DISABLE:
											   RCC ->CR &= ~Clock;

											   switch (Clock)
												   {
												   case RCC_CLOCK_HSI:          while((RCC->CR & HSIRDY_MASK)){}; Local_ErrorState= RCC_OK; break;

												   case RCC_CLOCK_HSE_BYPASS:   while((RCC->CR & HSERDY_MASK)){}; Local_ErrorState= RCC_OK; break;
												   case RCC_CLOCK_HSE_CRYSTAL:  while((RCC->CR & HSERDY_MASK)){}; Local_ErrorState= RCC_OK; break;
												   case RCC_CLOCK_PLL:          while((RCC->CR & PLLRDY_MASK)){}; Local_ErrorState= RCC_OK; break;

												   default :
															 Local_ErrorState= RCC_NOK;break;
												   }


											   break;

							default :
								Local_ErrorState= RCC_NOK; break;
				}
	return Local_ErrorState;

}



/*
 * CR->  The on and ready bits in  swa2 bypass ll HSE w 8ero
 *
 * PLLCFGR -> Q for USB not for now
 *          bit 22 PLLSRC for 0 :HSI , 1 : HSE
 *
 *          PLLP p factor -> devison must pll disabled first
 *          output = VCO /PLLP = 2, 4, 6, 8
 *          bit 17:16            00 01 10 11
 *
 *          PLLN miltip factor to be between 192 and 432 bits from (6:14)
 *
 *          PLLM between 2 and 63
 */


RCC_ErrorStatus_t	 RCC_ConfigurePLL(uint32 PLL_Input,uint32 PLLM ,uint32 PLLN,uint32 PLLP,uint32 PLLQ)
{
	RCC_ErrorStatus_t Local_ErrorState= RCC_NOK;
	uint32 Local_Temp = RCC->PLLCFGR;

	if (PLL_Input== RCC_PLL_INPUT_HSI)
		// check if the clock is enabled 1st
		{
		if(RCC->CR & HSIRDY_MASK)
		{  // clear bit 22 to hsi inpit
			Local_Temp &= RCC_PLL_INPUT_HSI;
		    Local_ErrorState= RCC_OK;
		}
		else
		{
			Local_ErrorState= RCC_NOK;
		}
		}
	else if (PLL_Input== RCC_PLL_INPUT_HSE)
	{   if (RCC->CR & HSERDY_MASK)

        	{ // set bit 22 for HSE input
		       Local_Temp |= RCC_PLL_INPUT_HSE;
		       Local_ErrorState= RCC_OK;
        	}
		else
		{
			Local_ErrorState= RCC_NOK;

		}
	}
	else
		Local_ErrorState= RCC_NOK;

// Prescaler configuration

//first clear the bits of prescalar

Local_Temp &= ~PLL_MASK_PRES;

// then set the values
if ((PLLM <= PLL_BOUNDARY2_PLLM && PLLM >= PLL_BOUNDARY1_PLLM ) &&
	(PLLN <= PLL_BOUNDARY2_PLLN && PLLN >= PLL_BOUNDARY1_PLLN ) &&
	(PLLP <= PLL_BOUNDARY2_PLLP && PLLP >= PLL_BOUNDARY1_PLLP ) &&
	(PLLQ <= PLL_BOUNDARY2_PLLQ && PLLN >= PLL_BOUNDARY1_PLLQ ))
{   PLLP= (uint32)(PLLP/2)-1;

     Local_Temp |= ((PLLM << PLL_SHIFT_PLLM )|(PLLN<< PLL_SHIFT_PLLN)|(PLLP << PLL_SHIFT_PLLP)|(PLLQ << PLL_SHIFT_PLLQ));
     RCC->PLLCFGR= Local_Temp;
	 Local_ErrorState= RCC_OK;
}
else
{
	Local_ErrorState= RCC_NOK;
}



	return Local_ErrorState;
}














/*
 *
 * CFGR -> 15:13 PPRE2 APB2 prescaler
 *               APB1 prescalar
 *
 */




 /*
 *         bit 3:2 SWS system clock switch status by HW
 *             00 HSI , 01 HSE ,10 PLL
 *         bit 1:0 SW system cloch switch
 *              00 HSI , 01 HSE , 10 PLL
 *
 *
 */

RCC_ErrorStatus_t	 RCC_SelectSystemClock(uint32 Clock)
{
	RCC_ErrorStatus_t Local_ErrorState= RCC_NOK;
	 uint32 Local_Temp  =  RCC->CFGR;
            Local_Temp &= ~SW_SYSCLK_MASK;
	 switch (Clock)
	   {
	   case RCC_CLOCK_HSI:         //Local_Temp|= SW_HSI;
	                               RCC->CFGR =Local_Temp;
								   while((RCC->CFGR & SWS_SYSCLK_STATUS_MASK)!= SWS_HSI){};
							   	   Local_ErrorState= RCC_OK; break;

	   case RCC_CLOCK_HSE_BYPASS:
	   case RCC_CLOCK_HSE_CRYSTAL:
		                           Local_Temp|= SW_HSE;
                                   RCC->CFGR =Local_Temp;
								   while((RCC->CFGR & SWS_SYSCLK_STATUS_MASK)!= SWS_HSE){};
								   Local_ErrorState= RCC_OK; break;

	   case RCC_CLOCK_PLL:
								   Local_Temp|= SW_PLL;
								   RCC->CFGR =Local_Temp;
								   while((RCC->CFGR & SWS_SYSCLK_STATUS_MASK)!= SWS_PLL){};
								   Local_ErrorState= RCC_OK; break;


	   default :
				 Local_ErrorState= RCC_NOK;break;
	   }



	return Local_ErrorState;
}





/*
 * Buses with peripherals
 *  AHB2 -> USB
 *
 *
 * AHB1 -> DMA2 DMA1 Flash RCC
 *  CRC GPIOH-E-D-C-B-A
 *
 *  APB2 -> TIM11 TIM10 TIM9
 *  EXTI SYSCFG SPI4 SPI1
 *  SDIO ADC1 USART6 USART1
 *   TIM1
 *
 *  APB1-> PWR I2C3 I2C2 I2C1 USART2
 *  I2S3ext  SPI3/I2S3  SPI2/12S2 I2S2ext
 *  IWDG WWDG RTC
 *  TIM5 TIM4 TIM3 TIM2
 */
RCC_ErrorStatus_t	 RCC_ControlPeripheral(RCC_Bus_t Bus , uint32 Peripheral,uint32 STATE )
{
RCC_ErrorStatus_t Local_ErrorState= RCC_NOK;
switch (STATE)
{
case STATE_ENABLE:
                    switch(Bus)
						{
                              case RCC_AHB1 :
                            	                RCC ->AHB1ENR |= Peripheral;Local_ErrorState= RCC_OK;break;
                              case RCC_AHB2 :
												RCC ->AHB2ENR |= Peripheral;Local_ErrorState= RCC_OK;break;
                              case RCC_APB1 :
												RCC ->APB1ENR |= Peripheral;Local_ErrorState= RCC_OK;break;
                              case RCC_APB2 :
												RCC ->APB2ENR |= Peripheral;Local_ErrorState= RCC_OK;break;

							 default :
									Local_ErrorState= RCC_NOK;
									break;
						}
	                 break;
case STATE_DISABLE:
	                switch(Bus)
	                    {

							case RCC_AHB1 :
											    RCC ->AHB1ENR &= ~Peripheral;Local_ErrorState= RCC_OK;break;
							case RCC_AHB2 :
												RCC ->AHB2ENR &= ~Peripheral;Local_ErrorState= RCC_OK;break;
							case RCC_APB1 :
												RCC ->APB1ENR &= ~Peripheral;Local_ErrorState= RCC_OK;break;
							case RCC_APB2 :
												RCC ->APB2ENR &= ~Peripheral;Local_ErrorState= RCC_OK;break;


							default :
								Local_ErrorState= RCC_NOK;
								break;
	                    }
	                 break;
default :
	Local_ErrorState= RCC_NOK;
	break;
}
return Local_ErrorState;
}
/*
 * AHB pescalar / 1,2,... 512
 *
 * Bits 7:4 HPRE: (RCC_CFGR)
 * 0x xx: system clock not divided
 * 1000: system clock divided by 2    8
 * 1001: system clock divided by 4    9
 *  1010: system clock divided by 8   a
 *  1011: system clock divided by 16  b
 * 100: system clock divided by 64    c
 * 1101: system clock divided by 128  d
 * 1110: system clock divided by 256  e
 * 1111: system clock divided by 512  f
 *
 * /8 to cortex
 *
 * APB /1,2,4,8,16
 * if apb pres=1 =1*1
 * else *2
 *
 * Bits 15:13 PPRE2:
 *  0xx: AHB clock not divided   3 2 1 0
 *	100: AHB clock divided by 2  4
 *	101: AHB clock divided by 4  5
 *	110: AHB clock divided by 8  6
 *  111: AHB clock divided by 16 7
 *
 *	Bits 12:10 PPRE1: 12 11 10
 *	0xx: AHB clock not divided
 *	100: AHB clock divided by 2 4
 *	101: AHB clock divided by 4 5
 *	110: AHB clock divided by 8 6
 *	111: AHB clock divided by 16 7
 */
RCC_ErrorStatus_t	 RCC_ConfigurePrescalar(RCC_Bus_t  Bus , uint32 Bus_Prescaler)
{
	RCC_ErrorStatus_t Local_ErrorState= RCC_NOK;
    uint32 Local_Temp = RCC ->CFGR;



    if (
    		Bus_Prescaler == RCC_AHB_PRESC_2  ||
			Bus_Prescaler == RCC_AHB_PRESC_4  ||
			Bus_Prescaler == RCC_AHB_PRESC_8  ||
			Bus_Prescaler == RCC_AHB_PRESC_16 ||
			Bus_Prescaler == RCC_AHB_PRESC_64 ||
			Bus_Prescaler == RCC_AHB_PRESC_128||
			Bus_Prescaler == RCC_AHB_PRESC_256 ||
    		Bus_Prescaler == RCC_AHB_PRESC_512 ||
			Bus_Prescaler == RCC_AHB_NO_PRESC  ||

    		Bus_Prescaler == RCC_APB2_PRESC_2 ||
    		Bus_Prescaler == RCC_APB2_PRESC_4 ||
			Bus_Prescaler == RCC_APB2_PRESC_8 ||
			Bus_Prescaler == RCC_APB2_PRESC_16 ||
			Bus_Prescaler ==  RCC_APB2_NO_PRESC ||

			Bus_Prescaler == RCC_APB1_PRESC_2 ||
			Bus_Prescaler == RCC_APB1_PRESC_4 ||
			Bus_Prescaler == RCC_APB1_PRESC_8 ||
			Bus_Prescaler == RCC_APB1_PRESC_16 ||
			Bus_Prescaler ==  RCC_APB1_NO_PRESC   )

       {
    	switch (Bus)
			{
			case RCC_AHB1 :   Local_Temp &= RCC_AHB_CLEAR_PRESC; // CLEAR BITS OF AHB PRESCALER in local variable
							  Local_ErrorState= RCC_OK;
							 // Local_Temp |= Bus_Prescaler;

								break;
			case RCC_APB1 :   Local_Temp &= RCC_APB1_CLEAR_PRESC;
							  Local_ErrorState= RCC_OK;
								break;
			case RCC_APB2 :
							  Local_Temp &= RCC_APB2_CLEAR_PRESC;
							  Local_ErrorState= RCC_OK;
								break;
			default :
											Local_ErrorState= RCC_NOK;
											break;
			}

			Local_Temp |= Bus_Prescaler;
			RCC->CFGR = Local_Temp;
    }
	else
		Local_ErrorState= RCC_NOK;



	return Local_ErrorState;
}
