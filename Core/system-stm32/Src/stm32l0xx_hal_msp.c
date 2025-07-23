/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32l0xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
#include "usart.h"
#include "lpuart.h"
#include "i2c.h"
#include "rtc.h"
#include "tim2.h"

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base) {
  if(htim_base->Instance == TIM2) {
    TIM2_MspInit(htim_base);
  }
}

/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base) {
  if(htim_base->Instance == TIM2) {
    TIM2_MspDeInit(htim_base);
  }
}

/**
* @brief TIM Period Elapsed Callback
* This function is called when the TIM period is elapsed
* @param htim: TIM handle pointer
* @retval None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    TIM2_tick();
  }
}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/


/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/


/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
  if(huart->Instance == LPUART1) {
    LPUART_MspInit(huart);
  } else if (huart->Instance == USART2) {
    USART_MspInit(huart);
  }
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart) {
  if(huart->Instance == LPUART1) {
    LPUART_MspDeInit(huart);
  } else if (huart->Instance == USART2) {
    USART_MspDeInit(huart);
  }
}

/**
* @brief UART Rx Complete Callback
* This function is called when the UART receives a complete message
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    USART_RxCpltCallback(huart);
  }
}

/**
* @brief UART Rx Event Callback
* This function is called when the UART receives an event
* @param huart: UART handle pointer
* @param Size: Size of the message
* @retval None
*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart->Instance == LPUART1) {
    LPUART_RxEventCallback(huart, Size);
  }
}

/**
* @brief UART Error Callback
* This function is called when the UART encounters an error
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  while (1) {
    (void) huart->ErrorCode; // Captura y maneja el código de error si es necesario
  }
}

/**
* @brief UART Rx Half Complete Callback
* This function is called when the UART receives half of a message
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
  // Implementación futura (si es necesario)
}

/**
* @brief RTC MSP Initialization
* This function configures the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc) {
  if (hrtc->Instance == RTC) {
    RTC_MspInit(hrtc);
  }
}

/**
* @brief RTC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc) {
  if (hrtc->Instance == RTC) {
    RTC_MspDeInit(hrtc);
  }
}
