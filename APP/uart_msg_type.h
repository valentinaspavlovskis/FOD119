#ifndef __UART_MSG_TYPE_H__
#define __UART_MSG_TYPE_H__

typedef enum {
  MSG_UART_IDLE = 0,  
  MSG_UART_SEND
}UARTMTypeDef;


typedef struct uart_msg_t_{
  UARTMTypeDef id; /* control message id */
  uint32_t p1; /* param1 */
  uint32_t p2; /* param2 */
}uart_msg_t;

int8_t uart_send_msg(UARTMTypeDef id, uint32_t p1, uint32_t p2, int8_t use_alloc);


#endif //__UART_MSG_TYPE_H__