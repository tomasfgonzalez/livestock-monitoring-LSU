#include "gps_parser.h"
#include <string.h>

// Variables globales para almacenar los datos de las tramas
NAV_POSLLH navPosllhData;
NAV_STATUS navStatusData;

// Indicadores para saber si las tramas han sido recibidas
bool isNavPosllhReceived = false;
bool isNavStatusReceived = false;


const uint8_t UBX_HEADER[] = { 0xB5, 0x62 };
const uint8_t NAV_POSLLH_HEADER[] = { 0x01, 0x02 };
const uint8_t NAV_STATUS_HEADER[] = { 0x01, 0x03 };



UBXMessage ubxMessage;

void calcChecksum(uint8_t* CK, int msgSize) {
  memset(CK, 0, 2);
  for (int i = 0; i < msgSize; i++) {
    CK[0] += ((uint8_t*)(&ubxMessage))[i];
    CK[1] += CK[0];
  }
}

bool compareMsgHeader(const uint8_t* msgHeader) {
  uint8_t* ptr = (uint8_t*)(&ubxMessage);
  return ptr[0] == msgHeader[0] && ptr[1] == msgHeader[1];
}


void processUBXData(uint8_t* data, uint16_t size) {
    for (int i = 0; i < size; i++) {
        ubxMsgType msgType = processGPS(data[i]);  // Procesa byte por byte

        // Verificar si se han recibido ambas tramas
        if (isNavPosllhReceived && isNavStatusReceived) {
            // Ambas tramas han sido recibidas, procesar los datos
            // Reiniciar los indicadores
            isNavPosllhReceived = false;
            isNavStatusReceived = false;
        i=size; // break
        }
    }

}

void get_UBX_LatLon(int32_t* UBX_Lat, int32_t* UBX_Lon) {
	  *UBX_Lat = navPosllhData.lat;
	  *UBX_Lon = navPosllhData.lon;
}

uint8_t get_UBX_GpsFixStatus(void) {
    return navStatusData.gpsFix;
}



ubxMsgType processGPS(uint8_t c) {
  static int fpos = 0;
  static uint8_t checksum[2];
  static ubxMsgType currentMsgType = MT_NONE;
  static int payloadSize = sizeof(UBXMessage);

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
          isNavPosllhReceived = true;
        } else if (currentMsgType == MT_NAV_STATUS) {
          navStatusData = ubxMessage.navStatus;
          isNavStatusReceived = true;
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
