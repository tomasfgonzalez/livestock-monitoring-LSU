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

/* Structure declarations ----------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* Function prototypes -------------------------------------------------------*/
void USART2_Init(void);
void USART2_Start(void);
bool USART2_isDataReady(void);
uint8_t* USART2_getData(void);
uint16_t USART2_getDataLength(void);
bool USART2_hasError(void);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
