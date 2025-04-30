/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_link.h
  * @brief          : Header for FSM link file
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
#ifndef __FSM_LINK_H__
#define __FSM_LINK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ---------------------------------------------------------------*/
#include <stdbool.h>
#include "rylr998.h"
#include "usart.h"
#include "lsu_comms.h"

/* FSM states --------------------------------------------------------------- */
typedef enum {
  LINK_LISTENING,
  LINK_IDLE,
  LINK_FETCH,
  LINK_ESTABLISHED,
} FSM_Link_State;

/* FSM interface ------------------------------------------------------------ */
void FSM_Link_init(void);
void FSM_Link_handle(bool* isLinkEstablished, bool* isLinkError);

#ifdef __cplusplus
}
#endif

#endif /* FSM_LINK_H */
