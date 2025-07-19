/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_gps.c
  * @brief          : Source file for GPS sensor, serves as interface for GPS
  *                   sensor measurements and readings
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sensor_gps.h"
#include <stdint.h>
#include <stdbool.h>

#include "dma.h"
#include "usart.h"
#include "neo6m.h"

/* Defines -------------------------------------------------------------------*/
#define LAT_LONG_SCALE 0.0000001

/* Private variables ---------------------------------------------------------*/
static SensorGPSStatus sensor_status = SENSOR_GPS_STARTING;

/* Private functions ---------------------------------------------------------*/
static bool processAndValidate(void) {
  neo6m_ProcessData(USART_getData(), USART_getDataLength());
  uint8_t isFixed = neo6m_GetFixStatus();
  return isFixed >= 3;
}

/* Public status check functions ---------------------------------------------*/
bool sensor_gps_has_started(void) {
  return sensor_status == SENSOR_GPS_IDLE || sensor_status == SENSOR_GPS_MEASUREMENT_READY;
}

bool sensor_gps_is_measurement_ready(void) {
  if (sensor_status == SENSOR_GPS_MEASUREMENT_READY) {
    return true;
  }

  if (USART_isDataReady()) {
    bool isValidData = processAndValidate();
    if (isValidData) {
      sensor_status = SENSOR_GPS_MEASUREMENT_READY;
    } else {
      USART_clearData(); // Restart measurement
    }
  }
  return sensor_status == SENSOR_GPS_MEASUREMENT_READY;
}

bool sensor_gps_has_error(void) {
  return sensor_status == SENSOR_GPS_ERROR;
}

/* Public management functions -----------------------------------------------*/
void sensor_gps_init(void) {
  DMA_Init();
  USART_Init();
  if (USART_hasError()) {
    sensor_status = SENSOR_GPS_ERROR;
  } else {
    sensor_status = SENSOR_GPS_IDLE;
  }
}

void sensor_gps_stop(void) {
  USART_DeInit();
  DMA_Stop();
  sensor_status = SENSOR_GPS_IDLE;
}

void sensor_gps_read(GPSData *data) {
  data->latitude = neo6m_GetLatitude() * LAT_LONG_SCALE;
  data->longitude = neo6m_GetLongitude() * LAT_LONG_SCALE;

  sensor_status = SENSOR_GPS_IDLE;
}
