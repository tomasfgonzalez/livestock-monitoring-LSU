/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_main.c
  * @brief          : Main FSM file, handles the main state machine of the device
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Gonazalez & Morris.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "fsm_main.h"

#define INIT_TIMEOUT 50

/* Private variables ---------------------------------------------------------*/
static FSM_Main_State currentState = INIT;
static uint32_t initTimer = 0;

/* Private functions ---------------------------------------------------------*/
static bool isInitError(void) {
  return false;
  return sensor_temperature_has_error() || sensor_heartrate_has_error() || sensor_gps_has_error();
}

static void performRestart(void) {
  __disable_irq();
  NVIC_SystemReset();
}

static bool isInitSuccess(void) {
  return true;
  return sensor_temperature_has_started();
  // TODO: Implement other sensors check
  // return sensor_temperature_has_started() && sensor_heartrate_has_started() && sensor_gps_has_started();
}

static bool isLinkErrorResolved(void) {
  // TODO: Implement error resolution check
  return true;
}

static bool isBackupTransmissionComplete(void) {
  // TODO: Implement backup completion check
  return false;
}

/* Public functions ----------------------------------------------------------*/
void FSM_Main_init(void) {
  currentState = INIT;
  // Initialize sensors
  sensor_temperature_init();
  sensor_heartrate_init();
  sensor_gps_init();

  // But stop such sensors until they are needed
  // sensor_temperature_stop();
  // sensor_heartrate_stop();
  // sensor_gps_stop();

  initTimer = INIT_TIMEOUT;
}

void FSM_Main_handle(void) {
  switch (currentState) {
    /* ------------------------- INIT ----------------------------- */
    case INIT:
      if (isInitError()) {
        currentState = INIT_ERROR;
      } else if (isInitSuccess()) {
        FSM_Link_init();
        currentState = LINK;
      }
      break;

    /* ------------------------- INIT_ERROR ------------------------- */
    case INIT_ERROR:
      performRestart();
      break;

    /* ------------------------- LINK ----------------------------- */
    case LINK:
      static bool isLinkEstablished = false;
      static bool isLinkError = false;

      FSM_Link_handle(&isLinkEstablished, &isLinkError);

      if (isLinkEstablished) {
        FSM_Transmit_init();
        currentState = TRANSMIT;
      } else if (isLinkError) {
        currentState = LINK_ERROR;
      }
      break;

    /* ------------------------- LINK_ERROR ------------------------- */
    case LINK_ERROR:
      if (isLinkErrorResolved()) {
        FSM_Link_init();
        currentState = LINK;
      }
      break;

    /* ------------------------- TRANSMIT ----------------------------- */
    case TRANSMIT:
      static bool hasMainChannelFailed = false;

      FSM_Transmit_handle(&hasMainChannelFailed);
      if (hasMainChannelFailed) {
        currentState = TRANSMIT_BACKUP;
        FSM_TransmitBackup_init();
      }
      break;

    /* ------------------------- TRANSMIT_BACKUP ------------------------- */
    case TRANSMIT_BACKUP:
      static bool hasBackupChannelFailed = false;

      FSM_TransmitBackup_handle(&hasBackupChannelFailed);
      if (hasBackupChannelFailed) {
        currentState = LINK_ERROR;
      } else if (isBackupTransmissionComplete()) {
        currentState = TRANSMIT;
        FSM_Transmit_init();
      }
      break;

    /* ------------------------- DEFAULT ----------------------------- */
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
  }
  if (currentState == TRANSMIT) {
    FSM_Transmit_tick_1s();
  }
  if (currentState == LINK) {
    FSM_Link_tick_1s();
  }
  sensor_temperature_tick_1s();
}
