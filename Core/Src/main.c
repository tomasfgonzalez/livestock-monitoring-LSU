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
#include "rtc.h"
#include "tim2.h"
#include "gpio_temperature_power.h"

#include "tests.h"
#include "i2c.h"
#include "lsu_comms.h"

#include "fsm/fsm_main.h"

TIM_HandleTypeDef htim2;

uint32_t mockTimer = 5;



void run_tests(void) {
	adc_test();
	gps_test();
	hr_test();
	gps_test();


	adc_test();
	hr_test();
	gps_test();

	run_rtc_test();
	LSU_setAddress(0x03);
	LSU_setChannelMain();
}

void run_rtc_test(void) {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  HAL_SuspendTick();
  RTC_setWakeUpTimer(10);

  /* Enter STOP 2 mode */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  RTC_clearWakeUpTimer();
  SystemClock_Config();
  HAL_ResumeTick();
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    // Peripherals initialization
    GPIO_Init();

    RTC_Init();
    MX_I2C1_Init();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(1000);


    DMA_Init();
    USART2_Init();
    MX_I2C1_Init();


    DMA_Start();
    USART2_Start();

    // Clock initialization
    TIM2_Init();
    MX_LPUART1_UART_Init();
    INIT_RX_UART2();

    run_tests();

    // System start
    FSM_Main_init();
    HAL_TIM_Base_Start_IT(&htim2);
    while (1) {
      FSM_Main_handle();
//      run_rtc_test();
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

