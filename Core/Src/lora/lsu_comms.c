/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : lsu_comms.c
  * @brief          : Source file for LSU communications, handles the API to
  *                   communicate with the Central Unit
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lsu_comms.h"
#include "rylr998.h"
#include "stm32l0xx_hal.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

uint8_t address = 0x00;

/* Private variables ---------------------------------------------------------*/
static char txBuffer[TX_BUFFER_SIZE];
static char payloadString[40];

/* Private functions ---------------------------------------------------------*/
void parsePayloadString(LSU_Payload* payload) {
  sprintf(payloadString, "%lu-%lu-%u-%u-%u",
    payload->latitude,
    payload->longitude,
    payload->temperature_livestock,
    payload->temperature_environment,
    payload->heartrate
  );
}

/* Public functions ----------------------------------------------------------*/

void LSU_setAddress(uint8_t addr){
  address = addr;
}

void LSU_setChannelMain(void) {
  rylr998_setChannel(1, address);
}

void LSU_setChannelAux(void){
  rylr998_setChannel(0, address);
}

// We need delays to ensure the data is sent before performing the next operation
#define SEND_DATA_DELAY 500
void LSU_sendParameters(uint16_t destination, LSU_Payload* payload) {
  parsePayloadString(payload);

  memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
  uint8_t data_length = strlen(payloadString);
  sprintf(txBuffer, AT"SEND=%u,%u,%s"END, destination, data_length, payloadString);

  rylr998_sendCommand(txBuffer);
  HAL_Delay(SEND_DATA_DELAY);
  if(getlast_cmd() !=RYLR_OK){
  		Error_Handler();
  	}
}

#define SEND_SYNC_DELAY 100
void LSU_sendSyncRequest(uint16_t destination){
  memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
  uint8_t data_length = 4;
  sprintf(txBuffer, AT"SEND=%u,%u,SYNC"END, destination, data_length);

  rylr998_sendCommand(txBuffer);
  HAL_Delay(SEND_SYNC_DELAY);
  if(getlast_cmd() !=RYLR_OK){
  		Error_Handler();
  	}
}

bool LSU_checkChannelBusy(void) {
  if (rylr998_GetInterruptFlag()) {
	  if(getlast_cmd()==RYLR_RCV)    return true;
  }
  return false;
}

RYLR_RX_data_t* LSU_getData(void){
 /* if (rylr998_GetInterruptFlag()) {
    return rylr998_getCommand(RYLR_RCV);
  }*/
  return rylr998_readCurrentPacket();
}
