/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : time_config.c
  * @brief          : Source file for time configuration, holds functions to configure
  *                   the time events of the device
  ******************************************************************************
  */
#include <stdbool.h>
#include <stdint.h>

// Default values
static uint32_t timeInSeconds = 0;
static uint32_t timePeriod = 20;           // Total cycle period in seconds
static uint32_t sensingWindowStart = 5;     // When sensing window starts within period
static uint32_t sensingWindowDuration = 5;  // How long sensing window lasts
static uint32_t transmitWindowStart = 15;   // When transmit window starts within period  
static uint32_t transmitWindowDuration = 5; // How long transmit window lasts

// Clock
// TODO: Use real time instead of a mock timer
void time_config_tick_1s(void) {
    timeInSeconds++;
}

// Setters
void time_config_set_period(uint32_t period) {
    timePeriod = period;
}

void time_config_set_sensing_window(uint32_t start, uint32_t duration) {
    sensingWindowStart = start;
    sensingWindowDuration = duration;
}

void time_config_set_transmit_window(uint32_t start, uint32_t duration) {
    transmitWindowStart = start;
    transmitWindowDuration = duration;
}

// Getters
uint32_t time_config_get_period(void) {
    return timePeriod;
}

uint32_t time_config_get_sensing_start(void) {
    return sensingWindowStart;
}

uint32_t time_config_get_sensing_duration(void) {
    return sensingWindowDuration;
}

uint32_t time_config_get_transmit_start(void) {
    return transmitWindowStart;
}

uint32_t time_config_get_transmit_duration(void) {
    return transmitWindowDuration;
}

// Window check functions
bool time_config_on_sensing_window(void) {
    uint32_t moduledTime = timeInSeconds % timePeriod;
    return (moduledTime >= sensingWindowStart) && 
           (moduledTime < (sensingWindowStart + sensingWindowDuration));
}

bool time_config_on_transmit_window(void) {
    uint32_t moduledTime = timeInSeconds % timePeriod;
    return (moduledTime >= transmitWindowStart) && 
           (moduledTime < (transmitWindowStart + transmitWindowDuration));
}
