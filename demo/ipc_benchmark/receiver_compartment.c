/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021 Hesham Almatary
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "logging.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "portstatcounters.h"

typedef struct taskParams
{
    UBaseType_t xBufferSize;
    UBaseType_t xTotalSize;
    QueueHandle_t xQueue;
    TaskHandle_t mainTask;
} IPCTaskParams_t;
/*-----------------------------------------------------------*/

extern uint64_t xStartTime;
extern uint64_t xStartInstRet;
extern uint64_t xStartDCacheLoad;
extern uint64_t xStartDCacheMiss;
extern uint64_t xEndDCacheMiss;
extern uint64_t xStartICacheLoad;
extern uint64_t xStartICacheMiss;
extern uint64_t xStartL2CacheMiss;
extern uint64_t xEndTime;
extern uint64_t xEndInstRet;
extern uint64_t xEndDCacheLoad;
extern uint64_t xEndDCacheMiss;
extern uint64_t xEndICacheLoad;
extern uint64_t xEndICacheMiss;
extern uint64_t xEndL2CacheMiss;
/*-----------------------------------------------------------*/

void queueReceiveTask( void * pvParameters );

void queueReceiveTask( void * pvParameters )
{
    UBaseType_t xTotalSize = IPC_TOTAL_SIZE;
    UBaseType_t xBufferSize = IPC_BUFFER_SIZE;
    QueueHandle_t xQueue = NULL;

    IPCTaskParams_t * params = ( IPCTaskParams_t * ) pvParameters;

    if( params != NULL )
    {
        xBufferSize = params->xBufferSize;
        xTotalSize = params->xTotalSize;
        xQueue = params->xQueue;
    }

    unsigned int cnt = xTotalSize / xBufferSize;

    if( !cnt )
    {
        log( "Invalid biffer sizes\n" );
    }

    char * pReceiveBuffer = pvPortMalloc( xBufferSize );

    if( !pReceiveBuffer )
    {
        log( "Failed to allocated a receive buffer of size %d\n", xBufferSize );
    }

    /* Zero the buffer to warm up the cache */
    memset( pReceiveBuffer, 0, xBufferSize );

    for( int i = 0; i < DISCARD_RUNS; i++ )
    {
        xQueueReceive( xQueue, pReceiveBuffer, portMAX_DELAY );
    }

    for( int i = 0; i < cnt; i++ )
    {
        /* Wait until something arrives in the queue - this task will block
         * indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
         * FreeRTOSConfig.h. */
        xQueueReceive( xQueue, pReceiveBuffer, portMAX_DELAY );
    }

    xEndTime = get_cycle_count();
    xEndInstRet = portCounterGet(COUNTER_INSTRET);

    xEndDCacheLoad = portCounterGet(COUNTER_DCACHE_LOAD);
    xEndDCacheMiss = portCounterGet(COUNTER_DCACHE_LOAD_MISS);
    xEndICacheLoad = portCounterGet(COUNTER_ICACHE_LOAD);
    xEndICacheMiss = portCounterGet(COUNTER_ICACHE_LOAD_MISS);
    xEndL2CacheMiss = portCounterGet(COUNTER_LLCACHE_LOAD_MISS);

    portENABLE_INTERRUPTS();

    log( "IPC Performance Results for: buffer size: %lu - total size: %lu\n"
         "\tCYCLE:\t\t%lu\n"
         "\tINSTRET:\t%lu\n"
         "\tDCACHE_LOAD:\t%lu\n"
         "\tDCACHE_LOAD_MISS:\t%lu\n"
         "\tICACHE_LOAD:\t%lu\n"
         "\tICACHE_LOAD_MISS:\t%lu\n"
         "\tLLCACHE_LOAD_MISS:\t%lu\n",
         xBufferSize, xTotalSize,
         xEndTime - xStartTime,
         xEndInstRet - xStartInstRet,
         xEndDCacheLoad - xStartDCacheLoad,
         xEndDCacheMiss - xStartDCacheMiss,
         xEndICacheLoad - xStartICacheLoad,
         xEndICacheMiss - xStartICacheMiss,
         xEndL2CacheMiss - xStartL2CacheMiss
         );

    vPortFree( pReceiveBuffer );
    /* Notify main task we are finished */
    xTaskNotifyGive( params->mainTask );
    vTaskDelete( NULL );

    while( 1 )
    {
    }
}
/*-----------------------------------------------------------*/
