/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : MAX30102_Buffer.h
  * @brief          : Header file for MAX30102 Buffer
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
#ifndef __MAX30102_BUFFER_H__
#define __MAX30102_BUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/
#define MAX30102_BUFFER_SIZE 1000 // Buffer size if change go check bpm
#define MAX30102_UNUSED_DATA 128    // Skip first 128 samples

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initialize the buffer
 */
void max30102_Buffer_Init(void);

/**
 * @brief Reset the buffer
 */
void max30102_Buffer_Reset(void);

/**
 * @brief Put a sample in the buffer
 * @param red_sample Sample to put
 */
void max30102_Buffer_Put(uint32_t red_sample);

/**
 * @brief Check if the buffer is ready
 * @return 1 if the buffer is ready, 0 otherwise
 */
uint8_t max30102_Buffer_IsReady(void);

/**
 * @brief Get buffer
 * @return Buffer
 */
uint16_t* max30102_Buffer_GetBuffer(void);

/**
 * @brief Get elapsed time
 * @return Elapsed time
 */
volatile uint16_t max30102_Buffer_GetElapsedTime(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAX30102_BUFFER_H__ */
