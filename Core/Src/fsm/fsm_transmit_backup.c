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
#define LISTENING_TIMEOUT_IN_SECONDS 2
#define MAX_RETRY_ATTEMPTS 3

/* Private variables ------------------------------------------------------*/
static FSM_TransmitBackup_State current_state = TRANSMIT_BACKUP_LISTENING;
static uint32_t responseTimeoutTimer = 0;
static uint32_t CSMARandomTimeoutTimer = 0;
static uint32_t listeningTimeoutTimer = 0;
static uint8_t retryAttempts = 0;

static bool ackReceived = false;

/* Private functions ------------------------------------------------------*/
static void startResponseTimeoutTimer(void) {
  responseTimeoutTimer = RESPONSE_TIMEOUT_IN_SECONDS;
}

static void startCSMATimer(void) {
  uint8_t random_backoff = (HAL_GetTick() % CSMA_BACKOFF_MAX) + 1;
  CSMARandomTimeoutTimer = random_backoff;
}

static void startListeningTimeoutTimer(void) {
  listeningTimeoutTimer = LISTENING_TIMEOUT_IN_SECONDS;
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

  // Start in listening state to check if channel is clear
  startListeningTimeoutTimer();
  CSMARandomTimeoutTimer = 0;
  ackReceived = false;
  retryAttempts = 0;
  current_state = TRANSMIT_BACKUP_LISTENING;
}

void FSM_TransmitBackup_handle(bool* isBackupTransmissionComplete, bool* isBackupTransmissionError) {
  switch (current_state) {
    /* ------------------------- TRANSMIT_BACKUP_LISTENING ------------------------- */
    case TRANSMIT_BACKUP_LISTENING:
      // Check if we received a message during listening period
      if (LSU_checkChannelBusy()) {
        // Channel is busy, start CSMA backoff
        startCSMATimer();
        current_state = TRANSMIT_BACKUP_IDLE;
      } else if (listeningTimeoutTimer == 0) {
        // Channel is clear, try to send data
        postData();
        current_state = TRANSMIT_BACKUP_WAITING_RESPONSE;
      }
      break;

    /* ------------------------- TRANSMIT_BACKUP_IDLE ------------------------- */
    case TRANSMIT_BACKUP_IDLE:
      if (CSMARandomTimeoutTimer == 0) {
        // CSMA backoff completed, go to listening state
        startListeningTimeoutTimer();
        current_state = TRANSMIT_BACKUP_LISTENING;
      }
      break;

    /* ------------------------- TRANSMIT_BACKUP_WAITING_RESPONSE ------------------------- */
    case TRANSMIT_BACKUP_WAITING_RESPONSE:
      RYLR_RX_data_t* rx_data = LSU_getData();

      if (rx_data != NULL && rx_data->id == 0x01) {
        ackReceived = strcmp(rx_data->data, "ACK") == 0;
        if (ackReceived) {
          // Successfully transmitted
          LSU_deinitPeripherals();
          current_state = TRANSMIT_BACKUP_COMPLETE;
        } else {
          // Invalid response, try again
          retryAttempts++;
          if (retryAttempts >= MAX_RETRY_ATTEMPTS) {
            // Max retries exceeded, signal failure
            LSU_deinitPeripherals();
            current_state = TRANSMIT_BACKUP_FAILED;
          } else {
            postData();
          }
        }
      } else if (responseTimeoutTimer == 0) {
        // No response received, increment retry counter
        retryAttempts++;
        if (retryAttempts >= MAX_RETRY_ATTEMPTS) {
          // Max retries exceeded, signal failure
          LSU_deinitPeripherals();
          current_state = TRANSMIT_BACKUP_FAILED;
        } else {
          // Try again with CSMA backoff
          startCSMATimer();
          current_state = TRANSMIT_BACKUP_IDLE;
        }
      }
      break;

    /* ------------------------- TRANSMIT_BACKUP_COMPLETE ------------------------- */
    case TRANSMIT_BACKUP_COMPLETE:
      // Do nothing
      *isBackupTransmissionComplete = true;
      *isBackupTransmissionError = false;
      break;

    /* ------------------------- TRANSMIT_BACKUP_FAILED ------------------------- */
    case TRANSMIT_BACKUP_FAILED:
      // Signal failure to main FSM
      *isBackupTransmissionComplete = false;
      *isBackupTransmissionError = true;
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
  if (listeningTimeoutTimer > 0) {
    listeningTimeoutTimer--;
  }
}
