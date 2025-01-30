#ifndef SENSOR_TEMPERATURE_H
#define SENSOR_TEMPERATURE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SENSOR_TEMPERATURE_STARTING,
    SENSOR_TEMPERATURE_IDLE,
    SENSOR_TEMPERATURE_MEASUREMENT_READY,
    SENSOR_TEMPERATURE_ERROR
} SensorTemperatureStatus;

void sensor_temperature_init(void);
void sensor_temperature_start(void);
void sensor_temperature_stop(void);

bool sensor_temperature_has_started(void);
bool sensor_temperature_is_measurement_ready(void);
bool sensor_temperature_has_error(void);

float sensor_temperature_read(void);

#endif /* SENSOR_TEMPERATURE_H */
