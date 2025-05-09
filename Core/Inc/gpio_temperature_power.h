/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : gpio_temperature_power.h
  * @brief          : Temperature sensor power control via GPIO
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
#ifndef __GPIO_TEMPERATURE_POWER_H__
#define __GPIO_TEMPERATURE_POWER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Function prototypes -------------------------------------------------------*/
void GPIO_temperature_power_init(void);
void GPIO_temperature_power_start(void);
void GPIO_temperature_power_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_TEMPERATURE_POWER_H */ 
