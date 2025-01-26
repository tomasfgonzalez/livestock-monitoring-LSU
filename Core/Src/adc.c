/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwADCxConvertedValue = 0;
__IO uint8_t   ubADC_overrun_status = RESET;


ADC_HandleTypeDef    hadc;
ADC_ChannelConfTypeDef        sConfig;

/* ADC Configuration */
void ADC_Init(void)
{



	  ADC_ChannelConfTypeDef sConfig = {0};


	  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	  */
	  hadc.Instance = ADC1;
	  hadc.Init.OversamplingMode = DISABLE;
	  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	  hadc.Init.Resolution = ADC_RESOLUTION_12B;
	  hadc.Init.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
	  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc.Init.ContinuousConvMode = DISABLE;
	  hadc.Init.DiscontinuousConvMode = DISABLE;
	  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc.Init.DMAContinuousRequests = DISABLE;
	  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	  hadc.Init.LowPowerAutoWait = ENABLE;
	  hadc.Init.LowPowerFrequencyMode = ENABLE;
	  hadc.Init.LowPowerAutoPowerOff = ENABLE;
	  if (HAL_ADC_Init(&hadc) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure for the selected ADC regular channel to be converted.
	  */
	  sConfig.Channel = ADC_CHANNEL_4;
	  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure for the selected ADC regular channel to be converted.
	  */
	  sConfig.Channel = ADC_CHANNEL_5;
	  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }


}


/* Get the last converted ADC value */
void Get_ADC_Val(uint16_t* adc_values) {
    // Verificar que el puntero no sea NULL
    if (adc_values == NULL) {
        return;  // Si el puntero es NULL, no hacemos nada
    }

    // Iniciar la conversión para el primer canal (Channel 4)
    HAL_ADC_Start(&hadc);
    if (HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY) == HAL_OK) {
        adc_values[0] = HAL_ADC_GetValue(&hadc);  // Obtener el valor de Channel 4
    }

    // Iniciar la conversión para el segundo canal (Channel 5)
    if (HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY) == HAL_OK) {
        adc_values[1] = HAL_ADC_GetValue(&hadc);  // Obtener el valor de Channel 5
    }

    // Detener la conversión
    HAL_ADC_Stop(&hadc);
}


/* ADC error callback (Overrun detection) */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  if (HAL_ADC_GetError(hadc) == HAL_ADC_ERROR_OVR)
  {
    /* Handle overrun error */
    ubADC_overrun_status = SET;  // Set overrun flag
  }
  else
  {
    /* In case of any other ADC error */
    Error_Handler();
  }
}
