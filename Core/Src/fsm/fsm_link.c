#include "fsm_link.h"

static FSM_Link_State current_state = IDLE;

void FSM_Link_init(void) {
  current_state = IDLE;
}

void FSM_Link_handle(void) {
  switch (current_state) {
    case IDLE:
      // Add state handling logic here
      break;

    default:
      // Handle invalid state
      break;
  }
}
