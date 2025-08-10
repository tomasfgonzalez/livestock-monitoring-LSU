/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : rtc.h
  * @brief          : Hardware Configuration Layer - RTC configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32l0xx_hal.h"

/* Defines -------------------------------------------------------------------*/
#define RTC_WAKEUP_PRESCALER_DIVISOR 2048

/* Structure declarations ----------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

/* HAL Functions ------------------------------------------------------------*/
void RTC_MspInit(RTC_HandleTypeDef* hrtc);
void RTC_MspDeInit(RTC_HandleTypeDef* hrtc);

/* Public functions ----------------------------------------------------------*/

/**
  * @brief Initializes the RTC peripheral.
  */
void RTC_Init(void);

/**
  * @brief Set the Wake Up Timer
  * @param seconds
  */
void RTC_setWakeUpTimer(uint32_t seconds);

/**
  * @brief Clear the Wake Up Timer
  */
void RTC_clearWakeUpTimer(void);

/**
  * @brief Get current RTC tick in milliseconds
  * @param None
  * @retval Current tick count in milliseconds
  */
uint32_t RTC_GetTick(void);

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

