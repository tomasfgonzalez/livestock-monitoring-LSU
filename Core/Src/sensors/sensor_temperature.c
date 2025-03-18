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
    GPIO_temperature_power_init();
    if (ADC_hasError()) {
        sensor_status = SENSOR_TEMPERATURE_ERROR;
    }
    sensor_status = SENSOR_TEMPERATURE_IDLE;
}

void sensor_temperature_start(void) {
    ADC_Enable();
    GPIO_temperature_power_start();
    sensor_status = SENSOR_TEMPERATURE_IDLE;
}

void sensor_temperature_stop(void) {
    ADC_Disable();
    GPIO_temperature_power_stop();
    sensor_status = SENSOR_TEMPERATURE_STARTING;
}

/**
 * Sensor reading
 */

void sensor_temperature_parse(uint16_t *raw, uint8_t *target) {
	uint16_t value = *raw;
    float voltage = (value * 3.3f) / 4095.0f;
    float temp = (voltage - 0.5f) * 100.0f;
    
    // Round and clamp temperature to valid range
    if (temp < 0.0f) temp = 0.0f;
    if (temp > 100.0f) temp = 100.0f;
    *target = (uint8_t) temp;
}

void sensor_temperature_parse_all(uint16_t* raw, uint8_t* target) {
    sensor_temperature_parse(&raw[0], &target[0]);
    sensor_temperature_parse(&raw[1], &target[1]);
}

bool sensor_temperature_read(uint8_t* target) {
    if (sensor_status != SENSOR_TEMPERATURE_MEASUREMENT_READY) {
        return false;
    }

    uint16_t raw[2];
    ADC_GetValues(raw);
    sensor_temperature_parse_all(raw, target);

    sensor_status = SENSOR_TEMPERATURE_IDLE;
    return true;
}
