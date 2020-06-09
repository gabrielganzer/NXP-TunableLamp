/**********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
* Filename : bsp_led.c
**********************************************************************************************************/
/**********************************************************************************************************
*                                            INCLUDE FILES
**********************************************************************************************************/
#include  <lib_def.h>
#include  "bsp_led.h"
#include  "bsp_adc.h"
#include  "S32K144.h"
/**********************************************************************************************************
*                                               DEFINES
**********************************************************************************************************/

#define  BSP_LED_RGB_RED_PIN      DEF_BIT_15
#define  BSP_LED_RGB_GREEN_PIN    DEF_BIT_16
#define  BSP_LED_RGB_BLUE_PIN     DEF_BIT_00

#define  BSP_LED_RGB_ALL_PINS    (BSP_LED_RGB_RED_PIN   | \
	                              BSP_LED_RGB_GREEN_PIN | \
								  BSP_LED_RGB_BLUE_PIN)
/**********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************/
/**********************************************************************************************************
*                                          GLOBAL FUNCTIONS
**********************************************************************************************************/
/**********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : Initializes the required pins that control the RGB LED.
* Argument(s) : none.
* Return(s)   : none.
* Note(s)     : none.
**********************************************************************************************************/
void BSP_LED_Init (void)
{
	/* Enable clock to PORT D */
	PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK;
	/* Enable clock for FTM0 associated to PORTD in ALT2 mode*/
	PCC->PCCn[PCC_FTM0_INDEX] = PCC_PCCn_PCS(6) | PCC_PCCn_CGC_MASK;
	/* Configure PD15 (RGB_RED) as a FTM0_CH0 function */
	PORTD->PCR[15] = PORT_PCR_MUX(2);
	/* Configure PD16 (RGB_GREEN) as a FTM0_CH1 function */
	PORTD->PCR[16] = PORT_PCR_MUX(2);
	/* Configure PD0 (RGB_BLUE) as a FTM0_CH2 function */
	PORTD->PCR[ 0] = PORT_PCR_MUX(2);
}
/**********************************************************************************************************
*                                            BSP_LED_On()
*
* Description : Turn ON the RGB LED in a specific color.
* Argument(s) : color    The color you want the RGB LED to show.
*                  LED_RED       Turns on RGB_LED in RED color
*                  LED_GREEN     Turns on RGB_LED in GREEN color
*                  LED_BLUE      Turns on RGB_LED in BLUE color
*                  LED_YELLOW    Turns on RGB_LED in YELLOW color
*                  LED_PURPLE    Turns on RGB_LED in PURPLE color
*                  LED_AQUA      Turns on RGB_LED in AQUA color
*                  LED_WHITE     Turns on RGB_LED in WHITE color
*                  LED_ALL       Turns on RGB_LED in WHITE color
* Return(s)   : none.
* Note(s)     : none.
**********************************************************************************************************/
void BSP_LED_On (BSP_LED color, CPU_INT32S FTM_period)
{
	//BSP_LED_Off();
	switch (color) {
		case LED_GREEN:
			/* Enable FTM0_CH1 and disable FTM0_CH0/FTM0_CH2 mask */
			FTM0->OUTMASK &= FTM_OUTMASK_CH1OM_MASK & (~FTM_OUTMASK_CH0OM_MASK) & (~FTM_OUTMASK_CH2OM_MASK);
			/* Update FTM0_CH1 counter value register */
			FTM0->CONTROLS[1].CnV=FTM_CnV_VAL(FTM_period);
			/* Trigger synchronization*/
			FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
		break;
		case LED_BLUE:
			/* Enable FTM0_CH2 and disable FTM0_CH0/FTM0_CH1 mask */
			FTM0->OUTMASK &= FTM_OUTMASK_CH2OM_MASK & (~FTM_OUTMASK_CH0OM_MASK) & (~FTM_OUTMASK_CH1OM_MASK);
			/* Update FTM0_CH2 counter value register */
			FTM0->CONTROLS[2].CnV=FTM_CnV_VAL(FTM_period);
			/* Trigger synchronization*/
			FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
		break;
		case LED_RED:
		default:
			/* Enable FTM0_CH0 and disable FTM0_CH1/FTM0_CH2 mask */
			FTM0->OUTMASK &= FTM_OUTMASK_CH0OM_MASK & (~FTM_OUTMASK_CH1OM_MASK) & (~FTM_OUTMASK_CH2OM_MASK);
			/* Update FTM0_CH0 counter value register */
			FTM0->CONTROLS[0].CnV=FTM_CnV_VAL(FTM_period);
			/* Trigger synchronization*/
			FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
		break;
	}
}
/**********************************************************************************************************
*                                            BSP_LED_Off()
*
* Description : Turns OFF the RGB LED.
* Argument(s) : none.
* Return(s)   : none.
* Note(s)     : none.
**********************************************************************************************************/
void BSP_LED_Off (BSP_LED color)
{
	switch (color) {
		case LED_RED:
			/* Disable FTM0_CH0 mask */
			FTM0->OUTMASK &= (~FTM_OUTMASK_CH0OM_MASK);
			/* Trigger synchronization*/
			FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
		break;
		case LED_GREEN:
			/* Disable FTM0_CH1 mask */
			FTM0->OUTMASK &= (~FTM_OUTMASK_CH1OM_MASK);
			/* Trigger synchronization*/
			FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
		break;
		case LED_BLUE:
			/* Disable FTM0_CH2 mask */
			FTM0->OUTMASK &= (~FTM_OUTMASK_CH2OM_MASK);
			/* Trigger synchronization*/
			FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
		break;
		case LED_ALL:
		default:
			/* Disable FTM0_CH0/FTM0_CH1/FTM0_CH2 mask */
			FTM0->OUTMASK &= (~FTM_OUTMASK_CH0OM_MASK) & (~FTM_OUTMASK_CH1OM_MASK) & (~FTM_OUTMASK_CH2OM_MASK);
			/* Trigger synchronization*/
			FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
		break;
	}
}
