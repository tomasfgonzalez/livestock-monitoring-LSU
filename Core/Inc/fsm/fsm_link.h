/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_link.h
  * @brief          : Header for FSM link file
  ******************************************************************************
  */
#ifndef FSM_LINK_H
#define FSM_LINK_H

/* Includes ---------------------------------------------------------------*/
#include <stdbool.h>
#include "rylr998.h"
#include "usart.h"
#include "lsu_comms.h"

typedef enum {
  LINK_LISTENING,
  LINK_IDLE,
  LINK_FETCH,
  LINK_ESTABLISHED,
} FSM_Link_State;

void FSM_Link_init(void);
void FSM_Link_handle(bool* isLinkEstablished, bool* isLinkError);

#endif /* FSM_LINK_H */
