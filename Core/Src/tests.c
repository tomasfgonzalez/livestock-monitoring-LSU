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
#include "tests.h"

int32_t Lat, Lon;
uint8_t fixStatus;
uint16_t Voltage_val[2];


void adc_test(void) {
	ADC_Init();
    ADC_Enable();

    HAL_Delay(2000);

    ADC_GetValues(Voltage_val);
}


void gps_test(void) {
	DMA_Init();
	USART2_Init();
	DMA_Start();
	USART2_Start();

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
	while(!get_bpm());
	max30102_stop();
}



