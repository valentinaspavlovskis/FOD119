/* Includes ------------------------------------------------------------------*/

#ifndef __USBD_FOD_BOT_H_
#define __USBD_FOD_BOT_H_

#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_fod_bot
  * @brief This file is the Header file for USBD_fod_bot.c
  * @{
  */


/** @defgroup usbd_fod_bot_Exported_Defines
  * @{
  */

#define FOD_DATA_MAX_PACKET_SIZE        0x40

#define FOD_DATA_IN_PACKET_SIZE                FOD_DATA_MAX_PACKET_SIZE
#define FOD_DATA_OUT_PACKET_SIZE               FOD_DATA_MAX_PACKET_SIZE

/*---------------------------------------------------------------------*/
/*  FOD definitions                                                    */
/*---------------------------------------------------------------------*/

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

//extern USBD_Class_cb_TypeDef  USBD_FOD_cb;
extern uint8_t FODUSBRequest;
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
/*********************************************
FOD Device library callbacks
*********************************************/
void  FOD_BOT_Init        (void  *pdev);
void  FOD_BOT_DeInit      (void  *pdev);
void  FOD_BOT_DataIn      (void *pdev, uint8_t epnum);
void  FOD_BOT_DataOut     (void *pdev, uint8_t epnum);
void  FOD_BOT_SOF         (void *pdev);

void Handle_USBAsynchXfer  (void *pdev);
void fodUSBMainLoop(void * pdev);
/**
  * @}
  */

#endif  /* __USBD_FOD_BOT_H_ */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
