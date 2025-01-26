/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : mef_mode.c
  * @brief          : Main Mef file
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <fsm_mode.h>

uint16_t adc_values[2];
/**
  * @brief  The application entry point.
  * @retval int
  */
void fsm_mode(void)
{


  while (1)
  {
  //TODO MEF Principal, en la kl46 usabamos systick para los timers, pensar que hacemos aca.
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);//LED ON
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);//LED OFF
  HAL_Delay(500);


  Get_ADC_Val(adc_values);
  uint16_t channel_4_value = adc_values[0];  //A3 - PA_4
  uint16_t channel_5_value = adc_values[1];  //A4 - PA_5
  }
}


