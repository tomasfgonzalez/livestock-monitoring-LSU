/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit.h
  * @brief          : Header for FSM transmit file
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
#ifndef __FSM_TRANSMIT_H__
#define __FSM_TRANSMIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

#include "sensor_temperature.h"
#include "sensor_heartrate.h"
#include "sensor_gps.h"

/* FSM states --------------------------------------------------------------- */
typedef enum {
  TRANSMIT_IDLE,
  TRANSMIT_SENSE,
  TRANSMIT_SEND,
  TRANSMIT_FAILED,
} FSM_Transmit_State;

/* FSM interface ------------------------------------------------------------ */
void FSM_Transmit_init(void);

void FSM_Transmit_handle(bool *mainChannelFail);

void FSM_Transmit_tick_1s(void);

#ifdef __cplusplus
}
#endif

#endif /* FSM_TRANSMIT_H */
