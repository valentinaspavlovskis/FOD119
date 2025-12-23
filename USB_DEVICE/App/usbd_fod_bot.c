/* Includes ------------------------------------------------------------------*/
#include "usbd_fod_bot.h"
#include "usbd_fod.h"
#include "fodcomm.h"
#include "usb_bulk_cmd.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/

#define REG_N                         (3)    /* number of suppoerted registers */
#define REG_ADDR_VER                  (0x0000)
#define REG_ADDR_CTRL                 (0x0001)
#define REG_ADDR_STATUS               (0x0002)

enum {
  REG_CTRL_CMD_RESET_FGPA = 0x0001,
};

/* register array structure */
typedef union RegS_{
  struct{
    uint16_t sw_ver;
    uint16_t ctrl;
    uint16_t status;
  }s;
  uint16_t buf[REG_N];
} RegS;

typedef struct Port_T_{
  uint16_t addr;
  uint32_t n;
  uint32_t n_done;
} Port_T, *Port_Tp;

typedef int8_t (*FuncPtr)();


/* Private define ------------------------------------------------------------*/
#define PROTOCOL_VERSION (0x01)
#define SOFTWARE_VERSION (0x0001)

#define FOD_PKT_HDR_SZ (16)

#define PKT_BUF_PAYLOAD_SZ      (64 * 10-FOD_PKT_HDR_SZ)//(64*469-FOD_PKT_HDR_SZ)
#define FOD_PKT_BURST_SZ        (64 * 10)//(64*469)
#define PKT_BUF_SZ              (PKT_BUF_PAYLOAD_SZ + FOD_PKT_HDR_SZ)

#define BOT_IDLE                      0       /* Idle state */
#define BOT_DATA_OUT                  1       /* Data Out state */
#define BOT_DATA_IN                   2       /* Data In state */
#define BOT_DATA_IN_LAST              3       /* Last Data In Last */
#define BOT_ERROR                     5       /* error state */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Bot_State;
uint32_t Data_Len;
int32_t Data_Len_Done = 0;

__ALIGN_BEGIN uint8_t fod_packet_buf[PKT_BUF_SZ] __ALIGN_END;// @ ".sram";
uint8_t *fod_tx_packet_buf;

data_pkt_hdr_T hdr_out;
data_pkt_T pkt_out = {&hdr_out, 0};

uint32_t USB_Tx_Len;
uint8_t USB_Tx_Mode = 0;
uint8_t FODUSBRequest = 0;

Port_T portS;

RegS fodUSBRegList = {SOFTWARE_VERSION, 0x0000, 0x0000};

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void fod_send_packet();
static void fod_prepare_receive_packet(void *pdev);
void fodUSBMainLoop(void *pdev);
static void fod_send_packet_continue();
int8_t fod_cmd_nop(data_pkt_Tp in, data_pkt_Tp out);
int8_t fod_cmd_protocol_version(data_pkt_Tp in, data_pkt_Tp out);
int8_t fod_cmd_port8_read(data_pkt_Tp in, data_pkt_Tp out);
int8_t fod_cmd_port8_write(data_pkt_Tp in, data_pkt_Tp out);
int8_t fod_cmd_get_protocol_info(data_pkt_Tp in, data_pkt_Tp out);
int8_t fod_cmd_reg16_read(data_pkt_Tp in, data_pkt_Tp out);
int8_t fod_cmd_reg16_write(data_pkt_Tp in, data_pkt_Tp out);
int8_t fod_cmd_i2c_addr8_mem_read(data_pkt_Tp in, data_pkt_Tp out);
int8_t fod_cmd_i2c_addr8_mem_write(data_pkt_Tp in, data_pkt_Tp out);


