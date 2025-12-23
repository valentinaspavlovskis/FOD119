/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW_PWR__Pin GPIO_PIN_2
#define SW_PWR__GPIO_Port GPIOE
#define SW_UP__Pin GPIO_PIN_3
#define SW_UP__GPIO_Port GPIOE
#define SW_DOWN__Pin GPIO_PIN_4
#define SW_DOWN__GPIO_Port GPIOE
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOE
#define LED_G_Pin GPIO_PIN_6
#define LED_G_GPIO_Port GPIOE
#define LED_CH16_Pin GPIO_PIN_13
#define LED_CH16_GPIO_Port GPIOC
#define LED_CH15_Pin GPIO_PIN_0
#define LED_CH15_GPIO_Port GPIOC
#define LED_CH14_Pin GPIO_PIN_1
#define LED_CH14_GPIO_Port GPIOC
#define LED_CH13_Pin GPIO_PIN_2
#define LED_CH13_GPIO_Port GPIOC
#define LED_CH12_Pin GPIO_PIN_3
#define LED_CH12_GPIO_Port GPIOC
#define LED_CH11_Pin GPIO_PIN_4
#define LED_CH11_GPIO_Port GPIOC
#define LED_CH10_Pin GPIO_PIN_5
#define LED_CH10_GPIO_Port GPIOC
#define LED_CH9_Pin GPIO_PIN_2
#define LED_CH9_GPIO_Port GPIOB
#define LD_ON_Pin GPIO_PIN_9
#define LD_ON_GPIO_Port GPIOE
#define LD_CH11_Pin GPIO_PIN_10
#define LD_CH11_GPIO_Port GPIOE
#define LD_CH12_Pin GPIO_PIN_11
#define LD_CH12_GPIO_Port GPIOE
#define LD_CH13_Pin GPIO_PIN_12
#define LD_CH13_GPIO_Port GPIOE
#define LD_CH14_Pin GPIO_PIN_13
#define LD_CH14_GPIO_Port GPIOE
#define LD_CH15_Pin GPIO_PIN_14
#define LD_CH15_GPIO_Port GPIOE
#define LD_CH16_Pin GPIO_PIN_15
#define LD_CH16_GPIO_Port GPIOE
#define LD_CH9_Pin GPIO_PIN_12
#define LD_CH9_GPIO_Port GPIOD
#define LD_CH10_Pin GPIO_PIN_13
#define LD_CH10_GPIO_Port GPIOD
#define UC_PWROFF_Pin GPIO_PIN_14
#define UC_PWROFF_GPIO_Port GPIOD
#define PWR_MODE_Pin GPIO_PIN_15
#define PWR_MODE_GPIO_Port GPIOD
#define LED_CH8_Pin GPIO_PIN_6
#define LED_CH8_GPIO_Port GPIOC
#define LED_CH7_Pin GPIO_PIN_7
#define LED_CH7_GPIO_Port GPIOC
#define LED_CH6_Pin GPIO_PIN_8
#define LED_CH6_GPIO_Port GPIOC
#define LED_CH5_Pin GPIO_PIN_9
#define LED_CH5_GPIO_Port GPIOC
#define LED_CH1_Pin GPIO_PIN_10
#define LED_CH1_GPIO_Port GPIOC
#define LED_CH2_Pin GPIO_PIN_11
#define LED_CH2_GPIO_Port GPIOC
#define LED_CH3_Pin GPIO_PIN_12
#define LED_CH3_GPIO_Port GPIOC
#define LD_CH1_Pin GPIO_PIN_0
#define LD_CH1_GPIO_Port GPIOD
#define LD_CH2_Pin GPIO_PIN_1
#define LD_CH2_GPIO_Port GPIOD
#define LD_CH3_Pin GPIO_PIN_2
#define LD_CH3_GPIO_Port GPIOD
#define LD_CH4_Pin GPIO_PIN_3
#define LD_CH4_GPIO_Port GPIOD
#define LD_CH5_Pin GPIO_PIN_4
#define LD_CH5_GPIO_Port GPIOD
#define LD_CH6_Pin GPIO_PIN_5
#define LD_CH6_GPIO_Port GPIOD
#define LD_CH7_Pin GPIO_PIN_6
#define LD_CH7_GPIO_Port GPIOD
#define LD_CH8_Pin GPIO_PIN_7
#define LD_CH8_GPIO_Port GPIOD
#define LED_CH4_Pin GPIO_PIN_3
#define LED_CH4_GPIO_Port GPIOB
#define LED_CH_MODE_Pin GPIO_PIN_5
#define LED_CH_MODE_GPIO_Port GPIOB
#define LD_PWREN_Pin GPIO_PIN_0
#define LD_PWREN_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

#define LED_G_HIGH()  HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET)
#define LED_G_LOW()   HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET)

#define LD_ON_HIGH()  HAL_GPIO_WritePin(LD_ON_GPIO_Port, LD_ON_Pin, GPIO_PIN_SET)
#define LD_ON_LOW()   HAL_GPIO_WritePin(LD_ON_GPIO_Port, LD_ON_Pin, GPIO_PIN_RESET)


#define LED_CH1_HIGH()  HAL_GPIO_WritePin(LED_CH1_GPIO_Port, LED_CH1_Pin, GPIO_PIN_SET)
#define LED_CH1_LOW()   HAL_GPIO_WritePin(LED_CH1_GPIO_Port, LED_CH1_Pin, GPIO_PIN_RESET)

