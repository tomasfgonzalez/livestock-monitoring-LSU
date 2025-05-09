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

// ----------- Internal functions -----------
static void startSensingTimer(void) {
    sensingTimer = SENSING_TIMEOUT_IN_SECONDS;
}

static void startSensing(void) {
    sensor_gps_start();
    // sensor_heartrate_start();
    sensor_temperature_start();
    startSensingTimer();
}

static void stopSensing(void) {
    sensor_gps_stop();
    // sensor_heartrate_stop();
    sensor_temperature_stop();
    sensingTimer = 0;
}

static void startAckTimer(void) {
    ackTimer = ACK_TIMEOUT_IN_SECONDS;
}

static void createPayload(void) {
  uint8_t temperature[2];
  GPSData gps;

  sensor_temperature_read(temperature);
  sensor_gps_read(&gps);

  isPayloadReady = true;
}

static void sendPayload(void) {
	isPayloadReady = false;
    // TODO: Implementar la lógica para enviar el payload
}

// ---------- FSM functions -----------
void FSM_Transmit_init(void) {
    currentState = TRANSMIT_IDLE;

    // Clear flags or timers
    ackReceived = false;

    sensingTimer = 0;
    ackTimer     = 0;
}

void FSM_Transmit_handle(bool *mainChannelFail) {
    bool onSensingWindow = time_config_on_sensing_window();
    bool onTransmitWindow = time_config_on_transmit_window();

    switch (currentState)
    {
    case TRANSMIT_IDLE:

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

    case TRANSMIT_SENSE:
        bool temperatureReady = sensor_temperature_is_measurement_ready();
        bool gpsReady = sensor_gps_is_measurement_ready();
//         const heartrateReady = sensor_heartrate_is_ready();
        bool allSensorsReady = temperatureReady && gpsReady;

        if (allSensorsReady || (sensingTimer <= 0)) {
            createPayload();
            stopSensing();
            currentState = TRANSMIT_IDLE;
        }
        break;

    case TRANSMIT_SEND:
        if (ackReceived) {
            currentState = TRANSMIT_IDLE;
        } else if (ackTimer <= 0) {
            // currentState = TRANSMIT_IDLE;
            sendPayload();
            startAckTimer();
        }
        else if (!onTransmitWindow) {
            *mainChannelFail = true;
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
