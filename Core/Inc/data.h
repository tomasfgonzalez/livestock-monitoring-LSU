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
#include <data.h>
#include "usart.h"
#include "gps_parser.h"
#include "adc.h"
#include "dma.h"
#include "max30102_Process.h"
#include "max30102.h"

/* Function prototypes -------------------------------------------------------*/
void gps_data(void);
void adc_data(void);
void hr_data(void);
void run_sleep_mode(uint32_t);
int32_t get_Lat();
int32_t get_Lon();
uint8_t get_fixStatus();
uint16_t get_V_T1();
uint16_t get_V_T2();
uint16_t get_bpm();






struct data_package{
	int32_t Lat;
	int32_t Lon;
	uint8_t fixStatus;
	uint16_t V_T1;
	uint16_t V_T2;
	uint16_t bpm;
};

#ifdef __cplusplus
}
#endif

#endif /* __TESTS_H__ */

