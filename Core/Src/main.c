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
#include <data.h>
#include "main.h"

#include "sysClock.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "rtc.h"
#include "tim2.h"
#include "gpio_temperature_power.h"

#include "i2c.h"
#include "lsu_comms.h"

#include "fsm/fsm_main.h"

TIM_HandleTypeDef htim2;

uint32_t mockTimer = 5;



void run_tests(void) {
    GPIO_Init();
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(500);

    //Inits:

    I2C_Init();

    DMA_Init();
    USART2_Init();
    DMA_Start();
    USART2_Start();

    RTC_Init();
    TIM2_Init();

    LPUART1_Init();
    LPUART1_Start();

    //Start:







    ADC_Init();
    ADC_Start();


    adc_data();
	hr_data();
	gps_data();
	run_sleep_mode(10);   // segundos a dormir [s]



	LSU_setAddress(0x03); //Address a configurar
	LSU_setChannelMain(); //Elijo el canal principal

}





int main(void) {
    HAL_Init();
    SystemClock_Config();
    // Peripherals initialization




    run_tests();

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

