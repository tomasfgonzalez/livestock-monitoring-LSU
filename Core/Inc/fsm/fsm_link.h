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

/* FSM states --------------------------------------------------------------- */
typedef enum {
  LINK_LISTENING,
  LINK_IDLE,
  LINK_WAITING_RESPONSE,
  LINK_ESTABLISHED,
  LINK_FAILED,
} FSM_Link_State;

/* FSM interface ------------------------------------------------------------ */
void FSM_Link_init(void);
void FSM_Link_handle(bool* isLinkEstablished, bool* isLinkError);
void FSM_Link_tick_1s(void);

#ifdef __cplusplus
}
#endif

#endif /* FSM_LINK_H */
