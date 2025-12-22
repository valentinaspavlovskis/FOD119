#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <stdio.h>
#include "heap_5c.h"


/* Base address of the Flash sectors */

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 4 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08001000) /* Base @ of Sector 1, 4 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08002000) /* Base @ of Sector 2, 4 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08003000) /* Base @ of Sector 3, 4 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08004000) /* Base @ of Sector 4, 4 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08005000) /* Base @ of Sector 5, 4 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08006000) /* Base @ of Sector 6, 4 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08007000) /* Base @ of Sector 7, 4 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08008000) /* Base @ of Sector 8, 4 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x08009000) /* Base @ of Sector 9, 4 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x0800A000) /* Base @ of Sector 10, 4 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x0800B000) /* Base @ of Sector 11, 4 Kbytes */
#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x0800C000) /* Base @ of Sector 12, 4 Kbytes */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x0800D000) /* Base @ of Sector 13, 4 Kbytes */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x0800E000) /* Base @ of Sector 14, 4 Kbytes */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0800F000) /* Base @ of Sector 15, 4 Kbytes */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08010000) /* Base @ of Sector 0, 4 Kbytes */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08011000) /* Base @ of Sector 1, 4 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08012000) /* Base @ of Sector 2, 4 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08013000) /* Base @ of Sector 3, 4 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08014000) /* Base @ of Sector 4, 4 Kbytes */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x08015000) /* Base @ of Sector 5, 4 Kbytes */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x08016000) /* Base @ of Sector 6, 4 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x08017000) /* Base @ of Sector 7, 4 Kbytes */
#define ADDR_FLASH_SECTOR_24     ((uint32_t)0x08018000) /* Base @ of Sector 8, 4 Kbytes */
#define ADDR_FLASH_SECTOR_25     ((uint32_t)0x08019000) /* Base @ of Sector 9, 4 Kbytes */
#define ADDR_FLASH_SECTOR_26     ((uint32_t)0x0801A000) /* Base @ of Sector 10, 4 Kbytes */
#define ADDR_FLASH_SECTOR_27     ((uint32_t)0x0801B000) /* Base @ of Sector 11, 4 Kbytes */
#define ADDR_FLASH_SECTOR_28     ((uint32_t)0x0801C000) /* Base @ of Sector 12, 4 Kbytes */
#define ADDR_FLASH_SECTOR_29     ((uint32_t)0x0801D000) /* Base @ of Sector 13, 4 Kbytes */
#define ADDR_FLASH_SECTOR_30     ((uint32_t)0x0801E000) /* Base @ of Sector 14, 4 Kbytes */
#define ADDR_FLASH_SECTOR_31     ((uint32_t)0x0801F000) /* Base @ of Sector 15, 4 Kbytes */

/* Flash Memory map */
#define IMAGE0_AREA_BEG_ADDR	        ADDR_FLASH_SECTOR_0

#define IMAGE1_AREA_BEG_ADDR            ADDR_FLASH_SECTOR_16

#define DATABASE_AREA_BEG_ADDR	        ADDR_FLASH_SECTOR_30

/**
  * @brief  Allocate and zero-initialize array
  * @param  xSize: Size of element
  * @retval Memory address poiter 
  */
void * mem_calloc( size_t N, size_t S );

/**
  * @brief  Allocate memory block
  * @param  xSize: Size of element
  * @retval Memory address poiter 
  */
void * mem_malloc( size_t xSize );

/**
  * @brief  Deallocate memory block
  * @param  ptr: point to a block of memory allocated
  * @retval None 
  * @note If ptr is a null pointer, the function does nothing
  */
void mem_free( void *ptr );

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address);

#endif   //__MEMORY_H_