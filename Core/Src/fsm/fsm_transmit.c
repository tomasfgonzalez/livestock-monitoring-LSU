/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit.c
  * @brief          : Source file for FSM transmit, handles the transmission of
  *                   data from the device to the Central Unit
  ******************************************************************************
  */

#include "fsm_transmit.h"
#include "lsu_comms.h"
#include "string.h"

#define SENSING_TIMEOUT_IN_SECONDS 5
#define ACK_TIMEOUT_IN_SECONDS 20

// ----------- Internal variables -----------
static FSM_Transmit_State currentState = TRANSMIT_IDLE;

static int sensingTimer = 0;  
static int ackTimer = 0;

static bool ackReceived       = false;  // Indica que llegó un ACK

static LSU_Payload payload;
static bool isPayloadReady = false;

// ----------- Internal functions -----------
static void startSensingTimer(void) {
  sensingTimer = SENSING_TIMEOUT_IN_SECONDS;
}

static void startSensing(void) {
  // sensor_gps_start();
  // sensor_heartrate_start();
  // sensor_temperature_start();
  startSensingTimer();
}

static void stopSensing(void) {
    // sensor_gps_stop();
    // sensor_heartrate_stop();
    // sensor_temperature_stop();
    sensingTimer = 0;
}

static void startAckTimer(void) {
    ackTimer = ACK_TIMEOUT_IN_SECONDS;
}

static void createPayload(void) {
  // uint8_t temperature[2];
  // GPSData gps;
  // uint8_t heartrate;

  // sensor_temperature_read(temperature);
  // sensor_gps_read(&gps);
  // heartrate = sensor_heartrate_read();

  payload.latitude = 32312313;
  payload.longitude = 39532141;
  payload.temperature_livestock = 32;
  payload.temperature_environment = 21;
  payload.heartrate = 72;
  isPayloadReady = true;
}

static void sendPayload(void) {
  isPayloadReady = false;
  LSU_sendParameters(0, &payload);
  startAckTimer();
}

// ---------- FSM functions -----------
void FSM_Transmit_init(void) {
  currentState = TRANSMIT_IDLE;

  // Clear flags or timers
  ackReceived = false;
  sensingTimer = 0;
  ackTimer     = 0;

  // Debug time config
  uint32_t period = time_config_get_period();
  uint32_t sensing_start = time_config_get_sensing_start();
  uint32_t sensing_duration = time_config_get_sensing_duration();
  uint32_t transmit_start = time_config_get_transmit_start();
  uint32_t transmit_duration = time_config_get_transmit_duration();

  printf("Period: %lu\n", period);
  printf("Sensing window: %lu %lu\n", sensing_start, sensing_start + sensing_duration);
  printf("Transmit window: %lu %lu\n", transmit_start, transmit_start + transmit_duration);
}

void FSM_Transmit_handle(bool *mainChannelFail) {
  bool onSensingWindow = time_config_on_sensing_window();
  bool onTransmitWindow = time_config_on_transmit_window();

  switch (currentState) {
    /* ------------------------- TRANSMIT_IDLE ------------------------- */
    case TRANSMIT_IDLE:
      if (onSensingWindow && !isPayloadReady) {
          startSensing();
          currentState = TRANSMIT_SENSE;
      } else if (onTransmitWindow && isPayloadReady) {
          ackReceived = false;
          LSU_setChannelMain();
          sendPayload();
          currentState = TRANSMIT_SEND;
      }
      break;

        if (onSensingWindow && !isPayloadReady) {
            startSensing();
            currentState = TRANSMIT_SENSE;
        } else if (onTransmitWindow) {
            ackReceived = false;
            startAckTimer();
            sendPayload();
            currentState = TRANSMIT_SEND;
        }
        break;

    /* ------------------------- TRANSMIT_SENSE ------------------------- */
    case TRANSMIT_SENSE:
        // bool temperatureReady = sensor_temperature_is_measurement_ready();
        // bool gpsReady = sensor_gps_is_measurement_ready();
        // bool heartrateReady = sensor_heartrate_is_ready();
        // bool allSensorsReady = temperatureReady && gpsReady && heartrateReady;

        if (sensingTimer <= 0) {
            createPayload();
            stopSensing();
            currentState = TRANSMIT_IDLE;
        }
        break;

    /* ------------------------- TRANSMIT_SEND ------------------------- */
    case TRANSMIT_SEND:
      RYLR_RX_data_t* rx_data = LSU_getData();
      if (rx_data != NULL) {
        ackReceived = strcmp(rx_data->data, "ACK") == 0;
      }

      if (ackReceived) {
    	  ackReceived = false;
        currentState = TRANSMIT_IDLE;
      } else if (ackTimer <= 0) {
        sendPayload();
      } else if (!onTransmitWindow) {
        *mainChannelFail = true;
      }
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
