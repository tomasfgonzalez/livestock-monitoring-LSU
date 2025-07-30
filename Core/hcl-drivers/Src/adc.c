/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : adc.c
  * @brief          : Hardware Configuration Layer - ADC configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

#include <stdbool.h>

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

static bool initError = false;

static uint8_t conversionsFinished = 0;
static uint16_t adc_values[2];

/* HAL Functions -------------------------------------------------------------*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (adcHandle->Instance == ADC1) {
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* ADC1 GPIO clock enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* ADC1 GPIO Configuration */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle) {
  if(adcHandle->Instance == ADC1) {
    /* ADC1 clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /* ADC1 GPIO Configuration */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_4);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  if (hadc->Instance == ADC1) {
    static uint8_t current_channel = 0;
    adc_values[current_channel] = HAL_ADC_GetValue(hadc);
    current_channel ^= 1;  // Toggle between channel 0 and 1
    conversionsFinished++;
  }
}

/* Public functions ----------------------------------------------------------*/
void ADC_Init(void) {
  ADC_ChannelConfTypeDef sConfig = {0};

  /* Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc.Init.LowPowerAutoWait = ENABLE;
  hadc.Init.LowPowerFrequencyMode = ENABLE;
  hadc.Init.LowPowerAutoPowerOff = ENABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK) {
    initError = true;
  }

  /* Configure ADC channel 1 to be converted. */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
    initError = true;
  }

  /* Configure ADC channel 4 to be converted. */
  sConfig.Channel = ADC_CHANNEL_4;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
    initError = true;
  }

  /* Enable ADC interrupt */
  HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0); // Set priority for ADC interrupt
  HAL_NVIC_EnableIRQ(ADC1_IRQn);        // Enable ADC interrupt in NVIC

  /* Enable ADC end-of-conversion interrupt */
  __HAL_ADC_ENABLE_IT(&hadc, ADC_IT_EOC);
  HAL_ADC_Start_IT(&hadc);
}

void ADC_DeInit(void) {
  HAL_ADC_Stop_IT(&hadc);
  HAL_ADC_DeInit(&hadc);
  HAL_NVIC_DisableIRQ(ADC1_IRQn);

  adc_values[0] = adc_values[1] = 0;
}

bool ADC_hasError(void) {
  return initError;
}

void ADC_GetValues(uint16_t* val) {
  val[0] = adc_values[0];
  val[1] = adc_values[1];
}

bool ADC_isDataReady(void) {
  return conversionsFinished >= 2;
}
