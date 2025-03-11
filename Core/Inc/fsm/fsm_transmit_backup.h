/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit_backup.h
  * @brief          : Header for FSM transmit backup file
  ******************************************************************************
  */
#ifndef FSM_TRANSMIT_BACKUP_H
#define FSM_TRANSMIT_BACKUP_H

typedef enum {
    TRANSMIT_BACKUP_IDLE,
} FSM_TransmitBackup_State;

void FSM_TransmitBackup_init(void);
void FSM_TransmitBackup_handle(void);

#endif /* FSM_TRANSMIT_BACKUP_H */
