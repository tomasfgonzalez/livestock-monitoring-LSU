/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : dma.c
  * @brief          : Hardware Configuration Layer - DMA configuration
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
#include "dma.h"

/* Public functions ----------------------------------------------------------*/
void DMA_Init(void) {
  /* DMA clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

  /* DMA1_Channel4_5_6_7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);
}

void DMA_Start(void) {
  __HAL_RCC_DMA1_CLK_ENABLE();
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);
}

void DMA_Stop(void) {
  HAL_NVIC_DisableIRQ(DMA1_Channel4_5_6_7_IRQn);
  HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
  __HAL_RCC_DMA1_CLK_DISABLE();
}
