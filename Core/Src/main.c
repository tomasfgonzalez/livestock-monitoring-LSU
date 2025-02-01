/* USER CODE BEGIN Header */
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"



#include "adc.h"
#include "gps_parser.h"

//TODO Mover esta implementacion a otra parte del codigo, es solo de prueba

#define UBX_Rx_Size 2*sizeof(NAV_STATUS)+2*sizeof(NAV_POSLLH)
uint8_t UBX_Rx_Data[UBX_Rx_Size];


void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart){

}


int32_t Lat, Lon;
uint8_t fixStatus;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {


    if (huart == &huart2) {  // Verifica que la interrupción proviene de USART2


        // Procesa los datos en el buffer UBX_Rx_Data
        processUBXData(&UBX_Rx_Data, UBX_Rx_Size);

        // Obtener latitud y longitud
        Lat = get_UBX_Lat();
        Lon = get_UBX_Lon();
        // GPS FIX STATUS: 0 NO FIX, 1 DEAD RECKONING ONLY, 2 2D FIX, 3 3D FIX, 4 GPS + DEAD RECKONING COMBINED 5 TIME ONLY FIX
        fixStatus = get_UBX_GpsFixStatus();

        //
        // HAL_UART_DMAStop(&huart2);
    }
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
 while(1){
	 huart->ErrorCode;
 }
}




uint16_t adc_values[2];





int main(void)
{



  HAL_Init();

  SystemClock_Config();

  GPIO_Init();

  MX_ADC_Init();


  MX_DMA_Init();
  MX_USART2_UART_Init();

  //TODO ADC TEST
  HAL_UART_Receive_DMA(&huart2, UBX_Rx_Data, UBX_Rx_Size);
  HAL_Delay(5000);


  HAL_UART_Receive_DMA(&huart2, UBX_Rx_Data, UBX_Rx_Size);
  HAL_Delay(5000);



  adc_test();


  while (1) {



	  //TODO TEST PARA EL ADC Si llega una transferencia DMA se pijean los datos


	 // FSM_Main_handle();
  }
}

uint16_t Voltage_val[2];

void adc_test(void){

	 Start_ADC_IRQ();
	 HAL_Delay(3000);
	 get_ADC_values(&Voltage_val);



	 Start_ADC_IRQ();
	 HAL_Delay(3000);
	 get_ADC_values(&Voltage_val);

}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
