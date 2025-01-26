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
void Get_ADC_Val(uint16_t *adc_val);

/**
 * @brief Computes the voltage corresponding to the given ADC value.
 *
 * @param ADC_DATA ADC data value to be converted to voltage.
 * @return The computed voltage.
 */
uint16_t ADC_ComputeVoltage(const uint16_t ADC_DATA);  /* const for parameter safety */





#endif /* __ADC_H */


