/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : gps_structures.h
  * @brief          : Header for GPS structures file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GPS_STRUCTURES_H
#define GPS_STRUCTURES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* GPS message types --------------------------------------------------------*/
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

#ifdef __cplusplus
}
#endif

#endif /* GPS_STRUCTURES_H */
