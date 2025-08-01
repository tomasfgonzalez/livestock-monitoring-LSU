/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : neo6m_Structures.h
  * @brief          : Header for Neo6M structures file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Gonazalez & Morris.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NEO6M_STRUCTURES_H__
#define __NEO6M_STRUCTURES_H__

#ifdef __cplusplus
extern "C" {
#endif

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

#endif /* __NEO6M_STRUCTURES_H__ */
