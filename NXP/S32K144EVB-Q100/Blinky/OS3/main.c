/**********************************************************************************************************
*
*                                             uC/OS-III
*                                            FINAL ASSIGNMENT
*
* Filename : main.c
**********************************************************************************************************/

/**********************************************************************************************************
*                                            INCLUDE FILES
**********************************************************************************************************/
#include  <cpu.h>
#include  <lib_mem.h>
#include  <lib_def.h>
#include  <os.h>
#include  <bsp_os.h>
#include  <bsp_clk.h>
#include  <bsp_int.h>
#include  <bsp_led.h>
#include  <bsp_switch.h>
#include  <bsp_timer.h>
#include  <bsp_adc.h>
#include  "os_app_hooks.h"
#include  "../app_cfg.h"

/**********************************************************************************************************
*                                            LOCAL DEFINES
**********************************************************************************************************/

/**********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
**********************************************************************************************************/
//Task control block
static   OS_TCB   	StartupTaskTCB;
static   OS_TCB   	DutyCycleTaskTCB;
//Task stack
static   CPU_STK  	StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];
static   CPU_STK  	DutyCycleTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];
//Semaphores
volatile OS_SEM 	ADC0sem;
static   OS_SEM 	FTM0sem;
//Interruptions
extern 	 CPU_INT16U ADC0_result;
extern 	 CPU_INT08U SW2_status;
extern 	 CPU_INT08U SW3_status;
volatile BSP_LED 	color;
volatile CPU_INT32S FTM_period;

/**********************************************************************************************************
*                                         TASK PROTOTYPES
**********************************************************************************************************/
static  void  StartupTask (void  *p_arg);
static  void  DutyCycleTask (void  *p_arg);

/**********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
* Arguments   : none
* Returns     : none
* Notes       : none
**********************************************************************************************************/
int  main (void) {

	// Variable to store errors
    OS_ERR  os_err;
    // Initialize the main clock
    BSP_ClkInit();
    BSP_IntInit();
    // Initialize kernel tick timer
    BSP_OS_TickInit();
    // Initialize Memory Management Module
    Mem_Init();
    // Disable all Interrupts
    CPU_IntDis();
    // Initialize the uC/CPU services
    CPU_Init();
    // Initialize uC/OS-III
    OSInit(&os_err);
    if (os_err != OS_ERR_NONE) {
        while (1);
    }
    // Set all applications hooks
    App_OS_SetAllHooks();
    // Create the startup task
    OSTaskCreate(&StartupTaskTCB,									  // Pointer to the Task Control Block
                 "Startup Task",									  // String constant with the Name of the task
                 StartupTask,										  // Pointer to the task function
                 0u,												  // Pointer to arguments of the task
                 APP_CFG_STARTUP_TASK_PRIO,							  // Priority of the task
                 &StartupTaskStk[0u],								  // Base address of the task stack
                 StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE / 10u], // The amount of stack space before the stack is full
                 APP_CFG_STARTUP_TASK_STK_SIZE,						  // Size of the stack
                 0u,												  // Options for task creation
                 0u,												  // Options for task creation
                 0u,												  // Options for task creation
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),		  // Options for task creation
                 &os_err);											  //Variable storing errors
    if (os_err != OS_ERR_NONE) {
        while (1);
    }
    // Start multitasking (i.e. give control to uC/OS-III)
    OSStart(&os_err);
    // Should Never Get Here
    while (DEF_ON);
}

/**********************************************************************************************************
*                                            STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'StartupTask()' by 'OSTaskCreate()'.
* Returns     : none
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
**********************************************************************************************************/
static void StartupTask (void *p_arg) {

	OS_ERR os_err;

	(void)p_arg;
	OS_TRACE_INIT(); /* Initialize Trace recorder */
	BSP_OS_TickEnable(); /* Enable the tick timer and interrupt */

#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&os_err); /* Compute CPU capacity with no task running */
#endif
#ifdef CPU_CFG_INT_DIS_MEAS_EN
	CPU_IntDisMeasMaxCurReset();
#endif

	BSP_LED_Init();
	BSP_SWITCH_Init();
	BSP_ADC0_Init();
	BSP_TIMER_Init();
	color = LED_RED;

	 OSTaskCreate(&DutyCycleTaskTCB, "Duty Cycle Task", DutyCycleTask, 0u, APP_CFG_STARTUP_TASK_PRIO, &DutyCycleTaskStk[0u],
	              DutyCycleTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE / 10u], APP_CFG_STARTUP_TASK_STK_SIZE,
	              0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &os_err);

	OSSemCreate((OS_SEM*)&ADC0sem, "ADC0 Semaphore", 0u, &os_err);
	OSSemCreate(&FTM0sem, "FTM0 Semaphore", 1u, &os_err);

	while (DEF_TRUE) {
	//SW2 RED -> GREEN -> BLUE -> RED.
	//SW3 RED -> BLUE -> GREEN -> RED.
		switch(color){
			case LED_GREEN:
				//Entering critical section where FTM_period is used
				OSSemPend(&FTM0sem, 0u, OS_OPT_PEND_BLOCKING, 0u, &os_err);
				BSP_LED_On(color, FTM_period);
				//Exiting critical section
				OSSemPost(&FTM0sem, OS_OPT_POST_ALL, &os_err);
				if(SW2_status);
					color = LED_BLUE;
				if(SW3_status);
					color = LED_RED;
			break;
			case LED_BLUE:
				//Entering critical section where FTM_period is used
				OSSemPend(&FTM0sem, 0u, OS_OPT_PEND_BLOCKING, 0u, &os_err);
				BSP_LED_On(color, FTM_period);
				//Exiting critical section
				OSSemPost(&FTM0sem, OS_OPT_POST_ALL, &os_err);
				if(SW2_status);
					color = LED_RED;
				if(SW3_status);
					color = LED_GREEN;
			break;
			case LED_RED:
			default:
				//Entering critical section where FTM_period is used
				OSSemPend(&FTM0sem, 0u, OS_OPT_PEND_BLOCKING, 0u, &os_err);
				BSP_LED_On(color, FTM_period);
				//Exiting critical section
				OSSemPost(&FTM0sem, OS_OPT_POST_ALL, &os_err);
				if(SW2_status);
					color = LED_GREEN;
				if(SW3_status);
					color = LED_BLUE;
			break;
		}
	}
}

static void  DutyCycleTask (void  *p_arg)
{
	OS_ERR os_err;

	(void)p_arg;

	while (DEF_TRUE) {
		BSP_ADC0_Convert(12);
		//Waiting for conversion to be completed
		OSSemPend((OS_SEM*)&ADC0sem, 0u, OS_OPT_PEND_BLOCKING, 0u, &os_err);
		//Entering critical section where FTM_period is written
		OSSemPend(&FTM0sem, 0u, OS_OPT_PEND_BLOCKING, 0u, &os_err);
		FTM_period = 1.6 * ADC0_result;
		//Exiting critical section
		OSSemPost(&FTM0sem, OS_OPT_POST_ALL, &os_err);
	}
}
