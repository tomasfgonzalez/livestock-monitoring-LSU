/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_temperature.c
  * @brief          : Source file for temperature sensor, serves as interface for
  *                   temperature sensor measurements and readings
  ******************************************************************************
  */
#include "sensor_temperature.h"

/** 
 * Sensor status
 * */
static SensorTemperatureStatus sensor_status = SENSOR_TEMPERATURE_STARTING;

bool sensor_temperature_has_started(void) {
    return sensor_status == SENSOR_TEMPERATURE_IDLE || sensor_status == SENSOR_TEMPERATURE_MEASUREMENT_READY;
}

bool sensor_temperature_is_measurement_ready(void) {
    return sensor_status == SENSOR_TEMPERATURE_MEASUREMENT_READY;
}

bool sensor_temperature_has_error(void) {
    return sensor_status == SENSOR_TEMPERATURE_ERROR;
}

/**
 * Sensor management
 */
void sensor_temperature_init(void) {
    ADC_Init();
    // TODO: Need to initialize power supply for the sensor
    sensor_status = SENSOR_TEMPERATURE_IDLE;
}

void sensor_temperature_start(void) {
    // TODO: Implement sensor start procedure (a.k.a: start measurement)
    sensor_status = SENSOR_TEMPERATURE_MEASUREMENT_READY;
}

void sensor_temperature_stop(void) {
    // TODO: Implement sensor stop procedure (a.k.a: stop the sensor)
    sensor_status = SENSOR_TEMPERATURE_IDLE;
}

/**
 * Sensor reading
 */
bool sensor_temperature_read(uint16_t* target) {
    if (sensor_status != SENSOR_TEMPERATURE_MEASUREMENT_READY) {
        return false;
    }
    
    get_ADC_values(target);
    sensor_status = SENSOR_TEMPERATURE_IDLE;
    return true;
}
