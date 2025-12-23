/******************** (C) COPYRIGHT 2017  **************************************
* File Name          : drv_eeprom.h
* Author             : vpv
* Version            : V0.0.1
* Date               : 19/3/2017
* Description        : Description functons prototypes
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define EEPROM_BANK1_SIZE (DATA_EEPROM_BANK1_END - DATA_EEPROM_BASE)
#define EEPROM_BANK2_SIZE (DATA_EEPROM_BANK2_END - DATA_EEPROM_BANK2_BASE)
/* Exported macro ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
  
/*******************************************************************************
* Function Name         : EEPROM_If_Write_Buff
* Description           : This function writes a data buffer in eeprom.
* @param  SectorAddress : start address for writing data buffer
* @param  Buffer        : pointer on data buffer
* @param  DataLength    : length of data buffer (unit is 32-bit word)
* @retval 0             : Data successfully written to EEPROM memory
*         1             : Error occurred while writing data in EEPROM memory
*         2             : Written Data in flash memory is different from expected one
*******************************************************************************/
uint16_t EEPROM_If_Write_Buff(uint32_t SectorAddress,unsigned char *Buffer, uint32_t DataLength);

/*******************************************************************************
* Function Name         : EEPROM_If_Read_Buff
* Description           : This function read a data buffer from eeprom.
* @param  SectorAddress : start address for read data
* @param  Buffer        : pointer on data buffer
* @param  DataLength    : length of data buffer (unit is 32-bit word)
* @retval 0             : Data successfully read EEPROM memory
*******************************************************************************/
uint8_t EEPROM_If_Read_Buff (uint32_t SectorAddress,unsigned char *Buffer, uint32_t DataLength);


uint8_t EEPROM_If_Write_ByteBuff(uint32_t SectorAddress,unsigned char *Buffer, uint32_t DataLength);

#ifdef __cplusplus
}
#endif

#endif /* __EEPROM_H*/