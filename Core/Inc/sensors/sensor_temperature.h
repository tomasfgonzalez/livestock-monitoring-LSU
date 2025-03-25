/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_temperature.h
  * @brief          : Header for temperature sensor file
  ******************************************************************************
  */
#ifndef SENSOR_TEMPERATURE_H
#define SENSOR_TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "adc.h"

/* Typedefs ------------------------------------------------------------------*/
typedef enum {
    SENSOR_TEMPERATURE_STARTING,
    SENSOR_TEMPERATURE_IDLE,
    SENSOR_TEMPERATURE_MEASUREMENT_READY,
    SENSOR_TEMPERATURE_ERROR
} SensorTemperatureStatus;

/* Function prototypes -------------------------------------------------------*/
void sensor_temperature_init(void);
void sensor_temperature_start(void);
void sensor_temperature_stop(void);
void sensor_temperature_tick_1s(void);

bool sensor_temperature_has_started(void);
bool sensor_temperature_is_measurement_ready(void);
bool sensor_temperature_has_error(void);

bool sensor_temperature_read(uint8_t* target);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_TEMPERATURE_H */
