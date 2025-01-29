#ifndef FSM_LINK_H
#define FSM_LINK_H

#include <stdint.h>

typedef enum {
    LINK_IDLE,
} FSM_Link_State;

void FSM_Link_init(void);
void FSM_Link_handle(void);

#endif /* FSM_LINK_H */