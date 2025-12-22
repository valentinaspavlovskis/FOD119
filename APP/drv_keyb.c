/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "drv_keyb.h"
#include "main.h"

/* Private define ------------------------------------------------------------*/
 /* Number of keys in keyboard */
#define KEYB_N ( 3 )

/* Keyboard ports */
#define KEYB_PORT_UP    SW_UP__GPIO_Port
#define KEYB_PORT_DOWN  SW_DOWN__GPIO_Port

/* Keyboard pins */
#define KEYB_PIN_UP      SW_UP__Pin 
#define KEYB_PIN_DOWN    SW_DOWN__Pin 

/* Keyboard key bit mask in keyboard bitmap */
#define KEYB_CODE_BITMASK_NONE   ( 0x00 ) /* No pressed keys*/
#define KEYB_CODE_BITMASK_UP     ( 0x01 ) /* Bitmask of UP key */
#define KEYB_CODE_BITMASK_DOWN   ( 0x02 ) /* Bitmask of DOWN key */
#define KEYB_CODE_BITMASK_PWR    ( 0x04 ) /* Bitmask of POWER key */

/* Debounce counter Threshold */
#define KEYB_DEBOUNCE_CNT ( 1 )

/* Private typedef -----------------------------------------------------------*/

/** 
  * @brief  Keyboard State structure definition
  */ 
typedef struct KeybState_t_
{
  uint32_t CodeBitMap; /* Bitmap of keys */
  uint32_t LockBitFlg;  /* Locked Key Flag */  
  uint16_t PressedCnt[KEYB_N]; /* Pressed Key counter */
} KeybState_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Keybord key to bitmap translation table */
const uint32_t KeybKeyToBitmap[KEYB_N] = {
  KEYB_CODE_BITMASK_UP, KEYB_CODE_BITMASK_DOWN,KEYB_CODE_BITMASK_PWR
};
uint8_t KeybDebounceCnt[KEYB_N]; /* Debounce Counter for every Key */
KeybState_t KeybState; /* Keyboard state */

/* Private function prototypes -----------------------------------------------*/
/* Global variables ----------------------------------------------------------*/

/* Function prototypes -----------------------------------------------*/
void drv_Keyb_DeInit(void);
void drv_Keyb_Init(void);
uint8_t drv_Keyb_KeyGet(KeybKey_TypeDef keyb_key);
void drv_Keyb_KeyLock(KeybKey_TypeDef keyb_key);
uint16_t drv_Keyb_KeyPressedGet(KeybKey_TypeDef keyb_key);
void drv_Keyb_Scan(void);

static void KeybScanButton(uint8_t port_bit, KeybKey_TypeDef keyb_key);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Deinitialize Keyboard driver.
  * @param  None.
  * @retval None
  */
