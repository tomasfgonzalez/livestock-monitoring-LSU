/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_link.c
  * @brief          : Source file for FSM link, handles the link between the
  *                   device and the Central Unit
  ******************************************************************************
  */
#include "fsm_link.h"

static FSM_Link_State current_state = LINK_IDLE;

void FSM_Link_init(void) {
  current_state = LINK_IDLE;
}

void FSM_Link_handle(void) {
  switch (current_state) {
    case LINK_IDLE:
      // Add state handling logic here
      break;

    default:
      // Handle invalid state
      break;
  }
}
