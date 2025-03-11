/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_transmit.c
  * @brief          : Source file for FSM transmit, handles the transmission of
  *                   data from the device to the Central Unit
  ******************************************************************************
  */

#include "fsm_transmit.h"
#define SENSING_TIMEOUT_IN_SECONDS 5
#define ACK_TIMEOUT_IN_SECONDS 3

// ----------- Internal variables -----------
static FSM_Transmit_State currentState = TRANSMIT_IDLE;

static int sensingTimer = 0;  
static int ackTimer = 0;

static bool ackReceived       = false;  // Indica que llegó un ACK

static bool isPayloadReady = false;
static int payload = 0;

// ----------- Internal functions -----------
static void startSensing(void) {
    // sensor_gps_start();
    // sensor_heartrate_start();
    sensor_temperature_start();
} 

static void startSensingTimer(void) {
    sensingTimer = SENSING_TIMEOUT_IN_SECONDS;
}

static void startAckTimer(void) {
    ackTimer = ACK_TIMEOUT_IN_SECONDS;
}

static void createPayload(void) {
  uint16_t temperature[2];
  bool temperatureReadSuccess = sensor_temperature_read(&temperature);
  isPayloadReady = true;
  return;
}

static void sendPayload(void) {
	isPayloadReady = false;
    // TODO: Implementar la lógica para enviar el payload
}

// ---------- FSM functions -----------
void FSM_Transmit_init(void)
{
    currentState = TRANSMIT_IDLE;

    // Clear flags or timers
    ackReceived       = false;

    sensingTimer = 0;
    ackTimer     = 0;
}

void FSM_Transmit_handle(bool *mainChannelFail)

{
    switch (currentState)
    {
    case TRANSMIT_IDLE:
        bool onSensingWindow = time_config_on_sensing_window();
        bool onTransmitWindow = time_config_on_transmit_window();

        if (onSensingWindow && !isPayloadReady) {
            // testing_led1_on();
            startSensing();
            startSensingTimer();
            currentState = TRANSMIT_SENSE;
        } else if (onTransmitWindow) {
            // testing_led2_on();
            ackReceived = false;
            startAckTimer();
            sendPayload();
            currentState = TRANSMIT_SEND;
        }
        break;

    case TRANSMIT_SENSE:
        const temperatureReady = sensor_temperature_is_measurement_ready();
        // const gpsReady = sensor_gps_is_ready();
        // const heartrateReady = sensor_heartrate_is_ready();
        const allSensorsReady = temperatureReady;

        if (allSensorsReady || (sensingTimer <= 0)) {
            createPayload();
            currentState = TRANSMIT_IDLE;
            // testing_led1_off();
        }
        break;

    case TRANSMIT_SEND:
        if (ackReceived) {
            currentState = TRANSMIT_IDLE;
            // testing_led2_off();
        } else if (ackTimer <= 0) {
            // currentState = TRANSMIT_IDLE;
            // testing_led2_off();
            sendPayload();
            startAckTimer();
        }
        else if (!onTransmitWindow) {
            mainChannelFail = true;
        }
        break;

    default:
        currentState = TRANSMIT_IDLE;
        break;
    }
}

void FSM_Transmit_tick_1s(void)
{
    if (sensingTimer > 0)
        sensingTimer--;

    if (ackTimer > 0)
        ackTimer--;
}
