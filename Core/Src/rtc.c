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

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* Public functions ---------------------------------------------------------*/
void RTC_Init(void) {
  /* Initialize RTC Only */
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

  /* Enable the Wake-Up */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK) {
    Error_Handler();
  }
}

void RTC_setWakeUpTimer(uint32_t seconds) {
  HAL_SuspendTick();
  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, seconds, RTC_CR_WUCKSEL_2);
}

void RTC_clearWakeUpTimer(void) {
  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
  SystemClock_Config();
  HAL_ResumeTick();
}
