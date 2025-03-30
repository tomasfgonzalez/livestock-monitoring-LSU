/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : HR_Buffer.c
  * @brief          : Source file for HR Buffer
  ******************************************************************************
  */

#include "HR_Buffer.h"

uint16_t buffer[MAX30102_BUFFER_SIZE];
uint16_t buffer_index = 0;
volatile uint16_t elapsed_time_ms;
uint8_t is_buffer_full = 0;
uint8_t is_buffer_ready = 0;

/**
 * @brief Initialize buffer variables
 */
void HRBuffer_init(void) {
  buffer_index = 0;
  is_buffer_full = 0;
  is_buffer_ready = 0;
  elapsed_time_ms = 0;
  
  // Clear buffer
  for (uint16_t i = 0; i < MAX30102_BUFFER_SIZE; i++) {
    buffer[i] = 0;
  }
}

/**
 * @brief Reset buffer variables and clear buffer
 */
void HRBuffer_reset(void) {
  buffer_index = 0;
  is_buffer_full = 0;
  is_buffer_ready = 0;
  elapsed_time_ms = 0;
}


/**
 * @brief Store red LED sample in buffer and process data when full.
 * @param red_sample Red LED sample value from sensor.
 */
void HRBuffer_put(uint32_t red_sample) {
  if (is_buffer_full) {
    return;  // Evita sobrescribir datos si el buffer está lleno
  }

  static uint32_t start_time = 0;
  static uint32_t end_time = 0;

  if (buffer_index == 0) {
    //start_time = HAL_GetTick();  // Marca el tiempo de inicio
  }

  // Ignorar las primeras muestras no deseadas
  if (buffer_index < MAX30102_UNUSED_DATA) {
    buffer_index++;
    return;
  }

  // Guardar la muestra en el buffer si aún hay espacio
  uint16_t buffer_pos = buffer_index - MAX30102_UNUSED_DATA;
  if (buffer_pos < MAX30102_BUFFER_SIZE) {
    buffer[buffer_pos] = red_sample;
    buffer_index++;
  }

  // Si se llena el buffer, ejecutar acciones necesarias
  if (buffer_pos == MAX30102_BUFFER_SIZE - 1) {
    is_buffer_full = 1;
    //end_time = HAL_GetTick();
    //elapsed_time_ms = end_time - start_time;
  }
}

/**
 * @brief Check if buffer is ready to be processed
 * @return 1 if buffer is ready, 0 otherwise
 */
uint8_t HRBuffer_isReady(void) {
  if (buffer_index == MAX30102_BUFFER_SIZE + MAX30102_UNUSED_DATA) {
    is_buffer_ready = 1;
  }
  return is_buffer_ready;
}

/**
 * @brief Get buffer
 * @return Buffer
 */
uint16_t* HRBuffer_getBuffer(void) {
  return buffer;
}

/**
 * @brief Get elapsed time in milliseconds
 * @return Elapsed time in milliseconds
 */
volatile uint16_t HRBuffer_getElapsedTime(void) {
  return elapsed_time_ms;
}
