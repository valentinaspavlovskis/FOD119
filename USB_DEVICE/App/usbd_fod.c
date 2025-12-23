/**
  ******************************************************************************
  * @file    usbd_msc.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides all the MSC core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                FOD Vendor Specific Class  Description
  *          =================================================================== 
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_fod.h"
#include "usbd_fod_bot.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup FOD_CORE 
  * @brief Mass storage core module
  * @{
  */ 

/** @defgroup FOD_CORE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup FOD_CORE_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup FOD_CORE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup FOD_CORE_Private_FunctionPrototypes
  * @{
  */ 
uint8_t  USBD_FOD_Init (USBD_HandleTypeDef *pdev, 
                            uint8_t cfgidx);

uint8_t  USBD_FOD_DeInit (USBD_HandleTypeDef *pdev, 
                              uint8_t cfgidx);

uint8_t  USBD_FOD_DataIn (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum);


uint8_t  USBD_FOD_DataOut (USBD_HandleTypeDef *pdev, 
                               uint8_t epnum);

uint8_t  *USBD_FOD_GetHSCfgDesc (uint16_t *length);

uint8_t  *USBD_FOD_GetFSCfgDesc (uint16_t *length);

uint8_t  *USBD_FOD_GetOtherSpeedCfgDesc (uint16_t *length);

uint8_t  *USBD_FOD_GetDeviceQualifierDescriptor (uint16_t *length);

uint8_t  USBD_FOD_SOF (USBD_HandleTypeDef *pdev);

/**
  * @}
  */ 


/** @defgroup FOD_CORE_Private_Variables
  * @{
  */ 


USBD_ClassTypeDef  USBD_FOD = 
{
  USBD_FOD_Init,
  USBD_FOD_DeInit,
  NULL,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_FOD_DataIn,
  USBD_FOD_DataOut,
  USBD_FOD_SOF, /*SOF */ 
  NULL,  
  NULL,     
  USBD_FOD_GetHSCfgDesc,
  USBD_FOD_GetFSCfgDesc,  
  USBD_FOD_GetOtherSpeedCfgDesc,
  USBD_FOD_GetDeviceQualifierDescriptor,
};

/* USB Mass storage device Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
__ALIGN_BEGIN uint8_t USBD_FOD_CfgHSDesc[USB_FOD_CONFIG_DESC_SIZ]  __ALIGN_END =
{
  
  0x09,   /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,   /* bDescriptorType: Configuration */
  USB_FOD_CONFIG_DESC_SIZ,
  
  0x00,
  0x01,   /* bNumInterfaces: 1 interface */
  0x01,   /* bConfigurationValue: */
  0x00,   /* iConfiguration: */
  0xC0,   /* bmAttributes: */
  0x32,   /* MaxPower 100 mA */
  
  /********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  0x04,   /* bDescriptorType: */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints*/
  0xff,   /* bInterfaceClass: Vendor specific Class */
  0x00,   /* bInterfaceSubClass */
  0x00,   /* nInterfaceProtocol */
  0x00,   /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  0x05,   /*Endpoint descriptor type */
  FOD_EPOUT_ADDR,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(FOD_MAX_HS_PACKET),
  HIBYTE(FOD_MAX_HS_PACKET),
  0x00,   /*Polling interval in milliseconds */
  
  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  FOD_EPIN_ADDR,   /*Endpoint address (IN, address 1) */  
  0x02,   /*Bulk endpoint type */
  LOBYTE(FOD_MAX_HS_PACKET),
  HIBYTE(FOD_MAX_HS_PACKET),
  0x00     /*Polling interval in milliseconds*/
};

/* USB Mass storage device Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
uint8_t USBD_FOD_CfgFSDesc[USB_FOD_CONFIG_DESC_SIZ]  __ALIGN_END =
{
  
  0x09,   /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,   /* bDescriptorType: Configuration */
  USB_FOD_CONFIG_DESC_SIZ,
  
  0x00,
  0x01,   /* bNumInterfaces: 1 interface */
  0x01,   /* bConfigurationValue: */
  0x00,   /* iConfiguration: */
  0xC0,   /* bmAttributes: */
  0x32,   /* MaxPower 100 mA */
  
  /********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  0x04,   /* bDescriptorType: */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints*/
  0xff,   /* bInterfaceClass: Vendor Specific Class */
  0x00,   /* bInterfaceSubClass */
  0x00,   /* nInterfaceProtocol */
  0x00,   /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  0x05,   /*Endpoint descriptor type */
  FOD_EPIN_ADDR,   /*Endpoint address (IN, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(FOD_MAX_FS_PACKET),
  HIBYTE(FOD_MAX_FS_PACKET),
  0x00,   /*Polling interval in milliseconds */
  
  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  FOD_EPOUT_ADDR,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(FOD_MAX_FS_PACKET),
  HIBYTE(FOD_MAX_FS_PACKET),
  0x00     /*Polling interval in milliseconds*/
};

