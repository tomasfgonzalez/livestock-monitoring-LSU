#ifndef FSM_LINK_H
#define FSM_LINK_H

#include <stdint.h>

typedef enum {
    IDLE,
    // Add more states as needed
} FSM_Link_State;

void FSM_Link_init(void);
void FSM_Link_handle(void);

#endif /* FSM_LINK_H */