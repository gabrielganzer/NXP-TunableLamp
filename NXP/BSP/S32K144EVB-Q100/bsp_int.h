/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
* Filename : bsp_int.h
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_INT_PRESENT
#define  BSP_INT_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/

#ifdef __cplusplus
extern  "C" {                                                   /* See Note #1.                                         */
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*
* Note(s) : (1) The Cortex-M "Vector Table Offset Register" section states the following:
*
*               You must align the offset to the number of exception entries in the vector table. The
*               minimum alignment is 32 words, enough for up to 16 interrupts. For more interrupts,
*               adjust the alignment by rounding up to the next power of two. For example, if you require
*               21 interrupts, the alignment must be on a 64-word boundary because the required table
*               size is 37 words, and the next power of two is 64. See your vendor documentation for the
*               alignment details for your device.
*********************************************************************************************************
*/

#define  ARMV7M_CORE_EXCS             16u

#define  INT_ID_MAX_NBR              175u                       /* Maximum number of external interrupt sources.        */

                                                                /* 191 VTOR entries; next power of 2 is 256             */
#define  INT_VTOR_TBL_SIZE         (INT_ID_MAX_NBR + ARMV7M_CORE_EXCS)
#define  INT_VTOR_TBL_ALIGNMENT    (0x400uL)                    /* 256 * 4 = 1024 words (see note 1)                    */


/*
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*/

                                                    /* External Interrupt IDs.                                          */
typedef  enum  bsp_int_id {
	INT_ID_DMA0                 =   0u,             /* DMA channel 0 transfer complete                                  */
	INT_ID_DMA1                 =   1u,             /* DMA channel 1 transfer complete                                  */
	INT_ID_DMA2                 =   2u,             /* DMA channel 2 transfer complete                                  */
	INT_ID_DMA3                 =   3u,             /* DMA channel 3 transfer complete                                  */
	INT_ID_DMA4                 =   4u,             /* DMA channel 4 transfer complete                                  */
	INT_ID_DMA5                 =   5u,             /* DMA channel 5 transfer complete                                  */
	INT_ID_DMA6                 =   6u,             /* DMA channel 6 transfer complete                                  */
	INT_ID_DMA7                 =   7u,             /* DMA channel 7 transfer complete                                  */
	INT_ID_DMA8                 =   8u,             /* DMA channel 8 transfer complete                                  */
	INT_ID_DMA9                 =   9u,             /* DMA channel 9 transfer complete                                  */
	INT_ID_DMA10                =  10u,             /* DMA channel 10 transfer complete                                 */
	INT_ID_DMA11                =  11u,             /* DMA channel 11 transfer complete                                 */
	INT_ID_DMA12                =  12u,             /* DMA channel 12 transfer complete                                 */
	INT_ID_DMA13                =  13u,             /* DMA channel 13 transfer complete                                 */
	INT_ID_DMA14                =  14u,             /* DMA channel 14 transfer complete                                 */
	INT_ID_DMA15                =  15u,             /* DMA channel 15 transfer complete                                 */
	INT_ID_DMA_ERROR            =  16u,             /* DMA error interrupt channels 0-15                                */
	INT_ID_MCM                  =  17u,             /* FPU sources                                                      */
	INT_ID_FTFC                 =  18u,             /* FTFC command complete                                            */
	INT_ID_READ_COLLISION       =  19u,             /* FTFC read collision                                              */
	INT_ID_LVD_LVW              =  20u,             /* PMC low voltage detect interrupt                                 */
	INT_ID_FTFC_FAULT           =  21u,             /* FTFC double bit fault detect                                     */
	INT_ID_WDOG                 =  22u,             /* Watchdog timer interrupt                                         */
	INT_ID_EWM                  =  23u,             /* EWM interrupt                                                    */
	INT_ID_RCM                  =  24u,             /* RCM asynchronous interrupt                                       */
	INT_ID_LPI2C0_MASTER        =  25u,             /* LPI2C0 master interrupt                                          */
	INT_ID_LPI2C0_SLAVE         =  26u,             /* LPI2C0 slave interrupt                                           */
	INT_ID_LPSPI0               =  27u,             /* LPSPI0 interrupt                                                 */
	INT_ID_LPSPI1               =  28u,             /* LPSPI1 interrupt                                                 */
	INT_ID_LPSPI2               =  29u,             /* LPSPI2 interrupt                                                 */
	INT_ID_RESERVED30           =  30u,             /* Reserved interrupt 30                                            */
	INT_ID_RESERVED31           =  31u,             /* Reserved interrupt 31                                            */
	INT_ID_LPUART0_TX           =  32u,             /* LPUART0 transmit interrupt                                       */
	INT_ID_LPUART0_RX           =  33u,             /* LPUART0 receive interrupt                                        */
	INT_ID_LPUART0_ERR          =  34u,             /* LPUART0 error interrupt                                          */
	INT_ID_LPUART0_OVERRUN      =  35u,             /* LPUART0 overrun interrupt                                        */
	INT_ID_RESERVED36           =  36u,             /* Reserved interrupt 36                                            */
	INT_ID_LPUART1_TX           =  37u,             /* LPUART1 transmit interrupt                                       */
	INT_ID_LPUART1_RX           =  38u,             /* LPUART1 receive interrupt                                        */
	INT_ID_LPUART1_ERR          =  39u,             /* LPUART1 error interrupt                                          */
	INT_ID_LPUART1_OVERRUN      =  40u,             /* LPUART1 overrun interrupt                                        */
	INT_ID_RESERVED41           =  41u,             /* Reserved interrupt 41                                            */
	INT_ID_LPUART2_TX           =  42u,             /* LPUART2 transmit interrupt                                       */
	INT_ID_LPUART2_RX           =  43u,             /* LPUART2 receive interrupt                                        */
	INT_ID_LPUART2_ERR          =  44u,             /* LPUART2 error interrupt                                          */
	INT_ID_LPUART2_OVERRUN      =  45u,             /* LPUART2 overrun interrupt                                        */
	INT_ID_RESERVED46           =  46u,             /* Reserved interrupt 46                                            */
	INT_ID_RESERVED47           =  47u,             /* Reserved interrupt 47                                            */
	INT_ID_RESERVED48           =  48u,             /* Reserved interrupt 48                                            */
	INT_ID_RESERVED49           =  49u,             /* Reserved interrupt 49                                            */
	INT_ID_ADC0                 =  50u,             /* ADC0 interrupt request                                           */
	INT_ID_ADC1                 =  51u,             /* ADC1 interrupt request                                           */
	INT_ID_CMP0                 =  52u,             /* CMP0 interrupt request                                           */
	INT_ID_RESERVED53           =  53u,             /* Reserved interrupt 53                                            */
	INT_ID_RESERVED54           =  54u,             /* Reserved interrupt 54                                            */
	INT_ID_ERM_SINGLE_FAULT     =  55u,             /* ERM single bit error correction                                  */
	INT_ID_ERM_DOUBLE_FAULT     =  56u,             /* ERM double bit error non-correctable                             */
	INT_ID_RTC                  =  57u,             /* RTC alarm interrupt                                              */
	INT_ID_RTC_SECONDS          =  58u,             /* RTC seconds interrupt                                            */
	INT_ID_LPIT0_CH0            =  59u,             /* LPIT0 channel 0 overflow interrupt                               */
	INT_ID_LPIT0_CH1            =  60u,             /* LPIT0 channel 1 overflow interrupt                               */
	INT_ID_LPIT0_CH2            =  61u,             /* LPIT0 channel 2 overflow interrupt                               */
	INT_ID_LPIT0_CH3            =  62u,             /* LPIT0 channel 3 overflow interrupt                               */
	INT_ID_PDB0                 =  63u,             /* PDB0 interrupt                                                   */
	INT_ID_RESERVED64           =  64u,             /* Reserved interrupt 64                                            */
	INT_ID_RESERVED65           =  65u,             /* Reserved interrupt 65                                            */
	INT_ID_RESERVED66           =  66u,             /* Reserved interrupt 66                                            */
	INT_ID_RESERVED67           =  67u,             /* Reserved interrupt 67                                            */
	INT_ID_SCG                  =  68u,             /* SCG bus interrupt request                                        */
	INT_ID_LPTMR0               =  69u,             /* LPTIMER interrupt request                                        */
	INT_ID_PORTA                =  70u,             /* PORT A pin detect interrupt                                      */
	INT_ID_PORTB                =  71u,             /* PORT B pin detect interrupt                                      */
	INT_ID_PORTC                =  72u,             /* PORT C pin detect interrupt                                      */
	INT_ID_PORTD                =  73u,             /* PORT D pin detect interrupt                                      */
	INT_ID_PORTE                =  74u,             /* PORT E pin detect interrupt                                      */
	INT_ID_SWI                  =  75u,             /* Software interrupt                                               */
	INT_ID_RESERVED76           =  76u,             /* Reserved interrupt 76                                            */
	INT_ID_RESERVED77           =  77u,             /* Reserved interrupt 77                                            */
	INT_ID_RESERVED78           =  78u,             /* Reserved interrupt 78                                            */
	INT_ID_PDB1                 =  79u,             /* PDB1 interrupt                                                   */
	INT_ID_FLEXIO               =  80u,             /* FLEXIO interrupt                                                 */
	INT_ID_RESERVED81           =  81u,             /* Reserved interrupt 81                                            */
	INT_ID_RESERVED82           =  82u,             /* Reserved interrupt 82                                            */
	INT_ID_RESERVED83           =  83u,             /* Reserved interrupt 83                                            */
	INT_ID_RESERVED84           =  84u,             /* Reserved interrupt 84                                            */
	INT_ID_RESERVED85           =  85u,             /* Reserved interrupt 85                                            */
	INT_ID_RESERVED86           =  86u,             /* Reserved interrupt 86                                            */
	INT_ID_RESERVED87           =  87u,             /* Reserved interrupt 87                                            */
	INT_ID_RESERVED88           =  88u,             /* Reserved interrupt 88                                            */
	INT_ID_RESERVED89           =  89u,             /* Reserved interrupt 89                                            */
	INT_ID_RESERVED90           =  90u,             /* Reserved interrupt 90                                            */
	INT_ID_RESERVED91           =  91u,             /* Reserved interrupt 91                                            */
	INT_ID_RESERVED92           =  92u,             /* Reserved interrupt 92                                            */
	INT_ID_RESERVED93           =  93u,             /* Reserved interrupt 93                                            */
	INT_ID_RESERVED94           =  94u,             /* Reserved interrupt 94                                            */
	INT_ID_RESERVED95           =  95u,             /* Reserved interrupt 95                                            */
	INT_ID_RESERVED96           =  96u,             /* Reserved interrupt 96                                            */
	INT_ID_RESERVED97           =  97u,             /* Reserved interrupt 97                                            */
	INT_ID_RESERVED98           =  98u,             /* Reserved interrupt 98                                            */
	INT_ID_RESERVED99           =  99u,             /* Reserved interrupt 99                                            */
	INT_ID_CAN0_ORED            = 100u,             /* CAN0 or'ed [bus off or transmit warning or receive warning]      */
	INT_ID_CAN0_ERR_TX          = 101u,             /* CAN0 transmit error interrupt                                    */
	INT_ID_CAN0_ERR_RX          = 102u,             /* CAN0 receive error interrupt                                     */
	INT_ID_CAN0_ERR_BUS         = 103u,             /* CAN0 bus error interrupt                                         */
	INT_ID_CAN0_ERR_BUS_FBR     = 104u,             /* CAN0 bus error in fast bit rate region interrupt                 */
	INT_ID_CAN0_WAKE_UP         = 105u,             /* CAN0 valid message matches criteria during low power mode        */
	INT_ID_CAN0_WAKE_UP_NOMATCH = 106u,             /* CAN0 no reception of a matching message during low power mode    */
	INT_ID_CAN0_ORED_0_15_MB    = 107u,             /* CAN0 or'ed message buffer (0-15)                                 */
	INT_ID_CAN0_ORED_16_31_MB   = 108u,             /* CAN0 or'ed message buffer (16-31)                                */
	INT_ID_RESERVED109          = 109u,             /* Reserved interrupt 109                                           */
	INT_ID_RESERVED110          = 110u,             /* Reserved interrupt 110                                           */
	INT_ID_CAN1_ORED            = 111u,             /* CAN1 or'ed [bus off or transmit warning or receive warning]      */
	INT_ID_CAN1_ERR_TX          = 112u,             /* CAN1 transmit error interrupt                                    */
	INT_ID_CAN1_ERR_RX          = 113u,             /* CAN1 receive error interrupt                                     */
	INT_ID_CAN1_ERR_BUS         = 114u,             /* CAN1 bus error interrupt                                         */
	INT_ID_RESERVED115          = 115u,             /* Reserved interrupt 115                                           */
	INT_ID_RESERVED116          = 116u,             /* Reserved interrupt 116                                           */
	INT_ID_CAN1_ORED_0_15_MB    = 117u,             /* CAN1 or'ed interrupt for message buffer (0-15)                   */
	INT_ID_RESERVED118          = 118u,             /* Reserved interrupt 118                                           */
	INT_ID_RESERVED119          = 119u,             /* Reserved interrupt 119                                           */
	INT_ID_RESERVED120          = 120u,             /* Reserved interrupt 120                                           */
	INT_ID_CAN2_ERR_TX          = 122u,             /* CAN2 transmit error interrupt                                    */
	INT_ID_CAN2_ERR_RX          = 123u,             /* CAN2 receive error interrupt                                     */
	INT_ID_CAN2_ERR_BUS         = 124u,             /* CAN2 bus error interrupt                                         */
	INT_ID_CAN2_ORED            = 121u,             /* CAN2 or'ed [bus off or transmit warning or receive warning]      */
	INT_ID_RESERVED125          = 125u,             /* Reserved interrupt 125                                           */
	INT_ID_RESERVED126          = 126u,             /* Reserved interrupt 126                                           */
	INT_ID_CAN2_ORED_0_15_MB    = 127u,             /* CAN2 or'ed message buffer (0-15)                                 */
	INT_ID_RESERVED128          = 128u,             /* Reserved interrupt 128                                           */
	INT_ID_RESERVED129          = 129u,             /* Reserved interrupt 129                                           */
	INT_ID_RESERVED130          = 130u,             /* Reserved interrupt 130                                           */
	INT_ID_FTM0_CH1             = 132u,             /* FTM0 channel 1 interrupt                                         */
	INT_ID_FTM0_CH2             = 133u,             /* FTM0 channel 2 interrupt                                         */
	INT_ID_FTM0_CH3             = 134u,             /* FTM0 channel 3 interrupt                                         */
	INT_ID_FTM0_CH0             = 131u,             /* FTM0 channel 0 interrupt                                         */
	INT_ID_FTM0_CH4             = 135u,             /* FTM0 channel 4 interrupt                                         */
	INT_ID_FTM0_CH5             = 136u,             /* FTM0 channel 5 interrupt                                         */
	INT_ID_FTM0_CH6             = 137u,             /* FTM0 channel 6 interrupt                                         */
	INT_ID_FTM0_CH7             = 138u,             /* FTM0 channel 7 interrupt                                         */
	INT_ID_FTM0_FAULT           = 139u,             /* FTM0 fault interrupt                                             */
	INT_ID_FTM0_OVF             = 140u,             /* FTM0 counter overflow interrupt                                  */
	INT_ID_FTM0_RELOAD          = 141u,             /* FTM0 reload flag interrupt                                       */
	INT_ID_FTM1_CH0             = 142u,             /* FTM1 channel 0 interrupt                                         */
	INT_ID_FTM1_CH1             = 143u,             /* FTM1 channel 1 interrupt                                         */
	INT_ID_FTM1_CH2             = 144u,             /* FTM1 channel 2 interrupt                                         */
	INT_ID_FTM1_CH3             = 145u,             /* FTM1 channel 3 interrupt                                         */
	INT_ID_FTM1_CH4             = 146u,             /* FTM1 channel 4 interrupt                                         */
	INT_ID_FTM1_CH5             = 147u,             /* FTM1 channel 5 interrupt                                         */
	INT_ID_FTM1_CH6             = 148u,             /* FTM1 channel 6 interrupt                                         */
	INT_ID_FTM1_CH7             = 149u,             /* FTM1 channel 7 interrupt                                         */
	INT_ID_FTM1_FAULT           = 150u,             /* FTM1 fault interrupt                                             */
	INT_ID_FTM1_OVF             = 151u,             /* FTM1 counter overflow interrupt                                  */
	INT_ID_FTM1_RELOAD          = 152u,             /* FTM1 reload flag interrupt                                       */
	INT_ID_FTM2_CH0             = 153u,             /* FTM2 channel 0 interrupt                                         */
	INT_ID_FTM2_CH1             = 154u,             /* FTM2 channel 1 interrupt                                         */
	INT_ID_FTM2_CH2             = 155u,             /* FTM2 channel 2 interrupt                                         */
	INT_ID_FTM2_CH3             = 156u,             /* FTM2 channel 3 interrupt                                         */
	INT_ID_FTM2_CH4             = 157u,             /* FTM2 channel 4 interrupt                                         */
	INT_ID_FTM2_CH5             = 158u,             /* FTM2 channel 5 interrupt                                         */
	INT_ID_FTM2_CH6             = 159u,             /* FTM2 channel 6 interrupt                                         */
	INT_ID_FTM2_CH7             = 160u,             /* FTM2 channel 7 interrupt                                         */
	INT_ID_FTM2_FAULT           = 161u,             /* FTM2 fault interrupt                                             */
	INT_ID_FTM2_OVF             = 162u,             /* FTM2 counter overflow interrupt                                  */
	INT_ID_FTM2_RELOAD          = 163u,             /* FTM2 reload flag interrupt                                       */
	INT_ID_FTM3_CH0             = 164u,             /* FTM3 channel 0 interrupt                                         */
	INT_ID_FTM3_CH1             = 165u,             /* FTM3 channel 1 interrupt                                         */
	INT_ID_FTM3_CH2             = 166u,             /* FTM3 channel 2 interrupt                                         */
	INT_ID_FTM3_CH3             = 167u,             /* FTM3 channel 3 interrupt                                         */
	INT_ID_FTM3_CH4             = 168u,             /* FTM3 channel 4 interrupt                                         */
	INT_ID_FTM3_CH5             = 169u,             /* FTM3 channel 5 interrupt                                         */
	INT_ID_FTM3_CH6             = 170u,             /* FTM3 channel 6 interrupt                                         */
	INT_ID_FTM3_CH7             = 171u,             /* FTM3 channel 7 interrupt                                         */
	INT_ID_FTM3_FAULT           = 172u,             /* FTM3 fault interrupt                                             */
	INT_ID_FTM3_OVF             = 173u,             /* FTM3 counter overflow interrupt                                  */
	INT_ID_FTM3_RELOAD          = 174u              /* FTM3 reload flag interrupt                                       */
} BSP_INT_ID;


typedef enum bsp_int_type {                         /* Types of Interrupt.                                              */
    INT_IRQ,                                        /* Normal interrupt request.                                        */
} BSP_INT_TYPE;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_IntInit   (void);

void  BSP_IntEnable (BSP_INT_ID     int_id);

void  BSP_IntDisable(BSP_INT_ID     int_id);

void  BSP_IntClear  (BSP_INT_ID     int_id);

void  BSP_IntVectSet(BSP_INT_ID     int_id,
                     CPU_INT08U     int_prio,
                     BSP_INT_TYPE   int_type,
                     CPU_FNCT_VOID  isr_handler);


/*
*********************************************************************************************************
*                                   EXTERNAL C LANGUAGE LINKAGE END
*********************************************************************************************************
*/

#ifdef __cplusplus
}                                                               /* End of 'extern'al C lang linkage.                    */
#endif


/*
*********************************************************************************************************
*                                              MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
