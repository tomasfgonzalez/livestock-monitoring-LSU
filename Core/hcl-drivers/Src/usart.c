/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : usart.c
  * @brief          : Hardware Configuration Layer - USART configuration
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
#include "usart.h"

/* Defines -------------------------------------------------------------------*/
#define USART_RX_SIZE (2 * sizeof(NAV_STATUS) + 2 * sizeof(NAV_POSLLH))

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

static bool initError = false;
static bool dataReady = false;
static uint8_t usart_rx_data[USART_RX_SIZE];

/* HAL Functions ------------------------------------------------------------*/
void USART_MspInit(UART_HandleTypeDef* uartHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* USART2 clock enable */
  __HAL_RCC_USART2_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  /**
   * USART2 GPIO Configuration
   * PA9     ------> USART2_TX
   * PA10     ------> USART2_RX
   */
  GPIO_InitStruct.Pin = VCP_TX_Pin|VCP_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART2 DMA Init */
  /* USART2_RX Init */
  hdma_usart2_rx.Instance = DMA1_Channel5;
  hdma_usart2_rx.Init.Request = DMA_REQUEST_4;
  hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_rx.Init.Mode = DMA_NORMAL;
  hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK) {
    initError = true;
  }

  __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);
}

void USART_MspDeInit(UART_HandleTypeDef* uartHandle) {
  /* Peripheral clock disable */
  __HAL_RCC_USART2_CLK_DISABLE();

  /**
   * USART2 GPIO Configuration
   * PA9     ------> USART2_TX
   * PA10     ------> USART2_RX
   */
  HAL_GPIO_DeInit(GPIOA, VCP_TX_Pin|VCP_RX_Pin);

  /* USART2 DMA DeInit */
  HAL_DMA_DeInit(uartHandle->hdmarx);
}

void USART_RxCpltCallback(UART_HandleTypeDef *huart) {
  dataReady = true;
  // Opcional: Detener la transferencia DMA
  // HAL_UART_DMAStop(&huart2);
}

/* Public functions ---------------------------------------------------------*/
void USART_Init(void) {
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    initError = true;
  }
}

/**
 * USART management functions
 */
void USART_Start(void) {
  dataReady = false;
  HAL_UART_Receive_DMA(&huart2, usart_rx_data, USART_RX_SIZE);
}

bool USART_hasError(void) {
  return initError;
}

bool USART_isDataReady(void) {
  return dataReady;
}

uint8_t* USART_getData(void) {
  return usart_rx_data;
}

uint16_t USART_getDataLength(void) {
  return USART_RX_SIZE;
}
