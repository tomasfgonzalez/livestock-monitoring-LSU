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
    if (ADC_areConversionsFinished()) {
        sensor_status = SENSOR_TEMPERATURE_MEASUREMENT_READY;
    }
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
    if (ADC_hasError()) {
        sensor_status = SENSOR_TEMPERATURE_ERROR;
    }
    sensor_status = SENSOR_TEMPERATURE_IDLE;
}

void sensor_temperature_start(void) {
    ADC_Enable();
    sensor_status = SENSOR_TEMPERATURE_IDLE;
}

void sensor_temperature_stop(void) {
    ADC_Disable();
    sensor_status = SENSOR_TEMPERATURE_STARTING;
}

/**
 * Sensor reading
 */
bool sensor_temperature_read(uint16_t* target) {
    if (sensor_status != SENSOR_TEMPERATURE_MEASUREMENT_READY) {
        return false;
    }
    
    ADC_GetValues(target);
    sensor_status = SENSOR_TEMPERATURE_IDLE;
    return true;
}
