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
#include "string.h"
#include "stdio.h"

#define CHANNEL_FREE_TIMER 1

/* Private variables ----------------------------------------------------------*/
static FSM_Link_State current_state = LINK_IDLE;
static uint32_t channelFreeTimer = 0;
static uint32_t CSMARandomTimeoutTimer = 0;

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

void startChannelFreeTimer(void) {
  channelFreeTimer = CHANNEL_FREE_TIMER;
}

void startCSMATimer(void) {
  uint8_t random_backoff = (getRandomUint8() % 10) + 1;
  CSMARandomTimeoutTimer = random_backoff;
}

/* Public functions ------------------------------------------------------- */
void FSM_Link_init(void) {
  GPIO_Init();
  GPIO_Sensors_PowerOn();

  DMA_Init();
  LPUART_Init();
  LSU_setAddress(getRandomUint8());
  LSU_setChannelAux();
  startChannelFreeTimer();

  current_state = LINK_LISTENING;
}

void FSM_Link_handle(bool* isLinkEstablished, bool* isLinkError) {
  switch (current_state) {
    /* ------------------------- LINK_LISTENING ------------------------- */
    case LINK_LISTENING:
      bool isChannelBusy = LSU_checkChannelBusy();

      if (isChannelBusy) {
        startCSMATimer();
        current_state = LINK_IDLE;
      } else if (channelFreeTimer == 0) {
        LSU_sendSyncRequest(0);
        current_state = LINK_FETCH;
      }
      break;

    /* ------------------------- LINK_IDLE ------------------------- */
    case LINK_IDLE:
      if (CSMARandomTimeoutTimer == 0) {
        startChannelFreeTimer();
        current_state = LINK_LISTENING;
      }
      break;

    /* ------------------------- LINK_FETCH ------------------------- */
    case LINK_FETCH:
      RYLR_RX_data_t* rx_data = LSU_getData();

      if (rx_data != NULL && rx_data->id == 0x01) {
        bool isValid = processPacket(rx_data->data);

        if (isValid) {
          *isLinkEstablished = true;
          // LPUART_DeInit();
          DMA_Stop();
          current_state = LINK_ESTABLISHED;
        } else {
          // Invalid response
          startChannelFreeTimer();
          current_state = LINK_LISTENING;
        }
        break;
      }

    /* ------------------------- LINK_ESTABLISHED ------------------------- */
    case LINK_ESTABLISHED:
      // Do nothing
      break;
  }
}

void FSM_Link_tick_1s(void) {
  if (channelFreeTimer > 0) {
    channelFreeTimer--;
  }
  if (CSMARandomTimeoutTimer > 0) {
    CSMARandomTimeoutTimer--;
  }
}
