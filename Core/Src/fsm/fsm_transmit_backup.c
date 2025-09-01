/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit_backup.c
  * @brief          : Source file for FSM transmit backup, handles the backup
  *                   transmission of data from the device to the Central Unit
  ******************************************************************************
  */

/* Includes ---------------------------------------------------------------*/
#include "fsm_transmit_backup.h"

#include <string.h>

#include "lsu_comms.h"
#include "lora/lsuPayload.h"

/* Define -----------------------------------------------------------------*/
#define RESPONSE_TIMEOUT_IN_SECONDS 2
#define CSMA_BACKOFF_MAX 10

/* Private variables ------------------------------------------------------*/
static FSM_TransmitBackup_State current_state = TRANSMIT_BACKUP_IDLE;
static uint32_t responseTimeoutTimer = 0;
static uint32_t CSMARandomTimeoutTimer = 0;

static bool ackReceived = false;

/* Private functions ------------------------------------------------------*/
static void startResponseTimeoutTimer(void) {
  responseTimeoutTimer = RESPONSE_TIMEOUT_IN_SECONDS;
}

static void startCSMATimer(void) {
  uint8_t random_backoff = (HAL_GetTick() % CSMA_BACKOFF_MAX) + 1;
  CSMARandomTimeoutTimer = random_backoff;
}

static void sendPayload(void) {
  const LSU_Payload* payload = lsuPayload_get();
  if (payload != NULL) {
    LSU_sendParameters(0, payload);
  }
}

static void startTransmission(void) {
  LSU_initPeripherals();
  LSU_setChannelAux();
  HAL_Delay(50);

  sendPayload();
  startResponseTimeoutTimer();
}

static void postData(void) {
  if (lsuPayload_isValid()) {
    startTransmission();
  }
}

/* Public functions -------------------------------------------------------*/
void FSM_TransmitBackup_init(void) {
  LSU_initPeripherals();
  LSU_setChannelAux();
  HAL_Delay(50);

  postData();
  startResponseTimeoutTimer();
  CSMARandomTimeoutTimer = 0;
  ackReceived = false;
  current_state = TRANSMIT_BACKUP_WAITING_RESPONSE;
}

void FSM_TransmitBackup_handle(bool* isBackupTransmissionComplete) {
  switch (current_state) {
    /* ------------------------- TRANSMIT_BACKUP_IDLE ------------------------- */
    case TRANSMIT_BACKUP_IDLE:
      if (CSMARandomTimeoutTimer == 0) {
        // CSMA backoff completed, try again
        postData();
        current_state = TRANSMIT_BACKUP_WAITING_RESPONSE;
      }
      break;

    /* ------------------------- TRANSMIT_BACKUP_WAITING_RESPONSE ------------------------- */
    case TRANSMIT_BACKUP_WAITING_RESPONSE:
      RYLR_RX_data_t* rx_data = LSU_getData();

      if (rx_data != NULL && rx_data->id == 0x01) {
        ackReceived = strcmp(rx_data->data, "ACK") == 0;
        if (ackReceived) {
          // Successfully transmitted
          *isBackupTransmissionComplete = true;
          LSU_deinitPeripherals();
          current_state = TRANSMIT_BACKUP_COMPLETE;
        } else {
          // Invalid response, try again
          postData();
        }
      } else if (responseTimeoutTimer == 0) {
        // No response received, channel is busy
        startCSMATimer();
        current_state = TRANSMIT_BACKUP_IDLE;
      }
      break;

    /* ------------------------- TRANSMIT_BACKUP_COMPLETE ------------------------- */
    case TRANSMIT_BACKUP_COMPLETE:
      // Do nothing
      break;
  }
}

void FSM_TransmitBackup_tick_1s(void) {
  if (responseTimeoutTimer > 0) {
    responseTimeoutTimer--;
  }
  if (CSMARandomTimeoutTimer > 0) {
    CSMARandomTimeoutTimer--;
  }
}
