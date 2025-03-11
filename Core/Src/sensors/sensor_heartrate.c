/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_heartrate.c
  * @brief          : Source file for heart rate sensor, serves as interface for
  *                   heart rate sensor measurements and readings
  ******************************************************************************
  */
#include "sensor_heartrate.h"
/** 
 * Sensor status
 * */
static SensorHeartrateStatus sensor_status = SENSOR_HEARTRATE_STARTING;

bool sensor_heartrate_has_started(void) {
    return sensor_status == SENSOR_HEARTRATE_IDLE || sensor_status == SENSOR_HEARTRATE_MEASUREMENT_READY;
}

bool sensor_heartrate_is_measurement_ready(void) {
    return sensor_status == SENSOR_HEARTRATE_MEASUREMENT_READY;
}

bool sensor_heartrate_has_error(void) {
    return sensor_status == SENSOR_HEARTRATE_ERROR;
}

/**
 * Sensor management
 */
void sensor_heartrate_init(void) {
    // TODO: Implement sensor initialization
    sensor_status = SENSOR_HEARTRATE_IDLE;
}

void sensor_heartrate_start(void) {
    // TODO: Implement sensor start procedure (a.k.a: start measurement)
    sensor_status = SENSOR_HEARTRATE_MEASUREMENT_READY;
}

void sensor_heartrate_stop(void) {
    // TODO: Implement sensor stop procedure (a.k.a: stop the sensor)
    sensor_status = SENSOR_HEARTRATE_IDLE;
}

/**
 * Sensor reading
 */
uint8_t sensor_heartrate_read(void) {
    if (sensor_status != SENSOR_HEARTRATE_MEASUREMENT_READY) {
        return 0; // Error value
    }
    // TODO: Implement heart rate reading
    sensor_status = SENSOR_HEARTRATE_IDLE;
    return 75; // Default test value
}
