/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KERNEL_H
#define __KERNEL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void kernel_init();
void kernel_default_tsk_run();
void kernel_default_tsk_init();

void SetLockKeyb(void);
void SetUnLockKeyb(void);
int8_t GetKeybLock(void);


void suspendKernelTask(void);
void resumeKernelTask(void);

void SetPowerButtonOn(void);
#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_H */