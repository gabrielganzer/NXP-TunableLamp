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
* Filename : bsp_clk.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <lib_def.h>
#include  "bsp_clk.h"
#include  "S32K144.h"

#ifdef  CLK_MODULE_PRESENT
#include  <Source/clk.h>
#include  <clk_cfg.h>
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_CLK_XTAL_OSC                  8000000u             /* 8MHz crystal oscillator (see 'Y3' on schematics)     */

#define  BSP_CLK_BIT_SCG_CSR_SCS_SPLL      DEF_BIT_MASK(6u, 24u)

#define  BSP_CLK_BIT_SCG_RCCR_DIVCORE      DEF_BIT_FIELD(4u, 16u)

#define  BSP_CLK_BIT_SCG_SPLLCFG_PREDIV    DEF_BIT_FIELD(3u,  8u)
#define  BSP_CLK_BIT_SCG_SPLLCFG_MULT      DEF_BIT_FIELD(5u, 16u)


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_ClkInit()
*
* Description : Initializes the PLL and the system clock for the kernel.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Note(s)     : (1) Should be called before OSStart() in main.
*********************************************************************************************************
*/

void  BSP_ClkInit (void)
{
	                                                            /* ---- INITIALIZE SYSTEM OSC. FOR 8 MHZ. CRYSTAL ----- */
	SCG->SOSCDIV = 0x00000401u;                                 /* SOSCDIV1 & SOSCDIV2 =1: divide by 1                  */
	SCG->SOSCCFG = 0x00000024u;                                 /* Range = 2: Medium freq. (SOSC between 1MHz-8MHz)     */
	                                                            /* HGO   = 0: Configure xtal osc. for low power         */
	                                                            /* EREFS = 1: Input is external XTAL                    */

	while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK);                  /* Ensure SOSCCSR unlocked                              */

	SCG->SOSCCSR = 0x00000001u;                                 /* LK          = 0: SOSCCSR can be written              */
	                                                            /* SOSCCMRE    = 0: OSC CLK monitor IRQ if enabled      */
	                                                            /* SOSCCM      = 0: OSC CLK monitor disabled            */
	                                                            /* SOSCERCLKEN = 0: Sys OSC 3V ERCLK output clk disabled*/
	                                                            /* SOSCLPEN    = 0: Sys OSC disabled in VLP modes       */
	                                                            /* SOSCSTEN    = 0: Sys OSC disabled in Stop modes      */
	                                                            /* SOSCEN      = 1: Enable oscillator                   */

	while(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));          /* Wait for sys OSC clk valid                           */

	                                                            /* ---- INITIALIZE SPLL TO 160 MHZ WITH 8 MHZ SOSC ---- */
	while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK);                  /* Ensure SPLLCSR unlocked                              */
	SCG->SPLLCSR = 0x00000000u;                                 /* SPLLEN   =  0: SPLL is disabled (default)            */
	SCG->SPLLDIV = 0x00000302u;                                 /* SPLLDIV1 divide by 2; SPLLDIV2 divide by 4           */
	SCG->SPLLCFG = 0x00180000u;                                 /* PREDIV   =  0: Divide SOSC_CLK by 0+1=1              */
	                                                            /* MULT     = 24: Multiply sys pll by 24=24+16=40       */
	                                                            /* SPLL_CLK =  8 MHz / 1 * 40 / 2 = 160 MHz             */

	while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK);                  /* Ensure SPLLCSR unlocked                              */
	SCG->SPLLCSR = 0x00000001u;                                 /* LK       = 0: SPLLCSR can be written                 */
	                                                            /* SPLLCMRE = 0: SPLL CLK monitor IRQ if enabled        */
	                                                            /* SPLLCM   = 0: SPLL CLK monitor disabled              */
	                                                            /* SPLLSTEN = 0: SPLL disabled in Stop modes            */
	                                                            /* SPLLEN   = 1: Enable SPLL                            */

	while(!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK));          /* Wait for SPLL valid                                  */

	                                                            /* INIT CLOCKS: 80 MHZ SYSCLK, 40 MHZ BUS, 20 MHZ FLASH */
    SCG->RCCR = SCG_RCCR_SCS(6)                                 /* PLL as clock source                                  */
	          | SCG_RCCR_DIVCORE(0b01)                          /* DIVCORE = 1: Core clock = 160/2 MHz = 80 MHz         */
	          | SCG_RCCR_DIVBUS(0b01)                           /* DIVBUS  = 1: Bus clock = 40 MHz                      */
	          | SCG_RCCR_DIVSLOW(0b10);                         /* DIVSLOW = 2: SCG slow, flash clock= 26 2/3 MHz       */

                                                                /* Wait for sys clck src = SPLL                         */
    while (DEF_BIT_IS_CLR(SCG->CSR, BSP_CLK_BIT_SCG_CSR_SCS_SPLL));
}


