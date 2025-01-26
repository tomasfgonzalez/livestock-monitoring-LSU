/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSM_MODE_H
#define __FSM_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"   /* Ensure HAL is only included once */
#include "adc.h"              /* Include the ADC header for ADC operations */

/**
 * @brief FSM (Finite State Machine) mode function.
 *
 * This function implements the logic for the FSM mode. It handles the transitions
 * and actions within the finite state machine. The exact behavior should be defined
 * in the corresponding .c file.
 */
void fsm_mode(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
