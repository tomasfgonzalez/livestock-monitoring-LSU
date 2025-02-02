/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "test.h"
#include "usart.h"
#include "gps_parser.h"
#include "adc.h"

int32_t Lat, Lon;
uint8_t fixStatus;
uint16_t Voltage_val[2];



void adc_test(void) {

    Start_ADC_IRQ();

    HAL_Delay(2000);

    get_ADC_values(Voltage_val);
}


void gps_test(void) {

	Start_DMA_UART2();
	// Obtener latitud y longitud
	Lat = get_UBX_Lat();
	Lon = get_UBX_Lon();

	HAL_Delay(2000);


	// GPS FIX STATUS: 0 NO FIX, 1 DEAD RECKONING ONLY, 2 2D FIX, 3 3D FIX,
	// 4 GPS + DEAD RECKONING COMBINED, 5 TIME ONLY FIX
	fixStatus = get_UBX_GpsFixStatus();


}



