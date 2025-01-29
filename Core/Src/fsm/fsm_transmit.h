#ifndef FSM_TRANSMIT_H
#define FSM_TRANSMIT_H

typedef enum {
    TRANSMIT_IDLE,
} FSM_Transmit_State;

void FSM_Transmit_init(void);
void FSM_Transmit_handle(void);

#endif /* FSM_TRANSMIT_H */