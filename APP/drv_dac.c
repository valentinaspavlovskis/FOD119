/* Includes ------------------------------------------------------------------*/
#include "drv_dac.h"
#include "dac.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Function prototypes -------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//uint16_t dac_value;
/*Private function prototypes ------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
extern DAC_HandleTypeDef hdac;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Deinitialize DAC driver.
  * @param  None.
  * @retval None
  */
void drv_dac_DeInit(void)
{
  MX_DAC_DeInit();
}

/**
  * @brief  Initialize DAC driver.
  * @param  None.
  * @retval None
  */
void drv_dac_Init(void)
{
  MX_DAC_Init();
  
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
}


/**
  * @brief  Initialize DAC driver.
  * @param  None.
  * @retval None
  */
void drv_dac_set_(uint16_t value)
{
  if(value <= 4095){
    //dac_value = value;
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
  }
}