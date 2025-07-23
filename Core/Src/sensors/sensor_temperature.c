/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_temperature.c
  * @brief          : Source file for temperature sensor, serves as interface for
  *                   temperature sensor measurements and readings
  ******************************************************************************
  */
#include "sensor_temperature.h"
#include "gpio.h"
#include "adc.h"

/* Private variables ---------------------------------------------------------*/
static SensorTemperatureStatus sensor_status = SENSOR_TEMPERATURE_STARTING;

static uint8_t timer_count = 0;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Initializes the ADC peripheral, is diferred from the power on sequence
 *        to give time for the MCP to stabilize.
 */
void sensor_temperature_init_adc(void) {
  ADC_Init();
  if (ADC_hasError()) {
    sensor_status = SENSOR_TEMPERATURE_ERROR;
  } else {
    sensor_status = SENSOR_TEMPERATURE_IDLE;
  }
}

/**
 * @brief Parses the raw ADC count and converts it to a temperature value.
 * @param raw The raw ADC value.
 * @param target The target temperature value.
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

/**
 * @brief Parses both raw ADC values and converts them to temperature values.
 * @param raw The raw ADC array.
 * @param target The target temperature array.
 */
void sensor_temperature_parse_all(uint16_t* raw, uint8_t* target) {
  sensor_temperature_parse(&raw[0], &target[0]);
  sensor_temperature_parse(&raw[1], &target[1]);
}

/* Public status check functions ---------------------------------------------*/
bool sensor_temperature_has_started(void) {
  return sensor_status == SENSOR_TEMPERATURE_IDLE || sensor_status == SENSOR_TEMPERATURE_MEASUREMENT_READY;
}

bool sensor_temperature_is_measurement_ready(void) {
  if (ADC_isDataReady()) {
    sensor_status = SENSOR_TEMPERATURE_MEASUREMENT_READY;
  }
  return sensor_status == SENSOR_TEMPERATURE_MEASUREMENT_READY;
}

bool sensor_temperature_has_error(void) {
  return sensor_status == SENSOR_TEMPERATURE_ERROR;
}

/* Public management functions ----------------------------------------------*/
void sensor_temperature_init(void) {
  timer_count = 2;
  sensor_status = SENSOR_TEMPERATURE_STARTING;
}

void sensor_temperature_stop(void) {
  ADC_DeInit();
  sensor_status = SENSOR_TEMPERATURE_IDLE;
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

void sensor_temperature_tick_1s(void) {
  if (sensor_status == SENSOR_TEMPERATURE_STARTING) {
    timer_count--;
    if (timer_count == 0) {
      sensor_temperature_init_adc();
    }
  }
}
