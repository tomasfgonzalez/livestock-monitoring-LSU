/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : max30102_buffer.c
  * @brief          : MAX30102 Buffer management
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
#include "max30102_Buffer.h"
#include "stm32l0xx_hal.h"
#include "rtc.h"
/* Private variables ---------------------------------------------------------*/
static uint16_t buffer[MAX30102_BUFFER_SIZE];
static uint16_t buffer_index = 0;
static uint8_t is_buffer_full = 0;
static uint8_t is_buffer_ready = 0;

static uint32_t start_time = 0;
static uint32_t end_time = 0;
volatile static uint32_t elapsed_time_ms = 0;

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initialize buffer variables
 */
void max30102_Buffer_Init(void) {
  buffer_index = 0;
  is_buffer_full = 0;
  is_buffer_ready = 0;

  // Clear buffer
  for (uint16_t i = 0; i < MAX30102_BUFFER_SIZE; i++) {
    buffer[i] = 0;
  }
}

/**
 * @brief Reset buffer variables and clear buffer
 */
void max30102_Buffer_Reset(void) {
  buffer_index = 0;
  is_buffer_full = 0;
  is_buffer_ready = 0;
}

volatile uint32_t start,stop;

/**
 * @brief Store red LED sample in buffer and process data when full.
 * @param red_sample Red LED sample value from sensor.
 */
void max30102_Buffer_Put(uint32_t red_sample) {
  if (is_buffer_full) {
    return;  // Avoid overwriting data if buffer is full
  }

  if (buffer_index == MAX30102_UNUSED_DATA) {
	    start = RTC_GetTick_ms();
  }

  // Ignore unwanted samples
  if (buffer_index < MAX30102_UNUSED_DATA) {
    buffer_index++;
    return;
  }

  // Save sample in buffer if there is still space
  uint16_t buffer_pos = buffer_index - MAX30102_UNUSED_DATA;
  if (buffer_pos < MAX30102_BUFFER_SIZE) {
    buffer[buffer_pos] = red_sample;
    buffer_index++;
  }

  // If buffer is full, execute necessary actions
  if (buffer_pos == MAX30102_BUFFER_SIZE - 1) {
	stop = RTC_GetTick_ms();
    is_buffer_full = 1;
    if (stop > start){
    	elapsed_time_ms = stop-start;
    }else {
    	elapsed_time_ms = (UINT32_MAX - start) + stop + 1;
    }
  }
}

uint8_t max30102_Buffer_IsReady(void) {
  if (buffer_index == MAX30102_BUFFER_SIZE + MAX30102_UNUSED_DATA) {
    is_buffer_ready = 1;
    sensor_heartrate_stop();
  }
  return is_buffer_ready;
}

uint16_t* max30102_Buffer_GetBuffer(void) {
  return buffer;
}

uint16_t max30102_Buffer_GetElapsedTime(void) {
  return elapsed_time_ms;
}
