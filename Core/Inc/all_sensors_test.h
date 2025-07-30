/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : all_sensors_test.h
  * @brief          : Header for all sensors test file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ALL_SENSORS_TEST_H__
#define __ALL_SENSORS_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "sensor_all.h"
#include "sensor_temperature.h"
#include "sensor_heartrate.h"
#include "sensor_gps.h"

/* Function prototypes -------------------------------------------------------*/
void all_sensors_test(void);

#ifdef __cplusplus
}
#endif

#endif /* __ALL_SENSORS_TEST_H__ */
