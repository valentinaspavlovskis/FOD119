/**
******************************************************************************
* @file    usb_mems.c
* @author  al
* @version V0.0.1
* @date    2018.04.12
* @brief   USB mems control interface
******************************************************************************
*/

#include "usb_bulk_cmd.h"
#include <stdio.h>
#include <string.h>
#include "adc.h"
//#include "calibration.h"
#include "usb_device.h"
#include "FreeRTOS.h"
#include "task.h"

typedef int8_t (*FuncPtr_0)(void * buf, uint32_t *n);
typedef struct usb_bulk_cmd_func_t_{
  uint16_t addr;  
  FuncPtr_0 func_rd;
  FuncPtr_0 func_wr;  
}usb_bulk_cmd_func_t;

extern uint16_t cpal_err_cnt;
extern uint16_t cpal_err_timeout_cnt;
extern int32_t Temperature;


static int8_t usb_bulk_cmd_err(void * buf, uint32_t *n);
static int8_t usb_bulk_proc(void * buf, uint16_t addr, uint32_t *n, int8_t dir);

static int8_t usb_mems_cmd_read_device_info(void * buf, uint32_t *n);
static int8_t usb_bulk_cmd_adc_read(void * buf, uint32_t *n);
static int8_t usb_bulk_cmd_power_read(void * buf, uint32_t *n);
static int8_t usb_bulk_cmd_cal_get(void * buf, uint32_t *n);

static int8_t usb_bulk_cmd_ctrl_led(void * buf, uint32_t *n);
static int8_t usb_bulk_cmd_cal_set(void * buf, uint32_t *n);
static int8_t usb_bulk_cmd_cal_save(void * buf, uint32_t *n);

#define USB_BULK_FUNC_N (16)

