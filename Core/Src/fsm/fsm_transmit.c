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
#include "time_config.h"

#include "lpuart.h"
#include "dma.h"
#include "gpio.h"

#define SENSING_INIT_WAIT_IN_SECONDS 5
#define SENSING_TIMEOUT_IN_SECONDS 15
#define TRANSMIT_TIMEOUT_IN_SECONDS 10
#define ACK_TIMEOUT_IN_SECONDS 5

/* Private variables ----------------------------------------------------------*/
static FSM_Transmit_State currentState = TRANSMIT_IDLE;

static int sensingTimer = 0;
static int transmitTimer = 0;
static int ackTimer = 0;

static bool ackReceived = false;

static LSU_Payload payload;

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
  static uint8_t temperature[2];
  static GPSData gps;
  static uint8_t heartrate;

  sensor_temperature_read(temperature);
  sensor_gps_read(&gps);
  sensor_heartrate_read(&heartrate);

  payload.latitude = gps.latitude;
  payload.longitude = gps.longitude;
  payload.temperature_livestock = temperature[0];
  payload.temperature_environment = temperature[1];
  payload.heartrate = heartrate;
}

static void sendPayload(void) {
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
  GPIO_Sensors_PowerOn();
  DMA_Init();
  LPUART_Init();
  HAL_Delay(50);
  LSU_setChannelMain();
  HAL_Delay(50);

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
      if (isTimeToSense) {
        startSensing();
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
        
        bool temperatureReady = sensor_temperature_is_measurement_ready();
        bool gpsReady = sensor_gps_is_measurement_ready();
        bool heartrateReady = sensor_heartrate_is_measurement_ready();
        bool allSensorsReady = temperatureReady && gpsReady && heartrateReady;

        if (sensingTimer <= 0 || allSensorsReady) {
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
        rx_data->data == NULL;
      }

      if (ackReceived) {
    	 HAL_Delay(50);
        ackReceived = false;
        LPUART_DeInit();
        DMA_Stop();
        GPIO_Sensors_PowerOff();
        currentState = TRANSMIT_IDLE;
      } else if (ackTimer <= 0) {
        //restartTransmission();
      } else if (transmitTimer <= 0) {
      //mainChannelFail = true;
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
