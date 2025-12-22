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