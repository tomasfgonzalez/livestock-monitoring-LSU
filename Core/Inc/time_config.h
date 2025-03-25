/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : time_config.h
  * @brief          : Header for time configuration file
  ******************************************************************************
  */
#ifndef TIME_CONFIG_H
#define TIME_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// Clock
void time_config_tick_1s(void);

// Setters
void time_config_set_period(uint32_t period);
void time_config_set_sensing_window(uint32_t start, uint32_t duration);
void time_config_set_transmit_window(uint32_t start, uint32_t duration);

// Getters
uint32_t time_config_get_period(void);
uint32_t time_config_get_sensing_start(void);
uint32_t time_config_get_sensing_duration(void);
uint32_t time_config_get_transmit_start(void);
uint32_t time_config_get_transmit_duration(void);

// Window check functions
bool time_config_on_sensing_window();
bool time_config_on_transmit_window();

#ifdef __cplusplus
}
#endif
#endif /* TIME_CONFIG_H */
