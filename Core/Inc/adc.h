/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc;

/* USER CODE BEGIN Private defines */
/* Vref calibration value addres */
#define VREFINT_CAL ((uint16_t*) (uint32_t) 0x1FF80078)

/* Temperature sensor calibration value address */
#define TEMP130_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007E))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007A))
#define VDD_CALIB ((uint16_t) (300))
#define VDD_APPLI ((uint16_t) (330))

#define BAT_LOW_VOLT (115) //+4
#define BAT_FULL_VOLT (200)
#define BAT_OFF_VOLT (111)

#define BAT_DIVIDER //(100 + 56)

typedef enum 
{
  BAT_LEVEL_LOW = 0, 
  BAT_LEVEL_HI = !BAT_LEVEL_LOW
} BAT_LEVEL_T;

typedef struct{
  uint16_t  ch0;
  uint16_t  ch_vref;
  uint16_t  ch_tmp;
}adc_t;

typedef struct{
  uint16_t vbat;
  uint16_t soc;
  int16_t temp;
}measurement_t;

/* USER CODE END Private defines */

void MX_ADC_Init(void);

/* USER CODE BEGIN Prototypes */
//void ADC_Calibration(void);
//void Configure_ADC(void);
//void Activate_ADC(void);
//void ADC_Start(void);

//void AdcDmaTransferComplete_Callback(void);
//void AdcDmaTransferHalf_Callback(void);
//void AdcDmaTransferError_Callback(void);
//void AdcGrpRegularOverrunError_Callback(void);

void MX_ADC_Start(void);
adc_t* GetADCValues(void);
measurement_t* GetMeasurements ( void );
uint16_t readBatt_soc(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

