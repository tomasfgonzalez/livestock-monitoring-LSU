/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_link.c
  * @brief          : Source file for FSM link, handles the link between the
  *                   device and the Central Unit
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

/* Includes ------------------------------------------------------------------*/
#include "fsm_link.h"

#include "gpio.h"
#include "time_config.h"
#include "lsu_comms.h"
#include "usart.h"
#include "dma.h"
#include "lpuart.h"
#include "rylr998.h"

#include "string.h"
#include "stdio.h"
#include <stdint.h>

/* DefineS --------------------------------------------------------------------*/
#define RESPONSE_TIMEOUT_TIMER 15
#define CSMA_TIME_MINIMUM 10
#define CSMA_TIME_WINDOW 10

/* Private variables ----------------------------------------------------------*/
static FSM_Link_State current_state = LINK_IDLE;
static uint32_t CSMARandomTimeoutTimer = 0;
static uint32_t responseTimeoutTimer = 0;

/* Private functions ----------------------------------------------------------*/
bool processPacket(char* data) {
  if (strncmp(data, "CONFIG-", 7) == 0) {
    uint32_t id = 0;
    uint32_t period_ms = 0;
    uint32_t now_ms = 0;
    uint32_t time_slot_ms = 0;
    
    int parsed = sscanf(data, "CONFIG-%lu-%lu-%lu-%lu", &id, &period_ms, &now_ms, &time_slot_ms);
    if (parsed == 4) {
      LSU_setAddress(id);
      time_config_set(period_ms, now_ms, time_slot_ms);
      return true;
    }
  }
  // If we get here, either the format was wrong or parsing failed
  return false;
}

/**
 * @brief  Get a random uint8_t value that is unique per device
 * @retval Random uint8_t value
 * 
 * This function generates a value that should be different across different
 * device instances, even if called only once in the device's lifetime.
 */
static uint8_t getRandomUint8(void) {
    uint32_t device_id_0 = HAL_GetUIDw0();
    uint32_t device_id_1 = HAL_GetUIDw1();
    uint32_t device_id_2 = HAL_GetUIDw2();
    uint32_t tick = HAL_GetTick();
    
    // Mix all entropy sources
    uint32_t random_value = device_id_0 ^ device_id_1 ^ device_id_2 ^ tick;
    
    // Additional mixing for better distribution
    random_value = random_value ^ (random_value << 13);
    random_value = random_value ^ (random_value >> 17);
    random_value = random_value ^ (random_value << 5);
    
    // Return the lower 8 bits
    return (uint8_t)(random_value & 0xFF);
}

void startCSMATimer(void) {
  uint8_t random_backoff = (getRandomUint8() % CSMA_TIME_MINIMUM) + CSMA_TIME_WINDOW;
  CSMARandomTimeoutTimer = random_backoff;
}

void startResponseTimeoutTimer(void) {
  responseTimeoutTimer = RESPONSE_TIMEOUT_TIMER;
}

void fetchConfig(void) {
  LSU_sendSyncRequest(0);
  startResponseTimeoutTimer();
}

/* Public functions ------------------------------------------------------- */
void FSM_Link_init(void) {
  LSU_initPeripherals();
  LSU_setAddress(getRandomUint8());
  LSU_setChannelAux();

  sensor_heartrate_stop();

  // Start immediately by sending sync request
  fetchConfig();
  current_state = LINK_WAITING_RESPONSE;
}

void FSM_Link_handle(bool* isLinkEstablished, bool* isLinkError) {
  switch (current_state) {
    /* ------------------------- LINK_IDLE ------------------------- */
    case LINK_IDLE:
      if (CSMARandomTimeoutTimer == 0) {
        // CSMA backoff completed, try again
        fetchConfig();
        current_state = LINK_WAITING_RESPONSE;
      }
      break;

    /* ------------------------- LINK_WAITING_RESPONSE ------------------------- */
    case LINK_WAITING_RESPONSE:
      RYLR_RX_data_t* rx_data = LSU_getData();

      if (rx_data != NULL && rx_data->id == 0x01) {
        bool isValid = processPacket(rx_data->data);

        if (isValid) {
          *isLinkEstablished = true;
          LSU_deinitPeripherals();
          current_state = LINK_ESTABLISHED;
        } else {
          // Invalid response, try again
          fetchConfig();
        }
      } else if (responseTimeoutTimer == 0) {
        // No response received, channel is busy
        startCSMATimer();
        current_state = LINK_IDLE;
      }
      break;

    /* ------------------------- LINK_ESTABLISHED ------------------------- */
    case LINK_ESTABLISHED:
      // Do nothing
      break;
  }
}

void FSM_Link_tick_1s(void) {
  if (CSMARandomTimeoutTimer > 0) {
    CSMARandomTimeoutTimer--;
  }
  if (responseTimeoutTimer > 0) {
    responseTimeoutTimer--;
  }
}
