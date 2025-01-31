/*
 *  FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
 *  All rights reserved
 *
 *  VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.
 *
 *  This file is part of the FreeRTOS distribution.
 *
 *  FreeRTOS is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by the
 *  Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.
 *
 ***************************************************************************
 *  >>!   NOTE: The modification to the GPL is included to allow you to     !<<
 *  >>!   distribute a combined work that includes FreeRTOS without being   !<<
 *  >>!   obliged to provide the source code for proprietary components     !<<
 *  >>!   outside of the FreeRTOS kernel.                                   !<<
 ***************************************************************************
 *
 *  FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  Full license text is available on the following
 *  link: http://www.freertos.org/a00114.html
 *
 ***************************************************************************
 *                                                                       *
 *    FreeRTOS provides completely free yet professionally developed,    *
 *    robust, strictly quality controlled, supported, and cross          *
 *    platform software that is more than just the market leader, it     *
 *    is the industry's de facto standard.                               *
 *                                                                       *
 *    Help yourself get started quickly while simultaneously helping     *
 *    to support the FreeRTOS project by purchasing a FreeRTOS           *
 *    tutorial book, reference manual, or both:                          *
 *    http://www.FreeRTOS.org/Documentation                              *
 *                                                                       *
 ***************************************************************************
 *
 *  http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
 *  the FAQ page "My application does not run, what could be wrong?".  Have you
 *  defined configASSERT()?
 *
 *  http://www.FreeRTOS.org/support - In return for receiving this top quality
 *  embedded software for free we request you assist our global community by
 *  participating in the support forum.
 *
 *  http://www.FreeRTOS.org/training - Investing in training allows your team to
 *  be as productive as possible as early as possible.  Now you can receive
 *  FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
 *  Ltd, and the world's leading authority on the world's leading RTOS.
 *
 *  http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
 *  including FreeRTOS+Trace - an indispensable productivity tool, a DOS
 *  compatible FAT file system, and our tiny thread aware UDP/IP stack.
 *
 *  http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
 *  Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.
 *
 *  http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
 *  Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
 *  licenses offer ticketed support, indemnification and commercial middleware.
 *
 *  http://www.SafeRTOS.com - High Integrity Systems also provide a safety
 *  engineered and independently SIL3 certified version for use in safety and
 *  mission critical applications that require provable dependability.
 *
 *  1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stdint.h"
#include <stdio.h>
#ifdef __waf__
    #include "waf_config.h"
#endif

/*-----------------------------------------------------------
* Application specific definitions.
*
* These definitions should be adjusted for your particular hardware and
* application requirements.
*
* THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
* FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
*
* See http://www.freertos.org/a00110.html.
*----------------------------------------------------------*/
/*#define configISR_STACK_SIZE_WORDS      500 // NOTE: if using configISR_STACK_SIZE_WORDS the stack alignment assert doesn't pass */
#define configMTIME_BASE_ADDRESS                   ( configCLINT_BASE_ADDRESS + 0xBFF8UL )
#define configMTIMECMP_BASE_ADDRESS                ( configCLINT_BASE_ADDRESS + 0x4000UL )
#define configUSE_PREEMPTION                       1
#define configUSE_IDLE_HOOK                        1
#define configUSE_TICK_HOOK                        1
#ifndef configCPU_CLOCK_HZ
    #define configCPU_CLOCK_HZ                     ( ( unsigned long ) ( 100000000 ) )
#endif
#ifndef configPERIPH_CLOCK_HZ
    #define configPERIPH_CLOCK_HZ                  configCPU_CLOCK_HZ
#endif
#define configTICK_RATE_HZ                         ( 1000 )
#define configMAX_PRIORITIES                       ( 5 )
#define configMINIMAL_STACK_SIZE                   ( ( uint32_t ) 4096 ) /* Can be as low as 60 but some of the demo tasks that use this constant require it to be higher. */
#define configSTACK_DEPTH_TYPE                     uint32_t              /*the default ifndef is uint16_t */
#ifdef configCUSTOM_HEAP_SIZE
    #define configTOTAL_HEAP_SIZE                  ( ( size_t ) ( configCUSTOM_HEAP_SIZE * 1024 * 1024 ) )
