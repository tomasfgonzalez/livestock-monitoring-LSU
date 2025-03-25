/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_gps.h
  * @brief          : Header for GPS sensor file
  ******************************************************************************
  */
#ifndef SENSOR_GPS_H
#define SENSOR_GPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "dma.h"
#include "usart.h"
#include "gps_parser.h"

/* Typedefs ------------------------------------------------------------------*/
typedef enum {
  SENSOR_GPS_STARTING,
  SENSOR_GPS_IDLE,
  SENSOR_GPS_MEASUREMENT_READY,
  SENSOR_GPS_ERROR
} SensorGPSStatus;

/* Typedefs ------------------------------------------------------------------*/
typedef struct {
  float latitude;
  float longitude;
} GPSData;

void sensor_gps_init(void);
void sensor_gps_start(void);
void sensor_gps_stop(void);

bool sensor_gps_has_started(void);
bool sensor_gps_is_measurement_ready(void);
bool sensor_gps_has_error(void);

void sensor_gps_read(GPSData *data);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_GPS_H */
