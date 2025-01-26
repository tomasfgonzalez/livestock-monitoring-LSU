/**
  ******************************************************************************
  * @file    sysclock.h
  * @brief   This file contains all the function prototypes for Sysclock
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __SYSCLOCK_H__
#define __SYSCLOCK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"


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


#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

