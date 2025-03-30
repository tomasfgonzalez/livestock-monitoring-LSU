/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : HR_Buffer.h
  * @brief          : Header file for HR Buffer
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HR_BUFFER_H
#define HR_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/
#define MAX30102_BUFFER_SIZE 2000 // Buffer size if change go check bpm
#define MAX30102_UNUSED_DATA 128    // Skip first 128 samples


/* Functions -----------------------------------------------------------------*/
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
 
