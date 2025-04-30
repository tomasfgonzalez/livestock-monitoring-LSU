/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_link.c
  * @brief          : Source file for FSM link, handles the link between the
  *                   device and the Central Unit
  ******************************************************************************
  */
#include "fsm_link.h"
#include "time_config.h"
#include "lsu_comms.h"
#include "string.h"
#include "stdio.h"

static FSM_Link_State current_state = LINK_IDLE;

static int test_timer = 0;

bool checkChannelFree(void) {
  return test_timer <= 0;
}

bool processPacket(char* data) {
  if (strncmp(data, "CONFIG-", 7) == 0) {
    uint32_t id = 0;
    uint32_t time_slot_ms = 0;
    uint32_t period_ms = 0;
    
    int parsed = sscanf(data, "CONFIG-%lu-%lu-%lu", &id, &time_slot_ms, &period_ms);
    
    if (parsed == 3) {
      uint32_t period = period_ms / 1000;
      uint32_t sensing_start = ((time_slot_ms / 1000) + period / 2) % period;
      uint32_t sensing_duration = 5;
      uint32_t transmit_start = time_slot_ms / 1000;
      uint32_t transmit_duration = 5;

      time_config_set_period(period);
      time_config_set_sensing_window(sensing_start, sensing_duration);
      time_config_set_transmit_window(transmit_start, transmit_duration);
      
      return true;
    }
  }
  // If we get here, either the format was wrong or parsing failed
  return false;
}

void FSM_Link_init(void) {
  current_state = LINK_LISTENING;
  test_timer = 5;
  LSU_setChannelAux();
}

void FSM_Link_handle(bool* isLinkEstablished, bool* isLinkError) {
  switch (current_state) {
    case LINK_LISTENING:
      bool isChannelFree = checkChannelFree();

      if (isChannelFree) {
        LSU_sendSyncRequest(0);
        current_state = LINK_FETCH;
      }
      break;

    case LINK_IDLE:
      // Handle invalid state

      break;

    case LINK_FETCH:
      RYLR_RX_data_t* rx_data = LSU_getConfig();

      if (rx_data != NULL && rx_data->id == 0x01) {
        bool isValid = processPacket(rx_data->data);

        if (isValid) {
          *isLinkEstablished = true;
          current_state = LINK_ESTABLISHED;
        } else {
          printf("Invalid response\n");
          current_state = LINK_LISTENING;
        }
        break;
      }

    case LINK_ESTABLISHED:
      // Do nothing
      break;
  }
}

void FSM_Link_tick_1s(void) {
  if (test_timer > 0) {
    test_timer--;
  }
}
