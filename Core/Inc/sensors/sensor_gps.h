/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_gps.h
  * @brief          : Header for GPS sensor file
  ******************************************************************************
  */
#ifndef SENSOR_GPS_H
#define SENSOR_GPS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SENSOR_GPS_STARTING,
    SENSOR_GPS_IDLE,
    SENSOR_GPS_MEASUREMENT_READY,
    SENSOR_GPS_ERROR
} SensorGPSStatus;

typedef struct {
    // TODO: Add necessary fields
    float latitude;
    float longitude;
    bool fix_valid;
} GPSData;

void sensor_gps_init(void);
void sensor_gps_start(void);
void sensor_gps_stop(void);

bool sensor_gps_has_started(void);
bool sensor_gps_is_measurement_ready(void);
bool sensor_gps_has_error(void);

GPSData sensor_gps_read(void);

#endif /* SENSOR_GPS_H */
