/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_gps.c
  * @brief          : Source file for GPS sensor, serves as interface for GPS
  *                   sensor measurements and readings
  ******************************************************************************
  */
#include "sensor_gps.h"

/** 
 * Sensor status
 * */
static SensorGPSStatus sensor_status = SENSOR_GPS_STARTING;

/**
 * Helper functions
 * */
static bool processAndValidate(void) {
  processUBXData(USART2_getData(), USART2_getDataLength());

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

  if (USART2_isDataReady()) {
    bool isValidData = processAndValidate();
    if (isValidData) {
      sensor_status = SENSOR_GPS_MEASUREMENT_READY;
    } else {
      USART2_Start(); // Restart measurement
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
  USART2_Init();
  if (USART2_hasError()) {
    sensor_status = SENSOR_GPS_ERROR;
  } else {
    sensor_status = SENSOR_GPS_IDLE;
  }
}

void sensor_gps_start(void) {
  DMA_Start();
  USART2_Start();
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
