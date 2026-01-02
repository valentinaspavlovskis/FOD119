/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_OPTIC_H
#define __DRV_OPTIC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

/* OSW Channels */
#define OPTIC_OSW_CHANNELS ( 16 )


/* Exported Global Variables ------------------------------------------------ */
/* Exported functions ------------------------------------------------------- */
void drv_Optic_DeInit(void);
void drv_Optic_Init(void);
void drv_Optic_DACPowerONInit(void);

int8_t drv_Optic_probe(void);

uint16_t drv_Optic_GetChannelNr(void);
int8_t drv_Optic_SetChannel(uint16_t nr);
uint16_t drv_Optic_GetChannel(void);

uint16_t drv_Optic_reg_SaveCalTable();
void drv_Optic_reg_LoadCalTable(void);

void drv_Optic_SetDAC(uint16_t value);

uint8_t drv_mems_pwr_ctrl(uint8_t state);

uint8_t drv_Optic_Update_DAC();
uint16_t* drv_Optic_GetDAC(void);

void drv_Optic_Test_DAC();

void drv_Optic_Print_DAC();
void SetDefault_CH_WL();
uint8_t drv_Optic_write_cal_val(uint8_t ch, uint16_t value);
uint8_t drv_Optic_read_cal_val(uint8_t ch, uint16_t *value);
/* External variables --------------------------------------------------------*/

#endif  /*__DRV_OPTIC_H*/

