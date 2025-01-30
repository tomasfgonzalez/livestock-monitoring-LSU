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
    // TODO: Implement sensor initialization
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
float sensor_temperature_read(void) {
    if (sensor_status != SENSOR_TEMPERATURE_MEASUREMENT_READY) {
        return -1; // Error value
    }
    // TODO: Implement temperature reading
    sensor_status = SENSOR_TEMPERATURE_IDLE;
    return 24.0f;
}