/*
*********************************************************************************************************
*                                          BSP_ClkFreqGet()
*
* Description : Gets the current frequency of the specified clock.
*
* Argument(s) : clk_id    Clock identifier
*
*                         SYS_CLK   Clock signal that feeds the ARM core, NVIC, Flash, etc.
*
* Return(s)   : The specific clock frequency, in Hz.
*
* Note(s)     : (1) This function will return 0 if the clock is not available. The caller should check
*                   the return value before using it.
*
*               (2) The VCO_CLK and SPLL_CLK calculation is referenced from chapter 27.3.19 of the S32K
*                   reference manual.
*********************************************************************************************************
*/

CPU_INT32U  BSP_ClkFreqGet (BSP_CLK_ID  clk_id)
{
    CPU_INT32U  clk_freq;
    CPU_INT32U  scg_spllcfg;
    CPU_INT08U  prediv;
    CPU_INT08U  mult;
    CPU_INT32U  vco_clk;
    CPU_INT32U  spll_clk;
    CPU_INT08U  divcore;


    scg_spllcfg =  SCG->SPLLCFG;
    prediv      = (scg_spllcfg & BSP_CLK_BIT_SCG_SPLLCFG_PREDIV) >>  8u;
    mult        = (scg_spllcfg & BSP_CLK_BIT_SCG_SPLLCFG_MULT  ) >> 16u;

                                                                /* See note (2).                                        */
    vco_clk     = (BSP_CLK_XTAL_OSC / (prediv + 1u)) * (mult + 16u);
    spll_clk    =  vco_clk / 2u;

    switch (clk_id) {
        case SYS_CLK:
        	 divcore  = (SCG->RCCR & BSP_CLK_BIT_SCG_RCCR_DIVCORE) >> 16u;
        	 clk_freq =  spll_clk / (divcore + 1u);
        	 break;


        default:
        	 clk_freq = 0u;
        	 break;
    }

    return (clk_freq);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            EXTENDED API
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      EXTERNAL TIMESTAMP TIMER
*********************************************************************************************************
*/

#ifdef  CLK_MODULE_PRESENT


/*
*********************************************************************************************************
*                                          Clk_ExtTS_Init()
*
* Description : Initialize & start External timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Clk_Init().
*
*               This function is an INTERNAL Clock module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CLK_ExtTS_Init() is an application/BSP function that MUST be defined by the developer
*                   if External timestamp is enabled.
*
*                   See 'clk_cfg.h  CLK CONFIGURATION  Note #1'.
*
*               (2) (a) External timestamp values MUST be returned via 'CLK_TS_SEC' data type.
*
*                   (b) External timestamp values SHOULD be returned on the epoch of Clock module and
*                       include the time zone offset.
*
*               (3) (a) External timestamp SHOULD be an 'up' counter whose values increase at each second.
*                       It's possible to use a 'down' counter, but a conversion MUST be applied when setting
*                       and getting timestamp.
*
*                   (b) External timestamp COULD come from another application (e.g. by SNTPc).
*********************************************************************************************************
*/

#if (CLK_CFG_EXT_EN == DEF_ENABLED)
void  Clk_ExtTS_Init (void)
{

}
#endif


/*
*********************************************************************************************************
*                                           Clk_ExtTS_Get()
*
* Description : Get Clock module's timestamp from converted External timestamp.
*
* Argument(s) : none.
*
* Return(s)   : Current Clock timestamp (in seconds, UTC+00).
*
* Caller(s)   : Clk_GetTS().
*
*               This function is an INTERNAL Clock module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) Clk_ExtTS_Get() is an application/BSP function that MUST be defined by the developer
*                   if External timestamp is enabled.
*
*                   See 'clk_cfg.h  CLK CONFIGURATION  Note #1'
*
*
*               (2) (a) Clock timestamp values MUST be returned via 'CLK_TS_SEC' data type.
*
*                   (b) (1) If the External timestamp has more bits than the 'CLK_TS_SEC' data type,
*                           Clk_ExtTS_Get() MUST truncate the External timestamp's higher order bits
*                           greater than the 'CLK_TS_SEC' data type.
*
*                       (2) If the External timestamp has less bits than the 'CLK_TS_SEC' data type,
*                           Clk_ExtTS_Get() MUST pad the Clock timestamp's higher order bits with
*                           0 bits.
*
*
*               (3) (a) External timestamp values MUST be returned from the reference of the Clock
*                       epoch start date/time.
*
*                   (b) External timestamp SHOULD start on midnight of January 1st of its epoch start
*                       year. Otherwise, the equations to convert between External timestamp & Clock
*                       timestamp MUST also include the External timestamp's epoch Day-of-Year, Hour,
*                       Minute, & Second (see Note #4).
*
*                   (c) Returned Clock timestamp MUST be representable in Clock epoch. Thus equivalent
*                       date of the External timestamp MUST be between :
*
*                       (1) >= CLK_EPOCH_YR_START
*                       (2) <  CLK_EPOCH_YR_END
*
*                   (d) If the External timestamp includes an (optional) external time zone,
*                       Clk_ExtTS_Get() MUST subtract the external time zone offset from the
*                       converted External timestamp.
*
*
*               (4) The Clock timestamp is calculated by one of the following equations (assuming
*                   Note #3b) :
*
*                   (a) When External epoch start year is less than Clock epoch start year
*                       ('CLK_EPOCH_YR_START') :
*
*                       Clock TS = External TS
*                                - [(((Clock start year - External start year) * 365) + leap day count)
*                                    * seconds per day]
*                                - External TZ
*
*                       Examples with a 32-bit External timestamp :
*
*                       (1)   Valid equivalent date to convert is after  Clock epoch start year :
*
*                             2010 Oct 8, 11:11:11 UTC-05:00
*                           External TS (in seconds)                                 = 1286536271
*                           External start year                                      =       1970
*                           Clock    start year                                      =       2000
*                           Leap day count between External & Clock epoch start year =          7
*                           External TZ (in seconds)                                 =     -18000
*                           Clock    TS (in seconds)                                 =  339869471
*                             2010 Oct 8, 16:11:11 UTC
*
*                           This example successfully converts an External timestamp into a
*                           representable Clock timestamp without underflowing.
*
*                       (2) Invalid equivalent date to convert is before Clock epoch start year :
*
*                             1984 Oct 8, 11:11:11 UTC-05:00
*                           External TS (in seconds)                                 =  466081871
*                           External start year                                      =       1970
*                           Clock    start year                                      =       2000
*                           Leap day count between External & Clock epoch start year =          7
*                           External TZ (in seconds)                                 =     -18000
*                           Clock    TS (in seconds)                                 = -480584929
*
*                           This example underflows to a negative Clock timestamp since the
*                           equivalent date to convert is incorrectly less than the Clock epoch
*                           start year ('CLK_EPOCH_YR_START').
*
*
*                   (b) When External epoch start year is greater than Clock epoch start year
*                       ('CLK_EPOCH_YR_START') :
*
*                       Clock TS = External TS
*                                + [(((External start year - Clock start year) * 365) + leap day count)
*                                    * seconds per day]
*                                - External TZ
*
*
*                       Examples with a 32-bit External timestamp :
*
*                       (1)   Valid equivalent date to convert is before Clock epoch end year :
*
*                             2010 Oct 8, 11:11:11 UTC-05:00
*                           External TS (in seconds)                                 =   24232271
*                           External start year                                      =       2010
*                           Clock    end   year                                      =       2136
*                           Leap day count between External & Clock epoch start year =          3
*                           External TZ (in seconds)                                 =     -18000
*                           Clock    TS (in seconds)                                 =  339869471
*                             2010 Oct 8, 16:11:11 UTC-05:00
*
*                           This example successfully converts an External timestamp into a
*                           representable Clock timestamp without overflowing.
*
*                       (2) Invalid equivalent date to convert is after  Clock epoch end year :
*
*                             2140 Oct 8, 11:11:11 UTC-05:00
*                           External TS (in seconds)                                 = 4126677071
*                           External start year                                      =       2010
*                           Clock    end   year                                      =       2136
*                           Leap day count between External & Clock epoch start year =          3
*                           External TZ (in seconds)                                 =     -18000
*                           Clock    TS (in seconds)                                 = 4442314271
*
*                           This example overflows the Clock timestamp (32-bit) 'CLK_TS_SEC' data
*                           type with an equivalent date incorrectly greater than or equal to the
*                           Clock epoch end year ('CLK_EPOCH_YR_END').
*
*
*                   (c) Where
*
*                       (1) Clock    TS             Converted Clock timestamp (in seconds,
*                                                       from UTC+00)
*                       (2) External TS             External timestamp to convert (in seconds)
*                       (3) Clock    start year     Clock epoch start year ('CLK_EPOCH_YR_START')
*                       (4) Clock    end   year     Clock epoch end   year ('CLK_EPOCH_YR_END')
*                       (5) External start year     External timestamp epoch start year
*                       (6) Leap day count          Number of leap days between Clock epoch
*                                                       start year & External epoch start year
*                       (7) Seconds per day         Number of seconds per day (86400)
*                       (8) External TZ             Time zone offset applied to External TS
*                                                       (in seconds, from UTC+00)
*********************************************************************************************************
*/

#if (CLK_CFG_EXT_EN == DEF_ENABLED)
CLK_TS_SEC  Clk_ExtTS_Get (void)
{
    return (0u);
}
#endif


/*
*********************************************************************************************************
*                                           Clk_ExtTS_Set()
*
* Description : Set External timestamp.
*
* Argument(s) : ts_sec      Timestamp value to set (in seconds, UTC+00).
*
* Return(s)   : DEF_OK,     if External timestamp succesfully set.
*
*               DEF_FAIL,   otherwise.
*
* Caller(s)   : Clk_SetTS().
*
*               This function is an INTERNAL Clock module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CLK_ExtTS_Set() is an application/BSP function that MUST be defined by the developer
*                   if External timestamp is enabled.
*
*                       See 'clk_cfg.h  CLK CONFIGURATION  Note #1'.
*
*                   (a) If External timestamp is provided by another application, it's possible that the
*                       External timestamp may NOT be set (e.g. by SNTPc) in which case CLK_ExtTS_Set()
*                       MUST ALWAYS return 'DEF_FAIL'.
*
*
*               (2) (a) External timestamp values are converted from Clock timestamp's 'CLK_TS_SEC'
*                       data type.
*
*                   (b) (1) If the External timestamp has more bits than the 'CLK_TS_SEC' data type,
*                           Clk_ExtTS_Set() MUST pad the External timestamp's higher order bits with
*                           0 bits.
*
*                       (2) If the External timestamp has less bits than the 'CLK_TS_SEC' data type,
*                           Clk_ExtTS_Set() MUST truncate the Clock timestamp's higher order bits
*                           greater than the External timestamp.
*
*
*               (3) (a) External timestamp values MUST be converted from the reference of the Clock
*                       epoch start date/time.
*
*                   (b) External timestamp SHOULD start on midnight of January 1st of its epoch start
*                       year. Otherwise, the equations to convert between External timestamp & Clock
*                       timestamp MUST also include the External timestamp's epoch Day-of-Year, Hour,
*                       Minute, & Second (see Note #4).
*
*                   (c) Converted External timestamp MUST be representable in External epoch. Thus
*                       equivalent date of the External timestamp MUST be between :
*
*                       (1) External epoch start year
*                       (2) External epoch end   year
*
*                   (d) If the External timestamp includes an (optional) external time zone,
*                       Clk_ExtTS_Set() MUST add the external time zone offset to the converted
*                       External timestamp.
*
*
*               (4) The External timestamp is calculated by one of the following equations (assuming
*                   Note #3b) :
*
*                   (a) When External epoch start year is less than Clock epoch start year
*                       ('CLK_EPOCH_YR_START') :
*
*                       External TS = Clock TS
*                                   + [(((Clock start year - External start year) * 365) + leap day count)
*                                       * seconds per day]
*                                   + External TZ
*
*                       Examples with a 32-bit External timestamp :
*
*                       (1)   Valid equivalent date to convert is before External epoch end year :
*
*                             2010 Oct 8, 16:11:11 UTC
*                           Clock    TS (in seconds)                                 =  339869471
*                           External start year                                      =       1970
*                           External end   year                                      =       2106
*                           Leap day count between External & Clock epoch start year =          7
*                           External TZ (in seconds)                                 =     -18000
*                           External TS (in seconds)                                 = 1286536271
*                             2010 Oct 8, 11:11:11 UTC-05:00
*
*                           This example successfully converts an External timestamp into a
*                           representable Clock timestamp without overflowing.
*
*                       (2) Invalid equivalent date to convert is after  External epoch end year :
*
*                             2120 Oct 8, 11:11:11 UTC
*                           Clock    TS (in seconds)                                 = 3811144271
*                           External start year                                      =       1970
*                           External end   year                                      =       2106
*                           Leap day count between External & Clock epoch start year =          7
*                           External TZ (in seconds)                                 =     -18000
*                           External TS (in seconds)                                 = 4757811071
*
*                           This example overflows the External (32-bit) timestamp with an equivalent
*                           date incorrectly greater than or equal to the External epoch end year.
*
*
*                   (b) When External epoch start year is greater than Clock epoch start year
*                       ('CLK_EPOCH_YR_START') :
*
*                       External TS = Clock TS
*                                   - [(((External start year - Clock start year) * 365) + leap day count)
*                                       * seconds per day]
*                                   + External TZ
*
*
*                       Examples with a 32-bit External timestamp :
*
*                       (1)   Valid equivalent date to convert is after  External epoch start year :
*
*                             2010 Oct 8, 16:11:11 UTC
*                           Clock    TS (in seconds)                                 =  339869471
*                           External start year                                      =       2010
*                           Leap day count between External & Clock epoch start year =          3
*                           External TZ (in seconds)                                 =     -18000
*                           External TS (in seconds)                                 =   24232271
*                             2010 Oct 8, 11:11:11 UTC-05:00
*
*                           This example successfully converts an External timestamp into a
*                           representable Clock timestamp without underflowing.
*
*                       (2) Invalid equivalent date to convert is before External epoch start year :
*
*                             2005 Oct 8, 11:11:11 UTC
*                           Clock    TS (in seconds)                                 =  182085071
*                           External start year                                      =       2010
*                           Leap day count between External & Clock epoch start year =          3
*                           External TZ (in seconds)                                 =     -18000
*                           External TS (in seconds)                                 = -133552129
*
*                           This example underflows to a negative External timestamp since the
*                           equivalent date to convert is incorrectly less than the External
*                           epoch start year.
*
*
*                   (c) where
*
*                       (1) Clock    TS             Clock timestamp (in seconds, from UTC+00)
*                       (2) External TS             Converted External timestamp (in seconds)
*                       (3) Clock    start year     Clock epoch start year ('CLK_EPOCH_YR_START')
*                       (4) External start year     External timestamp epoch start year
*                       (5) External end   year     External timestamp epoch end   year
*                       (6) Leap day count          Number of leap days between Clock epoch
*                                                       start year & External epoch start year
*                       (7) Seconds per day         Number of seconds per day (86400)
*                       (8) External TZ             Time zone offset applied to External TS
*                                                       (in seconds, from UTC+00)
*********************************************************************************************************
*/

#if (CLK_CFG_EXT_EN == DEF_ENABLED)
CPU_BOOLEAN  Clk_ExtTS_Set (CLK_TS_SEC  ts_sec)
{
    return (DEF_FAIL);
}
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of uC/Clk module include.       */
