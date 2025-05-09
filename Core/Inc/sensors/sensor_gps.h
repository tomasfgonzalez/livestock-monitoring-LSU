/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_gps.h
  * @brief          : Header for GPS sensor file
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
#ifndef __SENSOR_GPS_H__
#define __SENSOR_GPS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "dma.h"
#include "usart.h"
#include "gps_parser.h"

/* Structure declarations ----------------------------------------------------*/
typedef enum {
  SENSOR_GPS_STARTING,
  SENSOR_GPS_IDLE,
  SENSOR_GPS_MEASUREMENT_READY,
  SENSOR_GPS_ERROR
} SensorGPSStatus;

typedef struct {
  float latitude;
  float longitude;
} GPSData;

/* Function prototypes -------------------------------------------------------*/
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

#endif /* __SENSOR_GPS_H__ */
