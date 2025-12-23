/******************** (C) COPYRIGHT 2017  **************************************
* File Name          : drv_eeprom.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 19/2/2017
* Description        : Functions for use EEPROM memory
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "drv_eeprom.h"
#include "stm32l0xx_hal_flash_ex.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define DATA_EEPROM_BASE       ((uint32_t)0x08080000U) /*!< DATA_EEPROM base address in the alias region */
//#define DATA_EEPROM_BANK2_BASE ((uint32_t)0x08080C00U) /*!< DATA EEPROM BANK2 base address in the alias region */
//#define DATA_EEPROM_BANK1_END  ((uint32_t)0x08080BFFU) /*!< Program end DATA EEPROM BANK1 address */
//#define DATA_EEPROM_BANK2_END  ((uint32_t)0x080817FFU) /*!< Program end DATA EEPROM BANK2 address */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EEPROM_If_Read
  * @param  start address for read data
  * @retval buffer address pointer
  */
static uint8_t *EEPROM_If_Read (uint32_t SectorAddress);

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
uint16_t EEPROM_If_Write_Buff(uint32_t SectorAddress,unsigned char *Buffer, uint32_t DataLength)
{
  uint32_t idx = 0;
    
  if  (DataLength & 0x3) /* Not an aligned data */
  {
    for (idx = DataLength; idx < ((DataLength & 0xFFFC) + 4); idx++)
    {
      Buffer[idx] = 0xFF;
    }
  }
  HAL_FLASHEx_DATAEEPROM_Unlock();

  /* Data received are Word multiple */
  for (idx = 0; idx <  DataLength; idx = idx + 4)
  {
    if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,SectorAddress,*(uint32_t *)(Buffer + idx))!= HAL_OK){
      HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Lock();
      return 0;
    }
    HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Lock();
    SectorAddress += 4;
  }
  return 1;
}

/*******************************************************************************
* Function Name         : EEPROM_If_Write_ByteBuff
* Description           : This function writes a data buffer in eeprom.
* @param  SectorAddress : start address for writing data buffer
* @param  Buffer        : pointer on data buffer
* @param  DataLength    : length of data buffer (unit is 32-bit word)
* @retval 0             : Data successfully written to EEPROM memory
*         1             : Error occurred while writing data in EEPROM memory
*         2             : Written Data in flash memory is different from expected one
*******************************************************************************/
uint8_t EEPROM_If_Write_ByteBuff(uint32_t SectorAddress,unsigned char *Buffer, uint32_t DataLength)
{
  uint32_t idx = 0;
  uint32_t SaveLength = DataLength;
/*  
  if(SectorAddress >= DATA_EEPROM_BASE ||  SectorAddress <= DATA_EEPROM_BANK1_END){
    if(DataLength > (DATA_EEPROM_BANK1_END - SectorAddress)){
      SaveLength = DATA_EEPROM_BANK1_END - SectorAddress;
    }
  }else if (SectorAddress >= DATA_EEPROM_BANK2_BASE ||  SectorAddress <= DATA_EEPROM_BANK2_END){
    if(DataLength > (DATA_EEPROM_BANK2_END - SectorAddress)){
      SaveLength = DATA_EEPROM_BANK2_END - SectorAddress;
    }
    */
  
  if(SectorAddress >= DATA_EEPROM_BASE ||  SectorAddress <= DATA_EEPROM_BANK2_END){
    if(DataLength > (DATA_EEPROM_BANK2_END - SectorAddress)){
      SaveLength = DATA_EEPROM_BANK2_END - SectorAddress;
    }
  }else{
    return 2;
  }
  HAL_FLASHEx_DATAEEPROM_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_FWWERR);
  
  /* Data received are byre multiple */
  for (idx = 0; idx <  SaveLength; idx++)
  {
    if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,SectorAddress,*(uint8_t *)(Buffer + idx))!= HAL_OK){
      HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Lock();
      return 1;
    }
    HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Lock();
    SectorAddress++;
  }
  return 0;
}


/*******************************************************************************
* Function Name  : EEPROM_If_Read
* Description           : Read sectors
* @param  SectorAddress : start address for read data
* Return                : buffer address pointer
*******************************************************************************/
static uint8_t *EEPROM_If_Read (uint32_t SectorAddress)
{
  return  (uint8_t*)(SectorAddress);
}

/*******************************************************************************
* Function Name         : EEPROM_If_Read_Buff
* Description           : This function read a data buffer from eeprom.
* @param  SectorAddress : start address for read data
* @param  Buffer        : pointer on data buffer
* @param  DataLength    : length of data buffer (unit is 32-bit word)
* @retval 0             : Data successfully read EEPROM memory
*******************************************************************************/
uint8_t EEPROM_If_Read_Buff (uint32_t SectorAddress,unsigned char *Buffer, uint32_t DataLength)
{
  uint32_t ReadLength = 0;
  if(SectorAddress >= DATA_EEPROM_BASE && SectorAddress <= DATA_EEPROM_BANK1_END){
     ReadLength = DATA_EEPROM_BANK1_END - SectorAddress;
  }else if (SectorAddress >= DATA_EEPROM_BANK2_BASE && SectorAddress <= DATA_EEPROM_BANK2_END){
     ReadLength = DATA_EEPROM_BANK2_END - SectorAddress;
  }else{
    return 1;
  }
  
  for (uint32_t idx = 0; idx <  DataLength; idx++)
  {
    if(((SectorAddress + idx) > DATA_EEPROM_BANK2_END) || idx > ReadLength){
      Buffer[idx] = 0xFF;
    }else{
      Buffer[idx] = *EEPROM_If_Read(SectorAddress + idx);
    }
  }
  
  return 0;
}
  


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/