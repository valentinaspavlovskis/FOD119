/**
  ******************************************************************************
  * @file    usbd_customhid.c
  * @author  MCD Application Team
  * @brief   This file provides the IAD core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                IAD Class  Description
  *          ===================================================================
  *           This module manages the IAD class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (IAD) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - Usage Page : Generic Desktop
  *             - Usage : Vendor
  *             - Collection : Application
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

  /* BSPDependencies
  - "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
  - "stm32xxxxx_{eval}{discovery}_io.c"
  EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbd_iad.h"
#include "usbd_ctlreq.h"

#include "usbd_hid.h"
#include "usbd_hid_if.h"
#include "usbd_vsc.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_IAD
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_IAD_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_IAD_Private_Defines
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_IAD_Private_Macros
  * @{
  */
/**
  * @}
  */
/** @defgroup USBD_IAD_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_IAD_Init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx);

static uint8_t  USBD_IAD_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx);

static uint8_t  USBD_IAD_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req);

static uint8_t  *USBD_IAD_GetFSCfgDesc (uint16_t *length);

static uint8_t  *USBD_IAD_GetHSCfgDesc (uint16_t *length);

static uint8_t  *USBD_IAD_GetOtherSpeedCfgDesc (uint16_t *length);

static uint8_t  *USBD_IAD_GetDeviceQualifierDesc (uint16_t *length);

static uint8_t  USBD_IAD_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_IAD_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_IAD_EP0_RxReady (USBD_HandleTypeDef  *pdev);
static uint8_t  USBD_IAD_SOF (USBD_HandleTypeDef *pdev);
/**
  * @}
  */

/** @defgroup USBD_IAD_Private_Variables
  * @{
  */

USBD_ClassTypeDef  USBD_IAD_ClassDriver =
{
  USBD_IAD_Init,
  USBD_IAD_DeInit,
  USBD_IAD_Setup,
  NULL, /*EP0_TxSent*/
  USBD_IAD_EP0_RxReady, /*EP0_RxReady*/ /* STATUS STAGE IN */
  USBD_IAD_DataIn, /*DataIn*/
  USBD_IAD_DataOut,
  USBD_IAD_SOF,//NULL, /*SOF */
  NULL, /* IsoINIncomplete */
  NULL, /* IsoOUTIncomplete */
  USBD_IAD_GetHSCfgDesc,//NULL,//USBD_IAD_GetHSCfgDesc,
  USBD_IAD_GetFSCfgDesc,
  USBD_IAD_GetOtherSpeedCfgDesc,//NULL,//USBD_IAD_GetOtherSpeedCfgDesc,
  USBD_IAD_GetDeviceQualifierDesc,
};


/* USB IAD device FS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_IAD_CfgFSDesc[USB_IAD_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_IAD_CONFIG_DESC_SIZ,
  
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

/* USB IAD device HS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_IAD_CfgHSDesc[USB_IAD_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_IAD_CONFIG_DESC_SIZ,
  
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

/* USB IAD device Other Speed Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_IAD_OtherSpeedCfgDesc[USB_IAD_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_IAD_CONFIG_DESC_SIZ,
  
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

///* USB IAD device Configuration Descriptor */
//__ALIGN_BEGIN static uint8_t USBD_IAD_Desc[USB_IAD_DESC_SIZ] __ALIGN_END =
//{
//  /* 18 */
//  0x09,         /*bLength: IAD Descriptor size*/
//  IAD_DESCRIPTOR_TYPE, /*bDescriptorType: IAD*/
//  0x11,         /*bCUSTOM_HIDUSTOM_HID: IAD Class Spec release number*/
//  0x01,
//  0x00,         /*bCountryCode: Hardware target country*/
//  0x01,         /*bNumDescriptors: Number of IAD class descriptors to follow*/
//  0x22,         /*bDescriptorType*/
//  USBD_IAD_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
//  0x00,
//};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_IAD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Functions
  * @{
  */

/**
  * @brief  USBD_IAD_Init
  *         Initialize the IAD interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_IAD_Init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx)
{
#if 0
  uint8_t ret = 0U;
  USBD_IAD_HandleTypeDef     *hhid;

  /* Open EP IN */
  USBD_LL_OpenEP(pdev, CUSTOM_HID_EPIN_ADDR, USBD_EP_TYPE_INTR,
                 IAD_EPIN_SIZE);

  pdev->ep_in[CUSTOM_HID_EPIN_ADDR & 0xFU].is_used = 1U;

  /* Open EP OUT */
  USBD_LL_OpenEP(pdev, CUSTOM_HID_EPOUT_ADDR, USBD_EP_TYPE_INTR,
                 IAD_EPOUT_SIZE);

  pdev->ep_out[CUSTOM_HID_EPOUT_ADDR & 0xFU].is_used = 1U;

  pdev->pClassData = USBD_malloc(sizeof (USBD_IAD_HandleTypeDef));

  if(pdev->pClassData == NULL)
  {
    ret = 1U;
  }
  else
  {
    hhid = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

    hhid->state = IAD_IDLE;
    ((USBD_IAD_ItfTypeDef *)pdev->pUserData)->Init();

    /* Prepare Out endpoint to receive 1st packet */
    USBD_LL_PrepareReceive(pdev, CUSTOM_HID_EPOUT_ADDR, hhid->Report_buf,
                           USBD_IAD_OUTREPORT_BUF_SIZE);
  }
#endif  
  
  /* HID initialization */
  uint8_t ret = USBD_HID_Init (pdev, cfgidx);
  if(ret != 0)
          return ret;

  /* VSC initialization */
  ret = USBD_FOD_Init (pdev, cfgidx);
  if(ret != 0)
          return ret;

  return ret;
}

