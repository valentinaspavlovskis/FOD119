#include "kernel.h"
#include "kernel.h"
#include "cmsis_os.h"
#include "dac.h"
#include "os_keyb.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int8_t KeybLockFlg = 0;


void kernel_default_tsk_init()
{
  LED_G_HIGH();
  MX_DAC_Init();


LED_CH1_HIGH();
LED_CH2_HIGH();
LED_CH3_HIGH();
LED_CH4_HIGH();
LED_CH5_HIGH();
LED_CH6_HIGH();
LED_CH7_HIGH();
LED_CH8_HIGH();
LED_CH9_HIGH();
LED_CH10_HIGH();
LED_CH11_HIGH();
LED_CH12_HIGH();
LED_CH13_HIGH();
LED_CH14_HIGH();
LED_CH15_HIGH();
LED_CH16_HIGH();
  { /* DEBUG LOG */
    //printf("--- Boot finished.---\n");
  }
}



void kernel_default_tsk_run()
{
  
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