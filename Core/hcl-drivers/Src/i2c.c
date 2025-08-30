/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : i2c.c
  * @brief          : Hardware Configuration Layer - I2C configuration
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
#include "i2c.h"

#include <stdbool.h>

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

static bool initError = false;

/* HAL Functions ------------------------------------------------------------*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (i2cHandle->Instance == I2C1) {
    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 GPIO clock enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /**
     * I2C1 GPIO Configuration
     * PB6     ------> I2C1_SCL
     * PB7     ------> I2C1_SDA
     */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle) {
  if (i2cHandle->Instance == I2C1) {
    /* I2C1 clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**
     * I2C1 GPIO Configuration
     * PB6     ------> I2C1_SCL
     * PB7     ------> I2C1_SDA
     */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
  }
}

/* Public functions ----------------------------------------------------------*/
void I2C_Init(void) {
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00B07CB4;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
    initError = true;
  }

  /* Configure Analogue filter */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
    initError = true;
  }

  /* Configure Digital filter */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
    initError = true;
  }
}

void I2C_DeInit(void) {
  HAL_I2C_DeInit(&hi2c1);
}

bool I2C_hasError(void) {
  return initError;
}
