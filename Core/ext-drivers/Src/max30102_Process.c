/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : max30102_Process.c
  * @brief          : Processing data from MAX30102
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

/* Includes ------------------------------------------------------------------*/
#include "max30102_Process.h"

#include <stdint.h>

/* Private variables ---------------------------------------------------------*/
uint16_t resample_buffer[RESAMPLE_BUFFER_SIZE];
int peak_count = 0, peak_diff = 0;

/* Private functions ----------------------------------------------------------*/

/**
 * @brief Check if buffer contains valid data within threshold range.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 * @return 1 if data is valid, 0 otherwise.
 */
static uint8_t is_data_clear(uint16_t* buffer, uint16_t size) {
  for (int i = 0; i < size; i++) {
    if (buffer[i] < THRESHOLD_LOWER || buffer[i] > THRESHOLD_UPPER) {
      return 0;
    }
  }
  return 1;
}

/**
 * @brief Downsample input buffer by a given factor.
 * @param input Pointer to input buffer.
 * @param output Pointer to output buffer.
 * @param input_size Size of input buffer.
 * @param output_size Pointer to store output buffer size.
 */
static void downsample_buffer(uint16_t* input, uint16_t* output, uint16_t input_size) {
  uint16_t j = 0;
  for (uint16_t i = 0; i < input_size; i += DOWNSAMPLE_FACTOR) {
    output[j++] = input[i];
  }

}

/**
 * @brief Detect peaks in the signal within a window.
 * @param signal Pointer to signal buffer.
 * @param signal_length Length of signal buffer.
 * @param window_size Size of window to detect peaks.
 */
static void find_peaks(uint16_t* signal, uint16_t signal_length, int window_size) {
  int half_window = window_size / 2;
  int first_peak = 0, last_peak = 0;
  peak_count = 0;

  for (int i = half_window; i < signal_length - half_window; i++) {
    int is_peak = 1;
    for (int j = i - half_window; j <= i + half_window; j++) {
      if (signal[i] < signal[j]) {
        is_peak = 0;
        break;
      }
    }
    if (is_peak && signal[i] >= signal[i - 1] && signal[i] >= signal[i + 1] && signal[i - 1] != signal[i]) {
      peak_count++;
      if (first_peak == 0) {
        first_peak = i;
      }
      last_peak = i;
    }
  }
  peak_diff = last_peak - first_peak;
}

/* Public functions ----------------------------------------------------------*/
uint16_t max30102_Process_Run(uint16_t* buffer, uint16_t elapsed_time_ms) {
  if (!max30102_Buffer_IsReady()) return 0;

  uint16_t bpm = 0;
  if (is_data_clear(buffer, MAX30102_BUFFER_SIZE)) {
    downsample_buffer(buffer, resample_buffer, MAX30102_BUFFER_SIZE);
    find_peaks(resample_buffer, RESAMPLE_BUFFER_SIZE, PEAK_WINDOW_SIZE);
     elapsed_time_ms = elapsed_time_ms * peak_diff / 100;
    bpm = peak_count * 60 * 1000 / elapsed_time_ms;       // Revisar este calculo si cambio el tamaño del buffer
    max30102_Buffer_Reset();
  }
  if (50 < bpm && bpm < 140) {
    return bpm;
  }
  return 0;
}
