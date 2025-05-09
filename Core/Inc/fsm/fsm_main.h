/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_main.h
  * @brief          : Header for FSM main file
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
#ifndef __FSM_MAIN_H__
#define __FSM_MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"

#include <stdbool.h>
#include <stm32l0xx_hal.h>

#include "fsm_link.h"
#include "fsm_transmit.h"
#include "fsm_transmit_backup.h"

#include "sensor_temperature.h"
#include "sensor_heartrate.h"
#include "sensor_gps.h"

/* FSM states --------------------------------------------------------------- */
typedef enum {
  INIT,
  INIT_ERROR,
  LINK,
  LINK_ERROR,
  TRANSMIT,
  TRANSMIT_BACKUP
} FSM_Main_State;

/* FSM interface ------------------------------------------------------------ */
void FSM_Main_init(void);

void FSM_Main_handle(void);

void FSM_Main_tick_1s(void);

#ifdef __cplusplus
}
#endif

#endif /* FSM_MAIN_H */