/* fod protocol command processing function array */
const usb_bulk_cmd_func_t usb_bulk_cmd_func_array[USB_BULK_FUNC_N] = {
  /* addr,    read cb,                          write cb */
  { 0x0000, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x0001, &usb_mems_cmd_read_device_info,     &usb_bulk_cmd_err},   
  { 0x0002, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err}, 
  { 0x0003, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err}, 
  { 0x0004, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x0005, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x0006, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x0007, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x0008, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x0009, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x000A, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x000B, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x000C, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x000D, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x000E, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
  { 0x000F, &usb_bulk_cmd_err,                  &usb_bulk_cmd_err},
};

/* USB remote control read function */
int8_t usb_bulk_read(void * buf, uint16_t addr, uint32_t *n)
{
  return usb_bulk_proc(buf, addr, n, 0);
}

/* USB remote control write function */
int8_t usb_bulk_write(void * buf, uint16_t addr, uint32_t *n)
{
  return usb_bulk_proc(buf, addr, n, 1);
}

/* USB remote control read/write function. Finds callback by address */
static int8_t usb_bulk_proc(void * buf, uint16_t addr, uint32_t *n, int8_t dir)
{
  uint16_t i;

  for(i = 0; i < USB_BULK_FUNC_N; i++){
    if (addr == usb_bulk_cmd_func_array[i].addr){
      break;
    }
  }
  
  if (i < USB_BULK_FUNC_N){
    if (dir == 0){
      return usb_bulk_cmd_func_array[i].func_rd(buf, n);    
    }
    return usb_bulk_cmd_func_array[i].func_wr(buf, n);
  }
  
  *n = 0;
  return 1;
}

/* callback for non-supported response */
static int8_t usb_bulk_cmd_err(void * buf, uint32_t *n)
{
  *n = 0;
  return 1;
}

//==============================================================================

static int8_t usb_mems_cmd_read_device_info(void * buf, uint32_t *n){
/*  
  info_verson 
  device_name
  device_software_version
  serial
*/
  uint8_t len = 0;
  char txt[50];

  memset(txt,0,sizeof(txt));
  snprintf(txt,sizeof(txt)-1,"Product Name: %s",drv_hw_ProductNamePtrGet());
  strcpy(&((char*)buf)[len],  txt);
  len += strlen( txt) + 1;
  
  snprintf(txt,sizeof(txt)-1,"SN.: %s",drv_hw_SerialNumberPtrGet());
  strcpy(&((char*)buf)[len],  txt);
  len += strlen( txt) + 1;
  
  snprintf(txt,sizeof(txt)-1,"Software Ver.: %s",drv_hw_FrimwareVersionPtrGet());
  strcpy(&((char*)buf)[len], txt);
  len += strlen(txt) + 1;
  
  *n = len;
  return 0;
}


static int8_t usb_bulk_cmd_ctrl_led(void * buf, uint32_t *n){
  uint8_t led_reg = 0;
  if (*n != 1){
    return 1;
  }
  led_reg = *((uint8_t*)buf + 0);
//  //========== Led VD5 =============//
//  if(__TEST_BIT(led_reg,0)){
//    LED_PASS_G_0_ON();
//  }else{
//    LED_PASS_G_0_OFF();
//  }
//  
//  if(__TEST_BIT(led_reg,1)){
//    LED_PASS_R_0_ON();
//  }else{
//    LED_PASS_R_0_OFF();
//  }
//  //========== Led VD6 =============//
//  if(__TEST_BIT(led_reg,2)){
//    LED_PASS_G_1_ON();
//  }else{
//    LED_PASS_G_1_OFF();
//  }
//  
//  if(__TEST_BIT(led_reg,3)){
//    LED_PASS_R_1_ON();
//  }else{
//    LED_PASS_R_1_OFF();
//  }
//  //========== Led VD7 =============//
//  if(__TEST_BIT(led_reg,4)){
//    LED_PASS_G_2_ON();
//  }else{
//    LED_PASS_G_2_OFF();
//  }
//  
//  if(__TEST_BIT(led_reg,5)){
//    LED_PASS_R_2_ON();
//  }else{
//    LED_PASS_R_2_OFF();
//  }
  return 0;
}

static int8_t usb_bulk_cmd_adc_read(void * buf, uint32_t *n){
  union{
    uint16_t read_data;
    uint8_t b[2];    
  }d;
  uint8_t len = 0;
  
  vTaskSuspendAll();
  
  //__disable_irq();
  
//  measurement_t* curr_val = GetMeasurements();
//  
//  d.read_data = curr_val->aout0;
//  memcpy(&((uint8_t*)buf)[len],d.b,2);
//  len = 2;
//  
//  d.read_data = curr_val->aout1;
//  memcpy(&((uint8_t*)buf)[len],d.b,2);
//  len += 2;
//  
//  d.read_data = curr_val->aout2;
//  memcpy(&((uint8_t*)buf)[len],d.b,2);
//  len += 2;
//  
//  d.read_data = curr_val->vbat;
//  memcpy(&((uint8_t*)buf)[len],d.b,2);
//  len += 2;
//  
//  d.read_data = curr_val->soc;
//  memcpy(&((uint8_t*)buf)[len],d.b,2);
//  len += 2;
//  
//  d.read_data = curr_val->temp;
//  memcpy(&((uint8_t*)buf)[len],d.b,2);
//  len += 2;
//  
//  *n = len;
  
  //__enable_irq();
  xTaskResumeAll();
  return 0;
}

static int8_t usb_bulk_cmd_power_read(void * buf, uint32_t *n){
  union{
    uint16_t read_data;
    uint8_t b[2];    
  }d;
  uint8_t len = 0;
  
  vTaskSuspendAll();
//  measurement_t* curr_val = GetMeasurements();
//  
//  uint16_t wl = CalibrationValues.cbWL;
//  memcpy(&((uint8_t*)buf)[len],&wl,sizeof(uint16_t));
//  len += sizeof(uint16_t);
//  
//  d.read_data = curr_val->aout0;
//  memcpy(&((uint8_t*)buf)[len],d.b,2);
//  len += 2;
//  
//  float cal_coef = GetCalValue();
//  memcpy(&((uint8_t*)buf)[len],&cal_coef,sizeof(float));
//  len += sizeof(float);
//  
//  float pwr = -100.0;
//  
//  pwr = CalcPower(curr_val->aout0);
//  
//  memcpy(&((uint8_t*)buf)[len],&pwr,sizeof(float));
//  len += sizeof(float);
  
  xTaskResumeAll();
  *n = len;
  return 0;
}

static int8_t usb_bulk_cmd_cal_set(void * buf, uint32_t *n){
  if (*n != 6){
    return 1;
  }
//  uint16_t wl = 0;
//  float coef = 0.0;
//  
//  memcpy(&wl, &((uint8_t*)buf)[0], sizeof(wl));
//  memcpy(&coef, &((uint8_t*)buf)[2], sizeof(coef));
//  
//  SetCalPowerWL(wl);
//  SetCalValue(coef);
    
  return 0;
}

static int8_t usb_bulk_cmd_cal_get(void * buf, uint32_t *n){
  uint8_t len = 0;
//  float coef = GetCalValue();
//  uint16_t wl = CalibrationValues.cbWL;
//  memcpy(&((uint8_t*)buf)[0], &wl, 2);
//  len += 2;
//  memcpy(&((uint8_t*)buf)[len], &coef, sizeof(coef));
//  len += 4;
  
  *n = len;
  return 0;  
}

static int8_t usb_bulk_cmd_cal_save(void * buf, uint32_t *n){

//  if (drv_SaveCalTable() != 0){
//    return 1;
//  }
    
  return 0;
}
