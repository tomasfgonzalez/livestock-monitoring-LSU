/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : tim2.c
  * @brief          : Hardware Configuration Layer - TIM2 configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  */

/* Includes ------------------------------------------------------------------*/
#include "tim2.h"

#include <stdbool.h>

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

static bool initError = false;

/* HAL Functions ------------------------------------------------------------*/
void TIM2_MspInit(TIM_HandleTypeDef* htim) {
  __HAL_RCC_TIM2_CLK_ENABLE();

  /* TIM2 interrupt Init */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_MspDeInit(TIM_HandleTypeDef* htim) {
  __HAL_RCC_TIM2_CLK_DISABLE();

  /* TIM2 interrupt DeInit */
  HAL_NVIC_DisableIRQ(TIM2_IRQn);
}

/* Public functions ----------------------------------------------------------*/
void TIM2_Init(void) {
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8000 - 1; // 32 MHz / 8000 = 4 KHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4000; // 4 KHz / 4000 = 1 Hz -> 1 s
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
    initError = true;
  }

  // Configure TIM2 clock source
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
    initError = true;
  }

  // Master configuration (usually default for simple time base)
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
    initError = true;
  }
}

bool TIM2_hasError(void) {
  return initError;
}

