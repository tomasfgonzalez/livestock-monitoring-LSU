/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_main.h
  * @brief          : Header for FSM main file
  ******************************************************************************
  */

#ifndef FSM_MAIN_H
#define FSM_MAIN_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Public type definitions
typedef enum {
  INIT,
  INIT_ERROR,
  LINK,
  LINK_ERROR,
  TRANSMIT,
  TRANSMIT_BACKUP
} FSM_Main_State;

// Public function declarations
void FSM_Main_handle(uint8_t button_B4_pressed, uint8_t button_B5_pressed);

#ifdef __cplusplus
}
#endif

#endif /* FSM_MAIN_H */
