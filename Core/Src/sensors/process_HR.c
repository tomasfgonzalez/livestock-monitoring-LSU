/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : process_HR.c
  * @brief          : Source file for process HR
  ******************************************************************************
  */
#include "process_HR.h"

uint16_t resample_buffer[MAX30102_BUFFER_SIZE / DOWNSAMPLE_FACTOR];
int peak_count = 0, peak_diff = 0;

/**
 * @brief Print buffer contents with timestamps.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 */
void print_buffer(uint16_t* buffer, uint16_t size, uint16_t elapsed_time_ms) {
  for (int i = 0; i < size; i++) {
    printf("%d %d\n", buffer[i], elapsed_time_ms * i / size);
  }
}

/**
 * @brief Apply low-pass IIR filter for noise reduction.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 */
void low_pass_filter_IIR(int16_t* buffer, uint16_t size) {
  if (size == 0) return;
  for (uint16_t i = 1; i < size; i++) {
    buffer[i] = (int16_t)(ALPHA * buffer[i] + (1 - ALPHA) * buffer[i - 1]);
  }
}

/**
 * @brief Remove DC component using exponential moving average.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 */
void remove_dynamic_dc_component(uint16_t* buffer, uint16_t size) {
  static uint16_t dc_offset = 0;
  if (size == 0) return;
  for (uint16_t i = 0; i < size; i++) {
    dc_offset = (uint16_t)(ALPHA_DC * buffer[i] + (1 - ALPHA_DC) * dc_offset);
    buffer[i] = (buffer[i] >= dc_offset) ? buffer[i] - dc_offset : 0;
  }
}

/**
 * @brief Check if buffer contains valid data within threshold range.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 * @return 1 if data is valid, 0 otherwise.
 */
uint8_t is_data_clear(uint16_t* buffer, uint16_t size) {
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
void downsample_buffer(uint16_t* input, uint16_t* output, uint16_t input_size, uint16_t* output_size) {
  uint16_t j = 0;
  for (uint16_t i = 0; i < input_size; i += DOWNSAMPLE_FACTOR) {
    output[j++] = input[i];
  }
  *output_size = j;
}

/**
 * @brief Detect peaks in the signal within a window.
 * @param signal Pointer to signal buffer.
 * @param signal_length Length of signal buffer.
 * @param window_size Size of window to detect peaks.
 */
void find_peaks(uint16_t* signal, uint16_t signal_length, int window_size) {
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

/**
 * @brief Process buffer data and calculate BPM.
 */
uint16_t process_buffer(uint16_t* buffer, uint16_t elapsed_time_ms) {
  if (!HRBuffer_isReady()) return;

  uint16_t bpm = 0;
  if (is_data_clear(buffer, MAX30102_BUFFER_SIZE)) {
    low_pass_filter_IIR((int16_t*)buffer, MAX30102_BUFFER_SIZE);
    remove_dynamic_dc_component(buffer, MAX30102_BUFFER_SIZE);

    uint16_t downsampled_size;
    downsample_buffer(buffer, resample_buffer, MAX30102_BUFFER_SIZE, &downsampled_size);
    find_peaks(resample_buffer, downsampled_size, PEAK_WINDOW_SIZE);
    elapsed_time_ms = elapsed_time_ms * peak_diff / downsampled_size;
    bpm = peak_count * 60 * 1000 / elapsed_time_ms;       // Revisar este calculo si cambio el tamaño del buffer
    
    // print_buffer(resample_buffer, downsampled_size, elapsed_time_ms);
    HRBuffer_reset();
  }
  if (50 < bpm && bpm < 140) {
    return bpm;
  }
  return 0;
}
