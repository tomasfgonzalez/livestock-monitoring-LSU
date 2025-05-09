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

/* FSM states --------------------------------------------------------------- */
typedef enum {
  LINK_IDLE,
} FSM_Link_State;

/* FSM interface ------------------------------------------------------------ */
void FSM_Link_init(void);

void FSM_Link_handle(void);

#ifdef __cplusplus
}
#endif

#endif /* FSM_LINK_H */
