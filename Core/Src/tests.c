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
#include "gpio.h"
#include "sensor_temperature.h"
#include <assert.h>

// GPS testing variables
int32_t Lat, Lon;
uint8_t fixStatus;

// Temperature testing variables
uint16_t Voltage_val[2];
uint8_t Temperature_val[2];

// Heart rate testing variables
uint16_t bpm;


void adc_test(void) {
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	ADC_Init();
	HAL_Delay(2000);
	ADC_GetValues(Voltage_val);
	// Both inputs should be between 100 and 2000
	assert(100 < Voltage_val[0] && Voltage_val[0] < 2000);
	assert(100 < Voltage_val[1] && Voltage_val[1] < 2000);

	ADC_DeInit();
	HAL_Delay(2000);
	ADC_GetValues(Voltage_val);
	// Both inputs should be 0 when ADC is disabled
	assert(Voltage_val[0] == 0);
	assert(Voltage_val[1] == 0);

	ADC_Init();
	HAL_Delay(2000);
	ADC_GetValues(Voltage_val);
	// Both inputs should be valid even after re-initialization
	assert(100 < Voltage_val[0] && Voltage_val[0] < 2000);
	assert(100 < Voltage_val[1] && Voltage_val[1] < 2000);

	ADC_DeInit();
	GPIO_Sensors_PowerOff();
}

void temperature_test(void) {
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	sensor_temperature_init();
	while (!sensor_temperature_has_started()) {
		bool hasError = sensor_temperature_has_error();
		assert(!hasError);
	}
	// Init successful

	while (!sensor_temperature_is_measurement_ready()) {
		bool hasError = sensor_temperature_has_error();
		assert(!hasError);
	}
	// Measurement ready

	sensor_temperature_read(Temperature_val);
	assert(10 < Temperature_val[0] && Temperature_val[0] < 40);
	assert(10 < Temperature_val[1] && Temperature_val[1] < 40);

	sensor_temperature_stop();
	GPIO_Sensors_PowerOff();
}

void gps_test(void) {
	DMA_Init();
	USART_Init();
	DMA_Start();
	USART_Start();

	// Obtener latitud y longitud
	Lat = neo6m_GetLongitude();
	Lon = neo6m_GetLongitude();

	HAL_Delay(2000);


	// GPS FIX STATUS: 0 NO FIX, 1 DEAD RECKONING ONLY, 2 2D FIX, 3 3D FIX,
	// 4 GPS + DEAD RECKONING COMBINED, 5 TIME ONLY FIX
	fixStatus = neo6m_GetFixStatus();
}

void hr_test(void){
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	max30102_Init();
	max30102_Start();

	while (!max30102_IsReady());
	bpm = max30102_GetBPM();

	max30102_Stop();
	GPIO_Sensors_PowerOff();
}

void tests_tick_1s(void) {
	sensor_temperature_tick_1s();
}
