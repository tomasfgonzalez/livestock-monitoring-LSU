/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

#include "sysClock.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

#include "test.h"


int main(void) {
    HAL_Init();
    SystemClock_Config();

    GPIO_Init();
    ADC_Init();

    DMA_Init();
    USART2_Init();


    adc_test();

    gps_test();

    adc_test();

    gps_test();

    while (1) {

        // FSM_Main_handle();
    }
}

