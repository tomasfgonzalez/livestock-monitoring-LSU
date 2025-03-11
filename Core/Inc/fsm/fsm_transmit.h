/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit.h
  * @brief          : Header for FSM transmit file
  ******************************************************************************
  */
#ifndef FSM_TRANSMIT_H
#define FSM_TRANSMIT_H

#include <stdbool.h>
#include <stdint.h>

#include "sensor_temperature.h"
#include "time_config.h"
#include "testing_leds.h"

typedef enum {
    TRANSMIT_IDLE,
    TRANSMIT_SENSE,
    TRANSMIT_SEND,
} FSM_Transmit_State;

void FSM_Transmit_init(void);

void FSM_Transmit_handle(bool *mainChannelFail);

void FSM_Transmit_tick_1s(void);

#endif /* FSM_TRANSMIT_H */
