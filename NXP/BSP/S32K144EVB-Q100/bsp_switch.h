/*
*********************************************************************************************************
*
* MICRIUM BOARD SUPPORT PACKAGE
*
* Filename : bsp_switch.h
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
* BSP present pre-processor macro definition.
*********************************************************************************************************
*/
#ifndef BSP_SWITCH_PRESENT
#define BSP_SWITCH_PRESENT

/*
*********************************************************************************************************
* INCLUDE FILES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
* EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
* declarations for correct C language linkage.
*********************************************************************************************************
*/

#ifdef __cplusplus
extern "C" { /* See Note #1. */
#endif

/*
*********************************************************************************************************
* DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
* DATA TYPES
*********************************************************************************************************
*/

typedef enum bsp_switch {
	SW2,
	SW3
} BSP_SWITCH;

/*
*********************************************************************************************************
* FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void BSP_SWITCH_Init (void);
CPU_INT08U SW_Read (BSP_SWITCH sw);

/*
*********************************************************************************************************
* EXTERNAL C LANGUAGE LINKAGE END
*********************************************************************************************************
*/

#ifdef __cplusplus
} /* End of 'extern'al C lang linkage. */
#endif

/*
*********************************************************************************************************
* MODULE END
*********************************************************************************************************
*/

#endif /* End of module include. */
