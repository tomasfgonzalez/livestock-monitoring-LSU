/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_main.c
  * @brief          : Main FSM file, handles the main state machine of the device
  ******************************************************************************
  */

#include "fsm_main.h"

#define INIT_TIMEOUT 50

static FSM_Main_State currentState = INIT;
static uint32_t initTimer = 0;

// Transitions
static bool isInitError(void) {
  return sensor_temperature_has_error() || sensor_heartrate_has_error() || sensor_gps_has_error();
}

static void performRestart(void) {
  __disable_irq();
  NVIC_SystemReset();
}

static bool isInitSuccess(void) {
  return sensor_temperature_has_started();
  // TODO: Implement other sensors check
  // return sensor_temperature_has_started() && sensor_heartrate_has_started() && sensor_gps_has_started();
}

static bool isLinkEstablished(void) {
  // TODO: Implement link establishment check
  return true;
  return false;
}

static bool isLinkError(void) {
  // TODO: Implement link error check
  return false;
}

static bool isLinkErrorResolved(void) {
  // TODO: Implement error resolution check
  return true;
}

static bool isBackupTransmissionComplete(void) {
  // TODO: Implement backup completion check
  return false;
}

// Flags from internal FSMs
static bool hasMainChannelFailed = false;
static bool hasBackupChannelFailed = false;  

void FSM_Main_init(void) {
  currentState = INIT;
  // Initialize sensors
  sensor_temperature_init();
  sensor_heartrate_init();
  sensor_gps_init();

  // But stop such sensors until they are needed
  sensor_temperature_stop();
  sensor_heartrate_stop();
  sensor_gps_stop();

  initTimer = INIT_TIMEOUT;
}

void FSM_Main_handle(void) {
  switch (currentState) {
    case INIT:
      if (isInitError()) {
        currentState = INIT_ERROR;
      } else if (isInitSuccess()) {
        currentState = LINK;
      }
      break;

    case INIT_ERROR:
      performRestart();
      break;

    case LINK:
      FSM_Link_handle();

      if (isLinkError()) {
        currentState = LINK_ERROR;
      } else if (isLinkEstablished()) {
        currentState = TRANSMIT;
        FSM_Transmit_init();
      }
      break;

    case LINK_ERROR:
      if (isLinkErrorResolved()) {
        currentState = LINK;
        FSM_Link_init();
      }
      break;

    case TRANSMIT:
      FSM_Transmit_handle(&hasMainChannelFailed);
      if (hasMainChannelFailed) {
        currentState = TRANSMIT_BACKUP;
        FSM_TransmitBackup_init();
      }
      break;

    case TRANSMIT_BACKUP:
      FSM_TransmitBackup_handle();
      if (hasBackupChannelFailed) {
        currentState = LINK_ERROR;
      } else if (isBackupTransmissionComplete()) {
        currentState = TRANSMIT;
        FSM_Transmit_init();
      }
      break;

    default:
      currentState = INIT;
      break;
  }
}

void FSM_Main_tick_1s(void) {
  if (currentState == INIT) {
    initTimer--;
    if (initTimer <= 0) {
      currentState = INIT_ERROR;
    }

    if (currentState == TRANSMIT) {
      FSM_Transmit_tick_1s();
    }
  }
  sensor_temperature_tick_1s();
}
