/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : max30102_Process.h
  * @brief          : Header for max30102 signal processing file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAX30102_PROCESS_H__
#define __MAX30102_PROCESS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "max30102_Buffer.h"

#include <stdint.h>

/* Defines -------------------------------------------------------------------*/
#define ALPHA 0.05                 // Smoothing factor for low-pass filter
#define ALPHA_DC 0.05              // Smoothing factor for DC removal
#define THRESHOLD_LOWER 28000      // Lower threshold for valid data
#define THRESHOLD_UPPER 45000      // Upper threshold for valid data

#define FIR_WINDOW_SIZE 10         // Half window size for FIR low-pass filter
#define DOWNSAMPLE_FACTOR 5       // Downsampling factor
#define RESAMPLE_BUFFER_SIZE 400   // MAX30102_BUFFER_SIZE / DOWNSAMPLE_FACTOR

#define PEAK_WINDOW_SIZE 10        // Window used to detect peaks

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Process a buffer of data from MAX30102 that stores the red LED samples.
 * @param buffer Buffer to process
 * @param elapsed_time_ms Elapsed time
 * @return Heart rate in beats per minute (bpm)
 */
uint16_t max30102_Process_Run(uint16_t* buffer, uint16_t elapsed_time_ms);

#ifdef __cplusplus
}
#endif

#endif /* __MAX30102_PROCESS_H__ */
