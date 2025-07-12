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
#include "time_config.h"

#define SENSING_TIMEOUT_IN_SECONDS 5
#define TRANSMIT_TIMEOUT_IN_SECONDS 10
#define ACK_TIMEOUT_IN_SECONDS 20

/* Private variables ----------------------------------------------------------*/
static FSM_Transmit_State currentState = TRANSMIT_IDLE;

static int sensingTimer = 0;
static int transmitTimer = 0;
static int ackTimer = 0;

static bool ackReceived = false;

static LSU_Payload payload;
static bool isPayloadReady = false;

/* Private functions ----------------------------------------------------------*/
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

static void createPayload(void) {
  // uint8_t temperature[2];
  // GPSData gps;
  // uint8_t heartrate;

  // sensor_temperature_read(temperature);
  // sensor_gps_read(&gps);
  // heartrate = sensor_heartrate_read();

  static int rising_temperature = 10;
  rising_temperature += 10;
  payload.latitude = 32312313;
  payload.longitude = 39532141;
  payload.temperature_livestock = rising_temperature;
  payload.temperature_environment = 21;
  payload.heartrate = 72;
  isPayloadReady = true;
}

static void sendPayload(void) {
  isPayloadReady = false;
  LSU_sendParameters(0, &payload);
}

static void startTransmitTimer(void) {
  transmitTimer = TRANSMIT_TIMEOUT_IN_SECONDS;
}

static void startAckTimer(void) {
  ackTimer = ACK_TIMEOUT_IN_SECONDS;
}

static void startTransmission(void) {
  ackReceived = false;
  LSU_setChannelMain();
  sendPayload();

  startAckTimer();
  startTransmitTimer();
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
  transmitTimer = 0;
  ackTimer = 0;
}

void FSM_Transmit_handle(bool *mainChannelFail) {
  bool isTimeToSense = time_config_isReadyToSense();
  bool isTimeToTransmit = time_config_isReadyToTransmit();

  switch (currentState) {
    /* ------------------------- TRANSMIT_IDLE ------------------------- */
    case TRANSMIT_IDLE:
      if (isTimeToSense && !isPayloadReady) {
          startSensing();
          currentState = TRANSMIT_SENSE;
      } else if (isTimeToTransmit && isPayloadReady) {
          startTransmission();
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
        restartTransmission();
      } else if (transmitTimer <= 0) {
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

  if (transmitTimer > 0)
    transmitTimer--;

  if (ackTimer > 0)
    ackTimer--;
}