/**
  * @brief  USBD_CUSTOM_HID_Init
  *         DeInitialize the IAD layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_IAD_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx)
{
#if 0
  /* Close IAD EP IN */
  USBD_LL_CloseEP(pdev, CUSTOM_HID_EPIN_ADDR);
  pdev->ep_in[CUSTOM_HID_EPIN_ADDR & 0xFU].is_used = 0U;

  /* Close IAD EP OUT */
  USBD_LL_CloseEP(pdev, CUSTOM_HID_EPOUT_ADDR);
  pdev->ep_out[CUSTOM_HID_EPOUT_ADDR & 0xFU].is_used = 0U;

  /* FRee allocated memory */
  if(pdev->pClassData != NULL)
  {
    ((USBD_IAD_ItfTypeDef *)pdev->pUserData)->DeInit();
    USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }
#endif
    
  /* HID De-initialization */
  USBD_HID_DeInit(pdev, cfgidx);

  /* VSC De-initialization */
  USBD_FOD_DeInit(pdev, cfgidx);

  return USBD_OK;
}

/**
  * @brief  USBD_CUSTOM_HID_Setup
  *         Handle the IAD specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_IAD_Setup (USBD_HandleTypeDef *pdev,
                                       USBD_SetupReqTypedef *req)
{
#if 0
  USBD_IAD_HandleTypeDef *hhid = (USBD_IAD_HandleTypeDef*)pdev->pClassData;
  uint16_t len = 0U;
  uint8_t  *pbuf = NULL;
  uint16_t status_info = 0U;
  uint8_t ret = USBD_OK;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :
    switch (req->bRequest)
    {
    case IAD_REQ_SET_PROTOCOL:
      hhid->Protocol = (uint8_t)(req->wValue);
      break;

    case IAD_REQ_GET_PROTOCOL:
      USBD_CtlSendData (pdev, (uint8_t *)(void *)&hhid->Protocol, 1U);
      break;

    case IAD_REQ_SET_IDLE:
      hhid->IdleState = (uint8_t)(req->wValue >> 8);
      break;

    case IAD_REQ_GET_IDLE:
      USBD_CtlSendData (pdev, (uint8_t *)(void *)&hhid->IdleState, 1U);
      break;

    case IAD_REQ_SET_REPORT:
      hhid->IsReportAvailable = 1U;
      USBD_CtlPrepareRx (pdev, hhid->Report_buf, req->wLength);
      break;

    default:
      USBD_CtlError (pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_STATUS:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        USBD_CtlSendData (pdev, (uint8_t *)(void *)&status_info, 2U);
      }
      else
      {
        USBD_CtlError (pdev, req);
			  ret = USBD_FAIL;
      }
      break;

    case USB_REQ_GET_DESCRIPTOR:
      if( req->wValue >> 8 == IAD_REPORT_DESC)
      {
        len = MIN(USBD_IAD_REPORT_DESC_SIZE , req->wLength);
        pbuf =  ((USBD_IAD_ItfTypeDef *)pdev->pUserData)->pReport;
      }
      else
      {
        if( req->wValue >> 8 == IAD_DESCRIPTOR_TYPE)
        {
          pbuf = USBD_IAD_Desc;
          len = MIN(USB_IAD_DESC_SIZ , req->wLength);
        }
      }

      USBD_CtlSendData (pdev, pbuf, len);
      break;

    case USB_REQ_GET_INTERFACE :
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        USBD_CtlSendData (pdev, (uint8_t *)(void *)&hhid->AltSetting, 1U);
      }
      else
      {
        USBD_CtlError (pdev, req);
			  ret = USBD_FAIL;
      }
      break;

    case USB_REQ_SET_INTERFACE :
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        hhid->AltSetting = (uint8_t)(req->wValue);
      }
      else
      {
        USBD_CtlError (pdev, req);
			  ret = USBD_FAIL;
      }
      break;

    default:
      USBD_CtlError (pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  default:
    USBD_CtlError (pdev, req);
    ret = USBD_FAIL;
    break;
  }
  return ret;
#endif
  // Route requests to MSC interface or its endpoints to MSC class implementaion
  if(((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_INTERFACE && req->wIndex == VSC_INTERFACE_IDX) ||
          ((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_ENDPOINT && ((req->wIndex & 0x7F) == VSC_EP_IDX)))
  {
          return USBD_OK;
  }

  return USBD_HID_Setup(pdev, req);
  
}

/**
  * @brief  USBD_IAD_GetFSCfgDesc
  *         return FS configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_IAD_GetFSCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_IAD_CfgFSDesc);
  return USBD_IAD_CfgFSDesc;
}

/**
  * @brief  USBD_CUSTOM_HID_GetHSCfgDesc
  *         return HS configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_IAD_GetHSCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_IAD_CfgHSDesc);
  return USBD_IAD_CfgHSDesc;
}

/**
  * @brief  USBD_CUSTOM_HID_GetOtherSpeedCfgDesc
  *         return other speed configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_IAD_GetOtherSpeedCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_IAD_OtherSpeedCfgDesc);
  return USBD_IAD_OtherSpeedCfgDesc;
}

/**
  * @brief  USBD_IAD_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_IAD_DataIn (USBD_HandleTypeDef *pdev,
                                        uint8_t epnum)
{
  /* Ensure that the FIFO is empty before a new transfer, this condition could
  be caused by  a new transfer before the end of the previous transfer */
