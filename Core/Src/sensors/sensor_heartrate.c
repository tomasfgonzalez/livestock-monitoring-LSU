/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_heartrate.c
  * @brief          : Source file for heart rate sensor, serves as interface for
  *                   heart rate sensor measurements and readings
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

/* Includes ------------------------------------------------------------------*/
#include "sensor_heartrate.h"
#include "i2c.h"
#include "max30102.h"

/* Private variables ---------------------------------------------------------*/
static SensorHeartrateStatus sensor_status = SENSOR_HEARTRATE_STARTING;

/* Public status check functions ---------------------------------------------*/
bool sensor_heartrate_has_started(void) {
  return sensor_status == SENSOR_HEARTRATE_IDLE || sensor_status == SENSOR_HEARTRATE_MEASUREMENT_READY;
}

bool sensor_heartrate_is_measurement_ready(void) {
  if (max30102_IsReady()) {
    sensor_status = SENSOR_HEARTRATE_MEASUREMENT_READY;
  }
  return sensor_status == SENSOR_HEARTRATE_MEASUREMENT_READY;
}

bool sensor_heartrate_has_error(void) {
  return sensor_status == SENSOR_HEARTRATE_ERROR;
}

/* Public management functions -----------------------------------------------*/
void sensor_heartrate_init(void) {
  I2C_Init();
  if (I2C_hasError()) {
    sensor_status = SENSOR_HEARTRATE_ERROR;
    return;
  }
  
  max30102_Init();
  sensor_status = SENSOR_HEARTRATE_IDLE;
}

void sensor_heartrate_stop(void) {
  max30102_Stop();
  I2C_DeInit();
  sensor_status = SENSOR_HEARTRATE_IDLE;
}

bool sensor_heartrate_read(uint8_t* target) {
  if (sensor_status != SENSOR_HEARTRATE_MEASUREMENT_READY) {
    return false;
  }

  uint16_t bpm = max30102_GetBPM();
  *target = (uint8_t)bpm;
  
  sensor_status = SENSOR_HEARTRATE_IDLE;
  return true;
}
