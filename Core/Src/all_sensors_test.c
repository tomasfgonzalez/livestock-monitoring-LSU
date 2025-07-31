/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : all_sensors_test.c
  * @brief          : Source file for all sensors test
  ******************************************************************************
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "all_sensors_test.h"
#include <assert.h>

void all_sensors_test(void) {
	// Temperature testing variables
	static uint8_t Temperature_val[2];

	// Heart rate testing variables
	static uint8_t heartrate_val;

	// GPS testing variables
	static GPSData gps_data;

	sensor_all_init();
	while (!sensor_all_has_started()) {
		bool hasError = sensor_all_has_error();
		assert(!hasError);
	}
	// Init successful
	
	int readyMask = 0;
	while (readyMask != 7) {
		if (sensor_temperature_is_measurement_ready()) {
			sensor_temperature_read(Temperature_val);
			readyMask |= 1;
		}
		if (sensor_heartrate_is_measurement_ready()) {
			sensor_heartrate_read(&heartrate_val);
			readyMask |= 2;
		}
		if (sensor_gps_is_measurement_ready()) {
			sensor_gps_read(&gps_data);
			readyMask |= 4;
		}
	}

	assert(10 < Temperature_val[0] && Temperature_val[0] < 40);
	assert(10 < Temperature_val[1] && Temperature_val[1] < 40);
	assert(40 <= heartrate_val && heartrate_val <= 240);
	assert(gps_data.latitude != 0 && gps_data.longitude != 0);

	sensor_all_stop();
}
