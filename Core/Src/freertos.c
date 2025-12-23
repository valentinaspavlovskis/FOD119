/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "kernel.h"
#include "memory.h"
#include "os_keyb.h"
#include "usb_device.h"
#include "iwdg.h"
#include "usbd_fod.h"
#include "usbd_fod_bot.h"
#include "usbd_fod_desc.h"
#include "usb_fod_device.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USB Present Flag */
uint8_t usb_present = 0;
uint8_t UsbKeybLockFlag = 0;
/* USB Present Debounce counter */
uint8_t usb_debounce_cnt;
extern USBD_HandleTypeDef hUsbDeviceFS;

TickType_t os_prd_rate_ticks;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId timerTaskHandle;
osThreadId PrdHandle;
osThreadId UART_stdoutHandle;
osThreadId AppOpticHandle;
osThreadId usbTaskHandle;
osMessageQId kernelQueueHandle;
osMessageQId opticQueueHandle;
osMessageQId stdout_queueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTimerTask(void const * argument);
void prdTask(void const * argument);
void UART_stdout_func(void const * argument);
void appTaskOptic(void const * argument);
void appBulkUSB(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of kernelQueue */
  osMessageQDef(kernelQueue, 32, uint32_t);
  kernelQueueHandle = osMessageCreate(osMessageQ(kernelQueue), NULL);

  /* definition and creation of opticQueue */
  osMessageQDef(opticQueue, 5, uint32_t);
  opticQueueHandle = osMessageCreate(osMessageQ(opticQueue), NULL);

  /* definition and creation of stdout_queue */
  osMessageQDef(stdout_queue, 30, uint32_t);
  stdout_queueHandle = osMessageCreate(osMessageQ(stdout_queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 192);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of timerTask */
  osThreadDef(timerTask, StartTimerTask, osPriorityIdle, 0, 128);
  timerTaskHandle = osThreadCreate(osThread(timerTask), NULL);

  /* definition and creation of Prd */
  osThreadDef(Prd, prdTask, osPriorityIdle, 0, 128);
  PrdHandle = osThreadCreate(osThread(Prd), NULL);

  /* definition and creation of UART_stdout */
  osThreadDef(UART_stdout, UART_stdout_func, osPriorityIdle, 0, 128);
  UART_stdoutHandle = osThreadCreate(osThread(UART_stdout), NULL);

  /* definition and creation of AppOptic */
  osThreadDef(AppOptic, appTaskOptic, osPriorityIdle, 0, 160);
  AppOpticHandle = osThreadCreate(osThread(AppOptic), NULL);

  /* definition and creation of usbTask */
  osThreadDef(usbTask, appBulkUSB, osPriorityIdle, 0, 192);
  usbTaskHandle = osThreadCreate(osThread(usbTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  //MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  kernel_default_tsk_init();
  
/* init code for USB_DEVICE */
  BOARD_USB_FOD_Init();
  /* Infinite loop */
  for(;;)
  {
    kernel_default_tsk_run();
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTimerTask */
/**
* @brief Function implementing the timerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTimerTask */
void StartTimerTask(void const * argument)
{
  /* USER CODE BEGIN StartTimerTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTimerTask */
}

/* USER CODE BEGIN Header_prdTask */
/**
* @brief Function implementing the Prd thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_prdTask */
void prdTask(void const * argument)
{
  /* USER CODE BEGIN prdTask */
  TickType_t currentTime, lasTimeKeyb;
  
  /* Initialize OS Keyboard driver. Set Keyboard poll rate to 40ms */
  os_keyb_init(40);
  
  usb_debounce_cnt = 0;
  
  lasTimeKeyb = xTaskGetTickCount();
  drv_Keyb_Scan();
  
/* Infinite loop */
  for(;;)
  {
    
    vTaskDelay(os_prd_rate_ticks);  
    currentTime = xTaskGetTickCount();
    
    /* Periodical Keyboard Scan */
    if((currentTime - lasTimeKeyb) >= os_keyb_PollRateTicksGet()){
      lasTimeKeyb = currentTime;
      drv_Keyb_Scan();
    }
    
    /* USB Present Scan */
    {
      if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){
        if(usb_debounce_cnt >= 5){
          usb_present = 1;
        }else{
          usb_debounce_cnt++;
        }
      }else{
        usb_debounce_cnt = 0;
        usb_present = 0;
      }
    }
    
    if(GetKeybLock() && (usb_present == 0)){
      SetUnLockKeyb();
    }
    
    if(usb_present && UsbKeybLockFlag && (GetKeybLock() == 0)){
      SetLockKeyb();
    }
    
    /* Reset watchdog */
    MX_IWDG_Reset();
    
    osDelay(1);
  }
  /* USER CODE END prdTask */
}

/* USER CODE BEGIN Header_UART_stdout_func */
/**
* @brief Function implementing the UART_stdout thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UART_stdout_func */
void UART_stdout_func(void const * argument)
{
  /* USER CODE BEGIN UART_stdout_func */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END UART_stdout_func */
}

/* USER CODE BEGIN Header_appTaskOptic */
/**
* @brief Function implementing the AppOptic thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_appTaskOptic */
void appTaskOptic(void const * argument)
{
  /* USER CODE BEGIN appTaskOptic */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END appTaskOptic */
}

/* USER CODE BEGIN Header_appBulkUSB */
/**
* @brief Function implementing the usbTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_appBulkUSB */
void appBulkUSB(void const * argument)
{
  /* USER CODE BEGIN appBulkUSB */
  uint32_t DevTicksRef200ms = 0;
  uint32_t DevTicksRef500ms = 0;
  /* Infinite loop */
  for(;;)
  {
    uint32_t ticks = osKernelSysTick();  
  
    if ((ticks - DevTicksRef200ms) >= 200){ /* 200 ms */
      DevTicksRef200ms = ticks;
      /* Reset watchdog */
      //MX_IWDG_Reset();
    }
    
    if ((ticks - DevTicksRef500ms) >= 500){ /* 500 ms */
      DevTicksRef500ms = ticks;
    }
    if (FODUSBRequest){
      fodUSBMainLoop(&hUsbDeviceFS);
      FODUSBRequest = 0;
    }
    
    osDelay(1);
  }
  /* USER CODE END appBulkUSB */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

