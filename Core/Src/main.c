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

#include "system_clock.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "rtc.h"
#include "tim2.h"
#include "i2c.h"
#include "lpuart.h"

/* Test includes - only compiled in debug builds */
#ifdef DEBUG
// #include "tests.h"
// #include "all_sensors_test.h"
#endif

#include "sensor_temperature.h"

#include "lsu_comms.h"

#include "fsm/fsm_main.h"

uint32_t mockTimer = 5;

/* Test functions - only compiled in debug builds */
#ifdef DEBUG

#endif

int main(void) {
    HAL_Init();
    SystemClock_Config();

    // Peripherals and timers initialization
    GPIO_Init();
    RTC_Init();
    TIM2_Init();
    HAL_TIM_Base_Start_IT(&htim2);


    /* Test code - only executed in debug builds */
#ifdef DEBUG
    //  run_tests();
  //  all_sensors_test();
#endif

    // System start
    FSM_Main_init();

    while (1) {
      FSM_Main_handle();
    }
}

void TIM2_tick(void) {
  FSM_Main_tick_1s();
  mockTimer--;
  if (mockTimer <= 0) {
    mockTimer = 5;
  }

  // For testing purposes
//  tests_tick_1s();
  sensor_temperature_tick_1s();
}
