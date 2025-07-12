/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : gpio.h
  * @brief          : Header for GPIO file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Gonazalez & Morris.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* System debug defines ------------------------------------------------------*/
#define VCP_TX_GPIO_Port      GPIOA
#define VCP_RX_GPIO_Port      GPIOA
#define TMS_Pin               GPIO_PIN_13
#define TMS_GPIO_Port         GPIOA
#define TCK_Pin               GPIO_PIN_14
#define TCK_GPIO_Port         GPIOA

/* Private defines -----------------------------------------------------------*/
#define BOARD_LED_PORT        GPIOB
#define BOARD_LED_PIN         GPIO_PIN_3

#define SENSORS_POWER_PORT    GPIOA
#define SENSORS_POWER_PIN     GPIO_PIN_6

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initializes the GPIO pins.
 */
void GPIO_Init(void);

/**
 * @brief Powers on all the sensors.
 */
void GPIO_Sensors_PowerOn(void);

/**
 * @brief Powers off all the sensors.
 */
void GPIO_Sensors_PowerOff(void);

#ifdef __cplusplus
}
#endif

#endif /*__ GPIO_H__ */
