#include <lib_def.h>
#include <os.h>
#include <bsp_int.h>
#include "bsp_adc.h"
#include "S32K144.h"

extern   OS_SEM 	ADC0sem;
volatile CPU_INT16U ADC0_result;

void BSP_ADC0_Init(void) {

	/* Disable clock to change PCS */
	PCC->PCCn[PCC_ADC0_INDEX] &= (~PCC_PCCn_CGC_MASK);
	/* PCS=1: Select SOSCDIV2 */
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(1);
	/* Enable bus clock in ADC */
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK;
	/* ADCH=1F: Module is disabled for conversions*/
	ADC0->SC1[0] =0x00007F;
	/* AIEN=1: Interrupts are disabled */
	/* ADICLK=0: Input clk=ALTCLK1=SOSCDIV2 */
	ADC0->CFG1 = 0x000000004;
	/* ADIV=0: Prescaler=1 */
	/* MODE=1: 12-bit conversion */
	/* SMPLTS=12(default): sample time is 13 ADC clks */
	ADC0->CFG2 = 0x00000000C;
	/* ADTRG=0: SW trigger */
	ADC0->SC2 = 0x00000000;
	/* ACFE,ACFGT,ACREN=0: Compare func disabled */
	/* DMAEN=0: DMA disabled */
	/* REFSEL=0: Voltage reference pins= VREFH, VREEFL */
	ADC0->SC3 = 0x00000000; /* CAL=0: Do not start calibration sequence */
	/* ADCO=0: One conversion performed */
	/* AVGE,AVGS=0: HW average function disabled */
	/* Enable interrupt for ADC0 */
	BSP_IntVectSet(ADC0_IRQn, 0, 0, ADC0_IRQHandler);
	BSP_IntEnable(ADC0_IRQn);
}

void BSP_ADC0_Convert(CPU_INT16U adcChan) {

	/* For SW trigger mode, SC1[0] is used */
	/* Clear prior ADCH bits */
	ADC0->SC1[0]&= ~ADC_SC1_ADCH_MASK;
	/* Initiate Conversion and AIEN=1*/
	ADC0->SC1[0] = ADC_SC1_ADCH(adcChan) | 0x40;
}

void ADC0_IRQHandler(void)
{
	CPU_INT16U ADC0_adc_chx = 0;
	OS_ERR os_err;
	OSIntEnter();
	/* For SW trigger mode, R[0] is used */
	ADC0_adc_chx =ADC0->R[0];
	/* Convert result to mv for 0-5V range */
	ADC0_result = (CPU_INT16U) ((5000*ADC0_adc_chx)/0xFFF);
	/* Disable conversion & clear interrupt */
	ADC0->SC1[0] =0x00007F;
	OSSemPost((OS_SEM*)&ADC0sem, OS_OPT_POST_1 | OS_OPT_POST_NO_SCHED, &os_err);
	OSIntExit();
}
