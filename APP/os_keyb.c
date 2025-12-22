/**
  ******************************************************************************
  * @file    os_keyb.c 
  * @author  vk
  * @version V1.0.0
  * @date    2014.05.16
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "os_keyb.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Keyboard poll rate*/
TickType_t os_keyb_poll_rate_ticks;
uint32_t os_keyb_poll_rate_ms;
/* Keyboard task handle */
TaskHandle_t os_keyb_TaskHandle = NULL;

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void os_keyb_init(uint32_t poll_rate_ms);
uint8_t os_keyb_KeyGet(KeybKey_TypeDef keyb_key);
void os_keyb_KeyLock(KeybKey_TypeDef keyb_key);
uint32_t os_keyb_KeyPressedGet(KeybKey_TypeDef keyb_key);
void os_keyb_PollRateSet(uint32_t poll_rate_ms);
void os_keyb_PollRateUpdate();
TickType_t os_keyb_PollRateTicksGet();

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize OS Keyboard driver.
  * @param  poll_rate_ms: Specifies keyboard poll rate in units of ms.
  * @retval None
  */
void os_keyb_init(uint32_t poll_rate_ms)
{
  /* Initialize Keyboard driver */
  drv_Keyb_Init();

  /* Set Keyboard Poll Rate */
  os_keyb_PollRateSet(poll_rate_ms);
}

/**
  * @brief  Get Keyboard Key.
  * @param  keyb_key: Specifies which keyboard key to get.
  * @retval Returns non zero value if key is pressed
  */
uint8_t os_keyb_KeyGet(KeybKey_TypeDef keyb_key) 
{
  uint8_t pressed;
  taskENTER_CRITICAL();
  pressed = drv_Keyb_KeyGet(keyb_key);
  taskEXIT_CRITICAL();
  return pressed;
}

/**
  * @brief  Lock Keyboard Key.
  * @param  keyb_key: Specifies which keyboard key to lock.
  * @retval None
  */
void os_keyb_KeyLock(KeybKey_TypeDef keyb_key) 
{
  taskENTER_CRITICAL();
  drv_Keyb_KeyLock(keyb_key);
  taskEXIT_CRITICAL();
}

/**
  * @brief  Get Keyboard Key Pressed Counter.
  * @param  keyb_key: Specifies which keyboard key to get.
  * @retval Pressed Key Counter in units of ms
  */
uint32_t os_keyb_KeyPressedGet(KeybKey_TypeDef keyb_key)
{
  uint16_t pressed_cnt;
  taskENTER_CRITICAL();
  pressed_cnt = drv_Keyb_KeyPressedGet(keyb_key);
  taskEXIT_CRITICAL();
  return pressed_cnt * os_keyb_poll_rate_ms;
}

/**
  * @brief  Set Keyboard Poll Rate.
  * @param  poll_rate_ms: Specifies keyboard poll rate in units of ms.
  * @retval None
  */
void os_keyb_PollRateSet(uint32_t poll_rate_ms)
{
  os_keyb_poll_rate_ms = poll_rate_ms;
  os_keyb_poll_rate_ticks = (configTICK_RATE_HZ * os_keyb_poll_rate_ms) / (TickType_t)1000;
}

/**
  * @brief  Update Keyboard Poll Rate.
  * @param  None
  * @retval None
  */
void os_keyb_PollRateUpdate()
{
  os_keyb_poll_rate_ticks = (configTICK_RATE_HZ * os_keyb_poll_rate_ms) / (TickType_t)1000;
}

/**
  * @brief  Get Keyboard Poll Rate in ticks.
  * @param  None
  * @retval Keyboard Poll Rate in ticks
  */

TickType_t os_keyb_PollRateTicksGet()
{
  return os_keyb_poll_rate_ticks;
}

