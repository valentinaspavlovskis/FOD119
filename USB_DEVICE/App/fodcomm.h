#ifndef __FODCOMM_H__
#define __FODCOMM_H__

#define FOD_MSGTYPE_PROTOCOL_VERSION     (0x01)
#define FOD_MSGTYPE_GET_PROTOCOL_INFO    (0x02)
#define FOD_MSGTYPE_REG8_READ            (0x03)
#define FOD_MSGTYPE_REG8_WRITE           (0x04)
#define FOD_MSGTYPE_REG16_READ           (0x05)
#define FOD_MSGTYPE_REG16_WRITE          (0x06)
#define FOD_MSGTYPE_PORT8_READ           (0x07)
#define FOD_MSGTYPE_PORT8_WRITE          (0x08)
#define FOD_MSGTYPE_PORT16_READ          (0x09)
#define FOD_MSGTYPE_PORT16_WRITE         (0x0a)
#define FOD_MSGTYPE_MEM8_READ            (0x0b)
#define FOD_MSGTYPE_MEM8_WRITE           (0x0c)
#define FOD_MSGTYPE_MEM16_READ           (0x0d)
#define FOD_MSGTYPE_MEM16_WRITE          (0x0e)
#define FOD_MSGTYPE_I2C_ADDR8_MEM_READ   (0x0f)
#define FOD_MSGTYPE_I2C_ADDR8_MEM_WRITE  (0x10)
#define FOD_MSGTYPE_I2C_ADDR16_MEM_READ  (0x11)
#define FOD_MSGTYPE_I2C_ADDR16_MEM_WRITE (0x12)
#define FOD_MSGTYPE_SPI_READ             (0x13)
#define FOD_MSGTYPE_SPI_WRITE            (0x14)
#define FOD_MSGTYPE_N           (20)

#define FOD_PKT_MSG_ID (0x0DDFF0AB)

typedef struct data_pkt_hdr_T_
{
  uint32_t MsgID;      /* This is a magic number used to verify each message */
  uint32_t MsgType;    /* protocol command type */
  uint32_t PayloadLen; /* Total length of message related data that follows this header */
  uint32_t MsgStatus;  /* Protocol Status */
}data_pkt_hdr_T, *data_pkt_hdr_Tp;

typedef struct data_pkt_T_
{
  data_pkt_hdr_Tp hdr;
  uint8_t * payload;
}data_pkt_T, *data_pkt_Tp;


enum FNET_MESSAGE_STATUS{
  FOD_MSG_STATUS_ERROR = -1,
  FOD_MSG_STATUS_OK,       // correct transmission ended
  FOD_MSG_STATUS_CONTINUE, // continue to receive or send packets
};

#endif // __FODCOMM_H__