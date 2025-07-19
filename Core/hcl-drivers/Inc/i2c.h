/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : i2c.h
  * @brief          : Header for I2C file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

#include "stm32l0xx_hal.h"

/* Structure declarations ----------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initializes the I2C peripheral.
 */
void I2C_Init(void);

/**
 * @brief Deinitializes the I2C peripheral.
 */
void I2C_DeInit(void);

/**
 * @brief Checks if the I2C peripheral has an error.
 */
bool I2C_hasError(void);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
