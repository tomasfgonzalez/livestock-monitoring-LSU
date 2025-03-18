/**
  * @file    gpio_temperature_power.h
  * @brief   Temperature sensor power control via GPIO
  */

#ifndef GPIO_TEMPERATURE_POWER_H
#define GPIO_TEMPERATURE_POWER_H

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
