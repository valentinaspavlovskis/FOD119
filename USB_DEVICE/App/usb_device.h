/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.h
  * @version        : v2.0_Cube
  * @brief          : Header for usb_device.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DEVICE__H__
#define __USB_DEVICE__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"
#include "usbd_def.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup USBD_OTG_DRIVER
  * @{
  */

/** @defgroup USBD_DEVICE USBD_DEVICE
  * @brief Device file for Usb otg low level driver.
  * @{
  */

/** @defgroup USBD_DEVICE_Exported_Variables USBD_DEVICE_Exported_Variables
  * @brief Public variables.
  * @{
  */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
#define DATA_EEPROM_START_ADDR     (0x08080000)
#define DATA_EEPROM_END_ADDR       (0x08081FFF)
    
/* DataBase used to hold device parameters */
#define HW_DATABASE0_BASE_OFFSET        (0)
#define HW_DATABASE0_BASE_ADDR          (DATA_EEPROM_START_ADDR + HW_DATABASE0_BASE_OFFSET)
    
/* Product name descriptor string record */
/* Magic field*/
#define HW_DATABASE0_FIELD_MAGIC            (0xa5)
#define HW_DATABASE0_FIELD_MAGIC_SIZE       (1)
/* Checksum Field */
#define HW_DATABASE0_FIELD_CHECKSUM_SIZE    (1)
/* Product name descriptor string */
#define HW_DATABASE0_FIELD_PRODUCT_SIZE     (16+1)
/* Serial Number descriptor string */
#define HW_DATABASE0_FIELD_SERIALNUM_SIZE   (16+1) 
    
/**/
/* Product name descriptor string record */
#define HW_DATABASE0_RECORD_PRODUCT_SIZE     (HW_DATABASE0_FIELD_MAGIC_SIZE + HW_DATABASE0_FIELD_PRODUCT_SIZE + HW_DATABASE0_FIELD_CHECKSUM_SIZE)
#define HW_DATABASE0_RECORD_PRODUCT_OFFSET   (0)
#define HW_DATABASE0_RECORD_PRODUCT_ADDR     (HW_DATABASE0_BASE_ADDR + HW_DATABASE0_RECORD_PRODUCT_OFFSET)
/**/
/* Serial Number descriptor string record */
#define HW_DATABASE0_RECORD_SERIALNUM_SIZE     (HW_DATABASE0_FIELD_MAGIC_SIZE + HW_DATABASE0_FIELD_SERIALNUM_SIZE + HW_DATABASE0_FIELD_CHECKSUM_SIZE)
#define HW_DATABASE0_RECORD_SERIALNUM_OFFSET   (HW_DATABASE0_RECORD_PRODUCT_OFFSET + HW_DATABASE0_RECORD_PRODUCT_SIZE)
#define HW_DATABASE0_RECORD_SERIALNUM_ADDR     (HW_DATABASE0_BASE_ADDR + HW_DATABASE0_RECORD_SERIALNUM_OFFSET)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN VARIABLES */

/* USER CODE END VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_DEVICE_Exported_FunctionsPrototype USBD_DEVICE_Exported_FunctionsPrototype
  * @brief Declaration of public functions for Usb device.
  * @{
  */

/** USB Device initialization function. */
void MX_USB_DEVICE_Init(void);

/*
 * -- Insert functions declaration here --
 */
/* USER CODE BEGIN FD */
void IAD_USB_DEVICE_Init(void);

const uint8_t* drv_hw_FrimwareVersionPtrGet(void);
const uint8_t* drv_hw_ProductNamePtrGet(void);
const uint8_t* drv_hw_SerialNumberPtrGet(void);

void drv_hw_ProductNameDatabaseWrite(const uint8_t *str);
void drv_hw_SerialNumberDatabaseWrite(const uint8_t *str);
/* USER CODE END FD */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_DEVICE__H__ */
