/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_heartrate.h
  * @brief          : Header for heartrate sensor file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Gonazalez & Morris.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_HEARTRATE_H__
#define __SENSOR_HEARTRATE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Structure declarations ----------------------------------------------------*/
typedef enum {
    SENSOR_HEARTRATE_STARTING,
    SENSOR_HEARTRATE_IDLE,
    SENSOR_HEARTRATE_MEASUREMENT_READY,
    SENSOR_HEARTRATE_ERROR
} SensorHeartrateStatus;

/* Function prototypes -------------------------------------------------------*/
/**
 * @brief Checks if the sensor has initialized correctly.
 */
bool sensor_heartrate_has_started(void);

/**
 * @brief Checks if the sensor has finished processing the measurement,
 *        which is able to be read.
 */
bool sensor_heartrate_is_measurement_ready(void);

/**
 * @brief Checks if the sensor has an error.
 */
bool sensor_heartrate_has_error(void);

/**
 * @brief Initializes the sensor.
 */
void sensor_heartrate_init(void);

/**
 * @brief Stops and deinitializes the sensor.
 */
void sensor_heartrate_stop(void);

/**
 * @brief Reads the sensor's measurement.
 * @param target The target variable to store the measurement.
 * @return True iff the measurement was read successfully.
 */
bool sensor_heartrate_read(uint8_t* target);

/**
 * @brief Performs periodic tasks for the sensor (called every second).
 */
void sensor_heartrate_tick_1s(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_HEARTRATE_H */
