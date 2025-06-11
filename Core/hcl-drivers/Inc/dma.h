/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : dma.h
  * @brief          : Header for DMA file
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
#ifndef __DMA_H__
#define __DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief Initializes the DMA peripheral.
 */
void DMA_Init(void);

/**
 * @brief Starts the DMA peripheral.
 */
void DMA_Start(void);

/**
 * @brief Stops the DMA peripheral.
 */
void DMA_Stop(void);

#ifdef __cplusplus
}
#endif

#endif /* __DMA_H__ */
