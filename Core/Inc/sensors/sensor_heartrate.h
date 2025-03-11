/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_heartrate.h
  * @brief          : Header for heartrate sensor file
  ******************************************************************************
  */
#ifndef SENSOR_HEARTRATE_H
#define SENSOR_HEARTRATE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SENSOR_HEARTRATE_STARTING,
    SENSOR_HEARTRATE_IDLE,
    SENSOR_HEARTRATE_MEASUREMENT_READY,
    SENSOR_HEARTRATE_ERROR
} SensorHeartrateStatus;

void sensor_heartrate_init(void);
void sensor_heartrate_start(void);
void sensor_heartrate_stop(void);

bool sensor_heartrate_has_started(void);
bool sensor_heartrate_is_measurement_ready(void);
bool sensor_heartrate_has_error(void);

uint8_t sensor_heartrate_read(void);

#endif /* SENSOR_HEARTRATE_H */
