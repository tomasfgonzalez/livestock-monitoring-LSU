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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LPUART_H__
#define __LPUART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32l0xx_hal.h"

/* Defines -------------------------------------------------------------------*/
#define LPUART_RX_SIZE 64

/* Structure declarations ----------------------------------------------------*/
extern UART_HandleTypeDef hlpuart1;
extern DMA_HandleTypeDef hdma_lpuart1_rx;
extern DMA_HandleTypeDef hdma_lpuart1_tx;

/* HAL Functions ------------------------------------------------------------*/
void LPUART_MspInit(UART_HandleTypeDef* huart);
void LPUART_MspDeInit(UART_HandleTypeDef* huart);
void LPUART_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size);

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initializes the LPUART peripheral.
 */
void LPUART_Init(void);

/**
 * @brief Deinitializes the LPUART peripheral.
 */
void LPUART_DeInit(void);

/**
 * @brief Gets the receive buffer.
 */
uint8_t* LPUART_getRxBuff(void);

#ifdef __cplusplus
}
#endif

#endif /* __LPUART_H__ */
