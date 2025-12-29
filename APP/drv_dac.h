/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_DAC_H
#define __DRV_DAC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported Global Variables ------------------------------------------------ */
/* Exported functions ------------------------------------------------------- */

/* External variables --------------------------------------------------------*/

/**
  * @brief  Deinitialize Optic driver.
  * @param  None.
  * @retval None
  */
void drv_dac_DeInit(void);

/**
  * @brief  Initialize Optic driver.
  * @param  None.
  * @retval None
  */
void drv_dac_Init(void);


void drv_dac_set_(uint16_t value);

#endif  /*__DRV_DAC_H*/

