/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Function prototypes -------------------------------------------------------*/
/**
 * @brief Initializes the ADC peripheral.
 */
void ADC_Init(void);

/**
 * @brief Retrieves the ADC value and stores it in the provided pointer.
 *
 * @param adc_val Pointer to a 16-bit variable where the ADC value will be stored.
 */
extern ADC_HandleTypeDef hadc;

/**
 * @brief Computes the voltage corresponding to the given ADC value.
 *
 * @param ADC_DATA ADC data value to be converted to voltage.
 * @return The computed voltage.
 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
void get_ADC_values(uint16_t* val);

void Start_ADC_IRQ(void);

#endif /* __ADC_H */