#define LED_CH2_HIGH()  HAL_GPIO_WritePin(LED_CH2_GPIO_Port, LED_CH2_Pin, GPIO_PIN_SET)
#define LED_CH2_LOW()   HAL_GPIO_WritePin(LED_CH2_GPIO_Port, LED_CH2_Pin, GPIO_PIN_RESET)

#define LED_CH3_HIGH()  HAL_GPIO_WritePin(LED_CH3_GPIO_Port, LED_CH3_Pin, GPIO_PIN_SET)
#define LED_CH3_LOW()   HAL_GPIO_WritePin(LED_CH3_GPIO_Port, LED_CH3_Pin, GPIO_PIN_RESET)

#define LED_CH4_HIGH()  HAL_GPIO_WritePin(LED_CH4_GPIO_Port, LED_CH4_Pin, GPIO_PIN_SET)
#define LED_CH4_LOW()   HAL_GPIO_WritePin(LED_CH4_GPIO_Port, LED_CH4_Pin, GPIO_PIN_RESET)

#define LED_CH5_HIGH()  HAL_GPIO_WritePin(LED_CH5_GPIO_Port, LED_CH5_Pin, GPIO_PIN_SET)
#define LED_CH5_LOW()   HAL_GPIO_WritePin(LED_CH5_GPIO_Port, LED_CH5_Pin, GPIO_PIN_RESET)

#define LED_CH6_HIGH()  HAL_GPIO_WritePin(LED_CH6_GPIO_Port, LED_CH6_Pin, GPIO_PIN_SET)
#define LED_CH6_LOW()   HAL_GPIO_WritePin(LED_CH6_GPIO_Port, LED_CH6_Pin, GPIO_PIN_RESET)

#define LED_CH7_HIGH()  HAL_GPIO_WritePin(LED_CH7_GPIO_Port, LED_CH7_Pin, GPIO_PIN_SET)
#define LED_CH7_LOW()   HAL_GPIO_WritePin(LED_CH7_GPIO_Port, LED_CH7_Pin, GPIO_PIN_RESET)

#define LED_CH8_HIGH()  HAL_GPIO_WritePin(LED_CH8_GPIO_Port, LED_CH8_Pin, GPIO_PIN_SET)
#define LED_CH8_LOW()   HAL_GPIO_WritePin(LED_CH8_GPIO_Port, LED_CH8_Pin, GPIO_PIN_RESET)

#define LED_CH9_HIGH()  HAL_GPIO_WritePin(LED_CH9_GPIO_Port, LED_CH9_Pin, GPIO_PIN_SET)
#define LED_CH9_LOW()   HAL_GPIO_WritePin(LED_CH9_GPIO_Port, LED_CH9_Pin, GPIO_PIN_RESET)

#define LED_CH10_HIGH()  HAL_GPIO_WritePin(LED_CH10_GPIO_Port, LED_CH10_Pin, GPIO_PIN_SET)
#define LED_CH10_LOW()   HAL_GPIO_WritePin(LED_CH10_GPIO_Port, LED_CH10_Pin, GPIO_PIN_RESET)

#define LED_CH11_HIGH()  HAL_GPIO_WritePin(LED_CH11_GPIO_Port, LED_CH11_Pin, GPIO_PIN_SET)
#define LED_CH11_LOW()   HAL_GPIO_WritePin(LED_CH11_GPIO_Port, LED_CH11_Pin, GPIO_PIN_RESET)

#define LED_CH12_HIGH()  HAL_GPIO_WritePin(LED_CH12_GPIO_Port, LED_CH12_Pin, GPIO_PIN_SET)
#define LED_CH12_LOW()   HAL_GPIO_WritePin(LED_CH12_GPIO_Port, LED_CH12_Pin, GPIO_PIN_RESET)

#define LED_CH13_HIGH()  HAL_GPIO_WritePin(LED_CH13_GPIO_Port, LED_CH13_Pin, GPIO_PIN_SET)
#define LED_CH13_LOW()   HAL_GPIO_WritePin(LED_CH13_GPIO_Port, LED_CH13_Pin, GPIO_PIN_RESET)

#define LED_CH14_HIGH()  HAL_GPIO_WritePin(LED_CH14_GPIO_Port, LED_CH14_Pin, GPIO_PIN_SET)
#define LED_CH14_LOW()   HAL_GPIO_WritePin(LED_CH14_GPIO_Port, LED_CH14_Pin, GPIO_PIN_RESET)

#define LED_CH15_HIGH()  HAL_GPIO_WritePin(LED_CH15_GPIO_Port, LED_CH15_Pin, GPIO_PIN_SET)
#define LED_CH15_LOW()   HAL_GPIO_WritePin(LED_CH15_GPIO_Port, LED_CH15_Pin, GPIO_PIN_RESET)

#define LED_CH16_HIGH()  HAL_GPIO_WritePin(LED_CH16_GPIO_Port, LED_CH16_Pin, GPIO_PIN_SET)
#define LED_CH16_LOW()   HAL_GPIO_WritePin(LED_CH16_GPIO_Port, LED_CH16_Pin, GPIO_PIN_RESET)



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
