/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : tests.c
  * @brief          : Source file for tests, holds functions to test the device
  ******************************************************************************
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "test.h"
#include "usart.h"
#include "gps_parser.h"
#include "adc.h"
#include "process_HR.h"
#include "max30102_for_stm32_hal.h"



int32_t Lat, Lon;
uint8_t fixStatus;
uint16_t Voltage_val[2];



void adc_test(void) {

    ADC_Enable();

    HAL_Delay(2000);

    ADC_GetValues(Voltage_val);
}


void gps_test(void) {

	Start_DMA_UART2();
	DMA_Start();
	// Obtener latitud y longitud
	Lat = get_UBX_Lat();
	Lon = get_UBX_Lon();

	HAL_Delay(2000);


	// GPS FIX STATUS: 0 NO FIX, 1 DEAD RECKONING ONLY, 2 2D FIX, 3 3D FIX,
	// 4 GPS + DEAD RECKONING COMBINED, 5 TIME ONLY FIX
	fixStatus = get_UBX_GpsFixStatus();
}



void hr_test(void){

	max30102_init_conf();
	reset_bpm();
	while(!get_bpm()){
	max30102_irq_check();
	}
	max30102_set_shutdown();
}