__ALIGN_BEGIN uint8_t USBD_FOD_OtherSpeedCfgDesc[USB_FOD_CONFIG_DESC_SIZ]   __ALIGN_END  =
{
  
  0x09,   /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,   
  USB_FOD_CONFIG_DESC_SIZ,
  
  0x00,
  0x01,   /* bNumInterfaces: 1 interface */
  0x01,   /* bConfigurationValue: */
  0x00,   /* iConfiguration: */
  0xC0,   /* bmAttributes: */
  0x32,   /* MaxPower 100 mA */
  
  /********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  0x04,   /* bDescriptorType: */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints*/
  0xff,   /* bInterfaceClass: Vendor Specific Class */
  0x00,   /* bInterfaceSubClass */
  0x00,   /* nInterfaceProtocol */
  0x00,   /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  0x05,   /*Endpoint descriptor type */
  FOD_EPIN_ADDR,   /*Endpoint address (IN, address 1) */
  0x02,   /*Bulk endpoint type */
  0x40,
  0x00,
  0x00,   /*Polling interval in milliseconds */
  
  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  FOD_EPOUT_ADDR,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  0x40,
  0x00,
  0x00     /*Polling interval in milliseconds*/
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN  uint8_t USBD_FOD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC]  __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  FOD_MAX_FS_PACKET,
  0x01,
  0x00,
};
/**
  * @}
  */ 


/** @defgroup FOD_CORE_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_FOD_Init
  *         Initialize  the mass storage configuration
  * @param  pdev: device instance
  * @param  cfgidx: configuration index
  * @retval status
  */
uint8_t  USBD_FOD_Init (USBD_HandleTypeDef *pdev, 
                            uint8_t cfgidx)
{
  int16_t ret = 0;
   
  if(pdev->dev_speed == USBD_SPEED_HIGH  ) 
  {
    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   FOD_EPOUT_ADDR,
                   USBD_EP_TYPE_BULK,
                   FOD_MAX_HS_PACKET);
    
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   FOD_EPIN_ADDR,
                   USBD_EP_TYPE_BULK,
                   FOD_MAX_HS_PACKET);  
  }
  else
  {
    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   FOD_EPOUT_ADDR,
                   USBD_EP_TYPE_BULK,
                   FOD_MAX_FS_PACKET);
    
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   FOD_EPIN_ADDR,
                   USBD_EP_TYPE_BULK,
                   FOD_MAX_FS_PACKET);  
  }
  
  /* Init the BOT  layer */
  FOD_BOT_Init(pdev); 
  
  return ret;
}

/**
  * @brief  USBD_FOD_DeInit
  *         DeInitilaize  the FOD configuration
  * @param  pdev: device instance
  * @param  cfgidx: configuration index
  * @retval status
  */
uint8_t  USBD_FOD_DeInit (USBD_HandleTypeDef *pdev, 
                              uint8_t cfgidx)
{
  /* Close MSC EPs */
  USBD_LL_CloseEP(pdev,
                  FOD_EPOUT_ADDR);
  
  /* Open EP IN */
  USBD_LL_CloseEP(pdev,
                  FOD_EPIN_ADDR);
  
  
  /* De-Init the BOT layer */
  FOD_BOT_DeInit(pdev);
  
  return 0;
}

/**
* @brief  USBD_FOD_DataIn
*         handle data IN Stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
uint8_t  USBD_FOD_DataIn (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{
  FOD_BOT_DataIn(pdev , epnum);
  return 0;
}

/**
* @brief  USBD_FOD_DataOut
*         handle data OUT Stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
uint8_t  USBD_FOD_DataOut (USBD_HandleTypeDef *pdev, 
                               uint8_t epnum)
{
  FOD_BOT_DataOut(pdev , epnum);
  return 0;
}

/**
* @brief  usbd_audio_SOF
*         Start Of Frame event management
* @param  pdev: instance
* @param  epnum: endpoint number
* @retval status
*/
uint8_t  USBD_FOD_SOF (USBD_HandleTypeDef *pdev)
{
  FOD_BOT_SOF(pdev);
  return 0;
}

/**
* @brief  USBD_FOD_GetHSCfgDesc 
*         return configuration descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_FOD_GetHSCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_FOD_CfgHSDesc);
  return USBD_FOD_CfgHSDesc;
}

/**
* @brief  USBD_FOD_GetFSCfgDesc 
*         return configuration descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_FOD_GetFSCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_FOD_CfgFSDesc);
  return USBD_FOD_CfgFSDesc;
}

/**
* @brief  USBD_FOD_GetOtherSpeedCfgDesc 
*         return other speed configuration descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_FOD_GetOtherSpeedCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_FOD_OtherSpeedCfgDesc);
  return USBD_FOD_OtherSpeedCfgDesc;
}
/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_FOD_GetDeviceQualifierDescriptor (uint16_t *length)
{
  *length = sizeof (USBD_FOD_DeviceQualifierDesc);
  return USBD_FOD_DeviceQualifierDesc;
}


/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
