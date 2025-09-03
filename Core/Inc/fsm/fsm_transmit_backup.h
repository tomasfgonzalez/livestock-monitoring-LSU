/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit_backup.h
  * @brief          : Header for FSM transmit backup file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSM_TRANSMIT_BACKUP_H__
#define __FSM_TRANSMIT_BACKUP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* FSM states --------------------------------------------------------------- */
#include <stdbool.h>

/* Public type definitions ----------------------------------------------- */
typedef enum {
  TRANSMIT_BACKUP_LISTENING,
  TRANSMIT_BACKUP_IDLE,
  TRANSMIT_BACKUP_WAITING_RESPONSE,
  TRANSMIT_BACKUP_COMPLETE,
} FSM_TransmitBackup_State;

/* FSM interface ------------------------------------------------------------ */
void FSM_TransmitBackup_init(void);

void FSM_TransmitBackup_handle(bool* isBackupTransmissionComplete);

void FSM_TransmitBackup_tick_1s(void);



#ifdef __cplusplus
}
#endif

#endif /* FSM_TRANSMIT_BACKUP_H */
