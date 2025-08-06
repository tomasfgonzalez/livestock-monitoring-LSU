/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : sensor_all.h
  * @brief          : Header file for all sensors, serves as interface for all
  *                   sensors
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_ALL_H__
#define __SENSOR_ALL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

/* Public functions ----------------------------------------------------------*/
void sensor_all_init(void);
void sensor_all_stop(void);
bool sensor_all_has_error(void);
bool sensor_all_has_started(void);

#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_ALL_H__ */