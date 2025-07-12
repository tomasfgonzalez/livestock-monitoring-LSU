/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : system_clock.h
  * @brief          : Header for Sysclock file
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
#ifndef __SYSTEM_CLOCK_H__
#define __SYSTEM_CLOCK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Configures the system clock.
 *
 * This function sets up the system clock to the desired frequency and
 * configures the clock sources accordingly.
 */
void SystemClock_Config(void);

/**
 * @brief Handles errors during initialization.
 *
 * This function is called when an error occurs during the system clock
 * configuration. It typically enters an infinite loop or logs the error.
 */
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /*__ SYSTEM_CLOCK_H__ */
