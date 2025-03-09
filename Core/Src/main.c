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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "sysClock.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "tim2.h"

#include "test.h"

#include "fsm/fsm_main.h"

TIM_HandleTypeDef htim2;

uint32_t mockTimer = 5;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

int main(void) {
    HAL_Init();
    SystemClock_Config();

    // Peripherals initialization
    GPIO_Init();
    ADC_Init();
    DMA_Init();
    USART2_Init();

    // Clock initialization
    TIM2_Init();

    // Run tests
    adc_test();
     gps_test();
     adc_test();
     gps_test();

    // System start
    FSM_Main_init();
    HAL_TIM_Base_Start_IT(&htim2);
    while (1) {
    	FSM_Main_handle();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    FSM_Main_tick_1s();
    mockTimer--;
    if (mockTimer <= 0) {
      mockTimer = 5;
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    }
  }
}


