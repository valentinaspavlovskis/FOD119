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
uint16_t optic_osw_cal_table_idx[MAX_ADC_VALUES]={ 
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

int16_t optic_osw_idx_table[MAX_ADC_VALUES]={
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
}

/**
  * @brief  Initialize Optic driver.
  * @param  None.
  * @retval None
  */
void drv_Optic_Init(void)
{
  drv_dac_Init();
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



void drv_Optic_Update_DAC(){
  drv_dac_set_(dac_value);
}

void drv_Optic_SetDAC(uint16_t value){
  dac_value = value;
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
   
  drv_Optic_SetDAC(optic_osw_cal_table_idx[optic_osw_idx_table[optic_osw_ch]]);
  drv_Optic_Update_DAC();
  
  LED_CTRL( 0, curr_ch);
  curr_ch = optic_osw_ch;
  LED_CTRL( 1, curr_ch);  
  return 0;
}


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
