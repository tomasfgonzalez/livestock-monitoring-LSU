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
#include "sensor_heartrate.h"
#include "sensor_gps.h"

#include <assert.h>
#include <string.h>

// Temperature testing variables
uint16_t Voltage_val[2];
uint8_t Temperature_val[2];

// Heart rate testing variables
uint16_t bpm_val;
uint8_t heartrate_val;

// GPS testing variables
uint8_t usart_data[100];
uint16_t usart_data_length;
int32_t Latitude, Longitude;
uint8_t fixStatus;
GPSData gps_data;


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

void i2c_test(void){
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	I2C_Init();
	assert(!I2C_hasError());

	I2C_DeInit();
	GPIO_Sensors_PowerOff();
}

void max30102_test(void){
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	I2C_Init();
	max30102_Init();
	max30102_Start();

	while (!max30102_IsReady());
	bpm_val = max30102_GetBPM();
	assert(40 <= bpm_val && bpm_val <= 240);

	max30102_Stop();
	I2C_DeInit();
	GPIO_Sensors_PowerOff();
}

void heartrate_test(void) {
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	sensor_heartrate_init();
	while (!sensor_heartrate_has_started()) {
		bool hasError = sensor_heartrate_has_error();
		assert(!hasError);
	}
	// Init successful

	while (!sensor_heartrate_is_measurement_ready()) {
		bool hasError = sensor_heartrate_has_error();
		assert(!hasError);
	}
	// Measurement ready

	bool readSuccess = sensor_heartrate_read(&heartrate_val);
	assert(readSuccess);
	assert(40 <= heartrate_val && heartrate_val <= 240);

	sensor_heartrate_stop();
	GPIO_Sensors_PowerOff();
}

void usart_test(void) {
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	DMA_Init();
	USART_Init();
	assert(!USART_hasError());

	while (!USART_isDataReady());
	assert(USART_getDataLength() > 0);

	USART_DeInit();
	DMA_Stop();
	GPIO_Sensors_PowerOff();
}

void neo6m_test(void) {
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	DMA_Init();
	USART_Init();
	assert(!USART_hasError());

	while (!USART_isDataReady());
	uint8_t* usart_data_pointer = USART_getData();
	usart_data_length = USART_getDataLength();
	memcpy(usart_data, usart_data_pointer, usart_data_length);

	neo6m_ProcessData(usart_data, usart_data_length);
	// Obtener latitud y longitud
	Latitude = neo6m_GetLatitude();
	Longitude = neo6m_GetLongitude();
	assert(Latitude != 0 && Longitude != 0);

	USART_DeInit();
	DMA_Stop();
	GPIO_Sensors_PowerOff();
}

void neo6m_fix_test() {
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	DMA_Init();
	USART_Init();
	assert(!USART_hasError());

	while (fixStatus <= 2) {
		while (!USART_isDataReady());

		uint8_t* usart_data_pointer = USART_getData();
		usart_data_length = USART_getDataLength();
		neo6m_ProcessData(usart_data_pointer, usart_data_length);

		// Obtener latitud y longitud
		Latitude = neo6m_GetLatitude();
		Longitude = neo6m_GetLongitude();
		fixStatus = neo6m_GetFixStatus();

		USART_clearData();
	}

	// GPS FIX STATUS: 0 NO FIX, 1 DEAD RECKONING ONLY, 2 2D FIX, 3 3D FIX,
	// 4 GPS + DEAD RECKONING COMBINED, 5 TIME ONLY FIX
	assert(fixStatus > 2);
	USART_DeInit();
	DMA_Stop();
	GPIO_Sensors_PowerOff();
}

void gps_test(void) {
	GPIO_Init();
	GPIO_Sensors_PowerOn();

	sensor_gps_init();
	while (!sensor_gps_has_started()) {
		bool hasError = sensor_gps_has_error();
		assert(!hasError);
	}
	// Init successful
	
	while (!sensor_gps_is_measurement_ready()) {
		bool hasError = sensor_gps_has_error();
		assert(!hasError);
	}
	// Measurement ready

	sensor_gps_read(&gps_data);
	assert(gps_data.latitude != 0 && gps_data.longitude != 0);

	sensor_gps_stop();
	GPIO_Sensors_PowerOff();
}

void tests_tick_1s(void) {
	sensor_temperature_tick_1s();
}
