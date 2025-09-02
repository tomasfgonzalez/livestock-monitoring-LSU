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
// void run_rtc_test(void) {
//   HAL_GPIO_WritePin(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_PIN_SET);
//   HAL_Delay(1000);
//   HAL_GPIO_WritePin(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_PIN_RESET);

//   HAL_SuspendTick();
//   RTC_setWakeUpTimer(10);

//   /* Enter STOP 2 mode */
//   __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
//   HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

//   RTC_clearWakeUpTimer();
//   SystemClock_Config();
//   HAL_ResumeTick();
// }

// void run_tests(void) {
  // adc_test();
  // temperature_test();
  
	// i2c_test();
	// max30102_test();
	// heartrate_test();

  // usart_test();
  // neo6m_test();
  // neo6m_fix_test();
  // gps_test();

// 	run_rtc_test();

//   LSU_setAddress(0x03);
//   LSU_setChannelMain();
// }
#endif
#include "rtc.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();

    // Peripherals and timers initialization
    GPIO_Init();
    RTC_Init();
    TIM2_Init();
    HAL_TIM_Base_Start_IT(&htim2);

    //RTC_Init();
    //volatile uint32_t start = RTC_GetTick_ms();
    //HAL_Delay(100000);
    //volatile uint32_t stop = RTC_GetTick_ms();
    //volatile uint32_t elapsed_time = stop-start;
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
