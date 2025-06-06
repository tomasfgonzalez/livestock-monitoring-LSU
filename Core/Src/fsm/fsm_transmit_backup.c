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

/* Private variables ------------------------------------------------------*/
static FSM_TransmitBackup_State current_state = TRANSMIT_BACKUP_IDLE;

/* Public functions -------------------------------------------------------*/
void FSM_TransmitBackup_init(void) {
  current_state = TRANSMIT_BACKUP_IDLE;
}

void FSM_TransmitBackup_handle(bool* isBackupTransmissionComplete) {
  switch (current_state) {
    case TRANSMIT_BACKUP_IDLE:
      // Add state handling logic here
      break;

    default:
      // Handle invalid state
      break;
  }
}
