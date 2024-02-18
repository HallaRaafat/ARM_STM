/*
 *
 *
 *
 *
 *
 */


/***************************************************************************/
/*                           Clock Options                                 */
/***************************************************************************/
#define RCC_CLOCK_HSI            0x00000001
#define RCC_CLOCK_HSE_BYPASS     0x00050000
#define RCC_CLOCK_HSE_CRYSTAL    0x00010000
#define RCC_CLOCK_PLL            0x01000000


/***************************************************************************/
/*                           PLL CONFIGURE                                 */
/***************************************************************************/


//#define RCC_PLL_INPUT             0x400000
#define RCC_PLL_INPUT_HSI           0xffbfffff         // & = to clear the bit
#define RCC_PLL_INPUT_HSE           0x00400000           // |= to set this bit



/***************************************************************************/
/*                           States                                        */
/***************************************************************************/
#define STATE_ENABLE     1
#define STATE_DISABLE    0

/***************************************************************************/
/*                           PLL Options                                   */
/***************************************************************************/


/***************************************************************************/
/*                           Prescaler Options                             */
/***************************************************************************/

#define	RCC_AHB_PRESC_2        0x00000080
#define	RCC_AHB_PRESC_4        0x00000090
#define	RCC_AHB_PRESC_8        0x000000a0
#define	RCC_AHB_PRESC_16       0x000000b0
#define	RCC_AHB_PRESC_64       0x000000c0
#define	RCC_AHB_PRESC_128      0x000000d0
#define	RCC_AHB_PRESC_256      0x000000e0
#define	RCC_AHB_PRESC_512      0x000000f0
#define RCC_AHB_NO_PRESC       0X00000000
#define RCC_AHB_CLEAR_PRESC    0xFFFFFF0F


#define	RCC_APB2_PRESC_2       0x00008000
#define	RCC_APB2_PRESC_4       0x0000A000
#define	RCC_APB2_PRESC_8       0x0000C000
#define	RCC_APB2_PRESC_16      0x0000E000
#define RCC_APB2_NO_PRESC      0X00000000
#define RCC_APB2_CLEAR_PRESC    0xFFFF1FFF

#define	RCC_APB1_PRESC_2       0x00001000
#define	RCC_APB1_PRESC_4       0x00001400
#define	RCC_APB1_PRESC_8       0x00001800
#define	RCC_APB1_PRESC_16      0x00001C00
#define RCC_APB1_NO_PRESC      0X00000000
#define RCC_APB1_CLEAR_PRESC    0xFFFFE3FF


// f0 mask AHB
// 1C00 mask apb1
// E000 mask apb2

/***************************************************************************/
/*                     Peripherals   and  on which bus                     */
/***************************************************************************/

#define		USB      0x00000080      //AHB2

#define		DMA2     0x00400000  //AHB1
#define		DMA1     0x00200000  //AHB1
#define		CRC      0x00001000    //AHB1
#define		GPIOH    0x00000080      //AHB1
#define		GPIOE    0x00000010  //AHB1
#define		GPIOD    0x00000008 //AHB1
#define		GPIOC    0x00000004   //AHB1
#define		GPIOB    0x00000002  //AHB1
#define		GPIOA    0x00000001   //AHB1


#define		TIM11    0x00040000//APB2
#define		TIM10    0x00020000//APB2
#define		TIM9     0x00010000//APB2
#define		SYSCFG   0x00004000 //APB2
#define		SPI4     0x00002000 //APB2
#define		SPI1     0x00001000 //APB2
#define		SDIO     0x00000800  //APB2
#define		ADC1     0x00000100  //APB2
#define		USART6   0x00000020   //APB2
#define		USART1   0x00000010   //APB2
#define		TIM1     0x00000001    //APB2


#define		PWR      0x10000000  //APB1
#define		I2C3     0x00800000 //APB1
#define		I2C2     0x00400000 //APB1
#define		I2C1     0x00200000 //APB1
#define		USART2   0x00020000  //APB1
#define		SPI3     0x00008000   //APB1
#define		SPI2     0x00004000   //APB1
#define		WWDG     0x00000800   //APB1
#define		TIM5     0x00000008    //APB1
#define		TIM4     0x00000004    //APB1
#define		TIM3     0x00000002    //APB1
#define		TIM2     0x00000001 //APB1


typedef  unsigned char       u8 ;
typedef  unsigned short int  u16 ;
typedef  unsigned int        uint32 ;


typedef enum {
	RCC_NOK,
	RCC_OK
}RCC_ErrorStatus_t;

typedef enum {
	RCC_AHB1,
	RCC_AHB2,
	RCC_APB1,
	RCC_APB2
}RCC_Bus_t;
/*
 * @brief: to apply prescaler on the bus clock
 * @input: PLL_Input ->
 * but first use RCC_ManageClock to enable the input of pll
 *
 *
 */


RCC_ErrorStatus_t	 RCC_ConfigurePLL(uint32 PLL_Input,uint32 PLLM ,uint32 PLLN,uint32 PLLP,uint32 PLLQ);
/*
 * @brief : Enable or disable clock
 * @input : Clock ->  RCC_Clock_HSI , RCC_Clock_HSE , RCC_Clock_PLL
 *          STATE ->  STATE_ENABLE  STATE_DISABLE
*/
RCC_ErrorStatus_t    RCC_ManageClock(uint32 Clock ,uint32 STATE ); //HSE ,HSI,PLL
/*
 * @brief: to select clock to the system
 * @input: Clock -> type (RCC_CLOCK_ )  and choose    (HSI , HSE , PLL)
 */

RCC_ErrorStatus_t	 RCC_SelectSystemClock(uint32 Clock);
/*
 * @brief: to Control clock to peripheral
 * @input: Bus        -> RCC_AHB1,RCC_AHB2,RCC_APB1, RCC_APB2  depends on the peripheral on which Bus
 *         Peripheral -> options from Peripheral defines
 *         State      -> STATE_ENABLE , STATE_DISABLE
 */
RCC_ErrorStatus_t	 RCC_ControlPeripheral(RCC_Bus_t Bus , uint32 Peripheral,uint32 STATE );
/*
 * @brief: to apply prescaler on the bus clock
 * @input: Bus           -> RCC_AHB1,RCC_APB1, RCC_APB2
 *
 *         Bus_Prescaler -> RCC_AHB_PRESC_ (see the options )
 *                          RCC_APB1_PRESC_
 *                           RCC_APB2_PRESC_
 *
 */
RCC_ErrorStatus_t	 RCC_ConfigurePrescalar(RCC_Bus_t  Bus , uint32 Bus_Prescaler);

















