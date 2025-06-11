/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : gps_parser.c
  * @brief          : Source file for GPS parser, holds function to parse messages
  *                   received from GPS module
  ******************************************************************************
  */
#include "neo6m.h"
#include "neo6m_Structures.h"

#include <stdbool.h>
#include <string.h>

/* Private variables ---------------------------------------------------------- */
static UBXMessage ubxMessage;

static NAV_POSLLH navPosllhData;
static NAV_STATUS navStatusData;

/* Constant values ------------------------------------------------------------ */
const uint8_t UBX_HEADER[] = { 0xB5, 0x62 };
const uint8_t NAV_POSLLH_HEADER[] = { 0x01, 0x02 };
const uint8_t NAV_STATUS_HEADER[] = { 0x01, 0x03 };

/* Private functions ----------------------------------------------------------- */
static void calcChecksum(uint8_t* CK, uint16_t msgSize) {
  memset(CK, 0, 2);
  for (uint16_t i = 0; i < msgSize; i++) {
    CK[0] += ((uint8_t*)(&ubxMessage))[i];
    CK[1] += CK[0];
  }
}

static bool compareMsgHeader(const uint8_t* msgHeader) {
  uint8_t* ptr = (uint8_t*)(&ubxMessage);
  return ptr[0] == msgHeader[0] && ptr[1] == msgHeader[1];
}

ubxMsgType processGPS(uint8_t c) {
  static uint16_t fpos = 0;
  static uint8_t checksum[2];
  static ubxMsgType currentMsgType = MT_NONE;
  static uint16_t payloadSize = sizeof(UBXMessage);

  if (fpos < 2) {
    // Buscamos el header UBX (0xB5, 0x62)
    if (c == UBX_HEADER[fpos])
      fpos++;
    else
      fpos = 0;  // Reiniciamos si no coincide
  } else {
    // Si ya tenemos el header, procesamos el payload
    if ((fpos - 2) < payloadSize)
      ((uint8_t*)(&ubxMessage))[fpos - 2] = c;

    fpos++;

    if (fpos == 4) {
      // Determinamos el tipo de mensaje basado en el header
      if (compareMsgHeader(NAV_POSLLH_HEADER)) {
        currentMsgType = MT_NAV_POSLLH;
        payloadSize = sizeof(NAV_POSLLH);
      } else if (compareMsgHeader(NAV_STATUS_HEADER)) {
        currentMsgType = MT_NAV_STATUS;
        payloadSize = sizeof(NAV_STATUS);
      } else {
        // Mensaje desconocido, reiniciamos
        fpos = 0;
        return MT_NONE;
      }
    }

    if (fpos == (payloadSize + 2)) {
      // Calculamos el checksum
      calcChecksum(checksum, payloadSize);
    } else if (fpos == (payloadSize + 3)) {
      // Verificamos el primer byte del checksum
      if (c != checksum[0]) {
        fpos = 0;  // Reiniciamos si no coincide
      }
    } else if (fpos == (payloadSize + 4)) {
      // Verificamos el segundo byte del checksum
      fpos = 0;  // Reiniciamos en cualquier caso
      if (c == checksum[1]) {
        // Copiamos los datos a la estructura global correspondiente
        if (currentMsgType == MT_NAV_POSLLH) {
          navPosllhData = ubxMessage.navPosllh;
        } else if (currentMsgType == MT_NAV_STATUS) {
          navStatusData = ubxMessage.navStatus;
        }
        return currentMsgType;  // Mensaje válido
      }
    } else if (fpos > (payloadSize + 4)) {
      // Error: reiniciamos
      fpos = 0;
    }
  }

  return MT_NONE;  // No hay mensaje completo aún
}

/* Public functions ------------------------------------------------------------ */
void neo6m_ProcessData(uint8_t* data, uint16_t size) {
  for (uint16_t i = 0; i < size; i++) {
    processGPS(data[i]);
  }
}

int32_t neo6m_GetLatitude(void) {
	  return navPosllhData.lat;
}

int32_t neo6m_GetLongitude(void) {
	  return navPosllhData.lon;
}

uint32_t neo6m_GetTimeOfTheWeekUTC_ms(void){
	return navPosllhData.iTOW;      //domingo a las 00:00:00 UTC-> 000000.000 [s]
}

uint8_t neo6m_GetFixStatus(void) {
    return navStatusData.gpsFix;
}
