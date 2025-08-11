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
int peak_count = 0, peak_diff = 0;

/* Private functions ----------------------------------------------------------*/

/**
 * @brief Check if buffer contains valid data within threshold range.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 * @return 1 if data is valid, 0 otherwise.
 */
static uint8_t is_data_clear(uint16_t* buffer, uint16_t size,uint16_t flatline) {
	uint16_t low_val, high_val;
	low_val=0;
	high_val=0;
  for (int i = 0; i < size; i++) {
    if (buffer[i] < THRESHOLD_LOWER || buffer[i] > THRESHOLD_UPPER) {
      return 0;
    }
    if(buffer[i]<low_val){
    	low_val=buffer[i];
    }
    if(buffer[i]>high_val){
    	high_val=buffer[i];
    }
  }

  if(high_val-low_val<flatline){
	  return 0;
  }

  return 1;
}


static void downsample_buffer(uint16_t* input, uint16_t* output, uint16_t input_size) {
  uint16_t j = 0;
  for (uint16_t i = 0; i < input_size; i += 10) {
    output[j++] = input[i];
  }
}



/**
 * @brief Detect peaks in the signal within a window.
 * @param signal Pointer to signal buffer.
 * @param signal_length Length of signal buffer.
 * @param window_size Size of window to detect peaks.
 */



static void find_peaks(uint16_t* signal, uint16_t signal_length, int window_size,uint8_t debounce) {
  int half_window = window_size / 2;
  int first_peak = 0, last_peak = 0;
  peak_count = 0;

  for (int i = half_window; i < signal_length - half_window; i++) {
    uint8_t is_peak = 1;
    for (int j = i - half_window; j <= i + half_window; j++) {
      if (signal[i] < signal[j]) {
        is_peak = 0;
        break;
      }
    }
    if (is_peak){
    	peak_count++;
      if (first_peak == 0) {
        first_peak = i;
      }
      last_peak = i;
      i=i+debounce;
    }
  }
  peak_diff = last_peak - first_peak;
}

/* Public functions ----------------------------------------------------------*/
uint16_t max30102_Process_Run(uint16_t* buffer, uint16_t elapsed_time_ms) {
  if (!max30102_Buffer_IsReady()) return 0;

  uint16_t bpm = 0;
  if (is_data_clear(buffer, MAX30102_BUFFER_SIZE,THRESHOLD_FLATLINE)) {
    downsample_buffer(buffer, buffer, MAX30102_BUFFER_SIZE);
    find_peaks(buffer, 200, PEAK_WINDOW_SIZE,DEBOUNCE_SIZE);
     elapsed_time_ms = elapsed_time_ms * peak_diff / 200;
    bpm = (peak_count-1) * 60 * 1000 / elapsed_time_ms;
    max30102_Buffer_Reset();
  }
  if (50 < bpm && bpm < 140) {
    return bpm;
  }
  return 0;
}
