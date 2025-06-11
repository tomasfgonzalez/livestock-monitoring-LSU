/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : lpuart.c
  * @brief          : Hardware Configuration Layer - LPUART configuration
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
#include "lpuart.h"
#include "rylr998.h" //TODO: Remove

#include <stdbool.h>

/* Private variables --------------------------------------------------------*/
UART_HandleTypeDef hlpuart1;
DMA_HandleTypeDef hdma_lpuart1_rx;
DMA_HandleTypeDef hdma_lpuart1_tx;

static bool initError = false;
static uint8_t lpuart_rx_buff[LPUART_RX_SIZE];

/* HAL Functions ------------------------------------------------------------*/
void LPUART_MspInit(UART_HandleTypeDef* uartHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* LPUART1 clock enable */
  __HAL_RCC_LPUART1_CLK_ENABLE();

  /* LPUART1 GPIO clock enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /**
   * LPUART1 GPIO Configuration
   * PA2     ------> LPUART1_TX
   * PA3     ------> LPUART1_RX
   */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_LPUART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* LPUART1 DMA Init */
  /* LPUART1_RX Init */
  hdma_lpuart1_rx.Instance = DMA1_Channel3;
  hdma_lpuart1_rx.Init.Request = DMA_REQUEST_5;
  hdma_lpuart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_lpuart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_lpuart1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_lpuart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_lpuart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_lpuart1_rx.Init.Mode = DMA_CIRCULAR;
  hdma_lpuart1_rx.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_lpuart1_rx) != HAL_OK) {
    initError = true;
  }

  __HAL_LINKDMA(uartHandle,hdmarx,hdma_lpuart1_rx);

  /* LPUART1_TX Init */
  hdma_lpuart1_tx.Instance = DMA1_Channel2;
  hdma_lpuart1_tx.Init.Request = DMA_REQUEST_5;
  hdma_lpuart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_lpuart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_lpuart1_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_lpuart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_lpuart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_lpuart1_tx.Init.Mode = DMA_NORMAL;
  hdma_lpuart1_tx.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_lpuart1_tx) != HAL_OK) {
    initError = true;
  }

  __HAL_LINKDMA(uartHandle,hdmatx,hdma_lpuart1_tx);
  
  /* LPUART1 interrupt Init */
  HAL_NVIC_SetPriority(LPUART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(LPUART1_IRQn);
}

void LPUART_MspDeInit(UART_HandleTypeDef* uartHandle) {
  /* Peripheral clock disable */
  __HAL_RCC_LPUART1_CLK_DISABLE();

  /**
   * LPUART1 GPIO Configuration
   * PA2     ------> LPUART1_TX
   * PA3     ------> LPUART1_RX
   */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

  /* LPUART1 DMA DeInit */
  HAL_DMA_DeInit(uartHandle->hdmarx);
  HAL_DMA_DeInit(uartHandle->hdmatx);

  /* LPUART1 interrupt Deinit */
  HAL_NVIC_DisableIRQ(LPUART1_IRQn);
}

void LPUART_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (lpuart_rx_buff[Size-1] == '\n') {
    rylr998_SetInterruptFlag(true);
  }
  HAL_UARTEx_ReceiveToIdle_DMA(huart, lpuart_rx_buff, LPUART_RX_SIZE);
}

/* Public functions ----------------------------------------------------------*/
void LPUART_Init(void) {
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK) {
    initError = true;
  }
  HAL_UARTEx_ReceiveToIdle_DMA(&hlpuart1, lpuart_rx_buff, LPUART_RX_SIZE);
}

uint8_t* LPUART_getRxBuff(void){
  return lpuart_rx_buff;
}
