/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_gps.c
  * @brief          : Source file for GPS sensor, serves as interface for GPS
  *                   sensor measurements and readings
  ******************************************************************************
  */
#include "sensor_gps.h"
#include <stdint.h>

/** 
 * Sensor status
 * */
static SensorGPSStatus sensor_status = SENSOR_GPS_STARTING;

/**
 * Helper functions
 * */
static bool processAndValidate(void) {
  processUBXData(USART_getData(), USART_getDataLength());

  uint8_t isFixed = get_UBX_GpsFixStatus();
  uint8_t latitude = get_UBX_GpsLatitude();
  uint8_t longitude = get_UBX_GpsLongitude();

  return true;
}

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
      USART_Start(); // Restart measurement
    }
  }
  return sensor_status == SENSOR_GPS_MEASUREMENT_READY;
}

bool sensor_gps_has_error(void) {
  return sensor_status == SENSOR_GPS_ERROR;
}

/**
 * Sensor management
 */
void sensor_gps_init(void) {
  DMA_Init();
  USART_Init();
  if (USART_hasError()) {
    sensor_status = SENSOR_GPS_ERROR;
  } else {
    sensor_status = SENSOR_GPS_IDLE;
  }
}

void sensor_gps_start(void) {
  DMA_Start();
  USART_Start();
}

void sensor_gps_stop(void) {
  DMA_Stop();
  sensor_status = SENSOR_GPS_IDLE;
}

/**
 * Sensor reading
 */
void sensor_gps_read(GPSData *data) {
  data->latitude = get_UBX_GpsLatitude();
  data->longitude = get_UBX_GpsLongitude();

  sensor_status = SENSOR_GPS_IDLE;
}
