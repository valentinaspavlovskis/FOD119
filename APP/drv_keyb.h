/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_KEYB_H
#define __DRV_KEYB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/** 
  * @brief  Keyboard Keys enumeration
  */ 
typedef enum{
  Key_UP     = 0x00, /* UP key */
  Key_DOWN   = 0x01, /* DOWN key */
  Key_PWR    = 0x02, /* POWER key */
  Key_WL     = 0x03, /* WL key */
}KeybKey_TypeDef;
#define IS_KEYB_KEY(KEY) (((KEY) == Key_UP)  || ((KEY) == Key_DOWN) || ((KEY) == Key_WL))

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported Global Variables ------------------------------------------------ */
/* Exported functions ------------------------------------------------------- */
void drv_Keyb_DeInit(void);
void drv_Keyb_Init(void);
uint8_t drv_Keyb_KeyGet(KeybKey_TypeDef keyb_key);
void drv_Keyb_KeyLock(KeybKey_TypeDef keyb_key);
uint16_t drv_Keyb_KeyPressedGet(KeybKey_TypeDef keyb_key);
void drv_Keyb_Scan(void);

/* External variables --------------------------------------------------------*/

#endif  /*__DRV_KEYB_H*/