/* fod protocol command processing function array */
const FuncPtr fod_cmd_func_array[FOD_MSGTYPE_N] = {
  &fod_cmd_nop,
  &fod_cmd_protocol_version,    /* PROTOCOL_VERSION */
  &fod_cmd_get_protocol_info,   /* GET_PROTOCOL_INFO */
  &fod_cmd_nop,                 /* REG8_READ */
  &fod_cmd_nop,                 /* REG8_WRITE */
  &fod_cmd_reg16_read,          /* REG16_READ */
  &fod_cmd_reg16_write,         /* REG16_WRITE */
  &fod_cmd_port8_read,          /* PORT8_READ */
  &fod_cmd_port8_write,         /* PORT8_WRITE */
  &fod_cmd_nop,                 /* PORT16_READ */
  &fod_cmd_nop,                 /* PORT16_WRITE */
  &fod_cmd_nop,                 /* MEM8_READ */
  &fod_cmd_nop,                 /* MEM8_WRITE */
  &fod_cmd_nop,                 /* MEM16_READ */
  &fod_cmd_nop,                 /* MEM16_WRITE */
  &fod_cmd_i2c_addr8_mem_read,  /* I2C_ADDR8_MEM_READ */
  &fod_cmd_i2c_addr8_mem_write, /* I2C_ADDR8_MEM_WRITE */
  &fod_cmd_nop,                 /* I2C_ADDR16_MEM_READ */
  &fod_cmd_nop,                 /* I2C_ADDR16_MEM_WRITE */
  &fod_cmd_nop,                 /* SPI_READ */
};



/*********************************************
FOD specific management functions
*********************************************/

__ALIGN_BEGIN uint8_t USB_Rx_Buffer    [FOD_DATA_MAX_PACKET_SIZE] __ALIGN_END ;


/**
* @brief  usbd_fod_Init
*         Initilaize the FOD interface
* @param  pdev: device instance
* @retval status
*/
void FOD_BOT_Init (void *pdev)
{ 
  USBD_LL_FlushEP(pdev, FOD_EPOUT_ADDR);
  USBD_LL_FlushEP(pdev, FOD_EPIN_ADDR);  
  
  fodUSBRegList.s.status |= 0x0001;
  Data_Len_Done = 0;
  portS.n_done = 0;
  fod_prepare_receive_packet(pdev);
}

/**
* @brief  usbd_fod_Init
*         DeInitialize the FOD layer
* @param  pdev: device instance
* @retval status
*/
void FOD_BOT_DeInit (void *pdev)
{
}


/**
* @brief  usbd_audio_DataIn
*         Data sent on non-control IN endpoint
* @param  pdev: device instance
* @param  epnum: endpoint number
* @retval status
*/
void FOD_BOT_DataIn (void *pdev, uint8_t epnum)
{
  switch (Bot_State)
  {
  case BOT_DATA_IN:
    USB_Tx_Mode = 1;
    break;
  case BOT_DATA_IN_LAST:
    if (hdr_out.MsgStatus == FOD_MSG_STATUS_CONTINUE){
      fod_send_packet_continue();
    }else{
      fod_prepare_receive_packet(pdev);
    }
    break;
  default:
    break;
  }
}

/**
* @brief  usbd_fod_DataOut
*         Data received on non-control Out endpoint
* @param  pdev: device instance
* @param  epnum: endpoint number
* @retval status
*/
void FOD_BOT_DataOut (void *pdev, uint8_t epnum)
{
  /* Get the received data buffer and update the counter */
  Data_Len = USBD_LL_GetRxDataSize(pdev, epnum);
  FODUSBRequest = 1;
}

/**
* @brief  usbd_audio_SOF
*         Start Of Frame event management
* @param  pdev: instance
* @param  epnum: endpoint number
* @retval status
*/
void FOD_BOT_SOF (void *pdev)
{
  Handle_USBAsynchXfer(pdev);
}


