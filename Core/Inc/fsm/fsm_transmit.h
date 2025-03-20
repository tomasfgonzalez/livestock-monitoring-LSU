/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit.h
  * @brief          : Header for FSM transmit file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FSM_TRANSMIT_H
#define FSM_TRANSMIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

#include "sensor_temperature.h"
#include "sensor_heartrate.h"
#include "sensor_gps.h"

#include "time_config.h"
#include "testing_leds.h"

/* FSM states --------------------------------------------------------------- */
typedef enum {
  TRANSMIT_IDLE,
  TRANSMIT_SENSE,
  TRANSMIT_SEND,
} FSM_Transmit_State;

/* FSM functions ------------------------------------------------------------- */
void FSM_Transmit_init(void);

void FSM_Transmit_handle(bool *mainChannelFail);

void FSM_Transmit_tick_1s(void);

#ifdef __cplusplus
}
#endif

#endif /* FSM_TRANSMIT_H */
