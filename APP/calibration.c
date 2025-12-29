/******************** (C) COPYRIGHT 2017  **************************************
* File Name          : calibration.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 19/3/2015
* Description        : File description
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "calibration.h"
#include "drv_eeprom.h"
#include "string.h"
#include "crc16.h"
#include "iwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CalibrationTypeDef CalibrationValues;
float  cal_coef = 0.0;
//static CalibrationTypeDef WriteCalibrationValues;
/* Private function prototypes -----------------------------------------------*/
static int WriteDataToFLASH(uint32_t FlashFree_Address, int32_t *Data, uint32_t Size);
static ErrorStatus FLASH_ProgramWord(uint32_t Address, uint32_t Data);
static ErrorStatus EraseFLASH_Area(uint32_t Page_Address, uint32_t data_sz);
/*******************************************************************************
* Function Name  : FLASH_If_Read
* Description           : Read sectors
* @param  SectorAddress : start address for read data
* Return                : buffer address pointer
*******************************************************************************/
static uint8_t *FLASH_If_Read (uint32_t SectorAddress);
/*******************************************************************************
* Function Name         : FLASH_If_Read_Buff
* Description           : This function read a data buffer from eeprom.
* @param  SectorAddress : start address for read data
* @param  Buffer        : pointer on data buffer
* @param  DataLength    : length of data buffer (unit is 32-bit word)
* @retval 0             : Data successfully read FLASH memory
*******************************************************************************/
static uint8_t FLASH_If_Read_Buff (uint32_t SectorAddress,unsigned char *Buffer, uint32_t DataLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Function description
  * @param  param1
  * @param  param2
  * @retval None
  */
void LoadCalibrationData(void){
  ReadCalibration(&CalibrationValues); 
}

/**
  * @brief  Function description
  * @param  param1
  * @param  param2
  * @retval None
  */
bool ReadCalibration(CalibrationTypeDef* CalibValues){
    
  unsigned int i = sizeof(CalibrationTypeDef);

  EEPROM_If_Read_Buff (CALIBRATION_VALUES_AREA_BEG_ADDR,
                         (unsigned char *)CalibValues, 
                         i);
  
/*  FLASH_If_Read_Buff(CALIBRATION_VALUES_AREA_BEG_ADDR,
                         (unsigned char *)CalibValues, 
                         sizeof(CalibrationTypeDef));
*/  
  if(CalibValues->cbMagic != MAGIC){
    return false;
  }
  /*##-1- Configure the CRC peripheral #######################################*/
  CRC16Init();
  /*##-2- Compute the CRC of "DataBase" ###################################*/
  CRC16Calc ((uint8_t *)CalibValues, sizeof(CalibrationTypeDef));
  /*##-3- Compare the CRC value to the Expected one ##########################*/
  if (CRC16GetValue() != 0x0000){
    memset(CalibValues,0,sizeof(CalibrationTypeDef));
    return false;
  }
  
  return true;
}

bool ReadTestCalibration(CalibrationTypeDef* CalibValues){

  return true;
}

/**
  * @brief  Function description
  * @param  param1
  * @retval None
  */
uint16_t WriteCalibration(CalibrationTypeDef* CalibValues){
  uint16_t err = 0;
  uint32_t data_size = sizeof(CalibrationTypeDef);

  CRC16Init();
  
  CalibValues->cbMagic = MAGIC;

  CalibValues->cbCRC = CRC16Calc ((uint8_t *)CalibValues, data_size-2);
    
  err = EEPROM_If_Write_ByteBuff (CALIBRATION_VALUES_AREA_BEG_ADDR,(unsigned char *)CalibValues, sizeof(CalibrationTypeDef));
  
  /* Unlock the Flash to enable the flash control register access *************/
//  HAL_FLASH_Unlock();
//  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY);
//  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
//  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR);
//  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_FWWERR);
//  
//  if(EraseFLASH_Area(CALIBRATION_VALUES_AREA_BEG_ADDR,  data_size) == ERROR){
//    HAL_FLASH_Lock();
//    return 1;
//  }
//   
//  if(WriteDataToFLASH(CALIBRATION_VALUES_AREA_BEG_ADDR, (int32_t *)CalibValues, data_size) == ERROR){
//    HAL_FLASH_Lock();
//    return 1;
//  }
//    
//  HAL_FLASH_Lock();
    
  return err;
}



/**
  * @brief  Erases a specified FLASH pages.
  * @param  Page_Address: The page address to be erased.
  * @retval None
  */
static ErrorStatus EraseFLASH_Area(uint32_t Page_Address, uint32_t data_sz)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  ErrorStatus UPGR_FlashStatus = SUCCESS; 
  uint32_t PAGEError = 0;

  uint32_t NbPages =  (data_sz/FLASH_PAGE_SIZE) + ((data_sz%FLASH_PAGE_SIZE)?1:0);
  
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = Page_Address;
  EraseInitStruct.NbPages     = NbPages;
    
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
  {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PAGEError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    UPGR_FlashStatus = ERROR;
  }
  return UPGR_FlashStatus;
}