#else
    #define configTOTAL_HEAP_SIZE                  ( ( size_t ) ( 1024 * 1024 * 32 ) )
#endif
#define configMAX_TASK_NAME_LEN                    ( 16 )
#define configUSE_TRACE_FACILITY                   1
#define configUSE_16_BIT_TICKS                     0
#define configIDLE_SHOULD_YIELD                    0
#define configUSE_MUTEXES                          1
#define configQUEUE_REGISTRY_SIZE                  8
#define configCHECK_FOR_STACK_OVERFLOW             2
#define configUSE_RECURSIVE_MUTEXES                1
#define configUSE_MALLOC_FAILED_HOOK               1
#define configUSE_APPLICATION_TASK_TAG             0
#define configUSE_COUNTING_SEMAPHORES              1

#define configSUPPORT_STATIC_ALLOCATION            1
/* FreeRTOS+FAT requires 2 pointers if a CWD is supported. */
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS    3

/* TODO: use only for debugging */
#if DEBUG
#define configGENERATE_RUN_TIME_STATS              1
#define configRECORD_STACK_HIGH_ADDRESS            ( 1 )
#define INCLUDE_uxTaskGetStackHighWaterMark        1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#define configPORT_ALLOW_APP_EXCEPTION_HANDLERS    1
#endif

/* Runtime stats definitions */
/* TODO: use only for debugging */
#define configUSE_STATS_FORMATTING_FUNCTIONS    1
extern uint32_t port_get_current_mtime( void );
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE()    port_get_current_mtime()

/* Make newlib reentrant */
/* See http://www.nadler.com/embedded/newlibAndFreeRTOS.html */
/* Required for thread-safety of newlib sprintf and friends */
/* NOTE: this feature is optional */
#define configUSE_NEWLIB_REENTRANT         0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES              0
#define configMAX_CO_ROUTINE_PRIORITIES    ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS                   1
#define configTIMER_TASK_PRIORITY          ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH           4
#define configTIMER_TASK_STACK_DEPTH       ( configMINIMAL_STACK_SIZE )

/* Task priorities.  Allow these to be overridden. */
#ifndef uartPRIMARY_PRIORITY
    #define uartPRIMARY_PRIORITY    ( configMAX_PRIORITIES - 3 )
#endif

/* If configINCLUDE_DEMO_DEBUG_STATS is set to one, then a few basic IP trace
 * macros are defined to gather some UDP stack statistics that can then be viewed
 * through the CLI interface. */
#define configINCLUDE_DEMO_DEBUG_STATS       1

/* The size of the global output buffer that is available for use when there
 * are multiple command interpreters running at once (for example, one on a UART
 * and one on TCP/IP).  This is done to prevent an output buffer being defined by
 * each implementation - which would waste RAM.  In this case, there is only one
 * command interpreter running, and it has its own local output buffer, so the
 * global buffer is just set to be one byte long as it is not used and should not
 * take up unnecessary RAM. */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE    1

/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function. */
#define INCLUDE_vTaskPrioritySet             1
#define INCLUDE_uxTaskPriorityGet            1
#define INCLUDE_vTaskDelete                  1
#define INCLUDE_vTaskCleanUpResources        1
#define INCLUDE_vTaskSuspend                 1
#define INCLUDE_vTaskDelayUntil              1
#define INCLUDE_vTaskDelay                   1
#define INCLUDE_eTaskGetState                1
#define INCLUDE_xTimerPendFunctionCall       1
#define INCLUDE_xTaskAbortDelay              1
#define INCLUDE_xTaskGetHandle               1
#define INCLUDE_xSemaphoreGetMutexHolder     1

/* Normal assert() semantics without relying on the provision of an assert.h
 * header file. */
#define configASSERT( x )            \
    if( ( x ) == 0 )                 \
    {                                \
        taskDISABLE_INTERRUPTS();    \
        __asm volatile ( "ebreak" ); \
        for( ; ; )                   \
        ;                            \
        }

#endif /* FREERTOS_CONFIG_H */
