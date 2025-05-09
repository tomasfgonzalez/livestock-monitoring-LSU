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

/* Typedefs ------------------------------------------------------------------*/
typedef enum {
    SENSOR_HEARTRATE_STARTING,
    SENSOR_HEARTRATE_IDLE,
    SENSOR_HEARTRATE_MEASUREMENT_READY,
    SENSOR_HEARTRATE_ERROR
} SensorHeartrateStatus;

/* Function prototypes -------------------------------------------------------*/
void sensor_heartrate_init(void);
void sensor_heartrate_start(void);
void sensor_heartrate_stop(void);

bool sensor_heartrate_has_started(void);
bool sensor_heartrate_is_measurement_ready(void);
bool sensor_heartrate_has_error(void);

uint8_t sensor_heartrate_read(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_HEARTRATE_H */
