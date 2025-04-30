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
#include "stm32l0xx_hal.h"
#include "gps_structures.h"

/* Defines -------------------------------------------------------------------*/
#define VCP_TX_Pin GPIO_PIN_9
#define VCP_RX_Pin GPIO_PIN_10
#define UART_RX_BUFF_SIZE 64

/* Structure declarations ----------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef hlpuart1;
extern DMA_HandleTypeDef hdma_lpuart1_rx;
extern DMA_HandleTypeDef hdma_lpuart1_tx;

void INIT_RX_UART2(void);

void MX_LPUART1_UART_Init(void);

/* Function prototypes -------------------------------------------------------*/
void USART2_Init(void);
void USART2_Start(void);
bool USART2_isDataReady(void);
uint8_t* USART2_getData(void);
uint16_t USART2_getDataLength(void);
bool USART2_hasError(void);

uint8_t* LPUART1_getRxBuff(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
