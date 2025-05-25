/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : rtc.h
  * @brief          : This file provides code for the RTC.
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
#ifndef RTC_H
#define RTC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Public functions ----------------------------------------------------------*/

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
void RTC_Init(void);

/**
  * @brief Set the Wake Up Timer
  * @param seconds
  * @retval None
  */
void RTC_setWakeUpTimer(uint32_t seconds);

/**
  * @brief Clear the Wake Up Timer
  * @param None
  * @retval None
  */
void RTC_clearWakeUpTimer(void);

#ifdef __cplusplus
}
#endif

#endif /* RTC_H */

