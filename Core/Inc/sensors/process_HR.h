/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : process_HR.h
  * @brief          : Header for process HR file
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
#ifndef PROCESS_HR_H
#define PROCESS_HR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "HR_Buffer.h"

/* Defines -------------------------------------------------------------------*/
#define ALPHA 0.05                 // Smoothing factor for low-pass filter
#define ALPHA_DC 0.05              // Smoothing factor for DC removal
#define THRESHOLD_LOWER 28000      // Lower threshold for valid data
#define THRESHOLD_UPPER 45000      // Upper threshold for valid data

#define DOWNSAMPLE_FACTOR 20       // Downsampling factor
#define RESAMPLE_BUFFER_SIZE 100  //MAX30102_BUFFER_SIZE / DOWNSAMPLE_FACTOR

#define PEAK_WINDOW_SIZE 10        // Window used to detect peaks

/* Functions -----------------------------------------------------------------*/
uint16_t process_buffer(uint16_t* buffer, uint16_t elapsed_time_ms);

#ifdef __cplusplus
}
#endif

#endif /* PROCESS_HR_H */
