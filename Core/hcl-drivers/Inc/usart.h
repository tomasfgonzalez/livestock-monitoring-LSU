/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : usart.h
  * @brief          : Header for USART file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Gonazalez & Morris.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

#include "stm32l0xx_hal.h"
#include "neo6m_Structures.h"

/* Defines -------------------------------------------------------------------*/
#define VCP_TX_Pin GPIO_PIN_9
#define VCP_RX_Pin GPIO_PIN_10

/* Structure declarations ----------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

/* HAL Functions ------------------------------------------------------------*/
void USART_MspInit(UART_HandleTypeDef* uartHandle);
void USART_MspDeInit(UART_HandleTypeDef* uartHandle);
void USART_RxCpltCallback(UART_HandleTypeDef *huart);

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initializes the USART peripheral.
 */
void USART_Init(void);

/**
 * @brief Deinitializes the USART peripheral.
 */
void USART_DeInit(void);

/**
 * @brief Checks if the USART peripheral has data ready.
 */
bool USART_isDataReady(void);

/**
 * @brief Clears the data from the USART peripheral.
 */
void USART_clearData(void);

/**
 * @brief Gets the data from the USART peripheral.
 */
uint8_t* USART_getData(void);

/**
 * @brief Gets the length of the data from the USART peripheral.
 */
uint16_t USART_getDataLength(void);

/**
 * @brief Checks if the USART peripheral has an error.
 */
bool USART_hasError(void);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
