#include "kernel.h"
#include "kernel.h"
#include "cmsis_os.h"
#include "dac.h"
#include "os_keyb.h"
#include "drv_optic.h"
#include "kernel_msg_type.h"
#include "optic_msg_type.h"
#include "usb_fod_device.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

extern uint8_t starting_flag;  
uint8_t poweroff_flag  = 0;

int8_t KeybLockFlg = 0;
/* Keyboard Lock Flag */
uint8_t os_appv0_KeybLockFlg = 0;

uint32_t DevTicksRef1s = 0;
uint32_t DevTimeCaunter500ms = 0;
uint32_t currentTimeCaunter = 0;
uint32_t DevTicksRef500ms = 0;
uint32_t DevTicksRef50ms = 0;
uint32_t DevTicksRef10ms = 0;
uint32_t DevTicksRef100ms = 0;

uint32_t DAC_val = 0;

uint16_t key_pressed_cnt_up = 0;
uint16_t key_pressed_cnt_down = 0;
uint16_t key_pressed_cnt_power = 0;
uint8_t auto_sw_channal_on = 0;

typedef enum{
  mode_cw = 0,
  mode_2hz = 1,
}mode_t;

mode_t curr_mode = mode_cw;
uint16_t channel;


void kernel_default_tsk_init()
{
  LED_G_HIGH();
  

  drv_Optic_Init();
  
  /* init code for USB_DEVICE */
  BOARD_USB_FOD_Init();
  
  optic_send_msg(OPTIC_MSG_MSG_CHANNEL_SET, channel, 0, 1);
//LED_CH1_HIGH();
  { /* DEBUG LOG */
    //printf("--- Boot finished.---\n");
  }
}




void kernel_default_tsk_run()
{
  uint32_t ticks = osKernelSysTick();
  /* Get the time in MS. */
  TickType_t currentTime = pdTICKS_TO_MS( xTaskGetTickCount() );
  
  
  
    if(KeybLockFlg == 0){
    if(os_keyb_KeyGet(Key_UP) && !os_appv0_KeybLockFlg){

        os_keyb_KeyLock(Key_UP);
        /* Clear DOWN Key Pressed Counter */
        key_pressed_cnt_down = 0;

        if(key_pressed_cnt_up == 0){

          key_pressed_cnt_up++;
          
          if(auto_sw_channal_on){
              auto_sw_channal_on = 0;
              LED_AUTO_LOW();
          }
          /* Set and print OSW channel */
          channel = drv_Optic_GetChannel();
          
          channel++;
          if(channel >= drv_Optic_GetChannelNr()){
            auto_sw_channal_on = 1;
            LED_AUTO_HIGH();
            channel = 0;
          }
            
          optic_send_msg(OPTIC_MSG_MSG_CHANNEL_SET, channel, 0, 1);
        }else{

          if(key_pressed_cnt_up > 2){ /* Every 300 ms*/
            key_pressed_cnt_up = 0;
          }else{
            key_pressed_cnt_up++;
          }

        }

      }else if(os_keyb_KeyGet(Key_DOWN) && !os_appv0_KeybLockFlg){

        os_keyb_KeyLock(Key_DOWN); 
        
        /* Clear UP Key Pressed Counter */
        key_pressed_cnt_up = 0;

        if(key_pressed_cnt_down == 0){
          
          /* Set and print OSW channel */
          channel = drv_Optic_GetChannel();
          if(channel == 0){
            if(auto_sw_channal_on){
              uint16_t ch  = drv_Optic_GetChannelNr();
              if(drv_Optic_GetChannelNr() > 0){
                channel = drv_Optic_GetChannelNr() - 1;
              }else{
                channel = drv_Optic_GetChannelNr();
              }
              auto_sw_channal_on = 0;
              LED_AUTO_LOW();
            }else{
              auto_sw_channal_on = 1;
              LED_AUTO_HIGH();
            }
          }else{
            if(auto_sw_channal_on){
              auto_sw_channal_on = 0;
              LED_AUTO_LOW();
            }
            channel--;
          }
          optic_send_msg(OPTIC_MSG_MSG_CHANNEL_SET, channel, 0, 1);
            
          key_pressed_cnt_down++;

        }else{

          if(key_pressed_cnt_down > 2){ /* Every 300 ms*/
            key_pressed_cnt_down = 0;
          }else{
            key_pressed_cnt_down++;
          }
      }   

    }else if(os_keyb_KeyGet(Key_PWR) && !os_appv0_KeybLockFlg){
      if(!starting_flag){
        key_pressed_cnt_power++;

          if(key_pressed_cnt_power > 1000){
            //power off
            if(!poweroff_flag){
              poweroff_flag  = 1;
              SetPowerButtonOn();
              
              kernel_send_msg(MSG_POWER_OFF, 0, 0, 1);
              //poweroff_flag  = 0;
            }
          }
        //}
      }
    }else{
      /* Clear Key Pressed Counters */
      key_pressed_cnt_up = 0;
      key_pressed_cnt_down = 0;  
      if(starting_flag){
        starting_flag = 0;
      }
      if(key_pressed_cnt_power){
        key_pressed_cnt_power = 0;
      }

    }
  
  }else{
    /* Clear Key Pressed Counters */
    key_pressed_cnt_up = 0;
    key_pressed_cnt_down = 0;
    if(key_pressed_cnt_power){
      key_pressed_cnt_power = 0;
    }
  }
  
  
  
  
  
  if ((ticks - DevTicksRef10ms) >= 10){ /* 10 ms */
    DevTicksRef10ms = ticks;   
  }
    
  if ((ticks - DevTicksRef50ms) >= 50){ /* 50 ms */
        
    DevTicksRef50ms = ticks;      
  }

  if ((ticks - DevTicksRef100ms) >= 100){ /* 100 ms */
    DevTicksRef100ms = ticks;
    
//    drv_Optic_SetDAC(DAC_val);
//    
//    //suspendKernelTask();
//    optic_send_msg(OPTIC_MSG_MSG_UPDATE_DAC, 0, 0, 1);
//    //resumeKernelTask();
//    
//    DAC_val+=10;
//    if(DAC_val > 4095){
//      DAC_val = 0;
//    }
  }

  if((ticks - DevTicksRef500ms) >= 500){ /* 500ms */
    DevTicksRef500ms = ticks;
  }
  
  if((ticks - DevTicksRef1s) >= 1000){ /* 1s */
    DevTicksRef1s = ticks;
    
    if(auto_sw_channal_on){
        /* Set and print OSW channel */
        channel = drv_Optic_GetChannel();
        
        channel++;
        if(channel >= drv_Optic_GetChannelNr()){channel = 0;}
          
        optic_send_msg(OPTIC_MSG_MSG_CHANNEL_SET, channel, 0, 1);
    }
  }
}


void SetLockKeyb(void){
  KeybLockFlg = 1;
}

void SetUnLockKeyb(void){
  KeybLockFlg = 0;
}

int8_t GetKeybLock(void){
  return KeybLockFlg;
}