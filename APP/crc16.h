//
//CRC16 algo - 16 bit CCITT polynomial, x16 + x15 + x2 + 1 - tas pats algoritmas, kaip ir Modbus protokole
//http://www.modbustools.com/modbus_crc16.htm
//

#ifndef CRC16_H_INCLUDED
#define CRC16_H_INCLUDED

#include <stdlib.h>

//prepare for crc calc
void CRC16Init(void);

//calc crc for new byte
void CRC16AddByte(unsigned char newbyte);
//calc crc for many bytes
void CRC16AddBytes(const unsigned char *buf, unsigned long buflen);

//return calculated crc
unsigned short CRC16GetValue(void);

// calc crc for bytes array
unsigned short CRC16Calc (const unsigned char *pucBuff, unsigned long ulLen);

#endif // CRC16_H_INCLUDED