/**
  * @brief  Write Data in data buffer to Flash.
  * @param  FlashFree_Address: Page address
  * @param  Data: pointer to data buffer
  * @param  Size: data buffer size in bytes
  * @retval FLASH programming status
  */
static int WriteDataToFLASH(uint32_t FlashFree_Address, int32_t *Data, uint32_t Size)
{
  ErrorStatus UPGR_FlashStatus = SUCCESS;
  uint32_t Address = FlashFree_Address;
  uint32_t *data_ptr = (uint32_t *)Data;
  HAL_StatusTypeDef status;
  uint32_t NbPages =  (Size/(FLASH_PAGE_SIZE/2)) + ((Size%(FLASH_PAGE_SIZE/2))?1:0);

  while(NbPages--){
    /* Reset watchdog */
    MX_IWDG_Reset();
    
    status = HAL_FLASHEx_HalfPageProgram(Address, data_ptr);
    Address += (FLASH_PAGE_SIZE/2);
    data_ptr += (FLASH_PAGE_SIZE/8);//(FLASH_PAGE_SIZE/4);
    if(status != HAL_OK){
      UPGR_FlashStatus = ERROR;
    }
    
    /* Reset watchdog */
    MX_IWDG_Reset();
  }
  
/*  
  uint32_t words = (Size/sizeof(uint32_t)) + ((Size%sizeof(uint32_t))?1:0);

  for(index = 0; index < words; index++)
  {
     /// Writing calibration parameters to the Flash Memory 
    UPGR_FlashStatus = FLASH_ProgramWord( FlashFree_Address, Data[index]);
    // Increasing Flash Memory Page Address 
    FlashFree_Address = FlashFree_Address + 4;
  }
*/
  return UPGR_FlashStatus;
}

/**
  * @brief  Programs a word at a specified address.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed
  * @retval None
  */
static ErrorStatus FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
   if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data) == HAL_OK){
      return SUCCESS;
   }else{
      return ERROR;
   }
} 

/*******************************************************************************
* Function Name  : FLASH_If_Read
* Description           : Read sectors
* @param  SectorAddress : start address for read data
* Return                : buffer address pointer
*******************************************************************************/
static uint8_t *FLASH_If_Read (uint32_t SectorAddress)
{
  return  (uint8_t*)(SectorAddress);
}

/*******************************************************************************
* Function Name         : FLASH_If_Read_Buff
* Description           : This function read a data buffer from eeprom.
* @param  SectorAddress : start address for read data
* @param  Buffer        : pointer on data buffer
* @param  DataLength    : length of data buffer (unit is 32-bit word)
* @retval 0             : Data successfully read FLASH memory
*******************************************************************************/
static uint8_t FLASH_If_Read_Buff (uint32_t SectorAddress,unsigned char *Buffer, uint32_t DataLength)
{
  uint32_t ReadLength = 0;
  if(SectorAddress >= FLASH_BASE && SectorAddress <= FLASH_BANK1_END){
     ReadLength = FLASH_BANK1_END - SectorAddress;
  }else if (SectorAddress >= FLASH_BANK2_BASE && SectorAddress <= FLASH_BANK2_END){
     ReadLength = FLASH_BANK2_END - SectorAddress;
  }else{
    return 1;
  }
  
  for (uint32_t idx = 0; idx <  DataLength; idx++)
  {
    if(((SectorAddress + idx) > FLASH_BANK2_END) || idx > ReadLength){
      Buffer[idx] = 0xFF;
    }else{
      Buffer[idx] = *FLASH_If_Read(SectorAddress + idx);
    }
  }
  
  return 0;
}



void drv_LoadCalTable(void){
  if(ReadCalibration(&CalibrationValues)){
    if(CalibrationValues.cbMagic == MAGIC){
      //cal_coef = CalibrationValues.cbCalCoef;
    }
  }
}

uint16_t drv_SaveCalTable(void){ 
  //CalibrationValues.cbCalCoef = cal_coef;
  return WriteCalibration(&CalibrationValues);
}

void SetCalPowerWL(uint16_t wl){
  //CalibrationValues.cbWL = wl;
}

void SetCalValue(float val){
  cal_coef = val;
}

float GetCalValue(void){
  return cal_coef;
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/