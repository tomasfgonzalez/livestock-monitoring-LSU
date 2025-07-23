/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : rtc.c
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
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

#include <stdint.h>
#include <stdbool.h>
#include "stm32l0xx_hal.h"

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

static bool initError = false;

/* HAL Functions ------------------------------------------------------------*/
void RTC_MspInit(RTC_HandleTypeDef* hrtc) {
  __HAL_RCC_RTC_ENABLE();
  HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_MspDeInit(RTC_HandleTypeDef* hrtc) {
  __HAL_RCC_RTC_DISABLE();
  HAL_NVIC_DisableIRQ(RTC_IRQn);
}

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
    initError = true;
  }
}

void RTC_setWakeUpTimer(uint32_t seconds) {
  uint32_t necessaryCount = seconds * RTC_WAKEUP_PRESCALER_DIVISOR;
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, necessaryCount, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK) {
    initError = true;
  }
}

void RTC_clearWakeUpTimer(void) {
  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
}
