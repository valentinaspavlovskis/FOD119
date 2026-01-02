/* Includes ------------------------------------------------------------------*/
#include "drv_optic.h"
#include "drv_dac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calibration.h"
#include "drv_eeprom.h"
#include "kernel.h"
#include "optic_msg_type.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Function prototypes -------------------------------------------------------*/
void LED_CTRL( uint8_t ctrl,uint16_t LedIdx );
/* Private variables ---------------------------------------------------------*/

/* Iterrator register for channels */
uint16_t optic_osw_iter;
static uint16_t dac_value = 0;
static uint16_t curr_ch = 0;

#define MAX_ADC_VALUES (OPTIC_OSW_CHANNELS)
/* Channel Calibration Table: Pairs of X and Y calibration values */
uint16_t optic_cal_table[MAX_ADC_VALUES]={ 
/* 1 */   0x0000,
/* 2 */   0x0000,
/* 3 */   0x0000,
/* 4 */   0x0000,
/* 5 */   0x0000,
/* 6 */   0x0000,
/* 7 */   0x0000,
/* 8 */   0x0000,
/* 9 */   0x0000,
/* 10 */  0x0000,
/* 11 */  0x0000,
/* 12 */  0x0000,
/* 13 */  0x0000,
/* 14 */  0x0000,
/* 15 */  0x0000,
/* 16 */  0x0000
};

int16_t optic_idx_table[MAX_ADC_VALUES]={
/* 1 */   0x0000,      
/* 2 */   0x0001,
/* 3 */   0x0002,
/* 4 */   0x0003,
/* 5 */   0x0004,
/* 6 */   0x0005,
/* 7 */   0x0006,
/* 8 */   0x0007,
/* 9 */   0x0008,
/* 10 */  0x0009,
/* 11 */  0x000A,
/* 12 */  0x000B,
/* 13 */  0x000C,
/* 14 */  0x000D,
/* 15 */  0x000E,
/* 16 */  0x000F
};

GPIO_TypeDef * LDs_Ports[OPTIC_OSW_CHANNELS] = {
  LD_CH1_GPIO_Port,
  LD_CH2_GPIO_Port,
  LD_CH3_GPIO_Port,
  LD_CH4_GPIO_Port,
  LD_CH5_GPIO_Port,
  LD_CH6_GPIO_Port,
  LD_CH7_GPIO_Port,
  LD_CH8_GPIO_Port,
  LD_CH9_GPIO_Port,
  LD_CH10_GPIO_Port,
  LD_CH11_GPIO_Port,
  LD_CH12_GPIO_Port,
  LD_CH13_GPIO_Port,
  LD_CH14_GPIO_Port,
  LD_CH15_GPIO_Port,
  LD_CH16_GPIO_Port,
};

uint16_t LDs_Pins[OPTIC_OSW_CHANNELS] = {
  LD_CH1_Pin,
  LD_CH2_Pin,
  LD_CH3_Pin,
  LD_CH4_Pin,
  LD_CH5_Pin,
  LD_CH6_Pin,
  LD_CH7_Pin,
  LD_CH8_Pin,
  LD_CH9_Pin,
  LD_CH10_Pin,
  LD_CH11_Pin,
  LD_CH12_Pin,
  LD_CH13_Pin,
  LD_CH14_Pin,
  LD_CH15_Pin,
  LD_CH16_Pin,
};
/*Private function prototypes -----------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Deinitialize Optic driver.
  * @param  None.
  * @retval None
  */
void drv_Optic_DeInit(void)
{
  //Optic power off
  LD_PWREN_LOW();
  
  drv_dac_DeInit();
}

/**
  * @brief  Initialize Optic driver.
  * @param  None.
  * @retval None
  */
void drv_Optic_Init(void)
{
  drv_dac_Init();
  
  drv_Optic_reg_LoadCalTable();
  
  //Optic power on
  LD_PWREN_HIGH();
}

/* Logical MPO TRACER channel */
uint8_t optic_tr_ch = 0;

/**
  * @brief  Write to optic register.
  * @param  addr: register address.
  * @param  data: data.
  * @retval None
  */
void drv_Optic_reg_write_channel(uint16_t ch)
{
  if(ch < OPTIC_OSW_CHANNELS){
    optic_tr_ch = ch;
    
    optic_send_msg(OPTIC_MSG_MSG_UPDATE_DAC, 0, 0, 1);
  }
}

/**
  * @brief  Read optic register.
  * @param  addr: register address.
  * @retval data
  */
void* drv_Optic_reg_read(uint16_t addr)
{
  void* data = 0;

  return data;
}



uint8_t drv_Optic_Update_DAC(){
  return drv_dac_set_(dac_value);
}

void drv_Optic_SetDAC(uint16_t value){
  dac_value = value;
}

