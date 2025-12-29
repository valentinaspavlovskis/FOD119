#ifndef __KERNEL_MSG_TYPE_H__
#define __KERNEL_MSG_TYPE_H__

typedef enum {
  MSG_ERR = -1,
  MSG_IDLE = 0,
  MSG_INIT,
  MSG_USB_READY,
  MSG_USB_DISCONNECT,
  MSG_TIMER_10MS,
  MSG_LCD_TIMER,
  MSG_POWER_PROC, /* battery measurement (each 1-2 sec.) */
  MSG_POWER_OFF,
  
  MSG_HID_DESCRIPTOR_WRITE_BUFF, /* String descriptors */
  MSG_HID_DESCRIPTOR_CTRL, /* String descriptors control: 0-reserved, 1-Product, 2-SN, 3-firmware version */
  MSG_HID_DEVICE_CTRL

}WMTypeDef;


typedef struct kernel_msg_t_{
  WMTypeDef id; /* control message id */
  uint32_t p1; /* param1 */
  uint32_t p2; /* param2 */
}kernel_msg_t;

int8_t kernel_send_msg(WMTypeDef id, uint32_t p1, uint32_t p2, int8_t use_alloc);


#endif //__KERNEL_MSG_TYPE_H__