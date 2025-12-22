 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OS_KEYB_H
#define __OS_KEYB_H

/* Includes ------------------------------------------------------------------*/
#include "drv_keyb.h"
#include "FreeRTOS.h"
#include "task.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void os_keyb_init(uint32_t poll_rate_ms);
uint8_t os_keyb_KeyGet(KeybKey_TypeDef keyb_key);
void os_keyb_KeyLock(KeybKey_TypeDef keyb_key);
uint32_t os_keyb_KeyPressedGet(KeybKey_TypeDef keyb_key);
void os_keyb_PollRateSet(uint32_t poll_rate_ms);
void os_keyb_PollRateUpdate();
TickType_t os_keyb_PollRateTicksGet();

#endif /* __OS_KEYB_H */

