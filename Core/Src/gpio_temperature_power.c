/**
  * @file    gpio_temperature_power.c
  * @brief   Temperature sensor power control via GPIO implementation
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio_temperature_power.h"

/* Private defines ----------------------------------------------------------*/
#define TEMP_POWER_PORT     GPIOA
#define TEMP_POWER_PIN      GPIO_PIN_6

/* Functions -----------------------------------------------------------------*/
void GPIO_temperature_power_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(TEMP_POWER_PORT, TEMP_POWER_PIN, GPIO_PIN_RESET);

    /* Configure GPIO pin */
    GPIO_InitStruct.Pin = TEMP_POWER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TEMP_POWER_PORT, &GPIO_InitStruct);
}

void GPIO_temperature_power_start(void) {
    HAL_GPIO_WritePin(TEMP_POWER_PORT, TEMP_POWER_PIN, GPIO_PIN_SET);
}

void GPIO_temperature_power_stop(void) {
    HAL_GPIO_WritePin(TEMP_POWER_PORT, TEMP_POWER_PIN, GPIO_PIN_RESET);
}
