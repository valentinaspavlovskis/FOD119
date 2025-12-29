/******************** (C) COPYRIGHT 2017  **************************************
* File Name          : calibration.h
* Author             : vpv
* Version            : V1.0.0
* Date               : 19/02/2017
* Description        : Calibracion functions prototypes
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CALIBRATION_H
#define __CALIBRATION_H

/* Includes ------------------------------------------------------------------*/
#include "datatype.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/
#define MAGIC 0x0D0F
#define FW_VERSION "0.1" 
#define CALIBRATION_VALUES_AREA_BEG_ADDR	(DATA_EEPROM_BASE) //DATA_EEPROM_BANK2_BASE ((uint32_t)0x0801E000)//

#define MAX_CH_NR 17
/* Exported macro ------------------------------------------------------------*/

#pragma pack (push, 1)
typedef struct{
  volatile uint16_t cbMagic;//Magic number used for validate image
  volatile uint16_t cbCoefIdx[MAX_CH_NR];
  volatile uint16_t  cbCalCoef[MAX_CH_NR];
  volatile uint16_t cbCRC;//Examine cabibration values crc
} CalibrationTypeDef;
#pragma pack (pop)


/* Exported functions ------------------------------------------------------- */
void LoadCalibrationData(void);
bool ReadCalibration(CalibrationTypeDef* CalibValues);
bool ReadTestCalibration(CalibrationTypeDef* CalibValues);
uint16_t WriteCalibration(CalibrationTypeDef* CalibValues);
bool EraseCalibration(void);

/* External variables --------------------------------------------------------*/
extern CalibrationTypeDef CalibrationValues;


void drv_LoadCalTable(void);
uint16_t drv_SaveCalTable(void);
void SetCalValue(float val);
void SetCalPowerWL(uint16_t wl);
float GetCalValue(void);
#endif  /*__CALIBRATION_H*/

/******************* (C) COPYRIGHT ****************END OF FILE****/
