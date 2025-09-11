/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_gps.c
  * @brief          : Source file for GPS sensor, serves as interface for GPS
  *                   sensor measurements and readings
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sensor_all.h"

#include "gpio.h"
#include "sensor_temperature.h"
#include "sensor_heartrate.h"
#include "sensor_gps.h"

/* Public functions ----------------------------------------------------------*/
void sensor_all_init(void) {
  GPIO_Init();
  GPIO_Sensors_PowerOn();




  //sensor_temperature_init();
 // sensor_heartrate_init();
  //sensor_gps_init();
}

void sensor_all_stop(void) {
 // sensor_temperature_stop();
 // sensor_heartrate_stop();
 // sensor_gps_stop();


  GPIO_Sensors_PowerOff();
}

bool sensor_all_has_error(void) {
  return sensor_temperature_has_error() || sensor_heartrate_has_error() || sensor_gps_has_error();
}

bool sensor_all_has_started(void) {
  return sensor_temperature_has_started() && sensor_heartrate_has_started() && sensor_gps_has_started();
}
