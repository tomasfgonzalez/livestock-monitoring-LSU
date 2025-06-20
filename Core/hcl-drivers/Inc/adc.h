/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : adc.h
  * @brief          : Header for ADC file
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
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

#include "stm32l0xx_hal.h"

/* Structure declarations ----------------------------------------------------*/
extern ADC_HandleTypeDef hadc;

/* HAL Functions ------------------------------------------------------------*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initializes the ADC peripheral.
 */
void ADC_Init(void);

/**
 * @brief Deinitializes the ADC peripheral.
 */
void ADC_DeInit(void);

/**
 * @brief Retrieves the ADC value stored in the provided pointer.
 *
 * @param reg points to a 16-bit variable where the ADC value will be stored.
 */
void ADC_GetValues(uint16_t* reg);

/**
 * @brief Checks if the ADC peripheral has data ready.
 */
bool ADC_isDataReady(void);

/**
 * @brief Checks if the ADC peripheral has an error.
 */
bool ADC_hasError(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
