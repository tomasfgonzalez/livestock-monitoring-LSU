/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : time_config.c
  * @brief          : Source file for time configuration, holds functions to configure
  *                   the time events of the device
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "time_config.h"
#include "stm32l0xx_hal.h"
#include "rtc.h"

/* Defines -------------------------------------------------------------------*/
#define SENSING_TRANSMIT_GAP_MS 5000 // Sense 1 minute before transmitting

/* Private variables ---------------------------------------------------------*/
static uint32_t timeToNextTransmission_ms;
static uint32_t timeToNextSensing_ms;

static bool readyToSense = false;
static bool readyToTransmit = false;
static bool shouldSenseNext = false;

extern RTC_HandleTypeDef hrtc;

/* Private functions ----------------------------------------------------------*/
void startNewTimeWindow() {
  if (shouldSenseNext) {
    readyToSense = true;
    RTC_clearWakeUpTimer();
    RTC_setWakeUpTimer(timeToNextTransmission_ms / 1000);
  } else {
    readyToTransmit = true;
    RTC_clearWakeUpTimer();
    RTC_setWakeUpTimer(timeToNextSensing_ms / 1000);
  }
  shouldSenseNext = !shouldSenseNext;
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {
  // Override the RTC interrupt handler to handle the time events
  if (hrtc->Instance == RTC) {
    startNewTimeWindow();
  }
}

/* Public functions ----------------------------------------------------------*/
void time_config_set(uint32_t period, uint32_t now, uint32_t time_slot) {
  if (period < SENSING_TRANSMIT_GAP_MS) {
      printf("Period is too short to fit sensing and transmit windows\n");
      return;
  }
  timeToNextTransmission_ms = SENSING_TRANSMIT_GAP_MS;
  timeToNextSensing_ms = period - SENSING_TRANSMIT_GAP_MS;

  uint32_t timeToFirstSensing_ms = (time_slot + 2*period) - now - SENSING_TRANSMIT_GAP_MS;
  timeToFirstSensing_ms %= period;

  shouldSenseNext = true;
  RTC_setWakeUpTimer(timeToFirstSensing_ms / 1000);
}

bool time_config_isReadyToSense(void) {
  if (readyToSense) {
    readyToSense = false;
    return true;
  }
  return false;
}

bool time_config_isReadyToTransmit(void) {
  if (readyToTransmit) {
    readyToTransmit = false;
    return true;
  }
  return false;
}