void drv_Keyb_DeInit(void)
{
#if 0
  {
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Disbale EXTI Line 1, 2 interrupts */
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line2;  /* PE1, PE2 for Keyboard buttons */
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Clear EXTI Line 1, 2 pending bits */
    EXTI_ClearITPendingBit(EXTI_Line1 | EXTI_Line2);
    
    /* Clear the EXTI1 pending bit in NVIC */
    NVIC_ClearPendingIRQ(EXTI1_IRQn);
    /* Clear the EXTI2 pending bit in NVIC */
    NVIC_ClearPendingIRQ(EXTI2_IRQn);

    /* Disable EXTI1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Disable EXTI2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
  }
#endif
}

/**
  * @brief  Initialize Keyboard driver.
  * @param  None.
  * @retval None
  */
void drv_Keyb_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  {
    int i;
    KeybState.CodeBitMap = 0;
    KeybState.LockBitFlg = 0;
    for(i=0; i<KEYB_N; i++){KeybState.PressedCnt[i] = 0;}
  }
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pins : SW_UP_Pin SW_DOWN_Pin */  
  GPIO_InitStruct.Pin = SW_UP__Pin|SW_DOWN__Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/**
  * @brief  Get Keyboard Key.
  * @param  keyb_key: Specifies which keyboard key to get.
  * @retval Returns non zero value if key is pressed
  */
uint8_t drv_Keyb_KeyGet(KeybKey_TypeDef keyb_key) 
{
  uint32_t bitmap = KeybKeyToBitmap[(int)keyb_key];
  return (KeybState.CodeBitMap & bitmap) && !(KeybState.LockBitFlg & bitmap);
}

/**
  * @brief  Lock Keyboard Key.
  * @param  keyb_key: Specifies which keyboard key to lock.
  * @retval None
  */
void drv_Keyb_KeyLock(KeybKey_TypeDef keyb_key) 
{
  uint32_t bitmap = KeybKeyToBitmap[(int)keyb_key];
  KeybState.LockBitFlg |= bitmap;
}

/**
  * @brief  Get Keyboard Key Pressed Counter.
  * @param  keyb_key: Specifies which keyboard key to get.
  * @retval Pressed Key Counter
  */
uint16_t drv_Keyb_KeyPressedGet(KeybKey_TypeDef keyb_key)
{
  int key_indx = keyb_key;
  uint32_t bitmap = KeybKeyToBitmap[key_indx];
  return (KeybState.LockBitFlg & bitmap) ? 0 : KeybState.PressedCnt[key_indx];
}

/**
  * @brief  Scan all keyboard buttons.
  * @param  None.
  * @retval None
  */
void drv_Keyb_Scan(void)
{
  /* Scan all Keys */
  //KeybScanButton(!GPIO_ReadInputDataBit(KEYB_PORT_UP, KEYB_PIN_UP), Key_UP);
  //KeybScanButton(!GPIO_ReadInputDataBit(KEYB_PORT_DOWN, KEYB_PIN_DOWN), Key_DOWN);
  //up = HAL_GPIO_ReadPin(KEYB_PORT_UP, Key_UP);
  //down = HAL_GPIO_ReadPin(KEYB_PORT_DOWN, Key_DOWN);
    
  KeybScanButton(!HAL_GPIO_ReadPin(GPIOE, SW_UP__Pin), Key_UP);
  KeybScanButton(!HAL_GPIO_ReadPin(GPIOE, SW_DOWN__Pin), Key_DOWN);
  KeybScanButton(!HAL_GPIO_ReadPin(SW_PWR__GPIO_Port, SW_PWR__Pin), Key_PWR);
}

/**
  * @brief  Scan one keyboard button.
  * @param  port_bit: Port bit value of Keyboard Button signal(0->inactive, else active).
  * @param  keyb_key: Specifies which keyboard button to scan.
  * @retval None
  */
static void KeybScanButton(uint8_t port_bit, KeybKey_TypeDef keyb_key) 
{
  int key_indx = keyb_key;
  if(KeybState.PressedCnt[key_indx]){ /* Key already pressed */
    if(port_bit){ /* Key still pressed */
      if(KeybState.PressedCnt[key_indx] < UINT16_MAX){ KeybState.PressedCnt[key_indx]++;}
      KeybDebounceCnt[key_indx] = KEYB_DEBOUNCE_CNT;
    }else{ /* Key released */
      if(KeybDebounceCnt[key_indx] == 0){ /* Debounce counter expired */
        KeybState.PressedCnt[key_indx] = 0;
        KeybState.LockBitFlg &= ~KeybKeyToBitmap[key_indx];
        KeybState.CodeBitMap &= ~KeybKeyToBitmap[key_indx];
      }else{
        KeybDebounceCnt[key_indx]--;
      }
    }
  }else{ /* Key allready released */
    if(port_bit == 0){ /* Key still released */
      KeybDebounceCnt[key_indx] = KEYB_DEBOUNCE_CNT;
    }else{ /* Key pressed */
      if(KeybDebounceCnt[key_indx] == 0){ /* Debounce counter expired */
        KeybState.PressedCnt[key_indx] = 1;
        KeybState.CodeBitMap |= KeybKeyToBitmap[key_indx];
      }else{
        KeybDebounceCnt[key_indx]--;
      }
    }
  }
}
