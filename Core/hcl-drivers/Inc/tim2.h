/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : tim2.h
  * @brief          : Header for TIM2 file
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
#ifndef __TIM2_H
#define __TIM2_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

#include "stm32l0xx_hal.h"

/* Structure declarations ----------------------------------------------------*/
extern TIM_HandleTypeDef htim2;

/* HAL Functions ------------------------------------------------------------*/
void TIM2_MspInit(TIM_HandleTypeDef* htim);
void TIM2_MspDeInit(TIM_HandleTypeDef* htim);

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief Initializes the TIM2 peripheral.
 */
void TIM2_Init(void);

/**
 * @brief Checks if the TIM2 peripheral has an error.
 */
bool TIM2_hasError(void);

/**
 * @brief Handles the TIM2 interrupt. Should be implemented in the main.c file.
 */
void TIM2_tick(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIM2_H */