/**
* @brief Handle_USBAsynchXfer
*        Asynchronous data transfer to USB host
* @param pdev: instance
* @retval
*/
void Handle_USBAsynchXfer (void *pdev)
{
  uint32_t send_n = 0;

  if (USB_Tx_Mode){
    USB_Tx_Mode = 0;

    if (USB_Tx_Len){
      if (USB_Tx_Len < FOD_PKT_BURST_SZ){
        send_n = USB_Tx_Len;
        USB_Tx_Len = 0;
        if (send_n%FOD_DATA_MAX_PACKET_SIZE){
          Bot_State = BOT_DATA_IN_LAST;
        }
      }else {
        send_n = FOD_PKT_BURST_SZ;
        USB_Tx_Len -= FOD_PKT_BURST_SZ;
      }
    }else{
      Bot_State = BOT_DATA_IN_LAST;
    }
   USBD_LL_Transmit (pdev,
               FOD_EPIN_ADDR,
               (uint8_t*)&fod_tx_packet_buf[0],
               send_n);
     
    
    fod_tx_packet_buf += send_n;
  }
}

/**
* @brief  fodUSBMainLoop
*         Process received packets from USB host
* @param  pdev: instance
* @retval
*/
void fodUSBMainLoop(void *pdev)
{
  int32_t error_code = -1;
  data_pkt_hdr_Tp hdr_in = (data_pkt_hdr_Tp)&fod_packet_buf[0];
  data_pkt_T pkt_in = {hdr_in, &fod_packet_buf[FOD_PKT_HDR_SZ]};

  pkt_out.payload = &fod_packet_buf[FOD_PKT_HDR_SZ];
  pkt_out.hdr->MsgStatus = FOD_MSG_STATUS_OK;

  if (Data_Len_Done + Data_Len < PKT_BUF_SZ){
    memcpy(&fod_packet_buf[Data_Len_Done], &USB_Rx_Buffer[0], Data_Len);
    Data_Len_Done += Data_Len;
    if (Data_Len == FOD_DATA_MAX_PACKET_SIZE || Data_Len == 0){
      Bot_State = BOT_DATA_IN;
      USBD_LL_PrepareReceive(pdev,
                       FOD_EPOUT_ADDR,
                       (uint8_t*)(USB_Rx_Buffer),
                       FOD_DATA_OUT_PACKET_SIZE);
      return;
    }
  }
  else{
    /* buffer overflow */
    error_code = -2;
    goto error;
  }

  if (hdr_in->MsgID == FOD_PKT_MSG_ID && hdr_in->MsgType && hdr_in->PayloadLen + FOD_PKT_HDR_SZ == Data_Len_Done){
    int8_t err;

    err = fod_cmd_func_array[hdr_in->MsgType](&pkt_in, &pkt_out);

    if (err){
      if (err == 1){ /* continue to receive next packet (do ;not send response packet ) */
          fod_prepare_receive_packet(pdev);
          return;
      }
      /* incorrect command */
      goto error;
    }
  }
  else{
    /* incorrect command */
    goto error;
  }

  if (hdr_in->MsgStatus == FOD_MSG_STATUS_CONTINUE){
    fod_prepare_receive_packet(pdev);
    return;
  }

  hdr_out.MsgID = hdr_in->MsgID;
  hdr_out.MsgType = hdr_in->MsgType;

  memcpy(&fod_packet_buf[0], &hdr_out, FOD_PKT_HDR_SZ);

  USB_Tx_Len = FOD_PKT_HDR_SZ + hdr_out.PayloadLen;
    
  fod_send_packet();
  return;

error:
  memcpy(&fod_packet_buf[0], &error_code, 4);
  USB_Tx_Len = 4;
  fod_send_packet();
  return;

}

/**
* @brief  fod_send_packet
*         Send packet with transport header
* @param
* @retval
*/
static void fod_send_packet()
{
  Data_Len_Done = 0;
  fod_tx_packet_buf = &fod_packet_buf[0];

  Bot_State = BOT_DATA_IN;
  USB_Tx_Mode = 1;
}

/**
* @brief  fod_prepare_receive_packet
*
* @param  pdev: instance
* @retval
*/
static void fod_prepare_receive_packet(void *pdev)
{
  /* Prepare Out endpoint to receive next packet */
  USBD_LL_PrepareReceive(pdev,
                   FOD_EPOUT_ADDR,
                   (uint8_t*)(USB_Rx_Buffer),
                   FOD_DATA_OUT_PACKET_SIZE);
}

