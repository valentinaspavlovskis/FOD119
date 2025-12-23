#ifndef __USB_MEMS_H__
#define __USB_MEMS_H__

#include "stm32l0xx.h"

int8_t usb_bulk_read(void * buf, uint16_t addr, uint32_t *n);
int8_t usb_bulk_write(void * buf, uint16_t addr, uint32_t *n);

#endif /* __USB_MEMS_H__ */