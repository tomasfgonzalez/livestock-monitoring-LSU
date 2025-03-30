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
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "gps_parser.h"
#include "adc.h"
#include "dma.h"
#include "process_HR.h"
#include "max30102.h"

/* Function prototypes -------------------------------------------------------*/
void gps_test(void);
void adc_test(void);
void hr_test(void);


#ifdef __cplusplus
}
#endif

#endif /* __TESTS_H__ */