/**
* @brief
*
* @param
* @retval
*/
uint32_t n_read_bytes;

static void fod_send_packet_continue()
{
  switch(hdr_out.MsgType)
  {
  case FOD_MSGTYPE_PORT8_READ:
    {
      n_read_bytes = portS.n - portS.n_done;

      if (n_read_bytes > PKT_BUF_PAYLOAD_SZ){
        hdr_out.MsgStatus = FOD_MSG_STATUS_CONTINUE;
        n_read_bytes = PKT_BUF_PAYLOAD_SZ;
        portS.n_done += n_read_bytes;
      }else{
        hdr_out.MsgStatus = FOD_MSG_STATUS_OK;
        portS.n_done = 0;
      }

      if (usb_bulk_read(&fod_packet_buf[FOD_PKT_HDR_SZ], portS.addr, &n_read_bytes)){
        hdr_out.MsgStatus = FOD_MSG_STATUS_ERROR;
      }
      

      hdr_out.PayloadLen = n_read_bytes;

      break;
    }
  default:
    return;
  }

  memcpy(&fod_packet_buf[0], &hdr_out, FOD_PKT_HDR_SZ);

  USB_Tx_Len = FOD_PKT_HDR_SZ + hdr_out.PayloadLen;
  fod_send_packet();
}

/**
* @brief  fod_cmd_nop
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_nop(data_pkt_Tp in, data_pkt_Tp out)
{
  return -1;
}

/**
* @brief fod_cmd_protocol_version
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_protocol_version(data_pkt_Tp in, data_pkt_Tp out)
{
  out->payload[0] = PROTOCOL_VERSION;
  out->hdr->PayloadLen = sizeof(uint8_t);
  
  return 0;
}

/**
* @brief  fod_cmd_port16_read
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_port8_read(data_pkt_Tp in, data_pkt_Tp out)
{
  //int8_t sta;
  uint32_t n_read;
  uint32_t sz;
  
  portS.addr = *(uint16_t*)&in->payload[0];
  memcpy(&sz, &in->payload[2], sizeof(sz));
  portS.n = sz; /* payload size in words */
    
  portS.n_done = 0;

  n_read = portS.n - portS.n_done;

  if (n_read > PKT_BUF_PAYLOAD_SZ){
    out->hdr->MsgStatus = FOD_MSG_STATUS_CONTINUE;
    n_read = PKT_BUF_PAYLOAD_SZ;
    portS.n_done += n_read;
  }
    
  memcpy(&fod_packet_buf[FOD_PKT_HDR_SZ], &in->payload[6], sz);
    
  if (usb_bulk_read(&fod_packet_buf[FOD_PKT_HDR_SZ], portS.addr, &sz)){
    hdr_out.MsgStatus = FOD_MSG_STATUS_ERROR;
  }
    
  out->hdr->PayloadLen = sz;

  return 0;
}

/**
* @brief  fod_cmd_port16_write
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_port8_write(data_pkt_Tp in, data_pkt_Tp out)
{
  int8_t sta = 0;
  uint32_t sz;
  uint32_t n_write = (in->hdr->PayloadLen - 6);
  uint16_t *fpgaPayload = (uint16_t*)&in->payload[6];
  
  if (portS.n_done == 0){
    portS.addr = *(uint16_t*)&in->payload[0];
    //portS.n = *(uint32_t*)&in->payload[2];
    
    memcpy(&sz, &in->payload[2], sizeof(sz));
    portS.n = sz; /* payload size in words */
  
    fpgaPayload = (uint16_t*)&in->payload[6];
    portS.n_done += n_write;
  }

  if (portS.addr <= 0x000F){// end start address
    sta = usb_bulk_write(fpgaPayload, portS.addr, &n_write);
    if (sta){
      out->hdr->MsgStatus = FOD_MSG_STATUS_ERROR;
    }
  }    

  if (in->hdr->MsgStatus == FOD_MSG_STATUS_CONTINUE){
    return 1;
  }else{
    portS.n_done = 0;
  }

  out->hdr->PayloadLen = 0;
  return 0;
}

