/*
*********************************************************************************************************
*
* MICRIUM BOARD SUPPORT PACKAGE
*
* Filename : bsp_switch.c
*********************************************************************************************************
*/
/**********************************************************************************************************
* 									INCLUDE FILES
**********************************************************************************************************/
#include <lib_def.h>
#include <cpu.h>
#include <bsp_int.h>
#include <os.h>
#include "bsp_switch.h"
#include "S32K144.h"
/**********************************************************************************************************
* 										DEFINES
**********************************************************************************************************/
volatile CPU_INT08U SW3_status = 0;
volatile CPU_INT08U SW2_status = 0;
/**********************************************************************************************************
* 								LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************/
static void SW_IRQHandler( void );
/**********************************************************************************************************
* 									GLOBAL FUNCTIONS
**********************************************************************************************************/
/**********************************************************************************************************
* 									BSP_Switch_Init()
*
* Description : Initializes the required pins that control the switches.
* Argument(s) : none.
* Return(s) : none.
* Note(s) : none.
**********************************************************************************************************/
void BSP_SWITCH_Init (void) {

	PCC->PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT C */
	/* Set PC12 (SW2) and PC12 (SW3) as input pins */
	PTC->PDDR &= ~(DEF_BIT_12 | DEF_BIT_13);
	/* Configure PC12 and PC13 as GPIO functions (ALT1) */
	PORTC->PCR[12] |= PORT_PCR_MUX(1);
	PORTC->PCR[13] |= PORT_PCR_MUX(1);
	/* Configure PC12 for rising edge interrupt */
	PORTC->PCR[12] |= PORT_PCR_IRQC(9);
	PORTC->PCR[13] |= PORT_PCR_IRQC(9);
	/* Configure amount of cycles for digital filter delay at 128kHz*/
	PORTC->DFWR = PORT_DFWR_FILT(384000);
	/* Filter clock source as LPO */
	PORTB->DFCR = PORT_DFCR_CS(1);
	/* Enable digital filter delay for ports PC12 and PC13*/
	PORTC->DFER = PORT_DFER_DFE(DEF_BIT_12 | DEF_BIT_13);
	/* Register Interrupt Handler */
	BSP_IntVectSet(PORTC_IRQn, 0,0, SW_IRQHandler);
	/* Enable the interrupt for PORTC */
	BSP_IntEnable( PORTC_IRQn );
}
/**********************************************************************************************************
* 											BSP_Switch_Read()
*
* Description : Read the required pin that control the switches.
* Argument(s) : switch to be read.
* Return(s) : value of the switch (0 or 1).
* Note(s) : Polling method.
**********************************************************************************************************/
CPU_INT08U SW_Read (BSP_SWITCH sw) {

	switch(sw) {
		case SW2:
			return( ((PTC->PDIR) >> 12) & 0x01u); /*Read the value on the selected input*/
			break;
		case SW3:
			return( ((PTC->PDIR) >> 13) & 0x01u); /*Read the value on the selected input*/
			break;
	}
	return( 0x00u );
}
/**********************************************************************************************************
* 											SW3_int_hdlr()
*
* Description : Respond to SW3 interrupt (PORTCn).
* Argument(s) : switch to be read.
* Return(s) : value of the switch (0 or 1).
* Note(s) : none.
**********************************************************************************************************/
static void SW_IRQHandler( void ) {

	uint32_t isf;

	//Inform the OS we are in an interrupt
	OSIntEnter();
	isf = ((PORTC->PCR[12]) & 0x01000000) | ((PORTC->PCR[13]) & 0x01000000);
	if( (isf) ) {
		SW2_status = SW_Read(SW2);
		SW3_status = SW_Read(SW3);
		//Clear the ISF bit
		PORTC->PCR[12] |= 0x01000000;
		PORTC->PCR[13] |= 0x01000000;
	}
	//Inform the OS we are exiting the interrupt;
	OSIntExit();
}
