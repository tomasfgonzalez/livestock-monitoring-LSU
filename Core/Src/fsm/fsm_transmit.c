/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit.c
  * @brief          : Source file for FSM transmit, handles the transmission of
  *                   data from the device to the Central Unit
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "fsm_transmit.h"

#include <string.h>

#include "sensor_temperature.h"
#include "sensor_gps.h"
#include "sensor_heartrate.h"
#include "sensor_all.h"
#include "lsu_comms.h"
#include "lsu_payload.h"

#include "time_config.h"

/* Define -----------------------------------------------------------------*/
#define SENSING_INIT_WAIT_IN_SECONDS 5
#define SENSING_TIMEOUT_IN_SECONDS 10
#define MAX_RETRY_COUNT 3
#define ACK_TIMEOUT_IN_SECONDS 12

/* Private variables ----------------------------------------------------------*/
static FSM_Transmit_State currentState = TRANSMIT_IDLE;

static int sensingTimer = 0;
static int retryCount = 0;
static int ackTimer = 0;

static bool ackReceived = false;

/* Private functions ----------------------------------------------------------*/
static void startSensingTimer(void) {
  sensingTimer = SENSING_TIMEOUT_IN_SECONDS;
}

static void startSensing(void) {
  sensor_all_init();
  startSensingTimer();
}

static void stopSensing(void) {
  sensor_all_stop();
  sensingTimer = 0;
}

static void createPayload(void) {
  static LSU_Payload payload;
  
  // Generate random seed based on system tick for variation
  uint32_t seed = HAL_GetTick();
  
  // Rosario, Argentina coordinates (center) with small random variations
  // Base coordinates: -32.9468, -60.6393
  // Convert to integer format (multiply by 1000000 for 6 decimal places)
  int32_t base_lat = -32946800;  // -32.9468 * 1000000
  int32_t base_lon = -60639300;  // -60.6393 * 1000000
  
  // Add random variation of ±0.01 degrees (±10000 in integer format)
  int32_t lat_variation = ((seed % 20001) - 10000);  // -10000 to +10000
  int32_t lon_variation = ((seed * 7) % 20001) - 10000;  // Different variation for longitude
  
  payload.latitude = base_lat + lat_variation;
  payload.longitude = base_lon + lon_variation;
  
  // Ambient temperature: 8-21°C (Rosario September average)
  // Convert to integer format (multiply by 100 for 2 decimal places)
  uint16_t ambient_temp = 800 + ((seed * 3) % 1301);  // 800 to 2100 (8.00°C to 21.00°C)
  payload.temperature_environment = ambient_temp / 100;
  
  // Livestock body temperature: 36.5-37.5°C (considerably higher than ambient)
  // Convert to integer format (multiply by 100 for 2 decimal places)
  uint16_t body_temp = 3650 + ((seed * 5) % 101);  // 3650 to 3750 (36.50°C to 37.50°C)
  payload.temperature_livestock = body_temp / 100;
  
  // Heart rate: 70-100 BPM for livestock (cattle typically 60-80, but can vary)
  uint8_t heartrate = 70 + ((seed * 11) % 31);  // 70 to 100 BPM
  payload.heartrate = heartrate;
  
  // Store in LSU payload for backup FSM to use
  lsuPayload_set(&payload);
}

static void sendPayload(void) {
  const LSU_Payload* payload = lsuPayload_get();
  if (payload != NULL) {
    LSU_sendParameters(0, payload);
  }
}

static void startAckTimer(void) {
  ackTimer = ACK_TIMEOUT_IN_SECONDS;
}

static void startTransmission(void) {
  ackReceived = false;
  LSU_initPeripherals();
  LSU_setChannelMain();
  HAL_Delay(50);

  sendPayload();
  startAckTimer();
  retryCount = 1;
}

static void restartTransmission(void) {
  sendPayload();
  startAckTimer();
}

/* Public functions ----------------------------------------------------------*/
void FSM_Transmit_init(void) {
  currentState = TRANSMIT_IDLE;

  // Clear flags or timers
  ackReceived = false;
  sensingTimer = 0;
  retryCount = 0;
  ackTimer = 0;

  mode_STOP();
}

void FSM_Transmit_handle(bool *mainChannelFail) {
  bool isTimeToSense = time_config_isReadyToSense();
  bool isTimeToTransmit = time_config_isReadyToTransmit();

  switch (currentState) {
    /* ------------------------- TRANSMIT_IDLE ------------------------- */
    case TRANSMIT_IDLE:
      if (isTimeToSense) {
        //startSensing();
        currentState = TRANSMIT_SENSE;
      } else if (isTimeToTransmit) {
        startTransmission();
        currentState = TRANSMIT_SEND;
      }
      break;

    /* ------------------------- TRANSMIT_SENSE ------------------------- */
    case TRANSMIT_SENSE:
        // Give sensors some time to start up before checking readiness
        if (sensingTimer > SENSING_TIMEOUT_IN_SECONDS - SENSING_INIT_WAIT_IN_SECONDS) {
            break;
        }

        if (sensingTimer <= 0) {
          createPayload();
          stopSensing();
          currentState = TRANSMIT_IDLE;
          mode_STOP();
        }
        break;

    /* ------------------------- TRANSMIT_SEND ------------------------- */
    case TRANSMIT_SEND:
       RYLR_RX_data_t* rx_data = LSU_getData();

      if (rx_data != NULL) {
        ackReceived = strcmp(rx_data->data, "ACK") == 0;
      }

      if (ackReceived) {
        HAL_Delay(50);
        ackReceived = false;
        LSU_deinitPeripherals();
        currentState = TRANSMIT_IDLE;
        sensingTimer = 0;
        retryCount = 0;
        ackTimer = 0;
        mode_STOP();
      } else if (ackTimer <= 0) {
        if (retryCount < MAX_RETRY_COUNT) {
          retryCount++;
          restartTransmission();
        } else {
          LSU_deinitPeripherals();
          currentState = TRANSMIT_FAILED;
          sensingTimer = 0;
          retryCount = 0;
          ackTimer = 0;
        }
      }
      break;

    /* ------------------------- TRANSMIT_FAILED ------------------------- */
    case TRANSMIT_FAILED:
      // Final state - transmission failed after all retries
      *mainChannelFail = true;
      break;

    /* ------------------------- DEFAULT ----------------------------- */
    default:
      currentState = TRANSMIT_IDLE;
      break;
  }
}

void FSM_Transmit_tick_1s(void) {
  if (sensingTimer > 0)
    sensingTimer--;

  if (ackTimer > 0)
    ackTimer--;
}