/**
* @brief  fod_cmd_get_protocol_info
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_get_protocol_info(data_pkt_Tp in, data_pkt_Tp out)
{
  /* Max RX buffer */
  out->payload[0] = PKT_BUF_PAYLOAD_SZ & 0xff;
  out->payload[1] = PKT_BUF_PAYLOAD_SZ >> 8;
  /* Max TX buffer */
  out->payload[2] = PKT_BUF_PAYLOAD_SZ & 0xff;
  out->payload[3] = PKT_BUF_PAYLOAD_SZ >> 8;
  out->hdr->PayloadLen = 2 * sizeof(uint16_t);
  
  return 0;
}

/**
* @brief  fod_cmd_reg16_read
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_reg16_read(data_pkt_Tp in, data_pkt_Tp out)
{
  uint16_t reg = in->payload[0] + ((uint16_t)in->payload[1] << 8); /* take requested register address */

  if (in->hdr->PayloadLen == sizeof(uint16_t) && reg < REG_N){
    /* put register value to TX buffer */
    out->payload[0] = fodUSBRegList.buf[reg] & 0x0f;
    out->payload[1] = fodUSBRegList.buf[reg] >> 8;
    out->hdr->PayloadLen = sizeof(uint16_t);
  }else{
    out->hdr->PayloadLen = 0;
    out->hdr->MsgStatus = FOD_MSG_STATUS_ERROR;
  }
  return 0;
}

/**
* @brief  fod_cmd_reg16_write
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_reg16_write(data_pkt_Tp in, data_pkt_Tp out)
{
  uint16_t reg = in->payload[0] + ((uint16_t)in->payload[1] << 8); /* take requested register address */
  uint16_t val = in->payload[2] + ((uint16_t)in->payload[3] << 8); /* take requested register value */

  if (in->hdr->PayloadLen == (2 * sizeof(uint16_t))){
    if (reg == REG_ADDR_CTRL){
      if (val == REG_CTRL_CMD_RESET_FGPA){
      //drv_fpga_reset();
      fodUSBRegList.s.status |= 0x0001;
      }
    }
  }else{
    out->hdr->MsgStatus = FOD_MSG_STATUS_ERROR;
  }
  out->hdr->PayloadLen = 0;
  return 0;
}

/**
* @brief  fod_cmd_i2c_addr8_mem_read
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_i2c_addr8_mem_read(data_pkt_Tp in, data_pkt_Tp out)
{
  int8_t err = 1;
  //uint8_t dev_addr = in->payload[0];
  //uint8_t mem_addr = in->payload[1];
  uint16_t len = in->payload[2] + (((uint16_t)in->payload[3]) << 8);

//  if (!drv_i2c_bq27510_set_addr(dev_addr)){
    //err = drv_i2c_bq27510_ReadBuffer(&out->payload[0], len, mem_addr);
  //}

  if (err == 0){
    out->hdr->PayloadLen = len;
  }else{
    out->hdr->PayloadLen = 0;
    out->hdr->MsgStatus = FOD_MSG_STATUS_ERROR;
  }
  return 0;
}

/**
* @brief  fod_cmd_i2c_addr8_mem_write
*
* @param  in: input packet
* @param  out: output packet
* @retval Status
*/
int8_t fod_cmd_i2c_addr8_mem_write(data_pkt_Tp in, data_pkt_Tp out)
{
  int8_t err = 1;
  //uint8_t dev_addr = in->payload[0];
  //uint8_t mem_addr = in->payload[1];
  //uint16_t     len = in->payload[2] + (((uint16_t)in->payload[3]) << 8);

  //if (!drv_i2c_bq27510_set_addr(dev_addr)){
//    err = drv_i2c_bq27510_WriteBuffer(&in->payload[4], len, mem_addr);
//  }

  out->hdr->PayloadLen = 0;
  if (err){
    out->hdr->MsgStatus = FOD_MSG_STATUS_ERROR;
  }
  return 0;
}
