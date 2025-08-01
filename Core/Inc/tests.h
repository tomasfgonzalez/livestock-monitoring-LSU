/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : test.h
  * @brief          : Header for test file
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
#ifndef __TESTS_H__
#define __TESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "tests.h"
#include "usart.h"
#include "neo6m.h"
#include "adc.h"
#include "dma.h"
#include "max30102.h"

/* Function prototypes -------------------------------------------------------*/
void adc_test(void);
void temperature_test(void);

void i2c_test(void);
void max30102_test(void);
void heartrate_test(void);

void usart_test(void);
void neo6m_test(void);
void neo6m_fix_test(void);
void gps_test(void);

void tests_tick_1s(void);

#ifdef __cplusplus
}
#endif

#endif /* __TESTS_H__ */

