/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : HR_Buffer.h
  * @brief          : Header file for HR Buffer
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
#ifndef __HR_BUFFER_H__
#define __HR_BUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/
#define MAX30102_BUFFER_SIZE 2000 // Buffer size if change go check bpm
#define MAX30102_UNUSED_DATA 128    // Skip first 128 samples

/* Function prototypes -------------------------------------------------------*/
void HRBuffer_init(void);
void HRBuffer_reset(void);
void HRBuffer_put(uint32_t red_sample);
uint8_t HRBuffer_isReady(void);

uint16_t* HRBuffer_getBuffer(void);
volatile uint16_t HRBuffer_getElapsedTime(void);

#ifdef __cplusplus
}
#endif

#endif /* HR_BUFFER_H */
 
