/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED_R_Pin|LED_G_Pin|LD_ON_Pin|LD_CH11_Pin
                          |LD_CH12_Pin|LD_CH13_Pin|LD_CH14_Pin|LD_CH15_Pin
                          |LD_CH16_Pin|LD_PWREN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_CH13_Pin|LED_CH15_Pin|LED_CH14_Pin|LED_CH13C2_Pin
                          |LED_CH12_Pin|LED_CH11_Pin|LED_CH10_Pin|LED_CH8_Pin
                          |LED_CH7_Pin|LED_CH6_Pin|LED_CH5_Pin|LED_CH1_Pin
                          |LED_CH2_Pin|LED_CH3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_CH9_Pin|LED_CH4_Pin|LED_CH_MODE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD_CH9_Pin|LD_CH10_Pin|UC_PWROFF_Pin|PWR_MODE_Pin
                          |LD_CH1_Pin|LD_CH2_Pin|LD_CH3_Pin|LD_CH4_Pin
                          |LD_CH5_Pin|LD_CH6_Pin|LD_CH7_Pin|LD_CH8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SW_PWR__Pin SW_UP__Pin SW_DOWN__Pin */
  GPIO_InitStruct.Pin = SW_PWR__Pin|SW_UP__Pin|SW_DOWN__Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_R_Pin LED_G_Pin LD_ON_Pin LD_CH11_Pin
                           LD_CH12_Pin LD_CH13_Pin LD_CH14_Pin LD_CH15_Pin
                           LD_CH16_Pin LD_PWREN_Pin */
  GPIO_InitStruct.Pin = LED_R_Pin|LED_G_Pin|LD_ON_Pin|LD_CH11_Pin
                          |LD_CH12_Pin|LD_CH13_Pin|LD_CH14_Pin|LD_CH15_Pin
                          |LD_CH16_Pin|LD_PWREN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_CH13_Pin LED_CH15_Pin LED_CH14_Pin LED_CH13C2_Pin
                           LED_CH12_Pin LED_CH11_Pin LED_CH10_Pin LED_CH8_Pin
                           LED_CH7_Pin LED_CH6_Pin LED_CH5_Pin LED_CH1_Pin
                           LED_CH2_Pin LED_CH3_Pin */
  GPIO_InitStruct.Pin = LED_CH13_Pin|LED_CH15_Pin|LED_CH14_Pin|LED_CH13C2_Pin
                          |LED_CH12_Pin|LED_CH11_Pin|LED_CH10_Pin|LED_CH8_Pin
                          |LED_CH7_Pin|LED_CH6_Pin|LED_CH5_Pin|LED_CH1_Pin
                          |LED_CH2_Pin|LED_CH3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_CH9_Pin LED_CH4_Pin LED_CH_MODE_Pin */
  GPIO_InitStruct.Pin = LED_CH9_Pin|LED_CH4_Pin|LED_CH_MODE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LD_CH9_Pin LD_CH10_Pin UC_PWROFF_Pin PWR_MODE_Pin
                           LD_CH1_Pin LD_CH2_Pin LD_CH3_Pin LD_CH4_Pin
                           LD_CH5_Pin LD_CH6_Pin LD_CH7_Pin LD_CH8_Pin */
  GPIO_InitStruct.Pin = LD_CH9_Pin|LD_CH10_Pin|UC_PWROFF_Pin|PWR_MODE_Pin
                          |LD_CH1_Pin|LD_CH2_Pin|LD_CH3_Pin|LD_CH4_Pin
                          |LD_CH5_Pin|LD_CH6_Pin|LD_CH7_Pin|LD_CH8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
