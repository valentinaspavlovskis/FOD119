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
#include "optic_msg_type.h"
#include "usb_device.h"
#include "iwdg.h"
#include "usbd_fod.h"
#include "usbd_fod_bot.h"
#include "usbd_fod_desc.h"
#include "usb_fod_device.h"
#include "drv_optic.h"
#include "kernel_msg_type.h"
#include "optic_msg_type.h"
#include "uart_msg_type.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern uint8_t starting_flag;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t power_release = 0;
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
  /* USER CODE BEGIN StartDefaultTask */
  uint8_t key_buf[3];
  /* Clear Key buffer */
  key_buf[0]=key_buf[1]=key_buf[2]=0;
  
  //kernel_default_tsk_init();
  
/* init code for USB_DEVICE */
  BOARD_USB_FOD_Init();
  /* Infinite loop */
  for(;;)
  {
    
    osEvent event;
    uint8_t SendBuffer[2];
    event = osMessageGet(kernelQueueHandle, 0);
    if(event.status == osEventMessage){
      uint32_t q_msg = event.value.v;
      kernel_msg_t* kernel_msg;
      kernel_msg_t msg_l;
      if(q_msg & 0xFFFF0000){
        /* use adders to struct */
        kernel_msg = (kernel_msg_t *)event.value.v;
      }else{
        msg_l.id = (WMTypeDef)q_msg;
        msg_l.p1 = 0;
        msg_l.p2 = 0;
        kernel_msg = &msg_l; 
      }
      
      switch(kernel_msg->id){  
        case MSG_IDLE: 
          {  
            
          }
          break;
        case MSG_INIT:
        {
          kernel_default_tsk_init();
        }
        break;
        case MSG_POWER_OFF:
          {
            IAD_USB_DEVICE_DeInit();
            for(;;){
              if(power_release){
                //wait for release
                if(!os_keyb_KeyGet(Key_PWR)){
                  power_release = 0;
                }
                osDelay(1);
//              }else if(HW_IS_CHARGER_CONNECTED()){
//                //wait for power on
//                if(os_keyb_KeyGet(Key_PWR)){
//                  starting_flag = 1;
//                  kernel_send_msg(MSG_INIT, 0, 0, 1);
//                  break;
//                }
//                osDelay(1);
              }else{
                vTaskSuspendAll();
                SHUT_DOWN();
                for(;;){
                }
              }
            }
          }
        
          case MSG_USB_READY:      
            {
            
            }break;
            
          case MSG_USB_DISCONNECT:
            {
              
            }break;
          default:
              break;
          }
          if(q_msg & 0xFFFF0000){
            mem_free(kernel_msg);
        }    
     }
   
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
  osEvent event;
  uint16_t ch;
  
  /* Infinite loop */
  for(;;)
  {
    event = osMessageGet(opticQueueHandle, 0);
    if(event.status == osEventMessage){
      uint32_t q_msg = event.value.v;
      optic_msg_t* optic_msg;
      optic_msg_t msg_l;
      if(q_msg & 0xFFFF0000){
          /* use adders to struct */
          optic_msg = (optic_msg_t *)event.value.v;
        }else{
          msg_l.id = (OsOptic_TypeDef)q_msg;
          msg_l.p1 = 0;
          msg_l.p2 = 0;
          optic_msg = &msg_l; 
        }  
      switch(optic_msg->id){  
          case OPTIC_MSG_IDLE: 
            {  
              
            }
            break;
          case OPTIC_MSG_MSG_CHANNEL_READ:      
            {
            
            }break;
            
          case OPTIC_MSG_MSG_CHANNEL_SET:
            {
              uint16_t channal = ((uint16_t)optic_msg->p1);
              ch = channal;
              drv_Optic_SetChannel(ch);
            }break;
          case OPTIC_MSG_MSG_UPDATE_DAC:
              {
                while(FODUSBRequest){
                  osDelay(1);
                }
                vTaskSuspend (usbTaskHandle);
                drv_Optic_Update_DAC();
                vTaskResume (usbTaskHandle);
              }break;
          case OPTIC_MSG_MSG_READ_DAC:
            {
            
            }break;
          default:
            break;
        }
        if(q_msg & 0xFFFF0000){
          mem_free(optic_msg);
        }    
      }
  
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
void suspendKernelTask(void){
  vTaskSuspend (defaultTaskHandle);
}

void resumeKernelTask(void){
  vTaskResume (defaultTaskHandle);
}
                
void SetPowerButtonOn(void){                   
 power_release = 1;
}

/**
* @brief Put a Message to a kernel Queue.
* @param  queue_id  message queue ID
* @param  p1  message information.
* @param  p2  additional information.
* @param  p3  using struct bit.
* @retval none.
* @note  after get massage each time must by call mem_free
*/
int8_t optic_send_msg(OsOptic_TypeDef id, uint32_t p1, uint32_t p2, int8_t use_alloc)
{
  if(use_alloc){
    optic_msg_t *msg = mem_malloc(sizeof(optic_msg_t));
    //assert_m(msg);
    msg->id = id;
    msg->p1 = p1;
    msg->p2 = p2;
    if (osMessagePut(opticQueueHandle, (uint32_t)msg, 0) != osOK){
      mem_free(msg);
      return 1;
    }
  }else{
    if (osMessagePut(opticQueueHandle, id, 0) != osOK){
      return 1;
    }
  }
  return 0;
} 

/**
* @brief Put a Message to a kernel Queue.
* @param  queue_id  message queue ID
* @param  p1  message information.
* @param  p2  additional information.
* @param  p3  using struct bit.
* @retval none.
* @note  after get massage each time must by call mem_free
*/
int8_t kernel_send_msg(WMTypeDef id, uint32_t p1, uint32_t p2, int8_t use_alloc)
{
  if(use_alloc){
    kernel_msg_t *msg = mem_malloc(sizeof(kernel_msg_t));
    //assert_m(msg);
    msg->id = id;
    msg->p1 = p1;
    msg->p2 = p2;
    if (osMessagePut(kernelQueueHandle, (uint32_t)msg, 0) != osOK){
      mem_free(msg);
      return 1;
    }
  }else{
    if (osMessagePut(kernelQueueHandle, id, 0) != osOK){
      return 1;
    }
  }
  return 0;
}

/**
* @brief Put a Message to a kernel Queue.
* @param  queue_id  message queue ID
* @param  p1  message information.
* @param  p2  additional information.
* @param  p3  using struct bit.
* @retval none.
* @note  after get massage each time must by call mem_free
*/
int8_t uart_send_msg(UARTMTypeDef id, uint32_t p1, uint32_t p2, int8_t use_alloc)
{
  if(use_alloc){
    uart_msg_t *msg = mem_malloc(sizeof(uart_msg_t));
    //assert_m(msg);
    msg->id = id;
    msg->p1 = p1;
    msg->p2 = p2;
    if (osMessagePut(stdout_queueHandle, (uint32_t)msg, 0) != osOK){
      mem_free(msg);
      return 1;
    }
  }else{
    if (osMessagePut(stdout_queueHandle, id, 0) != osOK){
      return 1;
    }
  }
  return 0;
}
/* USER CODE END Application */

