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
#include "max30102_for_stm32_hal.h"

/* Defines -------------------------------------------------------------------*/
#define MAX30102_BUFFER_SIZE 2000 // Buffer size if change go check bpm
#define MAX30102_UNUSED_DATA 64    // Skip first 64 samples
#define ALPHA 0.05                 // Smoothing factor for low-pass filter
#define ALPHA_DC 0.01              // Smoothing factor for DC removal
#define THRESHOLD_LOWER 28000      // Lower threshold for valid data
#define THRESHOLD_UPPER 45000      // Upper threshold for valid data
#define DOWNSAMPLE_FACTOR 20       // Downsampling factor
#define PEAK_WINDOW_SIZE 10        // Window used to detect peaks

extern uint16_t max30102_buffer[MAX30102_BUFFER_SIZE];
extern uint16_t resample_buffer[MAX30102_BUFFER_SIZE / DOWNSAMPLE_FACTOR];
extern uint16_t buffer_index;
extern uint32_t bpm;
extern int peak_count, peak_diff;
extern uint16_t start, end, elapsed_time_ms;

void max30102_put_Buffer(uint32_t red_sample);
void print_buffer(uint16_t *buffer, uint16_t size);
void low_pass_filter_IIR(int16_t *buffer, uint16_t size);
void remove_dynamic_dc_component(uint16_t *buffer, uint16_t size);
uint8_t is_data_clear(uint16_t *buffer, uint16_t size);
void downsample_buffer(uint16_t *input, uint16_t *output, uint16_t input_size, uint16_t *output_size);
void find_peaks(uint16_t* signal, uint16_t signal_length, int window_size);
void process_buffer(void);
uint16_t get_bpm(void);
void reset_bpm(void);

#ifdef __cplusplus
}
#endif

#endif /* PROCESS_HR_H */
