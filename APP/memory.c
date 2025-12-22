/**
  ******************************************************************************
  * @file    memory.c
  * @author
  * @version V0.0.1
  * @date    2016-03-22
  * @brief   This file contains
  *          functions for use RAM memory
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "memory.h"
#include <string.h>
#include <stdlib.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define NATIVE_HEAP 1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Allocate and zero-initialize array
  * @param  xSize: Size of element
  * @retval Memory address poiter 
  */
void * mem_calloc(size_t N, size_t S )
{
#if NATIVE_HEAP
  void * pv = NULL;
  vTaskSuspendAll();
  pv = calloc(N, S);
  xTaskResumeAll();
  return pv;
#else
  void *retp = pvPortMalloc2(N * S);
  memset(retp,0, N * S);
  return retp;
#endif
}

/**
  * @brief  Allocate memory block
  * @param  xSize: Size of element
  * @retval Memory address poiter 
  */
void * mem_malloc( size_t xSize )
{
#if NATIVE_HEAP
  void * pv = NULL;
  vTaskSuspendAll();
  pv = malloc(xSize);
  xTaskResumeAll();
  return pv;
#else
  return pvPortMalloc2(xSize);
#endif
}

/**
  * @brief  Deallocate memory block
  * @param  ptr: point to a block of memory allocated
  * @retval None 
  * @note If ptr is a null pointer, the function does nothing
  */
void mem_free( void *ptr ){
  if(ptr){
#if NATIVE_HEAP
    vTaskSuspendAll();
    free(ptr);
    xTaskResumeAll();
#else
    vPortFree2(ptr);
#endif
  }
}
/******************* (C) COPYRIGHT 2016 Lifodas *****END OF FILE****/
