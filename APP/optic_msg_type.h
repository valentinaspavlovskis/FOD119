#ifndef __OPTIC_MSG_TYPE_H__
#define __OPTIC_MSG_TYPE_H__

typedef enum {
  OPTIC_MSG_IDLE = 0,  
  OPTIC_MSG_MSG_CHANNEL_READ,
  OPTIC_MSG_MSG_CHANNEL_SET,
  OPTIC_MSG_MSG_UPDATE_DAC,
  OPTIC_MSG_MSG_READ_DAC,
  OPTIC_MSG_MSG_WL_SET,
  OPTIC_MSG_MSG_WL_NEXT,
  OPTIC_MSG_MSG_WL_PREVIOUS,
  OPTIC_MSG_MSG_MODE_SET,
}OsOptic_TypeDef;


typedef struct optic_msg_t_{
  OsOptic_TypeDef id; /* Mesage ID */
  uint32_t p1; /* param1 */
  uint32_t p2; /* param2 */
}optic_msg_t;

int8_t optic_send_msg(OsOptic_TypeDef id, uint32_t p1, uint32_t p2, int8_t use_alloc);


#endif //__OPTIC_MSG_TYPE_H__