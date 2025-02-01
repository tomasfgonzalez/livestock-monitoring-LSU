#ifndef GPS_PARSER_H
#define GPS_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32l0xx_hal.h"  // Asegúrate de incluir el archivo HAL adecuado para tu microcontrolador

extern const uint8_t UBX_HEADER[];
extern const uint8_t NAV_POSLLH_HEADER[];
extern const uint8_t NAV_STATUS_HEADER[];

typedef enum {
  MT_NONE,
  MT_NAV_POSLLH,
  MT_NAV_STATUS
} ubxMsgType;

typedef struct {
  uint8_t cls;
  uint8_t id;
  uint16_t len;
  uint32_t iTOW;
  int32_t lon;
  int32_t lat;
  int32_t height;
  int32_t hMSL;
  uint32_t hAcc;
  uint32_t vAcc;
} NAV_POSLLH;

typedef struct {
  uint8_t cls;
  uint8_t id;
  uint16_t len;
  uint32_t iTOW;
  uint8_t gpsFix;
  int8_t flags;
  int8_t fixStat;
  int8_t flags2;
  uint32_t ttff;
  uint32_t msss;
} NAV_STATUS;

typedef union {
  NAV_POSLLH navPosllh;
  NAV_STATUS navStatus;
} UBXMessage;

extern UBXMessage ubxMessage;

void calcChecksum(uint8_t* CK, int msgSize);
bool compareMsgHeader(const uint8_t* msgHeader);
ubxMsgType processGPS(uint8_t c);
void processUBXData(uint8_t* data, uint16_t size);
uint8_t get_UBX_GpsFixStatus(void);
int32_t get_UBX_Lon(void);
int32_t get_UBX_Lat(void);
#endif // GPS_PARSER_H
