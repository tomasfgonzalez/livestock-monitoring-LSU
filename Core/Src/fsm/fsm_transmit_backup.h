#ifndef FSM_TRANSMIT_BACKUP_H
#define FSM_TRANSMIT_BACKUP_H

typedef enum {
    IDLE,
} FSM_TransmitBackup_State;

void FSM_TransmitBackup_init(void);
void FSM_TransmitBackup_handle(void);

#endif /* FSM_TRANSMIT_BACKUP_H */