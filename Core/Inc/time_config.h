/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : time_config.h
  * @brief          : Header for time configuration file
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
#ifndef __TIME_CONFIG_H__
#define __TIME_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/* Function prototypes -------------------------------------------------------*/


void mode_STOP(void);
/**
 * @brief Set the time configuration
 * 
 * @param period The period of the time configuration
 * @param time_slot The time slot of the time configuration
 * @param now The current time
 */
void time_config_set(uint32_t period, uint32_t time_slot, uint32_t now);

/**
 * @brief Check if is RTC timer finished and it is time to start sensing
 * 
 * @return true iff is time to sense
 */
bool time_config_isReadyToSense(void);

/**
 * @brief Check if is RTC timer finished and it is time to transmit data
 * 
 * @return true iff is time to transmit
 */
bool time_config_isReadyToTransmit(void);

#ifdef __cplusplus
}
#endif

#endif /* TIME_CONFIG_H */