uint16_t* drv_Optic_GetDAC(void){
  return &dac_value;
}

void drv_Optic_Print_DAC(){
  
}

uint16_t drv_Optic_GetChannel(void){
  return curr_ch;
}

uint16_t drv_Optic_GetChannelNr(void){
  return MAX_ADC_VALUES;
}


int8_t drv_Optic_SetChannel(uint16_t nr){
  if(nr > MAX_ADC_VALUES)
    return 1;
   uint16_t optic_osw_ch = nr;
  // Laser off
  LD_ON_LOW();
    
  drv_Optic_SetDAC(0);
  drv_Optic_Update_DAC();
  
  // Deselect all lasers
  for(uint16_t ld_idx = 0;ld_idx < OPTIC_OSW_CHANNELS; ld_idx++ ){
    HAL_GPIO_WritePin(LDs_Ports[ld_idx], LDs_Pins[ld_idx], GPIO_PIN_SET);
  }
  
  drv_Optic_SetDAC(optic_cal_table[optic_idx_table[optic_osw_ch]]);
  drv_Optic_Update_DAC();
  
  // Select current laser
  HAL_GPIO_WritePin(LDs_Ports[optic_idx_table[optic_osw_ch]], LDs_Pins[optic_idx_table[optic_osw_ch]], GPIO_PIN_RESET);
  
  osDelay(20);
  
  // Laser on
  LD_ON_HIGH();
  
  LED_CTRL( 0, curr_ch);
  curr_ch = optic_osw_ch;
  LED_CTRL( 1, curr_ch);  
  return 0;
}

uint8_t drv_Optic_write_cal_val(uint8_t ch, uint16_t value){
  if(ch > MAX_ADC_VALUES)
    return 1;
  if(value <= 4095){
    optic_cal_table[ch] = value;
  }else{
    return 1;
  }
  return 0;
}

uint8_t drv_Optic_read_cal_val(uint8_t ch, uint16_t *value){
  if(ch > MAX_ADC_VALUES)
    return 1;
  
  *value = optic_cal_table[ch];
  
  return 0;
}

//==================== LEDs control =================================//

GPIO_TypeDef * LEDs_Ports[OPTIC_OSW_CHANNELS] = {
  LED_CH1_GPIO_Port,
  LED_CH2_GPIO_Port,
  LED_CH3_GPIO_Port,
  LED_CH4_GPIO_Port,
  LED_CH5_GPIO_Port,
  LED_CH6_GPIO_Port,
  LED_CH7_GPIO_Port,
  LED_CH8_GPIO_Port,
  LED_CH9_GPIO_Port,
  LED_CH10_GPIO_Port,
  LED_CH11_GPIO_Port,
  LED_CH12_GPIO_Port,
  LED_CH13_GPIO_Port,
  LED_CH14_GPIO_Port,
  LED_CH15_GPIO_Port,
  LED_CH16_GPIO_Port,
};

uint16_t LEDs_Pins[OPTIC_OSW_CHANNELS] = {
  LED_CH1_Pin,
  LED_CH2_Pin,
  LED_CH3_Pin,
  LED_CH4_Pin,
  LED_CH5_Pin,
  LED_CH6_Pin,
  LED_CH7_Pin,
  LED_CH8_Pin,
  LED_CH9_Pin,
  LED_CH10_Pin,
  LED_CH11_Pin,
  LED_CH12_Pin,
  LED_CH13_Pin,
  LED_CH14_Pin,
  LED_CH15_Pin,
  LED_CH16_Pin,
};

void LED_CTRL( uint8_t ctrl,uint16_t LedIdx ){
  if(ctrl){
    HAL_GPIO_WritePin(LEDs_Ports[LedIdx], LEDs_Pins[LedIdx], GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(LEDs_Ports[LedIdx], LEDs_Pins[LedIdx], GPIO_PIN_RESET);
  }
}

//=================== Calibration ============================//

void drv_Optic_reg_LoadCalTable(void){
  if(ReadCalibration(&CalibrationValues)){
    for(int curr_ch = 0;curr_ch < MAX_ADC_VALUES;curr_ch++){
      optic_cal_table[curr_ch] = CalibrationValues.cbCalCoef[curr_ch];
      optic_idx_table[curr_ch] = CalibrationValues.cbCoefIdx[curr_ch];
    }
  }else{

  }
}

uint16_t drv_Optic_reg_SaveCalTable(){ 

  for(int curr_ch = 0;curr_ch < MAX_ADC_VALUES;curr_ch++){
      CalibrationValues.cbCalCoef[curr_ch] = optic_cal_table[curr_ch];
      CalibrationValues.cbCoefIdx[curr_ch] = optic_idx_table[curr_ch];
  }
  return WriteCalibration(&CalibrationValues);
}