#if 0
  ((USBD_IAD_HandleTypeDef *)pdev->pClassData)->state = IAD_IDLE;
  
  return USBD_OK;
#endif  
  if(epnum == VSC_EP_IDX)
    return USBD_FOD_DataIn(pdev, epnum);

  return USBD_HID_DataIn(pdev, epnum);
}

/**
  * @brief  USBD_CUSTOM_HID_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_IAD_DataOut (USBD_HandleTypeDef *pdev,
                                          uint8_t epnum)
{
#if 0
  USBD_IAD_HandleTypeDef     *hhid = (USBD_IAD_HandleTypeDef*)pdev->pClassData;

  ((USBD_IAD_ItfTypeDef *)pdev->pUserData)->OutEvent(hhid->Report_buf[0],
                                                            hhid->Report_buf[1]);

  USBD_LL_PrepareReceive(pdev, CUSTOM_HID_EPOUT_ADDR , hhid->Report_buf,
                         USBD_IAD_OUTREPORT_BUF_SIZE);

  return USBD_OK;
#endif


  if(epnum == VSC_EP_IDX)
    return USBD_FOD_DataOut(pdev, epnum);

  return USBD_HID_DataOut(pdev, epnum);
}


static uint8_t  USBD_IAD_SOF (USBD_HandleTypeDef *pdev)
{
    return USBD_FOD_SOF (pdev);
}
/**
  * @brief  USBD_CUSTOM_HID_EP0_RxReady
  *         Handles control request data.
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_IAD_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
#if 0  
  USBD_IAD_HandleTypeDef     *hhid = (USBD_IAD_HandleTypeDef*)pdev->pClassData;

  if (hhid->IsReportAvailable == 1U)
  {
    ((USBD_IAD_ItfTypeDef *)pdev->pUserData)->OutEvent(hhid->Report_buf[0],
                                                              hhid->Report_buf[1]);
    hhid->IsReportAvailable = 0U;
  }

  return USBD_OK;
#endif 
  return USBD_HID_EP0_RxReady(pdev);
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t  *USBD_IAD_GetDeviceQualifierDesc (uint16_t *length)
{
  *length = sizeof (USBD_IAD_DeviceQualifierDesc);
  return USBD_IAD_DeviceQualifierDesc;
}

/**
* @brief  USBD_IAD_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: IAD Interface callback
  * @retval status
  */
uint8_t  USBD_IAD_RegisterInterface  (USBD_HandleTypeDef   *pdev,
                                             USBD_IAD_ItfTypeDef *fops)
{
  uint8_t  ret = USBD_FAIL;

  if(fops != NULL)
  {
    pdev->pUserData= fops;
    ret = USBD_OK;
  }

  return ret;
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
