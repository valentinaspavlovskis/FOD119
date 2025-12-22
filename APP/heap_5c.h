#ifndef HEAP_5C_H
#define HEAP_5C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mpu_wrappers.h"
#include "FreeRTOS.h"

/*
 * Used to define multiple heap regions for use by heap_5.c.  This function
 * must be called before any calls to pvPortMalloc() - not creating a task,
 * queue, semaphore, mutex, software timer, event group, etc. will result in
 * pvPortMalloc being called.
 *
 * pxHeapRegions passes in an array of HeapRegion_t structures - each of which
 * defines a region of memory that can be used as the heap.  The array is
 * terminated by a HeapRegions_t structure that has a size of 0.  The region
 * with the lowest start address must appear first in the array.
 */
void vPortDefineHeapRegions2( const HeapRegion_t * const pxHeapRegions ) PRIVILEGED_FUNCTION;


/*
 * Map to the memory management routines required for the port.
 */
void *pvPortMalloc2( size_t xSize ) PRIVILEGED_FUNCTION;
void vPortFree2( void *pv ) PRIVILEGED_FUNCTION;
void vPortInitialiseBlocks2( void ) PRIVILEGED_FUNCTION;
size_t xPortGetFreeHeapSize2( void ) PRIVILEGED_FUNCTION;
size_t xPortGetMinimumEverFreeHeapSize2( void ) PRIVILEGED_FUNCTION;


#ifdef __cplusplus
}
#endif

#endif /* HEAP_5C_H */

