/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.c
  * @version        : v2.0_Cube
  * @brief          : This file implements the USB Device
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN Includes */
#include "usbd_iad.h"
#include "memory.h"
#include "drv_eeprom.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* Firmware version */
const uint8_t* default_firmware_version = "v0.0.01";
/* Default Product name */
const uint8_t* default_product_name = "Power meter";
/* Default Serial Number */
const uint8_t* default_serial_number = "01234567";
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static int8_t drv_hw_DataBaseRecord_isValid(const uint8_t *addr, int16_t sz);
static void drv_hw_DataBaseRecord_write(const uint8_t *str, uint8_t *record_addr, int16_t record_size);
/* USER CODE END PFP */

/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceFS;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
/**
  * @brief  Calculate database checksum.
  * @param  addr : memory address.
  * @param  len : length of bytes included in checksum calculation.
  * @retval Checksum
  */
static uint8_t drv_hw_DataBase_checksum(const uint8_t *addr, int16_t len)
{
  int i;
  uint8_t checksum = 0;
  for(i=0; i<len; i++){
    checksum += *addr++;
  }
  return checksum;
}

/**
  * @brief  Check if database record is valid.
  * @param  addr : memory address.
  * @param  sz : record size in bytes.
  * @retval non zero if valid
  */
static int8_t drv_hw_DataBaseRecord_isValid(const uint8_t *addr, int16_t sz)
{
  int8_t valid;

  valid = 0;
  if(*addr == HW_DATABASE0_FIELD_MAGIC){
    uint8_t checksum = drv_hw_DataBase_checksum(addr, sz - HW_DATABASE0_FIELD_CHECKSUM_SIZE);
    if(*(addr+sz-HW_DATABASE0_FIELD_CHECKSUM_SIZE) == checksum){
      valid = 1;
    }
  }
  return valid;
}

/**
  * @brief  Write Product name record to database.
  * @param  str : pointer to string.
  * @retval non zero if valid
  */
void drv_hw_ProductNameDatabaseWrite(const uint8_t *str)
{
  drv_hw_DataBaseRecord_write(str, (uint8_t*)HW_DATABASE0_RECORD_PRODUCT_ADDR, HW_DATABASE0_RECORD_PRODUCT_SIZE);
}

/**
  * @brief  Write Serial Number record to database.
  * @param  str : pointer to string.
  * @retval non zero if valid
  */
void drv_hw_SerialNumberDatabaseWrite(const uint8_t *str)
{
  drv_hw_DataBaseRecord_write(str, (uint8_t*)HW_DATABASE0_RECORD_SERIALNUM_ADDR, HW_DATABASE0_RECORD_SERIALNUM_SIZE);
}


/**
  * @brief  Write record to database.
  * @param  str : pointer to string.
  * @param  record_addr : record address in memory.
  * @param  record_size : record size in bytes.
  * @retval non zero if valid
  */
static void drv_hw_DataBaseRecord_write(const uint8_t *str, uint8_t *record_addr, int16_t record_size)
{
  uint32_t addr;
  uint8_t checksum;
  uint8_t *record;
  size_t slen = strlen((char *)str);

  record = mem_calloc( record_size , sizeof(uint8_t));
  
  record[0] = HW_DATABASE0_FIELD_MAGIC;
  if((slen+HW_DATABASE0_FIELD_MAGIC_SIZE+HW_DATABASE0_FIELD_CHECKSUM_SIZE) > record_size){
    memcpy(record+HW_DATABASE0_FIELD_MAGIC_SIZE,str,(record_size - HW_DATABASE0_FIELD_MAGIC_SIZE - HW_DATABASE0_FIELD_CHECKSUM_SIZE));
  }else{
    memcpy(record+HW_DATABASE0_FIELD_MAGIC_SIZE,str,slen);
  } 

  /* Calculate checksum */
  checksum = drv_hw_DataBase_checksum(&record[0], record_size - HW_DATABASE0_FIELD_CHECKSUM_SIZE);
  
  /* Write checksum */
  memcpy(&record[record_size - HW_DATABASE0_FIELD_MAGIC_SIZE],&checksum,HW_DATABASE0_FIELD_MAGIC_SIZE);  
    
  /* Write */
  addr = (uint32_t)record_addr;  
  EEPROM_If_Write_ByteBuff(addr,record,record_size);
  
  mem_free( record );
}

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */
void IAD_USB_DEVICE_Init(void)
{
  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */
  
  /* USER CODE END USB_DEVICE_Init_PreTreatment */
  
  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_IAD_ClassDriver) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }
  
  /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */
  
  /* USER CODE END USB_DEVICE_Init_PostTreatment */
}

void IAD_USB_DEVICE_DeInit(void){

  PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *)hUsbDeviceFS.pData;

  if (hpcd != NULL)
  {
      if (HAL_PCD_DevDisconnect(hpcd) != HAL_OK)
      {
        Error_Handler();
      }
      if (USBD_DeInit(&hUsbDeviceFS) != USBD_OK)
      {
        Error_Handler();
      }
  }
  
}

/**
  * @brief  Get pointer to Frimware Version String.
  * @param  None.
  * @retval pointer to Frimware Version String
  */
const uint8_t* drv_hw_FrimwareVersionPtrGet(void)
{
  const uint8_t* ptr;
  ptr = default_firmware_version;
  return ptr;
}

/**
  * @brief  Get pointer to Product Name String.
  * @param  None.
  * @retval pointer to Product Name String
  */
const uint8_t* drv_hw_ProductNamePtrGet(void)
{
  const uint8_t* ptr;
  ptr = default_product_name;
  //name corection 
  if(drv_hw_DataBaseRecord_isValid((uint8_t*)HW_DATABASE0_RECORD_PRODUCT_ADDR, HW_DATABASE0_RECORD_PRODUCT_SIZE)){
    ptr = (uint8_t*)(HW_DATABASE0_RECORD_PRODUCT_ADDR + HW_DATABASE0_FIELD_MAGIC_SIZE);
  }
  return ptr;
}

/**
  * @brief  Get pointer to Serial Number String.
  * @param  None.
  * @retval pointer to Serial Number String
  */
const uint8_t* drv_hw_SerialNumberPtrGet(void)
{
  const uint8_t* ptr;
  ptr = default_serial_number;
  if(drv_hw_DataBaseRecord_isValid((uint8_t*)HW_DATABASE0_RECORD_SERIALNUM_ADDR, HW_DATABASE0_RECORD_SERIALNUM_SIZE)){
    ptr = (uint8_t*)(HW_DATABASE0_RECORD_SERIALNUM_ADDR + HW_DATABASE0_FIELD_MAGIC_SIZE);
  }
  return ptr;
}
/* USER CODE END 1 */

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_DEVICE_Init(void)
{
  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */
  
  /* USER CODE END USB_DEVICE_Init_PreTreatment */

  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CUSTOM_HID) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS, &USBD_CustomHID_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */
  
  /* USER CODE END USB_DEVICE_Init_PostTreatment */
}

/**
  * @}
  */

/**
  * @}
  */

