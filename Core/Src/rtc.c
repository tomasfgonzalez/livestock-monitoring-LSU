/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : rtc.c
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
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include "stm32l0xx_hal.h"
#include "sysClock.h"

/* Defines -------------------------------------------------------------------*/
#define RTC_WAKEUP_PRESCALER_DIVISOR 2048

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* Public functions ---------------------------------------------------------*/
void RTC_Init(void) {
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK) {
    Error_Handler();
  }
}

void RTC_setWakeUpTimer(uint32_t seconds) {
  uint32_t necessaryCount = seconds * RTC_WAKEUP_PRESCALER_DIVISOR;
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, necessaryCount, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK) {
    Error_Handler();
  }
}

void RTC_clearWakeUpTimer(void) {
  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
}
