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

bool sensor_gps_has_started(void) {
  return sensor_status == SENSOR_GPS_IDLE || sensor_status == SENSOR_GPS_MEASUREMENT_READY;
}

bool sensor_gps_is_measurement_ready(void) {
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
  // TODO: Implement sensor start procedure (a.k.a: start measurement)
  DMA_Start();
  sensor_status = SENSOR_GPS_MEASUREMENT_READY;
}

void sensor_gps_stop(void) {
  // TODO: Implement sensor stop procedure (a.k.a: stop the sensor)
  DMA_Stop();
  sensor_status = SENSOR_GPS_IDLE;
}

/**
 * Sensor reading
 */
GPSData sensor_gps_read(void) {
  GPSData data = {0};

  if (sensor_status != SENSOR_GPS_MEASUREMENT_READY) {
    data.fix_valid = false;
    return data; // Return invalid data
  }

  // TODO: Implement GPS reading
  sensor_status = SENSOR_GPS_IDLE;

  // Return test values
  data.latitude = 45.5017;
  data.longitude = -73.5673;
  data.fix_valid = true;

  return data;
}
