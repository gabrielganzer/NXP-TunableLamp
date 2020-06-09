#include <lib_def.h>
#include <bsp_int.h>
#include <os.h>
#include "bsp_timer.h"
#include "bsp_adc.h"
#include "S32K144.h"

void BSP_TIMER_Init (void) {

	/* Enable registers updating from write buffers */
	FTM0->MODE = FTM_MODE_FTMEN_MASK;
    /* Enable CnV and C(n+1)V sync for pair channel n=1 and n=2 */
    FTM0->COMBINE = FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_SYNCEN1_MASK;
	/* Set modulo to 10 KHz PWM frequency @ 80 MHz system clock with pre-scaler 1*/
	FTM0->MOD = FTM_MOD_MOD(8000-1);
	/* Set CNTIN */
	FTM0->CNTIN = FTM_CNTIN_INIT(0);
	/* Enable High-true pulses (clear Output on match) of PWM signals */
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK | (~FTM_CnSC_ELSB_MASK);
	FTM0->CONTROLS[1].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK | (~FTM_CnSC_ELSB_MASK);
	FTM0->CONTROLS[2].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK | (~FTM_CnSC_ELSB_MASK);
	/* Reset FTM counter */
	FTM0->CNT = 0;
	/* Set FTM0_CH0/FTM0_CH1/FTM0_CH2 output polarity to high, their safe value*/
	FTM0->POL = FTM_POL_POL0_MASK | FTM_POL_POL1_MASK | FTM_POL_POL2_MASK;
	/* Enable software synchronization and output mask update*/
	FTM0->SYNCONF |= FTM_SYNCONF_SWOM_MASK | FTM_SYNCONF_SYNCMODE_MASK
				  | FTM_SYNCONF_SWWRBUF_MASK | FTM_SYNCONF_SWRSTCNT_MASK;
	/* Select clock and prescale */
	FTM0->SC |= FTM_SC_CLKS(1) | FTM_SC_PS(0);
	/* Enable PWM generation */
	FTM0->SC |= FTM_SC_PWMEN0_MASK | FTM_SC_PWMEN1_MASK | FTM_SC_PWMEN2_MASK;
}
